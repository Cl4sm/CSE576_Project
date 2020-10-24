static int erf_seek_erf(libtrace_t *libtrace,uint64_t erfts)
{
	libtrace_packet_t *packet;
	off_t off = 0;

	if (DATA(libtrace)->seek.exists==INDEX_UNKNOWN) {
		char buffer[PATH_MAX];
		snprintf(buffer,sizeof(buffer),"%s.idx",libtrace->uridata);
		DATA(libtrace)->seek.index=wandio_create(buffer);
		if (DATA(libtrace)->seek.index) {
			DATA(libtrace)->seek.exists=INDEX_EXISTS;
		}
		else {
			DATA(libtrace)->seek.exists=INDEX_NONE;
		}
	}

	/* If theres an index, use it to find the nearest packet that isn't
	 * after the time we're looking for.  If there is no index we need
	 * to seek slowly through the trace from the beginning.  Sigh.
	 */
	switch(DATA(libtrace)->seek.exists) {
		case INDEX_EXISTS:
			erf_fast_seek_start(libtrace,erfts);
			break;
		case INDEX_NONE:
			erf_slow_seek_start(libtrace,erfts);
			break;
		case INDEX_UNKNOWN:
			assert(0);
			break;
	}

	/* Now seek forward looking for the correct timestamp */
	packet=trace_create_packet();
	do {
		trace_read_packet(libtrace,packet);
		if (trace_get_erf_timestamp(packet)==erfts)
			break;
		off=wandio_tell(libtrace->io);
	} while(trace_get_erf_timestamp(packet)<erfts);

	wandio_seek(libtrace->io,off,SEEK_SET);

	return 0;
}