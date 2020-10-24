void interpret_scp_header(header, pl)
     unsigned char	*header;	/* size 8 */
     struct pipeline	**pl;
{
    int	flags, session_id;
    unsigned int	length;
    struct pipeline	*found = 0;

    flags = header[0];
    session_id = ( header[1] << 16 ) | ( header[2] << 8 ) | header[3];
    length = ( header[4] << 24 ) | ( header[5] << 16 ) | ( header[6] << 8 ) | header[7];

#ifdef DEBUG
    fprintf(stderr, "%s: packet. flags=0x%02x, session=0x%06x, length=%d\n",
	    progname, flags, session_id, length);
#endif

    if ( (flags & (SCP_SYN|0xf)) == SCP_SYN) {
	struct pipeline	*curr;
	found = 0;
	/* see if there's one descriptor that's waiting for this
           particular session_id */
	for (curr = pipe_head; curr && !found; curr = curr->next ) {
	    if (curr->syn_received)
		continue;
	    switch(curr->code) {
	    case WRITEABLE: case DUPLEX_IO: case DUPLEX_OI: case DUPLEX:
		/* DUPLEX not actually possible */
		if (curr->session_id == session_id) {
		    found = curr;
		    found->syn_received = 1;
		}
		break;
	    case READABLE:
		break;		/* like, whatever */
	    }
	}
	if (!found) for (curr = pipe_head; curr && !found; curr = curr->next ) {
	    if (curr->syn_received)
		continue;
	    switch(curr->code) {
	    case WRITEABLE: case DUPLEX_IO: case DUPLEX_OI:
	    case DUPLEX: /* DUPLEX not actually possible */
		if (curr->session_id<0) {
		    found = curr;
		    found->session_id = session_id;
		    found->syn_received = 1;
		}
		break;
	    case READABLE:
		break;		/* like, whatever */
	    }
	}
	if (!found) {
	    fprintf(stderr, "%s: Warning! discarding SYN for session_id=0x%06x, to the bit bucket!\n", progname, session_id);
	    found = &pl_bit_bucket;
	} else {
#ifdef DEBUG
	    fprintf(stderr, "%s: received SYN for session 0x%06x", progname, session_id);
#endif
	    if (found->code == DUPLEX_IO) {
		char	*buf = malloc(8);
		build_packet_header(buf, SCP_SYN, session_id, 0);
		prime_packet_queue(buf, 8);
#ifdef DEBUG
		fprintf(stderr, "; sending one back");
#endif
	    }
#ifdef DEBUG
	    putc('\n', stderr);
#endif
	}
	/* fall through and accept any data that might be in the packet */
    } else if ( (flags & (SCP_FIN|0xf)) == SCP_FIN) {
	struct pipeline	*curr;
	for (curr = pipe_head; curr && !found; curr = curr->next ) {
	    if (! curr->syn_received)
		continue;
	    if (curr->session_id != session_id)
		continue;

	    switch(curr->code) {
	    case WRITEABLE: case DUPLEX_IO: case DUPLEX_OI:
	    case DUPLEX: /* DUPLEX not actually possible */
		found = curr;
		found->specact = CLOSE_TO_READ;
		break;
	    case READABLE:
		break;		/* like, whatever */
	    }
	}
	if (!found) {
	    fprintf(stderr, "%s: Warning! discarding FIN for session_id=0x%06x, to the bit bucket!\n", progname, session_id);
	    found = &pl_bit_bucket;
	} else {
#ifdef DEBUG
	    fprintf(stderr, "%s: received FIN for session 0x%06x\n", progname, session_id);
#endif
	}
    } else if ( (flags & (SCP_PUSH|0xf)) == SCP_PUSH) {
	/* like, whatever */
    } else if ( (flags & (SCP_RESET|0xf)) == SCP_RESET) {
	struct pipeline	*curr;
	/* why are they doing this to me? */
	fprintf(stderr, "%s: RESET received for session_id=%d.  closing descriptor bidirectionally\n", progname, session_id);
	for (curr = pipe_head; curr && !found; curr = curr->next ) {
	    if (! curr->syn_received)
		continue;
	    if (curr->session_id != session_id)
		continue;

	    switch(curr->code) {
	    case READABLE: case DUPLEX_IO: case DUPLEX_OI:
	    case DUPLEX: /* DUPLEX not actually possible */
	    case WRITEABLE:
		found = curr;
		found->specact = CLOSE_TO_RW;
		exitcode_warnings = EXITCODE_GOT_RESET;
		break;
	    }
	}
	if (!found) {
	    fprintf(stderr, "%s: Warning! discarding RESET for session_id=0x%06x, to the bit bucket!\n", progname, session_id);
	    found = &pl_bit_bucket;
	}
    } else if (flags != 0) {
	fprintf(stderr, "%s: Warning! funky flags 0x%02x on packet, to the bit bucket!\n", progname, flags);
	found = &pl_bit_bucket;
    } else {
	struct pipeline	*curr;
	for (curr = pipe_head; curr && !found; curr = curr->next ) {
	    if (curr->session_id == session_id) {
		found = curr;
		break;
	    }
	}
	if (!found) {
	    fprintf(stderr, "%s: Warning! discarding data packet for session_id=0x%06x, to the bit bucket!\n", progname, session_id);
	    found = &pl_bit_bucket;
	}
    }

    if (length) {

	if (found->code == READABLE) {
	    fprintf(stderr, "%s: Got data for session 0x%06x I can't write to.  RESETting\n", progname, session_id);
	    send_reset(found);
	    found = &pl_bit_bucket;
	}

	*pl = found;
	(*pl)->bytes_left = length;
    }
}