static char *ctrlval(const char *listdir, const char *subdir,
		const char *ctrlstr, int oneline)
{
	char *filename, *value = NULL;
	int ctrlfd, i;

	if(listdir == NULL)
		return NULL;

	filename = concatstr(5, listdir, "/", subdir, "/", ctrlstr);
	ctrlfd = open(filename, O_RDONLY);
	myfree(filename);

	if(ctrlfd < 0)
		return NULL;

	if (oneline) {
		value = mygetline(ctrlfd);
		chomp(value);
	} else {
		value = mygetcontent(ctrlfd);
		i = strlen(value) - 1;
		if (i >= 0 && value[i] == '\n') {
			value[i] = '\0';
			i--;
		}
		if (i >= 0 && value[i] == '\r') {
			value[i] = '\0';
			i--;
		}
	}
	close(ctrlfd);

	return value;
}