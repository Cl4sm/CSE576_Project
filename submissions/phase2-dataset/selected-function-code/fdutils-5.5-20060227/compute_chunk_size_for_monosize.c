static int compute_chunk_size_for_monosize(struct params *fd,
					   int gap,
					   int tailsize)
{
	int min_chunksize; /* minimal chunk size reached so far */
	int t_chunksize; /* tentative chunk size */
	int ceiling; /* maximal divisor */
	int t_sect_size; /* tentative sector size */
	int min_sect_size=0; /* actual sector size */
	int sector_size;
	int chunks_per_sect;
	int i;

	sector_size = SSIZE(sizeOfSector(fd,1));
	min_chunksize = 0;
	ceiling = sector_size /( 129 + header_size);
	for(i= 1; i <= ceiling; i++ ){
		t_chunksize = (sector_size - 1)/i + 1;
		
		/* unreachable chunk sizes */
		if (((t_chunksize-header_size-1) & 511) > 255 &&
		    t_chunksize > 768 + header_size)
			continue;
		
		chunks_per_sect = (sector_size - 1)/t_chunksize + 1;
		t_sect_size = chunks_per_sect * t_chunksize;

		/* find the smallest sector size */
		if (!min_chunksize || t_sect_size < min_sect_size ){
			min_sect_size = t_sect_size;
			min_chunksize = t_chunksize;
		}
		if (t_sect_size == sector_size)
			break;
	}
	if(min_chunksize != sector_size)
		fd->need_init = 1;
	return  min_chunksize;
}
