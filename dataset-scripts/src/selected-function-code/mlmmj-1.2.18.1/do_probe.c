void do_probe(const char *listdir, const char *mlmmjsend, const char *addr)
{
	text *txt;
	file_lines_state *fls;
	char *myaddr, *from, *a, *queuefilename, *listaddr;
	char *listfqdn, *listname, *probefile, *listdelim=getlistdelim(listdir);
	int fd;
	time_t t;

	myaddr = mystrdup(addr);

	listaddr = getlistaddr(listdir);
	listname = genlistname(listaddr);
	listfqdn = genlistfqdn(listaddr);

	from = concatstr(6, listname, listdelim, "bounces-probe-", myaddr, "@",
			 listfqdn);

	myfree(listaddr);
	myfree(listdelim);
	myfree(listfqdn);
	myfree(listname);

	a = strrchr(myaddr, '=');
	if (!a) {
		myfree(myaddr);
		myfree(from);
		log_error(LOG_ARGS, "do_probe(): malformed address");
		exit(EXIT_FAILURE);

	}
	*a = '@';

	txt = open_text(listdir, "probe", NULL, NULL, NULL, "bounce-probe");
	MY_ASSERT(txt);
	register_unformatted(txt, "bouncenumbers", "%bouncenumbers%"); /* DEPRECATED */
	fls = init_truncated_file_lines(addr, 0, ':');
	register_formatted(txt, "bouncenumbers",
			rewind_file_lines, get_file_line, fls);
	queuefilename = prepstdreply(txt, listdir, "$listowner$", myaddr, NULL);
	MY_ASSERT(queuefilename);
	close_text(txt);

	finish_file_lines(fls);

	probefile = concatstr(4, listdir, "/bounce/", addr, "-probe");
	MY_ASSERT(probefile);
	t = time(NULL);
	a = mymalloc(32);
	snprintf(a, 31, "%ld", (long int)t);
	a[31] = '\0';
	unlink(probefile);
	fd = open(probefile, O_WRONLY|O_TRUNC|O_CREAT, S_IRUSR|S_IWUSR);
	if(fd < 0)
		log_error(LOG_ARGS, "Could not open %s", probefile);
	else
		if(writen(fd, a, strlen(a)) < 0)
			log_error(LOG_ARGS, "Could not write time in probe");

	myfree(probefile);

	execlp(mlmmjsend, mlmmjsend,
				"-l", "5",
				"-L", listdir,
				"-T", myaddr,
				"-F", from,
				"-m", queuefilename, (char *)NULL);

	log_error(LOG_ARGS, "execlp() of '%s' failed", mlmmjsend);

	exit(EXIT_FAILURE);
}