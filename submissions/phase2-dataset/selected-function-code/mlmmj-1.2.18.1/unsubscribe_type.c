static void unsubscribe_type(char *listdir, char *listaddr, char *listdelim,
		char *address, char *mlmmjsend,
		enum subtype typesub, enum subreason reasonsub) {
	char *subdir, *subddirname, *sublockname;
	char *subreadname = NULL, *subwritename;
	int subread, subwrite, rlock, wlock;
	int sublock, sublockfd;
	int groupwritable = 0;
	int unsubres;
	struct stat st;
	DIR *subddir;
	struct dirent *dp;
	off_t suboff;

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
		}
	}

	if((subddir = opendir(subddirname)) == NULL) {
		log_error(LOG_ARGS, "Could not opendir(%s)",
				    subddirname);
		myfree(subddirname);
		exit(EXIT_FAILURE);
	}

	myfree(subddirname);

	while((dp = readdir(subddir)) != NULL) {
		if(!strcmp(dp->d_name, "."))
			continue;
		if(!strcmp(dp->d_name, ".."))
			continue;
		
		subreadname = concatstr(3, listdir, subdir, dp->d_name);

		subread = open(subreadname, O_RDWR);
		if(subread == -1) {
			myfree(subreadname);
			continue;
		}

		suboff = find_subscriber(subread, address);
		if(suboff == -1) {
			close(subread);
			myfree(subreadname);
			continue;
		}

		/* create a .name.lock file and aquire the lock */
		sublockname = concatstr(5, listdir, subdir, ".", dp->d_name,
					".lock");
		sublockfd = open(sublockname, O_RDWR | O_CREAT,
						S_IRUSR | S_IWUSR);
		if(sublockfd < 0) {
			log_error(LOG_ARGS, "Error opening lock file %s",
					sublockname);
			myfree(sublockname);
			continue;
		}

		sublock = myexcllock(sublockfd);
		if(sublock < 0) {
			log_error(LOG_ARGS, "Error locking '%s' file",
					sublockname);
			myfree(sublockname);
			close(sublockfd);
			continue;
		}
		
		rlock = myexcllock(subread);
		if(rlock < 0) {
			log_error(LOG_ARGS, "Error locking '%s' file",
					subreadname);
			close(subread);
			close(sublockfd);
			myfree(subreadname);
			myfree(sublockname);
			continue;
		}

		subwritename = concatstr(2, subreadname, ".new");

		subwrite = open(subwritename, O_RDWR | O_CREAT | O_TRUNC,
				S_IRUSR | S_IWUSR | groupwritable);
		if(subwrite == -1){
			log_error(LOG_ARGS, "Could not open '%s'",
					subwritename);
			close(subread);
			close(sublockfd);
			myfree(subreadname);
			myfree(subwritename);
			myfree(sublockname);
			continue;
		}

		wlock = myexcllock(subwrite);
		if(wlock < 0) {
			log_error(LOG_ARGS, "Error locking '%s'",
					subwritename);
			close(subread);
			close(subwrite);
			close(sublockfd);
			myfree(subreadname);
			myfree(subwritename);
			myfree(sublockname);
			continue;
		}

		unsubres = unsubscribe(subread, subwrite, address);
		if(unsubres < 0) {
			close(subread);
			close(subwrite);
			close(sublockfd);
			unlink(subwritename);
			myfree(subreadname);
			myfree(subwritename);
			myfree(sublockname);
			continue;
		}

		if(unsubres > 0) {
			if(rename(subwritename, subreadname) < 0) {
				log_error(LOG_ARGS,
					"Could not rename '%s' to '%s'",
					subwritename, subreadname);
				close(subread);
				close(subwrite);
				myfree(subreadname);
				myfree(subwritename);
				continue;
			}
		} else { /* unsubres == 0, no subscribers left */
			unlink(subwritename);
			unlink(subreadname);
		}

		close(subread);
		close(subwrite);
		close(sublockfd);
		myfree(subreadname);
		myfree(subwritename);
		unlink(sublockname);
		myfree(sublockname);

        }

	closedir(subddir);
}