void perform_special_actions(ibuf_len, obuf_len, curr_readable, curr_writeable)
     int	*ibuf_len;
     int	*obuf_len;
     struct pipeline	**curr_readable, **curr_writeable;
{
    struct pipeline	**curr;
    for (curr = &pipe_head;
	 *curr;
	 curr = curr ? (&(*curr)->next) : &pipe_head ) {
	switch ((*curr)->specact) {
	case NOTHING:
	    break;
	case CLOSE_TO_READ:
	    /* got a FIN packet */
	    if (*ibuf_len>0)
		break;		/* can't drop it yet */
	    if (*curr_writeable == *curr
		|| *curr_readable == *curr) {
		/* fprintf(stderr, "%s: weird, special action CLOSE_TO_READ applied to a curr_ pipeline %lx\n", progname, (long)*curr); */
		break;
	    }
	    switch((*curr)->code) {
	    case WRITEABLE:
#ifdef DEBUG
		fprintf(stderr, "%s: closing W child fd %d\n", progname, (*curr)->child_fd);
#endif
		close((*curr)->pipe[0]);
		remove_pipeline_(curr);
		curr = 0;	/* start scanning from the beginning */
		break;
	    case DUPLEX:	/* DUPLEX not actually possible */
	    case DUPLEX_IO:
	    case DUPLEX_OI:
#ifdef DEBUG
		fprintf(stderr, "%s: converting child fd %d to child-write-only\n", progname, (*curr)->child_fd);
#endif
		(*curr)->specact = NOTHING;
		(*curr)->code = READABLE;
		shutdown((*curr)->pipe[0], 1);

		break;
	    case READABLE:
		fprintf(stderr, "%s: internal error, attempt to CLOSE_TO_READ on a READABLE descriptor\n", progname);
		exit(1);
	    }
	    break;
	case CLOSE_TO_RW:
	    /* got a RESET packet.  get drastic */
	    if ( (*curr)->bytes_left >0) {
		fprintf(stderr, "%s: Freaky, %d bytes_left in CLOSE_TO_RW channel\n", progname, (*curr)->bytes_left);
		break;		/* can't dump it without corrupting the stream */
	    }
	    if (*curr_readable == *curr) {
		*obuf_len = 0;
		*curr_readable = 0;
	    }
	    if (*curr_writeable == *curr) {
		*ibuf_len = 0;
		*curr_writeable = 0;
	    }
#ifdef DEBUG
	    fprintf(stderr, "%s: RESETting child fd %d\n", progname, (*curr)->child_fd);
#endif
	    {
		struct pipeline *temp = *curr;
		close(temp->pipe[0]);
		*curr = temp->next;
		free(temp);
	    }
	    curr = 0;	/* start scanning from the beginning */
	    break;
	case CLOSE_TO_WRITE:
	    /* child closed the descriptor */
	    if (*curr_writeable == *curr
		|| *curr_readable == *curr) {
		/*fprintf(stderr, "%s: weird, special action CLOSE_TO_WRITE applied to a curr_ pipeline %lx\n", progname, (long)*curr);*/
		break;
	    }

	    {
	      char	*buf;
	      int	len;
	      len = 8;
	      buf = malloc(len);
	      build_packet_header(buf, SCP_FIN, (*curr)->session_id, 0);
	      prime_packet_queue(buf, len);
#ifdef DEBUG
	      fprintf(stderr, "%s: sending FIN for session 0x%06x\n", progname, (*curr)->session_id);
#endif
	    }

	    switch((*curr)->code) {
	    case READABLE:
#ifdef DEBUG
		fprintf(stderr, "%s: closing R child fd %d\n", progname, (*curr)->child_fd);
#endif
		{
		    struct pipeline *temp = *curr;
		    temp = *curr;
		    close(temp->pipe[0]);
		    *curr = temp->next;
		    free(temp);
		}
		curr = 0;	/* start scanning from the beginning */
		break;
	    case DUPLEX:	/* DUPLEX not actually possible */
	    case DUPLEX_IO:
	    case DUPLEX_OI:
#ifdef DEBUG
		fprintf(stderr, "%s: converting child fd %d to child-read-only\n", progname, (*curr)->child_fd);
#endif
		(*curr)->specact = NOTHING;
		(*curr)->code = WRITEABLE;
		shutdown((*curr)->pipe[0], 0);

		break;
	    case WRITEABLE:
		fprintf(stderr, "%s: internal error, attempt to CLOSE_TO_WRITE on a WRITEABLE descriptor\n", progname);
		exit(1);
	    }

	    break;
	}
    }
}