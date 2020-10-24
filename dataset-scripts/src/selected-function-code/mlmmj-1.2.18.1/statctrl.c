int statctrl(const char *listdir, const char *ctrlstr)
{
	char *filename = concatstr(3, listdir, "/control/", ctrlstr);
	struct stat st;
	int res;
	
	res = stat(filename, &st);
	myfree(filename);
	
	if(res < 0) {
		if(errno == ENOENT) {
			return 0;
		} else {
			log_error(LOG_ARGS, "Could not stat %s/control/%s. "
					    "Bailing out.", listdir, ctrlstr);
			exit(EXIT_FAILURE);
		}
	}

	return 1;
}