static void convert_chunksize(struct params *fd)
{
	int i;


	for (i=0; i < MAX_SIZECODE; ++i) {
		if (fd->chunksize < (128 << i) + header_size + 1) {
			fprintf(stderr,"Bad chunksize %d\n", fd->chunksize);
			exit(1);
		}
		if (fd->chunksize <= (128 << i) + 256 + header_size) {
			fd->sizecode = i;
			fd->fmt_gap = fd->chunksize - (128 << i) - header_size;
			break;
		}
	}
	if (i == MAX_SIZECODE){
		fprintf(stderr,"Chunksize %d too big\n", fd->chunksize );
		exit(1);
	}
}
