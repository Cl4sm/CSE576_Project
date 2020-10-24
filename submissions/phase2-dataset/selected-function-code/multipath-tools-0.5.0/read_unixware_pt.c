int
read_unixware_pt(int fd, struct slice all, struct slice *sp, int ns) {
	struct unixware_disklabel *l;
	struct unixware_slice *p;
	unsigned int offset = all.start;
	char *bp;
	int n = 0;

	bp = getblock(fd, offset+29); 	/* 1 sector suffices */
	if (bp == NULL)
		return -1;

	l = (struct unixware_disklabel *) bp;
	if (four2int(l->d_magic) != UNIXWARE_DISKMAGIC ||
	    four2int(l->vtoc.v_magic) != UNIXWARE_DISKMAGIC2)
		return -1;

	p = &l->vtoc.v_slice[1];	/* slice 0 is the whole disk. */
	while (p - &l->vtoc.v_slice[0] < UNIXWARE_NUMSLICE) {
		if (p->s_label == UNIXWARE_FS_UNUSED)
			/* nothing */;
		else if (n < ns) {
			sp[n].start = p->start_sect;
			sp[n].size = p->nr_sects;
			n++;
		} else {
			fprintf(stderr,
				"unixware_partition: too many slices\n");
			break;
		}
		p++;
	}
	return n;
}
