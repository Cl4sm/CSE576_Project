{	
	int cylinder, head;
	struct params *f = NULL;
	int cur_skew, next_cylinder_skew;
	int ind, rots;

	/* Amount to advance skew considering head skew already added in */
	next_cylinder_skew = cylinder_skew - head_skew;
	cur_skew = absolute_skew;

	rots = 0;
	for (cylinder=begin_cylinder; cylinder <= end_cylinder; ++cylinder) {
		for (head=0; head < heads; ++head) {
			if (!head && !cylinder && use_2m)
				f = fd0;
			else
				f = fd;
			cur_skew = cur_skew % f->raw_capacity;			
			pick_best(f, n, &cur_skew, 
				  &lskews[cylinder][head], 
				  &findex[cylinder][head]);

			rots += cur_skew / f->raw_capacity;
			ind = findex[cylinder][head];
			if(lskews[cylinder][head] * fd[ind].chunksize >
			   fd->raw_capacity){
				fprintf(stderr,"Skew too big\n");
				abort();
			}
			cur_skew += head_skew;
		}
		cur_skew += next_cylinder_skew;
	}
	return 0;
} /* End calc_skews */
