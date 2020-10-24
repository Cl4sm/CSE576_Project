int is_subbed_in(const char *subddirname, const char *address)
{
	int retval = 0, subread;
	char *subreadname;
	off_t suboff;
	DIR *subddir;
	struct dirent *dp;

	if((subddir = opendir(subddirname)) == NULL) {
		log_error(LOG_ARGS, "Could not opendir(%s)", subddirname);
		exit(EXIT_FAILURE);
	}

	while((dp = readdir(subddir)) != NULL) {
		if(!strcmp(dp->d_name, "."))
			continue;
		if(!strcmp(dp->d_name, ".."))
			continue;

		subreadname = concatstr(2, subddirname, dp->d_name);
		subread = open(subreadname, O_RDONLY);
		if(subread < 0) {
	                log_error(LOG_ARGS, "Could not open %s", subreadname);
			myfree(subreadname);
			continue;
		}

		suboff = find_subscriber(subread, address);
		close(subread);
		myfree(subreadname);

		if(suboff == -1) {
			continue;
		} else {
			retval = 1;
			break;
		}
	}
	closedir(subddir);

	return retval;
}