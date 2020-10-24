static int dag_start_input(libtrace_t *libtrace) {
        struct timeval zero, nopoll;
        uint8_t *top, *bottom;
	uint8_t diff = 0;
	top = bottom = NULL;

	zero.tv_sec = 0;
        zero.tv_usec = 10000;
        nopoll = zero;

	/* Attach and start the DAG stream */
	if (dag_attach_stream(FORMAT_DATA->device->fd,
				FORMAT_DATA->dagstream, 0, 0) < 0) {
                trace_set_err(libtrace, errno, "Cannot attach DAG stream");
                return -1;
        }

	if (dag_start_stream(FORMAT_DATA->device->fd,
				FORMAT_DATA->dagstream) < 0) {
                trace_set_err(libtrace, errno, "Cannot start DAG stream");
                return -1;
        }
	FORMAT_DATA->stream_attached = 1;
	
	/* We don't want the dag card to do any sleeping */
        dag_set_stream_poll(FORMAT_DATA->device->fd,
				FORMAT_DATA->dagstream, 0, &zero,
				&nopoll);

	/* Should probably flush the memory hole now */
	do {
		top = dag_advance_stream(FORMAT_DATA->device->fd,
					FORMAT_DATA->dagstream,
					&bottom);
		assert(top && bottom);
		diff = top - bottom;
		bottom -= diff;
	} while (diff != 0);
	FORMAT_DATA->top = NULL;
	FORMAT_DATA->bottom = NULL;
	FORMAT_DATA->processed = 0;
	FORMAT_DATA->drops = 0;

	return 0;
}