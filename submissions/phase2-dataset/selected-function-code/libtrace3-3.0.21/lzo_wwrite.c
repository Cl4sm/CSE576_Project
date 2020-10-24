static off_t lzo_wwrite(iow_t *iow, const char *buffer, off_t len)
{
	off_t ret = 0;
	while (len>0) {
		off_t size = len;
		off_t err;
		struct buffer_t outbuf;

		if (!DATA(iow)->threads) {
			size = min(len, MAX_BLOCK_SIZE);
			err=lzo_wwrite_block(buffer, size, &outbuf);
			/* Flush the data out */
			wandio_wwrite(DATA(iow)->child,
					outbuf.buffer,
					outbuf.offset);

			if (err < 0) {/* Error */
				if (ret == 0)
					return err;
				/* If we've written some data, return that fact now, let them call back
				 * and try and write more data, fail again then. 
				 */
				return ret;
			}
			else {
				assert(err == size);
				buffer += size;
				len -= size;
			}
		}
		else {
			off_t space;

			pthread_mutex_lock(&get_next_thread(iow)->mutex);
			/* If this thread is still compressing, wait for it to finish */
			while (get_next_thread(iow)->state == WAITING) {
				pthread_cond_wait(
					&get_next_thread(iow)->out_ready, 
					&get_next_thread(iow)->mutex);
			}

			/* Flush any data out thats there */
			if (get_next_thread(iow)->state == FULL) {
				assert(get_next_thread(iow)->outbuf.offset 
						< sizeof(get_next_thread(iow)->outbuf.buffer));
				wandio_wwrite(DATA(iow)->child,
						get_next_thread(iow)->outbuf.buffer,
						get_next_thread(iow)->outbuf.offset);
				get_next_thread(iow)->state = EMPTY;
				get_next_thread(iow)->inbuf.offset = 0;
			}

			assert(get_next_thread(iow)->state == EMPTY);

			/* Figure out how much space we can copy into this buffer */
			assert(MAX_BLOCK_SIZE <= sizeof(get_next_thread(iow)->inbuf.buffer));
			space = MAX_BLOCK_SIZE-get_next_thread(iow)->inbuf.offset;
			size = min(space, size);
			assert(size>0);
			assert(size <= MAX_BLOCK_SIZE);
			assert(get_next_thread(iow)->inbuf.offset + size <= MAX_BLOCK_SIZE);

			/* Move our data in */
			memcpy(&get_next_thread(iow)->inbuf.buffer[get_next_thread(iow)->inbuf.offset], 
				buffer, 
				size);
			get_next_thread(iow)->inbuf.offset += size;

			/* If the buffer is now full Trigger the thread to start compressing this block,
			 * and move onto the next block.
			 */
			if (get_next_thread(iow)->inbuf.offset >= sizeof(get_next_thread(iow)->inbuf.buffer)
			  ||get_next_thread(iow)->inbuf.offset >= MAX_BLOCK_SIZE) {
			  	assert(get_next_thread(iow)->state == EMPTY);
				get_next_thread(iow)->state = WAITING;
				pthread_cond_signal(&get_next_thread(iow)->in_ready);

				pthread_mutex_unlock(&get_next_thread(iow)->mutex);

				DATA(iow)->next_thread = 
						(DATA(iow)->next_thread+1) % DATA(iow)->threads;
			}
			else 
				pthread_mutex_unlock(&get_next_thread(iow)->mutex);

			/* Update the lengths */
			buffer += size;
			len -= size;
		}
	}
	return len;
}