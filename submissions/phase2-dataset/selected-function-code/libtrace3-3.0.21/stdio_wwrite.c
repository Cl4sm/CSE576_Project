static off_t stdio_wwrite(iow_t *iow, const char *buffer, off_t len)
{
	int towrite = len;
	/* Round down size to the nearest multiple of MIN_WRITE_SIZE */

	assert(towrite >= 0);

	while (DATA(iow)->offset + towrite >= MIN_WRITE_SIZE) {
		int err;
		struct iovec iov[2];
		int total = (DATA(iow)->offset+towrite);
		int amount;
		int count=0;
		/* Round down to the nearest multiple */
		total = total - (total % MIN_WRITE_SIZE);
		amount = total;
		if (DATA(iow)->offset) {
			iov[count].iov_base = DATA(iow)->buffer;
			iov[count].iov_len = min(DATA(iow)->offset,amount);
			amount -= iov[count].iov_len;
			++count;
		}
		/* How much to write from this buffer? */
		if (towrite) {
			iov[count].iov_base = (void*)buffer; 	/* cast away constness, which is safe 
								 * here 
								 */
			iov[count].iov_len = amount;
			amount -= iov[count].iov_len;
			++count;
		}
		assert(amount == 0);
		err=writev(DATA(iow)->fd, iov, count);
		if (err==-1)
			return -1;

		/* Drop off "err" bytes from the beginning of the buffers */
		amount = min(DATA(iow)->offset, err); /* How much we took out of the buffer */
		memmove(DATA(iow)->buffer, 
			DATA(iow)->buffer+amount,
			DATA(iow)->offset-amount);
		DATA(iow)->offset -= amount;

		err -= amount; /* How much was written */

		assert(err <= towrite);

		buffer += err;
		towrite -= err;

		assert(DATA(iow)->offset == 0);
	}

	/* Make sure we're not going to overflow the buffer.  The above writev should assure
 	 * that this is true
 	 */
	assert(DATA(iow)->offset + towrite <= MIN_WRITE_SIZE);
	assert(towrite >= 0);

	if (towrite > 0) {
		/* Copy the remainder into the buffer to write next time. */
		memcpy(DATA(iow)->buffer + DATA(iow)->offset, buffer, towrite);
		DATA(iow)->offset += towrite;
	}

	return len;
}