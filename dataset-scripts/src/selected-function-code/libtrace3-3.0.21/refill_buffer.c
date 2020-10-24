static off_t refill_buffer(io_t *io, off_t len)
{
	off_t bytes_read;
	assert(DATA(io)->length - DATA(io)->offset == 0);
	/* Select the largest of "len", PEEK_SIZE and the current peek buffer size
	 * and then round up to the nearest multiple of MIN_READ_SIZE 
	 */
	bytes_read = len < PEEK_SIZE ? PEEK_SIZE : len;
	bytes_read = bytes_read < DATA(io)->length ? DATA(io)->length : bytes_read;
	bytes_read += MIN_READ_SIZE - (bytes_read % MIN_READ_SIZE);
	/* Is the current buffer big enough? */
	if (DATA(io)->length < bytes_read) {
		int res = 0;
		void *buf_ptr = (void *)(DATA(io)->buffer);

		if (buf_ptr)
			free(buf_ptr);
		DATA(io)->length = bytes_read;
		DATA(io)->offset = 0;
#if _POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600
		/* We need to do this as read() of O_DIRECT might happen into 
		 * this buffer.  The docs suggest 512 bytes is all we need to 
		 * align to, but I'm suspicious. I expect disks with 4k blocks 
		 * will arrive soon, and thus 4k is the minimum I'm willing to 
		 * live with.
		 */
		res = posix_memalign(&buf_ptr, 4096, DATA(io)->length);
		if (res != 0) {
			fprintf(stderr, "Error aligning IO buffer: %d\n",
					res);
			return res;
		}
		DATA(io)->buffer = buf_ptr;
#else
		res = 0;	/* << Silly warning */ 
		DATA(io)->buffer = malloc(DATA(io)->length);
#endif
	}
	else
		DATA(io)->length = bytes_read;

	assert(DATA(io)->buffer);

	/* Now actually attempt to read that many bytes */
	bytes_read = DATA(io)->child->source->read(	
			DATA(io)->child, DATA(io)->buffer, bytes_read);

	DATA(io)->offset = 0;
	DATA(io)->length = bytes_read;
	
	/* Error? */
	if (bytes_read < 1)
		return bytes_read;

	return bytes_read;
	
}