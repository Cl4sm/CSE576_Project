static void probe_child()
{
    if (child_running || !child_unreaped)
	return;

    if ( 0>=wait(&local_return_code)) {
	fprintf(stderr, "%s: wait returned error or zero: ", progname);
	perror("");
	exit(EXITCODE_SYSCALLFAILED);
    }
    if (!WIFEXITED(local_return_code))
	local_return_code = EXITCODE_SIGNAL;
    else
	local_return_code = WEXITSTATUS(local_return_code);

    {
	char	*buf;
	int	len = 2;
#ifdef DEBUG
	fprintf(stderr, "%s: sending exit status %d\n",
		progname, local_return_code);
#endif
	buf = malloc(8+len);
	build_packet_header(buf, 0, SESSION_ENCAP, len);
	buf[8] = ECP_EXITCODE;
	buf[8+1] = local_return_code;
	prime_packet_queue(buf, len+8);

	if (pl_encapsulate_control->code == READABLE) {
	    remove_pipeline(pl_encapsulate_control);
	    pl_encapsulate_control = 0;
	} else {
	    pl_encapsulate_control->code = WRITEABLE;
	}
    }

    child_unreaped = 0;
}