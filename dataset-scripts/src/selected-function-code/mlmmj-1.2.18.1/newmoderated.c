static void newmoderated(const char *listdir, const char *mailfilename,
		  const char *mlmmjsend, const char *efromsender,
		  const char *subject, const char *posteraddr,
		  enum modreason modreason)
{
	char *from, *listfqdn, *listname, *moderators = NULL;
	char *buf, *replyto, *listaddr = getlistaddr(listdir), *listdelim;
	text *txt;
	memory_lines_state *mls;
	char *queuefilename = NULL, *moderatorsfilename, *efromismod = NULL;
	char *mailbasename = mybasename(mailfilename), *tmp, *to, *reject;
	int moderatorsfd, foundaddr = 0, notifymod = 0, status;
	pid_t childpid, pid;
#if 0
	printf("mailfilename = [%s], mailbasename = [%s]\n", mailfilename,
			                                     mailbasename);
#endif
	listfqdn = genlistfqdn(listaddr);
	listname = genlistname(listaddr);

	moderatorsfilename = concatstr(2, listdir, "/control/moderators");
	if((moderatorsfd = open(moderatorsfilename, O_RDONLY)) < 0) {
		log_error(LOG_ARGS, "Could not open '%s'", moderatorsfilename);
		myfree(moderatorsfilename);
		exit(EXIT_FAILURE);
	}
	myfree(moderatorsfilename);

	if(statctrl(listdir, "ifmodsendonlymodmoderate"))
		efromismod = concatstr(2, efromsender, "\n");

	while((buf = mygetline(moderatorsfd))) {
		if(efromismod && strcmp(buf, efromismod) == 0)
			foundaddr = 1;
		tmp = moderators;
		moderators = concatstr(2, moderators, buf);
		myfree(buf);
		myfree(tmp);
	}

	if(!foundaddr) {
		myfree(efromismod);
		efromismod = NULL;
	}

	if(efromismod) mls = init_memory_lines(efromismod);
	else mls = init_memory_lines(moderators);

	close(moderatorsfd);
	myfree(moderators);

	listdelim = getlistdelim(listdir);
	replyto = concatstr(6, listname, listdelim, "release-", mailbasename,
			    "@", listfqdn);
	reject = concatstr(6, listname, listdelim, "reject-", mailbasename,
			    "@", listfqdn);

	from = concatstr(4, listname, listdelim, "owner@", listfqdn);
	to = concatstr(3, listname, "-moderators@", listfqdn); /* FIXME JFA: Should this be converted? Why, why not? */

	myfree(listdelim);
	myfree(listname);
	myfree(listfqdn);

	txt = open_text(listdir, "moderate", "post",
			modreason_strs[modreason], NULL, "moderation");
	MY_ASSERT(txt);
	register_unformatted(txt, "subject", subject);
	register_unformatted(txt, "posteraddr", posteraddr);
	register_unformatted(txt, "moderateaddr", replyto); /* DEPRECATED */
	register_unformatted(txt, "releaseaddr", replyto);
	register_unformatted(txt, "rejectaddr", reject);
	register_unformatted(txt, "moderators", "%moderators%"); /* DEPRECATED */
	register_formatted(txt, "moderators",
			rewind_memory_lines, get_memory_line, mls);
	register_originalmail(txt, mailfilename);
	queuefilename = prepstdreply(txt, listdir, "$listowner$", to, replyto);
	MY_ASSERT(queuefilename);
	close_text(txt);

	/* we might need to exec more than one mlmmj-send */
	
	notifymod = !efromismod && statctrl(listdir,"notifymod");
	
	if (notifymod) {
		childpid = fork();
		if(childpid < 0)
			log_error(LOG_ARGS, "Could not fork; poster not notified");
	} else
		childpid = -1;

	if(childpid != 0) {
		if(childpid > 0) {
			do /* Parent waits for the child */
				pid = waitpid(childpid, &status, 0);
			while(pid == -1 && errno == EINTR);
		}

		finish_memory_lines(mls);

		if(efromismod)
			execlp(mlmmjsend, mlmmjsend,
					"-l", "1",
					"-L", listdir,
					"-F", from,
					"-m", queuefilename,
					"-T", efromsender, (char *)NULL);
		else
			execlp(mlmmjsend, mlmmjsend,
					"-l", "2",
					"-L", listdir,
					"-F", from,
					"-m", queuefilename, (char *)NULL);
		log_error(LOG_ARGS, "execlp() of '%s' failed", mlmmjsend);
		exit(EXIT_FAILURE);
	}

	myfree(queuefilename);

	/* send mail to poster that the list is moderated */

	txt = open_text(listdir, "wait", "post",
			modreason_strs[modreason], NULL, "moderation-poster");
	MY_ASSERT(txt);
	register_unformatted(txt, "subject", subject);
	register_unformatted(txt, "posteraddr", posteraddr);
	register_unformatted(txt, "moderators", "%moderators%"); /* DEPRECATED */
	register_formatted(txt, "moderators",
			rewind_memory_lines, get_memory_line, mls);
	register_originalmail(txt, mailfilename);
	queuefilename = prepstdreply(txt, listdir,
			"$listowner$", efromsender, NULL);
	MY_ASSERT(queuefilename);
	close_text(txt);

	finish_memory_lines(mls);

	execlp(mlmmjsend, mlmmjsend,
			"-l", "1",
			"-L", listdir,
			"-F", from,
			"-m", queuefilename,
			"-T", efromsender, (char *)NULL);

	log_error(LOG_ARGS, "execlp() of '%s' failed", mlmmjsend);
	exit(EXIT_FAILURE);
}