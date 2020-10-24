int do_all_the_voodoo_here(int infd, int outfd, int hdrfd, int footfd,
		 const char **delhdrs, struct mailhdr *readhdrs,
		 struct strlist *allhdrs, const char *prefix)
{
	char *hdrline, *subject, *unqp;
	int hdrsadded = 0;
	int subject_present = 0;

	allhdrs->count = 0;
	allhdrs->strs = NULL;

	while((hdrline = gethdrline(infd))) {
		/* Done with headers? Then add extra if wanted*/
		if((strncasecmp(hdrline, "mime", 4) == 0) ||
			((strlen(hdrline) == 1) && (hdrline[0] == '\n'))){

			/* add extra headers */
			if(!hdrsadded && hdrfd >= 0) {
				if(dumpfd2fd(hdrfd, outfd) < 0) {
					log_error(LOG_ARGS, "Could not "
						"add extra headers");
					myfree(hdrline);
					return -1;
				} else
					hdrsadded = 1;
			}
			
			fsync(outfd);

			/* end of headers, write single LF */ 
			if(hdrline[0] == '\n') {
				/* but first add Subject if none is present
				 * and a prefix is defined */
				if (prefix && !subject_present)
				{
					subject = concatstr(3, "Subject: ", 
								prefix, "\n");
					writen(outfd, subject, strlen(subject));
					myfree(subject);
					subject_present = 1;
				}

				if(writen(outfd, hdrline, strlen(hdrline))
						< 0) {
					myfree(hdrline);
					log_error(LOG_ARGS,
							"Error writing hdrs.");
					return -1;
				}
				myfree(hdrline);
				break;
			}
		}
		/* Do we want info from hdrs? Get it before it's gone */
		if(readhdrs)
			getinfo(hdrline, readhdrs);

		/* Snatch a copy of the header */
		allhdrs->count++;
		allhdrs->strs = myrealloc(allhdrs->strs,
					sizeof(char *) * (allhdrs->count + 1));
		allhdrs->strs[allhdrs->count-1] = mystrdup(hdrline);
		allhdrs->strs[allhdrs->count] = NULL;  /* XXX why, why, why? */

		/* Add Subject: prefix if wanted */
		if(prefix) {
			if(strncasecmp(hdrline, "Subject:", 8) == 0) {
				subject_present = 1;
				unqp = cleanquotedp(hdrline + 8);
				if(strstr(hdrline + 8, prefix) == NULL &&
				   strstr(unqp, prefix) == NULL) {
					subject = concatstr(3,
							"Subject: ", prefix,
							hdrline + 8);
					writen(outfd, subject,
							strlen(subject));
					myfree(subject);
					myfree(hdrline);
					myfree(unqp);
					continue;
				}
				myfree(unqp);
			}
		}
		
		/* Should it be stripped? */
		if(delhdrs) {
			if(!findit(hdrline, delhdrs))
				writen(outfd, hdrline, strlen(hdrline));
		} else
			writen(outfd, hdrline, strlen(hdrline));


		myfree(hdrline);
	}

	/* Just print the rest of the mail */
	if(dumpfd2fd(infd, outfd) < 0) {
		log_error(LOG_ARGS, "Error when dumping rest of mail");
		return -1;
	}

	/* No more, lets add the footer if one */
	if(footfd >= 0)
		if(dumpfd2fd(footfd, outfd) < 0) {
			log_error(LOG_ARGS, "Error when adding footer");
			return -1;
		}

	fsync(outfd);

	return 0;
}