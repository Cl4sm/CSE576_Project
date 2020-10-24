static void compute_all_sequences_for_size(struct params *fd,
					   int *offset,
					   int tracksize,
					   int sizecode,
					   int gap,
					   int mask,
					   int tailsize)
{
	int base = *offset;
	int i;

	/* no sectors of this size */
	if(!nrSectorsForSize(fd, tailsize))
		return;

	fd[*offset] = fd[0];
	if(compute_chunks_per_sect(fd + *offset, tracksize, sizecode,
				   &gap, /* gap. expressed in 1/256 bytes */
				   mask, tailsize) < 0) {
		/* not enough raw space for this arrangement */
		return;
	}

	for(i = firstSector(fd, tailsize);
	    i < lastSector(fd, tailsize);
	    i++) {
		fd[*offset] = fd[base];
		compute_sector_sequence(fd+*offset, i, gap);
		(*offset)++;
	}
}
