static off_t lzma_read(io_t *io, void *buffer, off_t len)
{
	if (DATA(io)->err == ERR_EOF)
		return 0; /* EOF */
	if (DATA(io)->err == ERR_ERROR) {
		errno=EIO;
		return -1; /* ERROR! */
	}

	DATA(io)->strm.avail_out = len;
	DATA(io)->strm.next_out = buffer;

	while (DATA(io)->err == ERR_OK && DATA(io)->strm.avail_out > 0) {
		while (DATA(io)->strm.avail_in <= 0) {
			int bytes_read = wandio_read(DATA(io)->parent,
				(char*)DATA(io)->inbuff,
				sizeof(DATA(io)->inbuff));
			if (bytes_read == 0) {
				/* EOF */
				if (DATA(io)->strm.avail_out == (uint32_t)len) {
					DATA(io)->err = ERR_EOF;
					return 0;
				}
                                /* Return how much data we've managed to read
                                 * so far. */
				return len-DATA(io)->strm.avail_out;
			}
			if (bytes_read < 0) { /* Error */
				/* errno should be set */
				DATA(io)->err = ERR_ERROR;
				/* Return how much data we managed to read ok */
				if (DATA(io)->strm.avail_out != (uint32_t)len) {
					return len-DATA(io)->strm.avail_out;
				}
				/* Now return error */
				return -1;
			}
			DATA(io)->strm.next_in = DATA(io)->inbuff;
			DATA(io)->strm.avail_in = bytes_read;
		}
		/* Decompress some data into the output buffer */
		lzma_ret err=lzma_code(&DATA(io)->strm, LZMA_RUN);
		switch(err) {
			case LZMA_OK:
				DATA(io)->err = ERR_OK;
				break;
			case LZMA_STREAM_END:
				DATA(io)->err = ERR_EOF;
				break;
			default:
				errno=EIO;
				DATA(io)->err = ERR_ERROR;
		}
	}
	/* Return the number of bytes decompressed */
	return len-DATA(io)->strm.avail_out;
}