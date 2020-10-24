int
read_sun_pt(int fd, struct slice all, struct slice *sp, int ns) {
	struct sun_disk_label *l;
	struct sun_raw_part *s;
	unsigned int offset = all.start, end;
	int i, j, n;
	char *bp;

	bp = getblock(fd, offset);
	if (bp == NULL)
		return -1;

	l = (struct sun_disk_label *) bp;
	if(be16_to_cpu(l->magic) != SUN_DISK_MAGIC)
		return -1;

	if (!sun_verify_checksum(l)) {
		fprintf(stderr, "Corrupted Sun disk label\n");
		return -1;
	}

	for(i=0, n=0; i<SUN_DISK_MAXPARTITIONS; i++) {
		s = &l->partitions[i];

		if (s->num_sectors == 0)
			continue;
		if (n < ns) {
			sp[n].start = offset +
				be32_to_cpu(s->start_cylinder) * be16_to_cpu(l->nsect) * be16_to_cpu(l->ntrks);
			sp[n].size = be32_to_cpu(s->num_sectors);
			n++;
		} else {
			fprintf(stderr,
				"sun_disklabel: too many slices\n");
			break;
		}
	}
	/*
	 * Convention has it that the SUN disklabel will always have
	 * the 'c' partition spanning the entire disk.
	 * So we have to check for contained slices.
	 */
	for(i = 0; i < SUN_DISK_MAXPARTITIONS; i++) {
		if (sp[i].size == 0)
			continue;

		end = sp[i].start + sp[i].size;
		for(j = 0; j < SUN_DISK_MAXPARTITIONS; j ++) {
			if ( i == j )
				continue;
			if (sp[j].size == 0)
				continue;

			if (sp[i].start < sp[j].start) {
				if (end > sp[j].start &&
				    end < sp[j].start + sp[j].size) {
					/* Invalid slice */
					fprintf(stderr,
						"sun_disklabel: slice %d overlaps with %d\n", i , j);
					sp[i].size = 0;
				}
			} else {
				if (end <= sp[j].start + sp[j].size) {
					sp[i].container = j + 1;
				}
			}
		}
	}
	return n;
}
