void send_help(const char *listdir, const char *queuefilename,
		const char *emailaddr, const char *mlmmjsend)
{
	char *listaddr, *listdelim, *listname, *listfqdn;
	char *fromaddr;

	listaddr = getlistaddr(listdir);
	listdelim = getlistdelim(listdir);
	listname = genlistname(listaddr);
	listfqdn = genlistfqdn(listaddr);

	fromaddr = concatstr(4, listname, listdelim, "bounces-help@", listfqdn);

	myfree(listname);
	myfree(listdelim);
	myfree(listfqdn);
	myfree(listaddr);

	execlp(mlmmjsend, mlmmjsend,
				"-l", "1",
				"-L", listdir,
				"-T", emailaddr,
				"-F", fromaddr,
				"-m", queuefilename, (char *)NULL);

	log_error(LOG_ARGS, "execlp() of '%s' failed", mlmmjsend);
	exit(EXIT_FAILURE);
}