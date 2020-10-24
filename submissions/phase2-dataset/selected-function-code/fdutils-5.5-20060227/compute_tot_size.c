static int compute_tot_size(struct params *fd,
			    int chunksize,
			    int gap,
			    int tailsize)
{
	int i, nr;

	fd->nssect = 0;
	fd->actual_interleave = 1;
	for(i= 0; i < MAX_SIZECODE; i++){
		nr = nrSectorsForSize(fd, i);
		fd->nssect += chunks_in_sect(fd, i, gap, chunksize) * nr;
		if(nr && GAPSIZE(i) < 34)
			fd->actual_interleave = 2;
	}

	if (tailsize >= 0)
		return (fd->nssect - 
			chunks_in_sect(fd, tailsize, gap, chunksize)) *
			chunksize + SSIZE(tailsize);
	else
		return fd->nssect * chunksize;
}
