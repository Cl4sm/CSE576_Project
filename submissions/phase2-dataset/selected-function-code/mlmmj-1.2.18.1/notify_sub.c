void notify_sub(const char *listdir, const char *listaddr,
		const char *listdelim, const char *subaddr,
		const char *mlmmjsend, enum subtype typesub, enum subreason reasonsub)
{
	char *listfqdn, *listname, *fromaddr, *tostr;
	text *txt;
	char *queuefilename = NULL, *listtext = NULL;

	listname = genlistname(listaddr);
	listfqdn = genlistfqdn(listaddr);

	fromaddr = concatstr(4, listname, listdelim, "bounces-help@", listfqdn);
	tostr = concatstr(4, listname, listdelim, "owner@", listfqdn);
	
	myfree(listname);
	myfree(listfqdn);

	switch(typesub) {
		default:
		case SUB_NORMAL:
			listtext = mystrdup("notifysub");
			break;
		case SUB_DIGEST:
			listtext = mystrdup("notifysub-digest");
			break;
		case SUB_NOMAIL:
			listtext = mystrdup("notifysub-nomail");
			break;
		case SUB_BOTH:
			/* No legacy list text as feature didn't exist. */
			listtext = mystrdup("notifysub");
			break;
	}

	txt = open_text(listdir, "notify", "sub",
			subreason_strs[reasonsub], subtype_strs[typesub],
			listtext);
	myfree(listtext);
	MY_ASSERT(txt);
	register_unformatted(txt, "subaddr", subaddr);
	register_unformatted(txt, "newsub", subaddr); /* DEPRECATED */
	queuefilename = prepstdreply(txt, listdir,
			"$listowner$", "$listowner$", NULL);
	MY_ASSERT(queuefilename);
	close_text(txt);

	execlp(mlmmjsend, mlmmjsend,
			"-l", "1",
			"-L", listdir,
			"-T", tostr,
			"-F", fromaddr,
			"-m", queuefilename, (char *)NULL);

	log_error(LOG_ARGS, "execlp() of '%s' failed", mlmmjsend);
	exit(EXIT_FAILURE);
}