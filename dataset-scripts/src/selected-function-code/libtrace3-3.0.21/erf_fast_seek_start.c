static int erf_fast_seek_start(libtrace_t *libtrace,uint64_t erfts)
{
	size_t max_off = DATA(libtrace)->seek.index_len/sizeof(erf_index_t);
	size_t min_off = 0;
	off_t current;
	erf_index_t record;
	do {
		current=(max_off+min_off)>>2;

		wandio_seek(DATA(libtrace)->seek.index,
				(int64_t)(current*sizeof(record)),
				SEEK_SET);
		wandio_read(DATA(libtrace)->seek.index,
				&record,sizeof(record));
		if (record.timestamp < erfts) {
			min_off=current;
		}
		if (record.timestamp > erfts) {
			max_off=current;
		}
		if (record.timestamp == erfts)
			break;
	} while(min_off<max_off);

	/* If we've passed it, seek backwards.  This loop shouldn't
	 * execute more than twice.
	 */
	do {
		wandio_seek(DATA(libtrace)->seek.index,
				(int64_t)(current*sizeof(record)),SEEK_SET);
		wandio_read(DATA(libtrace)->seek.index,
				&record,sizeof(record));
		current--;
	} while(record.timestamp>erfts);

	/* We've found our location in the trace, now use it. */
	wandio_seek(libtrace->io,(int64_t) record.offset,SEEK_SET);

	return 0; /* success */
}