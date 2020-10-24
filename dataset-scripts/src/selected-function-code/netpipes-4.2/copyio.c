void copyio(sock, aggressive_close)
    int	sock;
    int	aggressive_close;
{
    fd_set	readfds, writefds;
#define BSIZE	4096
    char	tosockbuf[BSIZE], fromsockbuf[BSIZE];
    int		tosocklen, fromsocklen;
    int		rval;
    int	exitval = 0;

    tosocklen = fromsocklen = 0;
    while (tosocklen>=0 || fromsocklen>=0) {
	/********************/
	FD_ZERO(&readfds);
	FD_ZERO(&writefds);
	if (tosocklen>=0) {
	    if (tosocklen==0) {
		FD_SET(0, &readfds);
	    } else {
		FD_SET(sock, &writefds);
	    }
	}
	if (fromsocklen>=0) {
	    if (fromsocklen==0) {
		FD_SET(sock, &readfds);
	    } else {
		FD_SET(1, &writefds);
	    }
	}

	if ( ( (aggressive_close&DONETSLAVE_CF)
	       && fromsocklen < 0
	       && tosocklen<1 )
	     ||
	     ( (aggressive_close&DONETSLAVE_CT)
	       && tosocklen < 0
	       && fromsocklen<1 )
	     ) {
	    /* One direction is closed and the other's buffer is empty.
	       Exit. */
	    close(sock);
	    break;
	}

	if ( ( (aggressive_close&DONETSLAVE_CF)
	       && fromsocklen < 0 )
	     ||
	     ( (aggressive_close&DONETSLAVE_CT)
	       && tosocklen < 0 )
	     ) {
	    /* One direction is closed but the other's buffer is not empty.
	       Don't accept any more input while we flush the buffer. */
	    FD_ZERO(&readfds);
	}
	/********************/

	rval=select(sock+1, &readfds, &writefds,
		    (fd_set*)0, (struct timeval*)0);
	/********************/
	if (rval<0) {
	    if (errno != EINTR) {
		perror("during copyio() select(2)");
		exit(EXITCODE_PIPE);
	    }
	} else if (rval==0) {
	    break;
	}
	/********************/
	if (FD_ISSET(1, &writefds)) {
	    rval = write(1, fromsockbuf, fromsocklen);
	    if (rval<0) {
		perror("during copyio() write(2)(1)");
		exitval = EXITCODE_PIPE;
		fromsocklen = -1;
		shutdown(sock, 0);
	    } else {
		memmove(fromsockbuf, fromsockbuf+rval, fromsocklen-rval);
		fromsocklen -= rval;
	    }
	}
	if (FD_ISSET(sock, &writefds)) {
	    rval = write(sock, tosockbuf, tosocklen);
	    if (rval<0) {
		perror("during copyio() write(2)(sock)");
		exitval = EXITCODE_PIPE;
		tosocklen = -1;
		shutdown(sock, 1);
	    } else {
		memmove(tosockbuf, tosockbuf+rval, tosocklen-rval);
		tosocklen -= rval;
	    }
	}
	if (FD_ISSET(0, &readfds)) {
	    tosocklen = read(0, tosockbuf, BSIZE);
	    if (tosocklen<0) {
		perror("during copyio() read(2)(0)");
		exitval = EXITCODE_PIPE;
		tosocklen = -1;
	    } else if (tosocklen==0) {
		tosocklen = -1;
		if (aggressive_close == 0)
		    shutdown(sock, 1);
	    }
	}
	if (FD_ISSET(sock, &readfds)) {
	    fromsocklen = read(sock, fromsockbuf, BSIZE);
	    if (fromsocklen<0) {
		perror("during copyio() read(2)(0)");
		exitval = EXITCODE_PIPE;
		fromsocklen = -1;
	    } else if (fromsocklen==0) {
		fromsocklen = -1;
		if (aggressive_close == 0)
		    shutdown(sock, 0);
	    }
	}
    }
    exit(exitval);
}