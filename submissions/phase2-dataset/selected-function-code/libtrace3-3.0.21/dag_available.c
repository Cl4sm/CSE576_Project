static int dag_available(libtrace_t *libtrace) {
	uint32_t diff = FORMAT_DATA->top - FORMAT_DATA->bottom;

	/* If we've processed more than 4MB of data since we last called
	 * dag_advance_stream, then we should call it again to allow the
	 * space occupied by that 4MB to be released */
	if (diff >= dag_record_size && FORMAT_DATA->processed < 4 * 1024 * 1024)
		return diff;
	
	/* Update the top and bottom pointers */
	FORMAT_DATA->top = dag_advance_stream(FORMAT_DATA->device->fd,
			FORMAT_DATA->dagstream,
			&(FORMAT_DATA->bottom));
	
	if (FORMAT_DATA->top == NULL) {
		trace_set_err(libtrace, errno, "dag_advance_stream failed!");
		return -1;
	}
	FORMAT_DATA->processed = 0;
	diff = FORMAT_DATA->top - FORMAT_DATA->bottom;
	return diff;
}