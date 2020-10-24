char *getlistaddr(const char *listdir)
{
	char *tmpstr;
	int listnamefd;

	tmpstr = concatstr(2, listdir, "/control/listaddress");;
	if((listnamefd = open(tmpstr, O_RDONLY)) < 0) {
		log_error(LOG_ARGS, "Could not open '%s'", tmpstr);
		exit(EXIT_FAILURE);
	}
	myfree(tmpstr);

	tmpstr = mygetline(listnamefd);

	if(tmpstr == NULL){
		log_error(LOG_ARGS, "FATAL. Could not get listaddress "
				    "in %s/control/listaddress", listdir);
		exit(EXIT_FAILURE);
	}

	if(strchr(tmpstr, '@') == NULL) {
		log_error(LOG_ARGS, "FATAL. No @ sign in listaddress");
		exit(EXIT_FAILURE);
	}

	chomp(tmpstr);
	close(listnamefd);

	return tmpstr;
}