int SumArticle (fd, artnum, bytes, np)
FILE *fd;
long bytes;
struct nrc_ent *np;
int artnum;
/*
 *  Summarize article
 */
{
	char subject[PATH_LEN];

	/* Get subject line */
	if (!GetHdr (fd, subject, bytes, "Subject: ")) return (0);

	/* Write group name if this is first article for group */
	if (sum_flag == 0) {
		sum_flag = 1;
		if((fprintf (sum_fd, "\r\n*** %s\r\n", np->name) < 0)
		&& ferror(fd) )
			return -1;
	}

	/* Write article number, subject line */
	if((fprintf(sum_fd, "%d:%s\r\n", artnum, subject) < 0) && ferror(fd))
		return -1;

	return 0;
}
