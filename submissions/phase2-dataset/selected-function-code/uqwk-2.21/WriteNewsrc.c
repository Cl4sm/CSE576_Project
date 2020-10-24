int WriteNewsrc()
/*
 *  Rewrite the updated .newsrc file
 */
{
	struct nrc_ent *np;
	int err = 0;

	if (read_only) return (0);

	if (NULL == (nrc_fd = fopen (nrc_file, "w"))) {
		fprintf (stderr, "%s: can't write %s\n",
			progname, nrc_file);
		exit(-1);	/* Return failure error code to caller */
	}

	for (np=nrc_list; np!=NULL; np=np->next) {
		/* Write this one */
		if (waf_mode) {
			if(np->subscribed) {
				if(fprintf(nrc_fd, "%s %d\r\n", np->name, np->sub->hi) < 0
				&& ferror(nrc_fd))
					err++;
			}
		} else {	/* Not waffle mode */
			if(np->subscribed) {
				if(fprintf(nrc_fd, "%s: ", np->name) < 0
				&& ferror(nrc_fd))
					err++;
			} else {
				if(fprintf(nrc_fd, "%s! ", np->name) < 0
				&& ferror(nrc_fd))
					err++;
			}
			if(!err)
				WriteSub (nrc_fd, np->sub);
		}
		if(err) {
			fprintf(stderr, "%s: can't write to %s\n",
				progname, nrc_file);
			exit(-1);
		}
	}
	fclose (nrc_fd);
	return 1;
}
