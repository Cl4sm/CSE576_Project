find_valid_gpt(int fd, gpt_header ** gpt, gpt_entry ** ptes)
{
        extern int force_gpt;
	int good_pgpt = 0, good_agpt = 0, good_pmbr = 0;
	gpt_header *pgpt = NULL, *agpt = NULL;
	gpt_entry *pptes = NULL, *aptes = NULL;
	legacy_mbr *legacymbr = NULL;
	uint64_t lastlba;
	if (!gpt || !ptes)
		return 0;

	if (!(lastlba = last_lba(fd)))
		return 0;
	good_pgpt = is_gpt_valid(fd, GPT_PRIMARY_PARTITION_TABLE_LBA,
				 &pgpt, &pptes);
        if (good_pgpt) {
		good_agpt = is_gpt_valid(fd,
                                         __le64_to_cpu(pgpt->alternate_lba),
					 &agpt, &aptes);
                if (!good_agpt) {
                        good_agpt = is_gpt_valid(fd, lastlba,
                                                 &agpt, &aptes);
                }
        }
        else {
                good_agpt = is_gpt_valid(fd, lastlba,
                                         &agpt, &aptes);
        }

        /* The obviously unsuccessful case */
        if (!good_pgpt && !good_agpt) {
                goto fail;
        }

	/* This will be added to the EFI Spec. per Intel after v1.02. */
        legacymbr = malloc(sizeof (*legacymbr));
        if (legacymbr) {
                memset(legacymbr, 0, sizeof (*legacymbr));
                read_lba(fd, 0, (uint8_t *) legacymbr,
                         sizeof (*legacymbr));
                good_pmbr = is_pmbr_valid(legacymbr);
                free(legacymbr);
                legacymbr=NULL;
        }

        /* Failure due to bad PMBR */
        if ((good_pgpt || good_agpt) && !good_pmbr && !force_gpt) {
                fprintf(stderr,
                       "  Warning: Disk has a valid GPT signature "
                       "but invalid PMBR.\n"
                       "  Assuming this disk is *not* a GPT disk anymore.\n"
                       "  Use gpt kernel option to override.  "
                       "Use GNU Parted to correct disk.\n");
                goto fail;
        }

        /* Would fail due to bad PMBR, but force GPT anyhow */
        if ((good_pgpt || good_agpt) && !good_pmbr && force_gpt) {
                fprintf(stderr, 
                       "  Warning: Disk has a valid GPT signature but "
                       "invalid PMBR.\n"
                       "  Use GNU Parted to correct disk.\n"
                       "  gpt option taken, disk treated as GPT.\n");
        }

        compare_gpts(pgpt, agpt, lastlba);

        /* The good cases */
        if (good_pgpt && (good_pmbr || force_gpt)) {
                *gpt  = pgpt;
                *ptes = pptes;
                if (agpt)  { free(agpt);   agpt = NULL; }
                if (aptes) { free(aptes); aptes = NULL; }
                if (!good_agpt) {
                        fprintf(stderr, 
			       "Alternate GPT is invalid, "
                               "using primary GPT.\n");
                }
                return 1;
        }
        else if (good_agpt && (good_pmbr || force_gpt)) {
                *gpt  = agpt;
                *ptes = aptes;
                if (pgpt)  { free(pgpt);   pgpt = NULL; }
                if (pptes) { free(pptes); pptes = NULL; }
                fprintf(stderr, 
                       "Primary GPT is invalid, using alternate GPT.\n");
                return 1;
        }

 fail:
        if (pgpt)  { free(pgpt);   pgpt=NULL; }
        if (agpt)  { free(agpt);   agpt=NULL; }
        if (pptes) { free(pptes); pptes=NULL; }
        if (aptes) { free(aptes); aptes=NULL; }
        *gpt = NULL;
        *ptes = NULL;
        return 0;
}
