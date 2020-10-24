void generate_unsubconfirm(const char *listdir, const char *listaddr,
			   const char *listdelim, const char *subaddr,
			   const char *mlmmjsend,
			   enum subtype typesub, enum subreason reasonsub)
{
	char *confirmaddr, *listname, *listfqdn, *tmpstr;
	text *txt;
	char *queuefilename, *fromaddr;
	char *randomstr = NULL, *confirmfilename = NULL, *listtext;
	int subconffd;

	listname = genlistname(listaddr);
	listfqdn = genlistfqdn(listaddr);

        do {
                myfree(confirmfilename);
                myfree(randomstr);
		randomstr = random_plus_addr(subaddr);
                confirmfilename = concatstr(3, listdir, "/unsubconf/",
					    randomstr);

                subconffd = open(confirmfilename, O_RDWR|O_CREAT|O_EXCL,
						  S_IRUSR|S_IWUSR);

        } while ((subconffd < 0) && (errno == EEXIST));

	if(subconffd < 0) {
		log_error(LOG_ARGS, "Could not open '%s'", confirmfilename);
                myfree(randomstr);
		myfree(confirmfilename);
		exit(EXIT_FAILURE);
	}

	myfree(confirmfilename);

	if(writen(subconffd, subaddr, strlen(subaddr)) < 0) {
		log_error(LOG_ARGS, "Could not write unsubconffile");
                myfree(randomstr);
		myfree(confirmfilename);
		exit(EXIT_FAILURE);
	}

	close(subconffd);

	fromaddr = concatstr(6, listname, listdelim, "bounces-confunsub-",
				randomstr, "@", listfqdn);

	switch(typesub) {
		default:
		case SUB_NORMAL:
			listtext = mystrdup("unsub-confirm");
			tmpstr = mystrdup("confunsub-");
			break;
		case SUB_DIGEST:
			listtext = mystrdup("unsub-confirm-digest");
			tmpstr = mystrdup("confunsub-digest-");
			break;
		case SUB_NOMAIL:
			listtext = mystrdup("unsub-confirm-nomail");
			tmpstr = mystrdup("confunsub-nomail-");
			break;
	}

	confirmaddr = concatstr(6, listname, listdelim, tmpstr, randomstr, "@",
				listfqdn);

	myfree(randomstr);
	myfree(tmpstr);

	txt = open_text(listdir, "confirm", "unsub",
			subreason_strs[reasonsub], subtype_strs[typesub],
			listtext);
	myfree(listtext);
	MY_ASSERT(txt);
	register_unformatted(txt, "subaddr", subaddr);
	register_unformatted(txt, "confaddr", confirmaddr); /* DEPRECATED */
	register_unformatted(txt, "confirmaddr", confirmaddr);
	queuefilename = prepstdreply(txt, listdir,
			"$helpaddr$", subaddr, confirmaddr);
	MY_ASSERT(queuefilename);
	close_text(txt);

	myfree(listname);
	myfree(listfqdn);

	execlp(mlmmjsend, mlmmjsend,
				"-l", "1",
				"-L", listdir,
				"-T", subaddr,
				"-F", fromaddr,
				"-m", queuefilename, (char *)NULL);
	log_error(LOG_ARGS, "execlp() of '%s' failed", mlmmjsend);
	exit(EXIT_FAILURE);
}