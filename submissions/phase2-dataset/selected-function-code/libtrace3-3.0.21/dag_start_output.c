static int dag_start_output(libtrace_out_t *libtrace) {
	struct timeval zero, nopoll;

	zero.tv_sec = 0;
	zero.tv_usec = 0;
	nopoll = zero;

	/* Attach and start the DAG stream */

	if (dag_attach_stream(FORMAT_DATA_OUT->device->fd,
			FORMAT_DATA_OUT->dagstream, 0, 1048576) < 0) {
		trace_set_err_out(libtrace, errno, "Cannot attach DAG stream");
		return -1;
	}

	if (dag_start_stream(FORMAT_DATA_OUT->device->fd,
			FORMAT_DATA_OUT->dagstream) < 0) {
		trace_set_err_out(libtrace, errno, "Cannot start DAG stream");
		return -1;
	}
	FORMAT_DATA_OUT->stream_attached = 1;

	/* We don't want the dag card to do any sleeping */

	dag_set_stream_poll(FORMAT_DATA_OUT->device->fd,
			FORMAT_DATA_OUT->dagstream, 0, &zero,
			&nopoll);

	return 0;
}