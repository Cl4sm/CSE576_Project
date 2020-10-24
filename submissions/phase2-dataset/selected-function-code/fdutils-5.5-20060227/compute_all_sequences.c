int compute_all_sequences(struct params *fd, 
			  int tracksize,
			  int sizecode,
			  int gap,
			  int mask, 
			  int biggest_last)
{	
	int offset, i;
	
	compute_sizes(fd, sectors*512,sizecode);

	offset = 0;
	for(i=MAX_SIZECODE - 1 ; i >= 0; i--) {
		compute_all_sequences_for_size(fd, &offset, tracksize, 
					       sizecode, gap, mask, i);
		if(biggest_last && offset)
			break;
	}
	
	if(! offset){
		fprintf(stderr,
			"Not enough raw space on this disk for this format\n");
		exit(1);
	}
	return offset;
}
