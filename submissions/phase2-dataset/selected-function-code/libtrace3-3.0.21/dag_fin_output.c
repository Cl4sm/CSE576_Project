static int dag_fin_output(libtrace_out_t *libtrace) {
	
	/* Commit any outstanding traffic in the txbuffer */
	if (FORMAT_DATA_OUT->waiting) {
		dag_tx_stream_commit_bytes(FORMAT_DATA_OUT->device->fd, FORMAT_DATA_OUT->dagstream,
				FORMAT_DATA_OUT->waiting );
	}

	/* Wait until the buffer is nearly clear before exiting the program, 
	 * as we will lose packets otherwise */
	dag_tx_get_stream_space(FORMAT_DATA_OUT->device->fd,
			FORMAT_DATA_OUT->dagstream,
			dag_get_stream_buffer_size(FORMAT_DATA_OUT->device->fd,
					FORMAT_DATA_OUT->dagstream) - 8
			);

	/* Need the lock, since we're going to be handling the device list */
	pthread_mutex_lock(&open_dag_mutex);

	/* Detach the stream if we are not paused */
	if (FORMAT_DATA_OUT->stream_attached)
		dag_pause_output(libtrace);
	FORMAT_DATA_OUT->device->ref_count --;

	/* Close the DAG device if there are no more references to it */
	if (FORMAT_DATA_OUT->device->ref_count == 0)
		dag_close_device(FORMAT_DATA_OUT->device);
	free(libtrace->format_data);
	pthread_mutex_unlock(&open_dag_mutex);
	return 0; /* success */
}