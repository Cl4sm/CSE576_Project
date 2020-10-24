int unsub_bouncers(const char *listdir, const char *mlmmjunsub)
{
	DIR *bouncedir;
	char *dirname = concatstr(2, listdir, "/bounce/");
	char *probefile, *address, *a, *firstbounce, *bouncedata;
	char *bouncelifestr;
	struct dirent *dp;
	struct stat st;
	pid_t pid, childpid;
	int status, fd;
	time_t bouncetime, t, bouncelife = 0;
	
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

	bouncelifestr = ctrlvalue(listdir, "bouncelife");
	if(bouncelifestr) {
		bouncelife = atol(bouncelifestr);
		myfree(bouncelifestr);
	}
	
	if(bouncelife == 0)
		bouncelife = BOUNCELIFE;

	while((dp = readdir(bouncedir)) != NULL) {
		if((strcmp(dp->d_name, "..") == 0) ||
		   (strcmp(dp->d_name, ".") == 0))
				continue;

		a = strrchr(dp->d_name, '-');
		if(a && (strcmp(a, "-probe") == 0))
			continue;

		a = strrchr(dp->d_name, '.');
		if(a && (strcmp(a, ".lastmsg") == 0))
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

		/* Get the first line of the bounce file to check if it's
		 * been bouncing for long enough
		 */
		fd = open(dp->d_name, O_RDONLY);
		if(fd < 0) {
			log_error(LOG_ARGS, "Could not open %s", dp->d_name);
			continue;
		}
		firstbounce = mygetline(fd);
		close(fd);
		if(firstbounce == NULL)
			continue;

		/* End the string at the comment */
		a = strchr(firstbounce, '#');
		if(a == NULL) {
			myfree(firstbounce);
			continue;
		}
		*a = '\0';
		bouncedata = mystrdup(a+1); /* Save for the log */
		chomp(bouncedata);
		a = strchr(firstbounce, ':');
		if(a == NULL) {
			myfree(firstbounce);
			myfree(bouncedata);
			continue;
		}

		a++; /* Increase to first digit */
		bouncetime = (time_t)strtol(a, NULL, 10);
		myfree(firstbounce);
		t = time(NULL);
		if(t - bouncetime < bouncelife + WAITPROBE) {
			myfree(bouncedata);
			continue; /* ok, don't unsub this one */
		}
		
		/* Ok, go ahead and unsubscribe the address */
		address = mystrdup(dp->d_name);
		a = strchr(address, '=');
		if(a == NULL) { /* skip malformed */
			myfree(address);
			myfree(bouncedata);
			continue;
		}
		*a = '@';

		childpid = fork();
		
		if(childpid < 0) {
			log_error(LOG_ARGS, "Could not fork");
			myfree(address);
			myfree(bouncedata);
			continue;
		}

		if(childpid > 0) {
			log_oper(listdir, OPLOGFNAME, "mlmmj-maintd: %s"
					" unsubscribed due to bouncing since"
					" %s", address, bouncedata);
			myfree(address);
			myfree(bouncedata);
			do /* Parent waits for the child */
				pid = waitpid(childpid, &status, 0);
			while(pid == -1 && errno == EINTR);
			unlink(dp->d_name);
			a = concatstr(2, dp->d_name, ".lastmsg");
			unlink(a);
			myfree(a);
		} else {
			execlp(mlmmjunsub, mlmmjunsub,
					"-L", listdir,
					"-b", "-a", address, (char *)NULL);
			log_error(LOG_ARGS, "Could not execlp %s",
						mlmmjunsub);
			/* This is the child. Exit on failure. */
			exit(EXIT_FAILURE);
		}
	}
	closedir(bouncedir);

	return 0;
}