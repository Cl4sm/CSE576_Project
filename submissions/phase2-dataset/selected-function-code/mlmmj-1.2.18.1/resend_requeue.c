int resend_requeue(const char *listdir, const char *mlmmjsend)
{
	DIR *queuedir;
	struct dirent *dp;
	char *dirname = concatstr(2, listdir, "/requeue/");
	char *archivefilename, *subfilename, *subnewname;
	struct stat st;
	pid_t childpid, pid;
	time_t t;
	int status, fromrequeuedir;

	if(chdir(dirname) < 0) {
		log_error(LOG_ARGS, "Could not chdir(%s)", dirname);
		myfree(dirname);
		return 1;
	}
		
	if((queuedir = opendir(dirname)) == NULL) {
		log_error(LOG_ARGS, "Could not opendir(%s)", dirname);
		myfree(dirname);
		return 1;
	}
	
	while((dp = readdir(queuedir)) != NULL) {
		if((strcmp(dp->d_name, "..") == 0) ||
			(strcmp(dp->d_name, ".") == 0))
				continue;

		if(stat(dp->d_name, &st) < 0) {
			log_error(LOG_ARGS, "Could not stat(%s)",dp->d_name);
			continue;
		}

		if(!S_ISDIR(st.st_mode))
			continue;

		/* Remove old empty directories */
		t = time(NULL);
		if(t - st.st_mtime > (time_t)3600)
			if(rmdir(dp->d_name) == 0)
				continue;

		archivefilename = concatstr(3, listdir, "/archive/",
						dp->d_name);

		/* Explicitly initialize for each mail we examine */
		fromrequeuedir = 0;

		if(stat(archivefilename, &st) < 0) {
			/* Might be it's just not moved to the archive
			 * yet because it's still getting sent, so just
			 * continue
			 */
			myfree(archivefilename);

			/* If the list is set not to archive we want to look
			 * in /requeue/ for a mailfile
			 */
			archivefilename = concatstr(4, listdir, "/requeue/",
							dp->d_name, "/mailfile");
			if(stat(archivefilename, &st) < 0) {
				myfree(archivefilename);
				continue;
			}
			fromrequeuedir = 1;
		}
		subfilename = concatstr(3, dirname, dp->d_name, "/subscribers");
		if(stat(subfilename, &st) < 0) {
			if (fromrequeuedir)
				unlink(archivefilename);
			myfree(archivefilename);
			myfree(subfilename);
			continue;
		}

		subnewname = concatstr(2, subfilename, ".resending");

		if(rename(subfilename, subnewname) < 0) {
			log_error(LOG_ARGS, "Could not rename(%s, %s)",
						subfilename, subnewname);
			myfree(archivefilename);
			myfree(subfilename);
			myfree(subnewname);
			continue;
		}
		myfree(subfilename);
		
		childpid = fork();

		if(childpid < 0) {
			myfree(archivefilename);
			myfree(subnewname);
			log_error(LOG_ARGS, "Could not fork");
			continue;
		}

		if(childpid > 0) {
			myfree(archivefilename);
			myfree(subnewname);
			do /* Parent waits for the child */
			      pid = waitpid(childpid, &status, 0);
			while(pid == -1 && errno == EINTR);
		} else {
			execlp(mlmmjsend, mlmmjsend,
					"-l", "3",
					"-L", listdir,
					"-m", archivefilename,
					"-s", subnewname,
					"-a",
					"-D", (char *)NULL);
			log_error(LOG_ARGS, "Could not execlp %s",
						mlmmjsend);
			/* This is the child. Exit on failure. */
			exit(EXIT_FAILURE);
		}
	}

	closedir(queuedir);

	myfree(dirname);

	return 0;
}