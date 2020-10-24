void confirm_unsub(const char *listdir, const char *listaddr,
		   const char *listdelim, const char *subaddr,
		   const char *mlmmjsend,
		   enum subtype typesub, enum subreason reasonsub)
{
	text *txt;
	char *queuefilename, *fromaddr, *listname, *listfqdn, *listtext;

	listname = genlistname(listaddr);
	listfqdn = genlistfqdn(listaddr);

	fromaddr = concatstr(4, listname, listdelim, "bounces-help@", listfqdn);

	myfree(listname);
	myfree(listfqdn);

	switch(typesub) {
		default:
		case SUB_NORMAL:
			listtext = mystrdup("unsub-ok");
			break;
		case SUB_DIGEST:
			listtext = mystrdup("unsub-ok-digest");
			break;
		case SUB_NOMAIL:
			listtext = mystrdup("unsub-ok-nomail");
			break;
	}

	txt = open_text(listdir, "finish", "unsub",
			subreason_strs[reasonsub], subtype_strs[typesub],
			listtext);
	myfree(listtext);
	MY_ASSERT(txt);
	register_unformatted(txt, "subaddr", subaddr);
	queuefilename = prepstdreply(txt, listdir,
			"$helpaddr$", subaddr, NULL);
	MY_ASSERT(queuefilename);
	close_text(txt);

	execlp(mlmmjsend, mlmmjsend,
				"-l", "1",
				"-L", listdir,
				"-T", subaddr,
				"-F", fromaddr,
				"-m", queuefilename, (char *)NULL);

	log_error(LOG_ARGS, "execlp() of '%s' failed", mlmmjsend);
	exit(EXIT_FAILURE);
}