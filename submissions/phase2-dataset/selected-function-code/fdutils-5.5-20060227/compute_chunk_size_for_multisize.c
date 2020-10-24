					    int gap,
					    int tailsize)
{
	int t_chunksize;
	int tot_size;
	int min_chunksize;
	int i;
	int min_tot_size = 0;

	min_chunksize = 0;
	fd->need_init = 1;
	for(t_chunksize = fd->max_chunksize; 
	    t_chunksize > 128+header_size; 
	    t_chunksize--){
		for(i=0; i < MAX_SIZECODE; i++ ){
			if(t_chunksize<(128<<i)+header_size+1){
				t_chunksize=(128<<(i-1)) +
					256 + header_size;
				break;
			}
			if (t_chunksize <= (128 << i) + 256 + header_size)
				break;
		}
		tot_size = compute_tot_size(fd, t_chunksize, gap, tailsize);
		if ( !min_chunksize || tot_size <= min_tot_size ){
#if 0
			if (verbosity >= 6)
				printf("%d chasing %d\n",
				       t_chunksize, min_chunksize);
#endif
			min_tot_size = tot_size;
			min_chunksize = t_chunksize;
		}
	}
	return min_chunksize;
}
