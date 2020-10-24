static off_t peek_peek(io_t *io, void *buffer, off_t len)
{
	off_t ret = 0;
	int res = 0;

	/* Is there enough data in the buffer to serve this request? */
	if (DATA(io)->length - DATA(io)->offset < len) {
		/* No, we need to extend the buffer. */
		off_t read_amount = len - (DATA(io)->length - DATA(io)->offset);
		/* Round the read_amount up to the nearest MB */
		read_amount += PEEK_SIZE - ((DATA(io)->length + read_amount) % PEEK_SIZE);
		DATA(io)->buffer = alignedrealloc(DATA(io)->buffer, 
			DATA(io)->length, 
			DATA(io)->length + read_amount, &res);

		if (DATA(io)->buffer == NULL) {
			return res;	
		}

		/* Use the child reader to read more data into our managed
		 * buffer */
		read_amount = wandio_read(DATA(io)->child, 
			DATA(io)->buffer + DATA(io)->length,
			read_amount);

		/* Pass errors up */
		if (read_amount <0) {
			return read_amount;
		}

		DATA(io)->length += read_amount;
	}

	/* Right, now return data from the buffer (that now should be large 
	 * enough, but might not be if we hit EOF) */
	ret = MIN(len, DATA(io)->length - DATA(io)->offset);
	memcpy(buffer, DATA(io)->buffer + DATA(io)->offset, ret);
	return ret;
}