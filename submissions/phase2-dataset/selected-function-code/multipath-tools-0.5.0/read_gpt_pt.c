int
read_gpt_pt (int fd, struct slice all, struct slice *sp, int ns)
{
	gpt_header *gpt = NULL;
	gpt_entry *ptes = NULL;
	uint32_t i;
	int n = 0;
        int last_used_index=-1;
	int sector_size_mul = get_sector_size(fd)/512;

	if (!find_valid_gpt (fd, &gpt, &ptes) || !gpt || !ptes) {
		if (gpt)
			free (gpt);
		if (ptes)
			free (ptes);
		return 0;
	}

	for (i = 0; i < __le32_to_cpu(gpt->num_partition_entries) && i < ns; i++) {
		if (!efi_guidcmp (NULL_GUID, ptes[i].partition_type_guid)) {
			sp[n].start = 0;
			sp[n].size = 0;
			n++;
		} else {
			sp[n].start = sector_size_mul *
				      __le64_to_cpu(ptes[i].starting_lba);
			sp[n].size  = sector_size_mul *
				      (__le64_to_cpu(ptes[i].ending_lba) -
				       __le64_to_cpu(ptes[i].starting_lba) + 1);
                        last_used_index=n;
			n++;
		}
	}
	free (ptes);
	free (gpt);
	return last_used_index+1;
}
