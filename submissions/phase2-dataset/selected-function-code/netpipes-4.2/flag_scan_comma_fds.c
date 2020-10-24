int flag_scan_comma_fds(s)
    char *s;
{
    int	rval=0;
    while (1) {
	int	fd;
	int	n;
	if (1 != sscanf(s, "%i%n", &fd, &n)) {
	    fprintf(stderr, "%s: parse error in file descriptor list at '%s'\n", progname, s);
	    usage();
	    exit(EXITCODE_ARGS);
	}
	add_fd(fd);
	rval +=n;
	s += n;
	if (*s == ',') {
	    rval++;
	    s++;
	} else {
	    break;
	}
    }
    return rval;
}