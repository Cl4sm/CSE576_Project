read_bsd_pt(int fd, struct slice all, struct slice *sp, int ns) {
	struct bsd_disklabel *l;
	struct bsd_partition *p;
	unsigned int offset = all.start, end;
	int max_partitions;
	char *bp;
	int n = 0, i, j;

	bp = getblock(fd, offset+1);    /* 1 sector suffices */
	if (bp == NULL)
		return -1;

	l = (struct bsd_disklabel *) bp;
	if (l->d_magic != BSD_DISKMAGIC)
		return -1;

	max_partitions = 16;
	if (l->d_npartitions < max_partitions)
		max_partitions = l->d_npartitions;
	for (p = l->d_partitions; p - l->d_partitions <  max_partitions; p++) {
		if (p->p_fstype == BSD_FS_UNUSED)
			/* nothing */;
		else if (n < ns) {
			sp[n].start = p->p_offset;
			sp[n].size = p->p_size;
			n++;
		} else {
			fprintf(stderr,
				"bsd_partition: too many slices\n");
			break;
		}
	}
	/*
	 * Convention has it that the bsd disklabel will always have
	 * the 'c' partition spanning the entire disk.
	 * So we have to check for contained slices.
	 */
	for(i = 0; i < n; i++) {
		if (sp[i].size == 0)
			continue;

		end = sp[i].start + sp[i].size;
		for(j = 0; j < n; j ++) {
			if ( i == j )
				continue;
			if (sp[j].size == 0)
				continue;

			if (sp[i].start < sp[j].start) {
				if (end > sp[j].start &&
				    end < sp[j].start + sp[j].size) {
					/* Invalid slice */
					fprintf(stderr,
						"bsd_disklabel: slice %d overlaps with %d\n", i , j);
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
