char *get_index_from_filename(const char *filename)
{
	char *myfilename, *indexstr, *ret;
	size_t len;

	myfilename = mystrdup(filename);
	if (!myfilename) {
		return NULL;
	}

	len = strlen(myfilename);
	if (len > 9 && (strcmp(myfilename + len - 9, "/mailfile") == 0)) {
		myfilename[len - 9] = '\0';
	}

	indexstr = strrchr(myfilename, '/');
	if (indexstr) {
		indexstr++;  /* skip the slash */
	} else {
		indexstr = myfilename;
	}

	ret = mystrdup(indexstr);
	myfree(myfilename);

	return ret;
}