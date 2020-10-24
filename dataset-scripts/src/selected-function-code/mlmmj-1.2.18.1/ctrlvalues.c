struct strlist *ctrlvalues(const char *listdir, const char *ctrlstr)
{
	struct strlist *ret;
	char *filename = concatstr(3, listdir, "/control/", ctrlstr);
	char *value;
	int ctrlfd;

	ctrlfd = open(filename, O_RDONLY);
	myfree(filename);

	if(ctrlfd < 0)
		return NULL;
		
	ret = mymalloc(sizeof(struct strlist));
	ret->count = 0;
	ret->strs = NULL;
	while((value = mygetline(ctrlfd)) != NULL) {
		chomp(value);
		/* Ignore empty lines */
		if (*value == '\0')
			continue;
		ret->count++;
		ret->strs = (char **) myrealloc(ret->strs, sizeof(char *) *
					(ret->count + 1));
		ret->strs[ret->count-1] = value;
		ret->strs[ret->count] = NULL;
	}
		
	close(ctrlfd);

	return ret;
}