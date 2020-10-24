void build_fd_lists(readfds, writefds,
		    curr_readable, curr_writeable,
		    sock_fd, maxfd,
		    ibuf_len, ibuf_size,
		    obuf_len, obuf_size,
		    obuf2_len, obuf2_off)
     fd_set	*readfds, *writefds;
     struct pipeline	*curr_readable, *curr_writeable;
     int	sock_fd, *maxfd;
     int	ibuf_len, ibuf_size;
     int	obuf_len, obuf_size;
     int	obuf2_len, obuf2_off;
{
	FD_ZERO(readfds);
	FD_ZERO(writefds);
	*maxfd = -1;
#define    MAX_FD(i)	if (*maxfd < (i)) *maxfd = (i)

	/* when can we write to a child? */
	if (ibuf_len>0 &&
	    curr_writeable->pipe[0]>=0 ) {
	    /* curr_writeable != 0 */
	    FD_SET(curr_writeable->pipe[0], writefds);
	    MAX_FD( curr_writeable->pipe[0] );
	}

	/* when can we read from the socket? */
	if ( curr_writeable == 0 
	     ||
	     (ibuf_len<ibuf_size &&
	      curr_writeable &&
	      curr_writeable->bytes_left > 0 )
	    ) {
#if 1
	    FD_SET(sock_fd, readfds);
	    MAX_FD(sock_fd);
#else
	    struct pipeline	*curr;
	    for (curr = pipe_head; curr; curr = curr ? curr->next : 0) {
		switch(curr->code) {
		case DUPLEX_IO:
		case DUPLEX_OI:
		case DUPLEX:	/* DUPLEX is not really possible */
		case WRITEABLE:
		    FD_SET(sock_fd, readfds);
		    MAX_FD(sock_fd);
		    curr = 0;	/* bail from for loop */
		    break;
		case READABLE:
		    /* we don't write to this one */
		    break;
		}
	    }
#endif
	}

	/* when can we read from a child? */
	if ( curr_readable == 0 ) {
	    /* select from ANY incoming child descriptor */
	    struct pipeline	*curr;
	    for (curr = pipe_head; curr; curr = curr->next) {
		if (curr->pipe[0]<0) continue;
		if ( curr->code == DUPLEX_IO &&
		     ! curr->syn_received)
		    continue;
		if ( curr->specact == CLOSE_TO_WRITE ) continue;
		switch(curr->code) {
		case READABLE:
		case DUPLEX_IO:
		case DUPLEX_OI:
		case DUPLEX:	/* DUPLEX is not really possible */
		    FD_SET(curr->pipe[0], readfds);
		    MAX_FD(curr->pipe[0]);
		    break;
		case WRITEABLE:
		    /* we don't read from this one */
		    break;
		}
	    }
	} else if (obuf_len < obuf_size &&
		   /* yes, the curr_readable could be WRITEABLE-only
		      when the child closes its descriptor, but we
		      haven't copied the bytes into the outgoing
		      socket buffer. In that case we don't want to
		      read from the descriptor. curr_readable will be
		      zeroed when the bytes get copied into the
		      outgoing socket buffer */
		   curr_readable->code != WRITEABLE &&
		   !( curr_readable->specact == CLOSE_TO_WRITE
		      || curr_readable->specact == CLOSE_TO_RW)
		   ) {
	    FD_SET(curr_readable->pipe[0], readfds);
	    MAX_FD(curr_readable->pipe[0]);
	}

	/* when can we write to the socket? */
	if (obuf2_len>obuf2_off) {
	    FD_SET(sock_fd, writefds);
	    MAX_FD(sock_fd);
	}
}