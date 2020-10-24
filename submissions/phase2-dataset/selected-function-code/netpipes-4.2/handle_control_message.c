void handle_control_message(buf, len)
     char	*buf;
     int	len;
{
    if (len<1) {
	fprintf(stderr, "%s: Internal Error: control message length <1. \n", progname);
	exit(1);
    }

    switch(buf[0]) {
    case ECP_EOF:
	if (len!=1) {
	    fprintf(stderr, "%s: Protocol Error: control message[0] length(%d) !=1. \n", progname, len);
	    exit(EXITCODE_PROTOCOL);
	}
#ifdef DEBUG
	fprintf(stderr, "%s: Got EOF from remote.\n", progname);
#endif

	if (pl_encapsulate_control->code == WRITEABLE) {
	    remove_pipeline(pl_encapsulate_control);
	    pl_encapsulate_control = 0;
	} else {
	    pl_encapsulate_control->code = READABLE;
	}

	break;

    case ECP_EXITCODE:
	if (len!=2) {
	    fprintf(stderr, "%s: Protocol Error: control message[1] length(%d) !=2. \n", progname, len);
	    exit(EXITCODE_PROTOCOL);
	}
	remote_return_code = (unsigned char )buf[1];
#ifdef DEBUG
	fprintf(stderr, "%s: remote exit status: %d\n",
		progname, remote_return_code);
#endif

	if (pl_encapsulate_control->code == WRITEABLE) {
	    remove_pipeline(pl_encapsulate_control);
	    pl_encapsulate_control = 0;
	} else {
	    pl_encapsulate_control->code = READABLE;
	}

	break;

    default:
	fprintf(stderr, "%s: Protocol Error: unknown control message [%d]. \n", progname, (unsigned char)buf[0]);
	exit(EXITCODE_PROTOCOL);
	break;
    }
}