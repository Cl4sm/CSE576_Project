static int dag_dump_packet(libtrace_out_t *libtrace,
		dag_record_t *erfptr, unsigned int pad, void *buffer) {
	int size;

	/*
	 * If we've got 0 bytes waiting in the txqueue, assume that we haven't
	 * requested any space yet, and request some, storing the pointer at
	 * FORMAT_DATA_OUT->txbuffer.
	 *
	 * The amount to request is slightly magical at the moment - it's
	 * 16Mebibytes + 128 kibibytes to ensure that we can copy a packet into
	 * the buffer and handle overruns.
	 */
	if (FORMAT_DATA_OUT->waiting == 0) {
		FORMAT_DATA_OUT->txbuffer = dag_tx_get_stream_space(FORMAT_DATA_OUT->device->fd,
				FORMAT_DATA_OUT->dagstream, 16908288);
	}

	/*
	 * Copy the header separately to the body, as we can't guarantee they 
	 * are in contiguous memory
	 */
	memcpy(FORMAT_DATA_OUT->txbuffer + FORMAT_DATA_OUT->waiting,erfptr,(dag_record_size + pad));
	FORMAT_DATA_OUT->waiting += (dag_record_size + pad);



	/*
	 * Copy our incoming packet into the outgoing buffer, and increment 
	 * our waiting count
	 */
	size = ntohs(erfptr->rlen)-(dag_record_size + pad);
	memcpy(FORMAT_DATA_OUT->txbuffer + FORMAT_DATA_OUT->waiting,buffer,size);
	FORMAT_DATA_OUT->waiting += size;

	/*
	 * If our output buffer has more than 16 Mebibytes in it, commit those 
	 * bytes and reset the waiting count to 0.
	 * Note: dag_fin_output will also call dag_tx_stream_commit_bytes() in 
	 * case there is still data in the buffer at program exit.
	 */

	if (FORMAT_DATA_OUT->waiting >= 16*1024*1024) {
		FORMAT_DATA_OUT->txbuffer = dag_tx_stream_commit_bytes(FORMAT_DATA_OUT->device->fd, FORMAT_DATA_OUT->dagstream,
			FORMAT_DATA_OUT->waiting );
		FORMAT_DATA_OUT->waiting = 0;
	}

	return size + pad + dag_record_size;

}