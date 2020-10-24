char *getlistdelim(const char *listdir)
{
	char *delimfn, *delimstr;
	int delimfd;

	delimfn = concatstr(2, listdir, "/control/delimiter");
	if(-1 != (delimfd = open(delimfn, O_RDONLY))){
		delimstr = mygetline(delimfd);
		close(delimfd);

		if(NULL == delimstr){
			log_error(LOG_ARGS,
				  "FATAL. Could not get list delimiter from %s",
				  delimfn);
			myfree(delimfn);
			exit(EXIT_FAILURE);
		}
	
		chomp(delimstr);
	
		if(0 == strlen(delimstr)){
			log_error(LOG_ARGS,
				  "FATAL. Zero-length delimiter found from %s",
				  delimfn);
			myfree(delimfn);
			exit(EXIT_FAILURE);
		}
	} else
		delimstr = mystrdup(DEFAULT_RECIPDELIM);

	myfree(delimfn);

	return delimstr;
}