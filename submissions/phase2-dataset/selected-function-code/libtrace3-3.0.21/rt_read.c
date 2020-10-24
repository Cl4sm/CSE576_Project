static int rt_read(libtrace_t *libtrace, void **buffer, size_t len, int block) 
{
        int numbytes;
	
	assert(len <= RT_BUF_SIZE);
	
	if (!RT_INFO->pkt_buffer) {
		RT_INFO->pkt_buffer = (char*)malloc((size_t)RT_BUF_SIZE);
		RT_INFO->buf_current = RT_INFO->pkt_buffer;
		RT_INFO->buf_filled = 0;
	}

#ifndef MSG_DONTWAIT
#define MSG_DONTWAIT 0
#endif

	if (block)
		block=0;
	else
		block=MSG_DONTWAIT;

	/* If we don't have enough buffer space for the amount we want to
	 * read, move the current buffer contents to the front of the buffer
	 * to make room */
	if (len > RT_INFO->buf_filled) {
		memcpy(RT_INFO->pkt_buffer, RT_INFO->buf_current, 
				RT_INFO->buf_filled);
		RT_INFO->buf_current = RT_INFO->pkt_buffer;
#ifndef MSG_NOSIGNAL
#  define MSG_NOSIGNAL 0
#endif
		/* Loop as long as we don't have all the data that we were
		 * asked for */
		while (len > RT_INFO->buf_filled) {
                	if ((numbytes = recv(RT_INFO->input_fd,
                                                RT_INFO->buf_current + 
						RT_INFO->buf_filled,
                                                RT_BUF_SIZE-RT_INFO->buf_filled,
                                                MSG_NOSIGNAL|block)) <= 0) {
				if (numbytes == 0) {
					trace_set_err(libtrace, TRACE_ERR_RT_FAILURE, 
							"No data received");
					return -1;
				}
				
                	        if (errno == EINTR) {
                	                /* ignore EINTR in case
                	                 * a caller is using signals
					 */
                	                continue;
                	        }
				if (errno == EAGAIN) {
					/* We asked for non-blocking mode, so
					 * we need to return now */
					trace_set_err(libtrace,
							EAGAIN,
							"EAGAIN");
					return -1;
				}
				
                        	perror("recv");
				trace_set_err(libtrace, errno,
						"Failed to read data into rt recv buffer");
                        	return -1;
                	}
			RT_INFO->buf_filled+=numbytes;
		}

        }
	*buffer = RT_INFO->buf_current;
	RT_INFO->buf_current += len;
	RT_INFO->buf_filled -= len;
        return len;
}