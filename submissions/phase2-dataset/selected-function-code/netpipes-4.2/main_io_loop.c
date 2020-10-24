static void main_io_loop(sock_fd)
     int	sock_fd;
{
    char incoming_buf[BUF_SIZE]; /* read from socket, will write to child */
    char outgoing_buf[BUF_SIZE]; /* read from child, will packetize into : */
    char outgoing2_buf[BUF_SIZE+8]; /* packet buf, will write to socket */

    /* bytes in the buffer to child */
    /* this is nonzero only if curr_writeable is nonNULL */
    int	incoming_len=0;

    /* this is nonzero only if curr_readable is nonNULL */
    /* bytes in the buffer from child */
    int	outgoing_len=0;

    /* bytes in the buffer to socket */
    /* this is independent of the curr_ variables */
    int	outgoing2_len=0;
    int	outgoing2_off=0;

    /* for reading SCP headers */
    char	header_buf[8];
    int		header_len;

    fd_set	readfds, writefds;
    int	maxfd;

    struct pipeline	*curr_readable=0; /* child descriptor we're reading */
    struct pipeline	*curr_writeable=0; /* child descriptor we're writing */

    while (1) {
	int	rval;

	build_fd_lists (
			&readfds, &writefds,
			curr_readable, curr_writeable,
			sock_fd, &maxfd,
			incoming_len, sizeof(incoming_len),
			outgoing_len, sizeof(outgoing_buf),
			outgoing2_len, outgoing2_off);

	if (
#if 0
	    maxfd<0
	    || 
#else
	    (!subproc || !child_unreaped)
	    &&
#endif
	    (0==pipe_head && 0 == outgoing2_len
		&& special_packet_queue == 0)
	    ) {
	    if (incoming_len != 0 ||
		outgoing_len != 0 ||
		outgoing2_len != 0) {
		fprintf(stderr, "%s: leftover bytes in buffers at end of encapsulation.  That is bad because it means Bob made a logic error in his code.\n", progname);
	    }
	    break;		/* run out of things to do */
	}

	if (maxfd>0) {
	    struct timeval	tv;
	    tv.tv_sec = 0;
	    tv.tv_usec = 500000;
	    rval = select(maxfd+1, &readfds, &writefds, (fd_set*)0, &tv);

	    if (rval<0) {
		if (errno == EINTR) {
		    continue;
		} else {
		    fprintf(stderr, "%s: error during select: ", progname);
		    perror("");
		    exit(EXITCODE_SYSCALLFAILED);
		}
		/* NOTREACHED */
	    } else if (rval==0) {
		/* got a timeout */

	    }

	    /* read bytes from the child */
	    {
		struct pipeline	*curr;
		for (curr = pipe_head; curr; curr = curr->next) {
		    switch (curr->code) {
		    case READABLE:
		    case DUPLEX_IO:
		    case DUPLEX_OI:
		    case DUPLEX:
			if (curr->pipe[0]>=0 &&
			    FD_ISSET(curr->pipe[0], &readfds) &&
			    (curr==curr_readable || 0==curr_readable)) {
			    if (read_from_child(curr, outgoing_buf, &outgoing_len,
						sizeof(outgoing_buf)) )
				curr_readable = curr;
			}
			break;
		    case WRITEABLE:
			break;
		    }
		}
	    }

	    /* write bytes to the child */
	    if (curr_writeable && incoming_len>0 &&
		curr_writeable->pipe[0] >=0 &&
		FD_ISSET(curr_writeable->pipe[0], &writefds) ) {
		write_to_child(curr_writeable, incoming_buf, &incoming_len);
		if (incoming_len<1 && curr_writeable->bytes_left <1) {
		    curr_writeable = 0;
		}
	    }


	    /* write bytes to the socket */
	    if (FD_ISSET(sock_fd, &writefds)) {
		write_to_socket(sock_fd, outgoing2_buf,
				&outgoing2_off, outgoing2_len);
		if (outgoing2_off >= outgoing2_len) {
		    outgoing2_len = 0;
		    outgoing2_off = 0;
		}
	    }

	    /* read bytes from the socket */
	    if (FD_ISSET(sock_fd, &readfds)) {

		if (curr_writeable) {
		    read_from_socket(sock_fd, curr_writeable,
				     incoming_buf, &incoming_len,
				     sizeof(incoming_buf) );
		} else {
		    read_from_socket(sock_fd, (struct pipeline *)0,
				     header_buf, &header_len,
				     sizeof(header_buf));

		    if (header_len==sizeof(header_buf)) {
			interpret_scp_header(header_buf, &curr_writeable);
			header_len = 0;
		    }
		}
	    }
	}

	maybe_inject_special_packets(outgoing2_buf, &outgoing2_len,
				     sizeof(outgoing2_buf));

	if (outgoing2_len==0 && outgoing_len>0) {
	    build_packet_header(outgoing2_buf, 0, curr_readable->session_id,
				outgoing_len);
	    memcpy(outgoing2_buf + 8, outgoing_buf, outgoing_len);
	    outgoing2_len = outgoing_len + 8;
	    outgoing_len = 0;
	}

	if (curr_readable && outgoing_len==0) {
	    curr_readable = 0;
	}

	if (curr_writeable
	    && curr_writeable->bytes_left == 0
	    && curr_writeable == pl_encapsulate_control
	    && incoming_len>0) {
	    handle_control_message(incoming_buf, incoming_len);
	    incoming_len = 0;
	    curr_writeable = 0;
	}

	if (subproc) {
	    if (!child_running && child_unreaped) {
		probe_child();
	    }
	} else {
	    if (pipe_head != 0 &&
		pipe_head == pl_encapsulate_control &&
		pipe_head->next == 0 &&
		pipe_head->code != WRITEABLE) {
		/* all channels are closed */
		char	*buf;
		int	len = 1;
#ifdef DEBUG
		fprintf(stderr, "%s: sending EOF\n",
			progname);
#endif
		buf = malloc(8+len);
		build_packet_header(buf, 0, SESSION_ENCAP, len);
		buf[8] = ECP_EOF;
		prime_packet_queue(buf, 8+len);

		if (pl_encapsulate_control->code == READABLE) {
		    remove_pipeline(pl_encapsulate_control);
		    pl_encapsulate_control = 0;
		} else {
		    pl_encapsulate_control->code = WRITEABLE;
		}
	    }
	}

	perform_special_actions(&incoming_len, &outgoing_len,
				&curr_readable, &curr_writeable);
    }
}