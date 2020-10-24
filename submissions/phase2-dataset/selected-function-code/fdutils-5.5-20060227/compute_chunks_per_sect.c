				    int tracksize,
				    int sizecode,
				    int *gap,
				    int mask,
				    int tailsize)
{
	int tot_size;

	if (! (mask & SET_FMTGAP))
		*gap = compute_gap(fd, tracksize);	
	while(1) {
		compute_chunk_size(fd, *gap, tailsize);
		tot_size=compute_tot_size(fd, fd->chunksize, *gap, tailsize);
		if(fd->raw_capacity >= tot_size)
			/* enough space available, ok */
			break;
		if ((mask & SET_FMTGAP) || *gap <= 0)
			/* does not fit on disk */
			return -1;

		*gap -= (tot_size-fd->raw_capacity) * GAP_DIVISOR / tracksize;
		if (*gap < 0)
			*gap = 0;
	}

	convert_chunksize(fd);
	
	if (mask & SET_INTERLEAVE)
		fd->actual_interleave = fd->preset_interleave;

	if(verbosity >= 9) {
		printf("%d raw bytes per cylinder\n", tot_size );
		printf("%d final gap\n",
			fd->raw_capacity - tot_size );
	}
	return 0;
}
