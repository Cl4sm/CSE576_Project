void send_list(const char *listdir, const char *emailaddr,
	       const char *mlmmjsend)
{
	text *txt;
	subs_list_state *normalsls, *digestsls, *nomailsls;
	char *queuefilename, *listaddr, *listdelim, *listname, *listfqdn;
	char *fromaddr, *subdir, *nomaildir, *digestdir;
	int fd;

	listaddr = getlistaddr(listdir);
	listdelim = getlistdelim(listdir);
	listname = genlistname(listaddr);
	listfqdn = genlistfqdn(listaddr);

	fromaddr = concatstr(4, listname, listdelim, "bounces-help@", listfqdn);
	myfree(listdelim);

	subdir = concatstr(2, listdir, "/subscribers.d/");
	digestdir = concatstr(2, listdir, "/digesters.d/");
	nomaildir = concatstr(2, listdir, "/nomailsubs.d/");
	normalsls = init_subs_list(subdir);
	digestsls = init_subs_list(digestdir);
	nomailsls = init_subs_list(nomaildir);
	myfree(subdir);
	myfree(digestdir);
	myfree(nomaildir);

	txt = open_text(listdir, "list", NULL, NULL, subtype_strs[SUB_ALL],
			"listsubs");
	MY_ASSERT(txt);
	register_formatted(txt, "listsubs",
			rewind_subs_list, get_sub, normalsls);
	register_formatted(txt, "normalsubs",
			rewind_subs_list, get_sub, normalsls);
	register_formatted(txt, "digestsubs",
			rewind_subs_list, get_sub, digestsls);
	register_formatted(txt, "nomailsubs",
			rewind_subs_list, get_sub, nomailsls);
	queuefilename = prepstdreply(txt, listdir, "$listowner$", emailaddr, NULL);
	MY_ASSERT(queuefilename);
	close_text(txt);

	/* DEPRECATED */
	/* Add lists manually if they weren't encountered in the list text */
	if (!normalsls->used && !digestsls->used && !nomailsls->used) {
		fd = open(queuefilename, O_WRONLY);
		if(fd < 0) {
			log_error(LOG_ARGS, "Could not open sub list mail");
			exit(EXIT_FAILURE);
		}
		if(lseek(fd, 0, SEEK_END) < 0) {
			log_error(LOG_ARGS, "Could not seek to end of file");
			exit(EXIT_FAILURE);
		}
		print_subs(fd, normalsls);
		writen(fd, "\n-- \n", 5);
		print_subs(fd, nomailsls);
		writen(fd, "\n-- \n", 5);
		print_subs(fd, digestsls);
		writen(fd, "\n-- \nend of output\n", 19);
		close(fd);
	}

	finish_subs_list(normalsls);
	finish_subs_list(digestsls);
	finish_subs_list(nomailsls);
	myfree(listaddr);
	myfree(listname);
	myfree(listfqdn);

	execlp(mlmmjsend, mlmmjsend,
				"-l", "1",
				"-L", listdir,
				"-T", emailaddr,
				"-F", fromaddr,
				"-m", queuefilename, (char *)NULL);

	log_error(LOG_ARGS, "execlp() of '%s' failed", mlmmjsend);
	exit(EXIT_FAILURE);
}