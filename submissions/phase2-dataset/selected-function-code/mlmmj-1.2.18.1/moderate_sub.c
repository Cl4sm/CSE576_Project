static void moderate_sub(const char *listdir, const char *listaddr,
		const char *listdelim, const char *subaddr,
		const char *mlmmjsend, enum subtype typesub, enum subreason reasonsub)
{
	int i, fd, status, nosubmodmails = 0;
	text *txt;
	memory_lines_state *mls;
	char *a = NULL, *queuefilename, *from, *listname, *listfqdn, *str;
	char *modfilename, *mods, *to, *replyto, *moderators = NULL;
	char *cookie, *obstruct;
	struct strlist *submods;
	pid_t childpid, pid;

	/* generate the file in moderation/ */
	switch(typesub) {
		default:
		case SUB_NORMAL:
			str = concatstr(4, subaddr, "\n", "SUB_NORMAL", "\n");
			break;
		case SUB_DIGEST:
			str = concatstr(4, subaddr, "\n", "SUB_DIGEST", "\n");
			break;
		case SUB_NOMAIL:
			str = concatstr(4, subaddr, "\n", "SUB_NOMAIL", "\n");
			break;
		case SUB_BOTH:
			str = concatstr(4, subaddr, "\n", "SUB_BOTH", "\n");
			break;
	}
	
	for (;;) {
		cookie = random_str();
		modfilename = concatstr(3, listdir, "/moderation/subscribe",
				cookie);
		fd = open(modfilename, O_RDWR|O_CREAT|O_EXCL, S_IRUSR|S_IWUSR);
		if (fd < 0) {
			if (errno == EEXIST) {
				myfree(cookie);
				myfree(modfilename);
				continue;
			}
			log_error(LOG_ARGS, "could not create %s"
					"ignoring request: %s", str);
			exit(EXIT_FAILURE);
		}
		break;
	}

	if(writen(fd, str, strlen(str)) < 0) {
		log_error(LOG_ARGS, "could not write to %s"
				"ignoring request: %s", str);
		exit(EXIT_FAILURE);
	}
	
	close(fd);
	
	myfree(str);

	submods = ctrlvalues(listdir, "submod");
	mods = concatstr(2, listdir, "/control/submod");
	/* check to see if there's adresses in the submod control file */
	for(i = 0; i < submods->count; i++)
		a = strchr(submods->strs[i], '@');

	/* no addresses in submod control file, use owner */
	if(a == NULL) {
		/* free the submods struct from above */
		for(i = 0; i < submods->count; i++)
			myfree(submods->strs[i]);
		myfree(submods->strs);
		myfree(submods);
		submods = ctrlvalues(listdir, "owner");
		myfree(mods);
		mods = concatstr(2, listdir, "/control/owner");
	}

	/* send mail to moderators about request pending */
	listdelim = getlistdelim(listdir);
	listfqdn = genlistfqdn(listaddr);
	listname = genlistname(listaddr);

	from = concatstr(4, listname, listdelim, "owner@", listfqdn);
	to = concatstr(3, listname, "-moderators@", listfqdn);
	replyto = concatstr(6, listname, listdelim, "permit-", cookie,
			"@", listfqdn);
	obstruct = concatstr(6, listname, listdelim, "obstruct-", cookie,
			"@", listfqdn);
	myfree(cookie);
	for(i = 0; i < submods->count; i++) {
		printf("%s", submods->strs[i]);
		str = moderators;
		moderators = concatstr(3, moderators, submods->strs[i], "\n");
		myfree(str);
	}
	mls = init_memory_lines(moderators);
	myfree(moderators);

	txt = open_text(listdir,
			"gatekeep", "sub",
			subreason_strs[reasonsub], subtype_strs[typesub],
			"submod-moderator");
	MY_ASSERT(txt);
	register_unformatted(txt, "subaddr", subaddr);
	register_unformatted(txt, "moderateaddr", replyto); /* DEPRECATED */
	register_unformatted(txt, "permitaddr", replyto);
	register_unformatted(txt, "obstructaddr", obstruct);
	register_unformatted(txt, "moderators", "%gatekeepers%"); /* DEPRECATED */
	register_formatted(txt, "gatekeepers",
			rewind_memory_lines, get_memory_line, mls);
	queuefilename = prepstdreply(txt, listdir, "$listowner$", to, replyto);
	MY_ASSERT(queuefilename);
	close_text(txt);
	
	/* we might need to exec more than one mlmmj-send */
	
	nosubmodmails = statctrl(listdir,"nosubmodmails");
	
	if (nosubmodmails)
		childpid = -1;
	else {
		childpid = fork();
		if(childpid < 0)
			log_error(LOG_ARGS, "Could not fork; requester not notified");
	}

	if(childpid != 0) {
		if(childpid > 0) {
			do /* Parent waits for the child */
				pid = waitpid(childpid, &status, 0);
			while(pid == -1 && errno == EINTR);
		}
		finish_memory_lines(mls);
		execl(mlmmjsend, mlmmjsend,
				"-a",
				"-l", "4",
				"-L", listdir,
				"-s", mods,
				"-F", from,
				"-R", replyto,
				"-m", queuefilename, (char *)NULL);
		log_error(LOG_ARGS, "execl() of '%s' failed", mlmmjsend);
		exit(EXIT_FAILURE);
	}

	myfree(to);
	myfree(replyto);
	
	/* send mail to requester that the list is submod'ed */

	from = concatstr(4, listname, listdelim, "bounces-help@", listfqdn);

	txt = open_text(listdir,
			"wait", "sub",
			subreason_strs[reasonsub], subtype_strs[typesub],
			"submod-requester");
	MY_ASSERT(txt);
	register_unformatted(txt, "subaddr", subaddr);
	register_unformatted(txt, "moderators", "%gatekeepers"); /* DEPRECATED */
	register_formatted(txt, "gatekeepers",
			rewind_memory_lines, get_memory_line, mls);
	queuefilename = prepstdreply(txt, listdir,
			"$listowner$", subaddr, NULL);
	MY_ASSERT(queuefilename);
	close_text(txt);

	finish_memory_lines(mls);
	myfree(listname);
	myfree(listfqdn);
	execl(mlmmjsend, mlmmjsend,
				"-l", "1",
				"-L", listdir,
				"-T", subaddr,
				"-F", from,
				"-m", queuefilename, (char *)NULL);
	log_error(LOG_ARGS, "execl() of '%s' failed", mlmmjsend);
	exit(EXIT_FAILURE);
}