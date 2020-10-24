static off_t peek_read(io_t *io, void *buffer, off_t len)
{
	off_t ret = 0;

	/* Is some of this data in the buffer? */
	if (DATA(io)->buffer) {
		ret = MIN(len,DATA(io)->length - DATA(io)->offset);

		/* Copy anything we've got into their buffer, and shift our
		 * offset so that we don't peek at the data we've read again */
		memcpy(buffer, 
			DATA(io)->buffer + DATA(io)->offset,
			ret);
		buffer += ret;
		DATA(io)->offset += ret;
		len -= ret;
	}

	/* Use the child reader to get the rest of the required data */
	if (len>0) {
		/* To get here, the buffer must be empty */
		assert(DATA(io)->length-DATA(io)->offset == 0);
		off_t bytes_read;
		/* If they're reading exactly a block size, just use that, no point in malloc'ing 
		 * and memcpy()ing needlessly.  However, if the buffer isn't aligned, we need to
		 * pass on an aligning buffer, skip this and do it into our own aligned buffer.
		 */
		if ((len % MIN_READ_SIZE  == 0) && ((ptrdiff_t)buffer % 4096)==0) {
			assert(((ptrdiff_t)buffer % 4096) == 0);
			bytes_read = DATA(io)->child->source->read(
					DATA(io)->child, buffer, len);
			/* Error? */
			if (bytes_read < 1) {
				/* Return if we have managed to get some data ok */
				if (ret > 0)
					return ret;
				/* Return the error upstream */
				return bytes_read;
			}
		}
		else {
			bytes_read = refill_buffer(io, len);
			if (bytes_read < 1) {
				/* Return if we have managed to get some data ok */
				if (ret > 0)
					return ret;
				/* Return the error upstream */
				return bytes_read;
			}
			/* Now grab the number of bytes asked for. */
			len = len < bytes_read ? len : bytes_read;
			memcpy(buffer, DATA(io)->buffer, len);

			DATA(io)->offset = len;
			bytes_read = len;
		}
		ret += bytes_read;
	}

	/* Have we read past the end of the buffer? */
	if (DATA(io)->buffer && DATA(io)->offset >= DATA(io)->length) {
		/* If so, free the memory it used */
		free(DATA(io)->buffer);
		DATA(io)->buffer = NULL;
		DATA(io)->offset = 0;
		DATA(io)->length = 0;
	}

	return ret;
}