static void compute_sizes(struct params *fd, 
			  int remaining, /* bytes per track */
			  int max_sizecode) /* size of biggest sector used */
{
	int cur_sector;
	int sizes; /* number of different sizes found along the track */
	int i;
	int nr_sectors;

	cur_sector = 1;
	sizes=0;
	for (i=MAX_SIZECODE-1; i>=0; --i) {
		if(i > max_sizecode)
			nr_sectors = 0;
		else {			
			nr_sectors = remaining >> (i+7);
			remaining -= nr_sectors << (i+7);
		}
		cur_sector += nr_sectors;
		fd->last_sect[i] = cur_sector;
		if(nr_sectors)
			sizes++;
	}
	fd->dsect = cur_sector-1; /* number of data sectors */
	if(sizes > 1)
		fd->need_init = 1;

	if (remaining) {
		fprintf(stderr,"Internal error: remaining not 0\n");
		abort();
	}
}
