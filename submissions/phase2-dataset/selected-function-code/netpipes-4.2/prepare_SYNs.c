static char * prepare_SYNs(len_ret)
     int	*len_ret;
{
    struct pipeline	*curr;
    int	size = 64;
    char *rval = (char*)malloc(size);
    *len_ret = 0;

    for (curr = pipe_head; curr; curr = curr->next) {
	switch (curr->code) {
	case WRITEABLE:
	case DUPLEX_IO:
	    /* we need to wait for the other side to send a SYN for
               these two */
	    /* curr->session_id = -1; it's already -1 */
	    break;
	case READABLE:
	case DUPLEX_OI:
	    if (*len_ret + 8 > size) {
		rval = (char*)realloc(rval, size *=2);
	    }
	    {
		char	*p = rval + *len_ret;
		if (curr->session_id<0) {
		    curr->session_id = session_id_;
		    /* XXX - gotta prevent stompage in later rev */
		    session_id_ += 2; /* odd for servers, even for clients */
		}
		p[0] = SCP_SYN;
		p[1] = (curr->session_id >>16 ) &0xff;
		p[2] = (curr->session_id >> 8 ) &0xff;
		p[3] = (curr->session_id >> 0 ) &0xff;
		p[4] = p[5] = p[6] = p[7] = 0;
		*len_ret += 8;
#ifdef DEBUG
		fprintf(stderr, "%s: prepared SYN for session 0x%06x\n",
			progname, curr->session_id);
#endif
	    }
	    break;
	case DUPLEX:		/* can't happen */
	    abort();
	    exit(1);
	}
    }

    return rval;
}