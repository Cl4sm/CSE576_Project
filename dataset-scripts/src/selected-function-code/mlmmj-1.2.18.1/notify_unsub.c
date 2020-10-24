void notify_unsub(const char *listdir, const char *listaddr,
		  const char *listdelim, const char *subaddr,
		  const char *mlmmjsend,
		  enum subtype typesub, enum subreason reasonsub)
{
        char *listfqdn, *listname, *fromaddr, *tostr;
	text *txt;
        char *queuefilename = NULL, *listtext;

        listname = genlistname(listaddr);
        listfqdn = genlistfqdn(listaddr);

        fromaddr = concatstr(4, listname, listdelim, "bounces-help@", listfqdn);
	tostr = concatstr(4, listname, listdelim, "owner@", listfqdn);

	myfree(listname);
	myfree(listfqdn);

	switch(typesub) {
		default:
		case SUB_NORMAL:
			listtext = mystrdup("notifyunsub");
			break;
		case SUB_DIGEST:
			listtext = mystrdup("notifyunsub-digest");
			break;
		case SUB_NOMAIL:
			listtext = mystrdup("notifyunsub-nomail");
			break;
	}
	
	txt = open_text(listdir, "notify", "unsub",
			subreason_strs[reasonsub], subtype_strs[typesub],
			listtext);
	myfree(listtext);
	MY_ASSERT(txt);
	register_unformatted(txt, "subaddr", subaddr);
	register_unformatted(txt, "oldsub", subaddr); /* DEPRECATED */
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