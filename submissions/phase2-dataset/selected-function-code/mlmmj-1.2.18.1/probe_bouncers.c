int probe_bouncers(const char *listdir, const char *mlmmjbounce)
{
	DIR *bouncedir;
	char *dirname = concatstr(2, listdir, "/bounce/");
	char *probefile, *s;
	struct dirent *dp;
	struct stat st;
	pid_t pid, childpid;
	int status;
	
	if(chdir(dirname) < 0) {
		log_error(LOG_ARGS, "Could not chdir(%s)", dirname);
		myfree(dirname);
		return 1;
	}
		
	if((bouncedir = opendir(dirname)) == NULL) {
		log_error(LOG_ARGS, "Could not opendir(%s)", dirname);
		myfree(dirname);
		return 1;
	}

	myfree(dirname);

	while((dp = readdir(bouncedir)) != NULL) {
		if((strcmp(dp->d_name, "..") == 0) ||
		   (strcmp(dp->d_name, ".") == 0))
				continue;

		s = strrchr(dp->d_name, '-');
		if(s && (strcmp(s, "-probe") == 0))
			continue;

		s = strrchr(dp->d_name, '.');
		if(s && (strcmp(s, ".lastmsg") == 0))
			continue;
			
		if(stat(dp->d_name, &st) < 0) {
			log_error(LOG_ARGS, "Could not stat(%s)", dp->d_name);
			continue;
		}
		
		probefile = concatstr(2, dp->d_name, "-probe");
		
		/* Skip files which already have a probe out */
		if(stat(probefile, &st) == 0) {
			myfree(probefile);
			continue;
		}
		myfree(probefile);

		childpid = fork();
		
		if(childpid < 0) {
			log_error(LOG_ARGS, "Could not fork");
			continue;
		}

		if(childpid > 0) {
			do /* Parent waits for the child */
				pid = waitpid(childpid, &status, 0);
			while(pid == -1 && errno == EINTR);
		} else {
			probefile = mystrdup(dp->d_name);
			execlp(mlmmjbounce, mlmmjbounce,
					"-L", listdir,
					"-a", probefile,
					"-p", (char *)NULL);
			log_error(LOG_ARGS, "Could not execlp %s",
						mlmmjbounce);
			/* This is the child. Exit on failure. */
			exit(EXIT_FAILURE);
		}
	}
	closedir(bouncedir);

	return 0;
}