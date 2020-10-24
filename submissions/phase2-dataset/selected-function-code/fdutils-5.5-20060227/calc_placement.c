{
	int cur_sector, i, max_offset;
	int track_end=0;
	int final_slack; /* slack space extending from data start of last
			  * sector on the track to fd->raw_capacity mark */

	/* now compute the placement in terms of small sectors */
	cur_sector = 0;
	for(i=0; i< fd->dsect; i++){
		fd->sequence[i].offset = cur_sector;
		max_offset = cur_sector;

		/* offset of the starting sector */
		if ( fd->sequence[i].sect == 1 )
			fd->min = cur_sector * fd->chunksize;

		/* offset of the end of the of the highest sector */
		if (fd->sequence[i].sect == fd->dsect)
			track_end = cur_sector * fd->chunksize + 
				header_size + index_size +
				SSIZE(fd->sequence[i].size);

		if(i == fd->dsect - 1)
			break;

		cur_sector += chunks_in_sect(fd, 
					     fd->sequence[i].size, 
					     gap, fd->chunksize);
	}
	final_slack = fd->raw_capacity - cur_sector * fd->chunksize -
		header_size - index_size - 1;
	if(final_slack < 0) {
		fprintf(stderr,
			"Internal error, negative final slack %d\n",
			final_slack);
		abort();
	}
	fd->max = fd->min + final_slack;

	fd->length = fd->rotations * fd->raw_capacity + track_end - fd->min;
	if(fd->length < 0) {
		fprintf(stderr,
			"Internal error, negative track length %d %d %d\n",
			fd->length, track_end, fd->min);
		abort();
		exit(1);
	}


	/* this format accepts any offsets ranging from fd->min to fd->max.
	 * After this track, the current offset will be:
	 *  fd->track_end + initial_offset - fd->min
	 */
}
