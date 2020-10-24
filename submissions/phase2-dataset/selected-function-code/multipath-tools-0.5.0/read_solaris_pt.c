read_solaris_pt(int fd, struct slice all, struct slice *sp, int ns) {
	struct solaris_x86_vtoc *v;
	struct solaris_x86_slice *s;
	unsigned int offset = all.start;
	int i, n;
	char *bp;

	bp = getblock(fd, offset+1); 	/* 1 sector suffices */
	if (bp == NULL)
		return -1;

	v = (struct solaris_x86_vtoc *) bp;
	if(v->v_sanity != SOLARIS_X86_VTOC_SANE)
		return -1;

	if(v->v_version != 1) {
		fprintf(stderr, "Cannot handle solaris version %ld vtoc\n",
		       v->v_version);
		return 0;
	}

	for(i=0, n=0; i<SOLARIS_X86_NUMSLICE; i++) {
		s = &v->v_slice[i];

		if (s->s_size == 0)
			continue;
		if (n < ns) {
			sp[n].start = offset + s->s_start;
			sp[n].size = s->s_size;
			n++;
		} else {
			fprintf(stderr,
				"solaris_x86_partition: too many slices\n");
			break;
		}
	}
	return n;
}
