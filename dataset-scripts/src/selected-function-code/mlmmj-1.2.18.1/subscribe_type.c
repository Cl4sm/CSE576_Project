static void subscribe_type(char *listdir, char *listaddr, char *listdelim,
		char *address, char *mlmmjsend,
		enum subtype typesub, enum subreason reasonsub) {
	char *subfilename = NULL;
	char chstr[2], *subdir;
	char *subddirname = NULL, *sublockname;
	int groupwritable = 0, sublock, sublockfd, lock, subfilefd;
	struct stat st;
	size_t len;

	switch(typesub) {
		default:
		case SUB_NORMAL:
			subdir = "/subscribers.d/";
			break;
		case SUB_DIGEST:
			subdir = "/digesters.d/";
			break;
		case SUB_NOMAIL:
			subdir = "/nomailsubs.d/";
			break;
	}

	subddirname = concatstr(2, listdir, subdir);
	if (stat(subddirname, &st) == 0) {
		if(st.st_mode & S_IWGRP) {
			groupwritable = S_IRGRP|S_IWGRP;
			umask(S_IWOTH);
			setgid(st.st_gid);
		}
	}

	chstr[0] = address[0];
	chstr[1] = '\0';
	
	subfilename = concatstr(3, listdir, subdir, chstr);

	sublockname = concatstr(5, listdir, subdir, ".", chstr, ".lock");
	sublockfd = open(sublockname, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	if(sublockfd < 0) {
		log_error(LOG_ARGS, "Error opening lock file %s",
				sublockname);
		myfree(sublockname);
		exit(EXIT_FAILURE);
	}

	sublock = myexcllock(sublockfd);
	if(sublock < 0) {
		log_error(LOG_ARGS, "Error locking '%s' file",
				sublockname);
		myfree(sublockname);
		close(sublockfd);
		exit(EXIT_FAILURE);
	}

	subfilefd = open(subfilename, O_RDWR|O_CREAT,
				S_IRUSR|S_IWUSR|groupwritable);
	if(subfilefd == -1) {
		log_error(LOG_ARGS, "Could not open '%s'", subfilename);
		myfree(sublockname);
		exit(EXIT_FAILURE);
	}

	lock = myexcllock(subfilefd);
	if(lock) {
		log_error(LOG_ARGS, "Error locking subscriber file");
		close(subfilefd);
		close(sublockfd);
		myfree(sublockname);
		exit(EXIT_FAILURE);
	}

	lseek(subfilefd, 0L, SEEK_END);
	len = strlen(address);
	address[len] = '\n';
	writen(subfilefd, address, len + 1);
	address[len] = 0;
	close(subfilefd);
	close(sublockfd);
	unlink(sublockname);
	myfree(sublockname);
}