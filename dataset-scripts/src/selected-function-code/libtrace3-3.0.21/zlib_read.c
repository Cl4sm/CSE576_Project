static off_t zlib_read(io_t *io, void *buffer, off_t len)
{
	if (DATA(io)->err == ERR_EOF)
		return 0; /* EOF */
	if (DATA(io)->err == ERR_ERROR) {
		errno=EIO;
		return -1; /* ERROR! */
	}

	DATA(io)->strm.avail_out = len;
	DATA(io)->strm.next_out = (Bytef*)buffer;

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
				/* Return how much data we've managed to read so far. */
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
		int err=inflate(&DATA(io)->strm, 0);
		switch(err) {
			case Z_OK:
				DATA(io)->err = ERR_OK;
				break;
			case Z_STREAM_END:
				/* You would think that an "EOF" on the stream would mean we'd
 				 * want to pass on an EOF?  Nope.  Some tools (*cough* corel *cough*)
 				 * annoyingly close and reopen the gzip stream leaving Z_STREAM_END
 				 * mines for us to find.
 				 */
				inflateEnd(&DATA(io)->strm);
				inflateInit2(&DATA(io)->strm, 15 | 32);
				DATA(io)->err = ERR_OK;
				break;
			default:
				errno=EIO;
				DATA(io)->err = ERR_ERROR;
		}
	}
	/* Return the number of bytes decompressed */
	return len-DATA(io)->strm.avail_out;
}