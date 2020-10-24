int resend_queue(const char *listdir, const char *mlmmjsend)
{
	DIR *queuedir;
	struct dirent *dp;
	char *mailname, *fromname, *toname, *reptoname, *from, *to, *repto;
	char *ch, *dirname = concatstr(2, listdir, "/queue/");
	char *bouncelifestr;
	pid_t childpid, pid;
	struct stat st;
	int fromfd, tofd, fd, status, err = 0;
	time_t t, bouncelife = 0;

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
	myfree(dirname);

	while((dp = readdir(queuedir)) != NULL) {
		mailname = concatstr(3, listdir, "/queue/", dp->d_name);

		if(stat(mailname, &st) < 0) {
			log_error(LOG_ARGS, "Could not stat(%s)", mailname);
			myfree(mailname);
			continue;
		}

		if(!S_ISREG(st.st_mode)) {
			myfree(mailname);
			continue;
		}

		if(strchr(dp->d_name, '.')) {
			ch = strrchr(mailname, '.');
			MY_ASSERT(ch);
			*ch = '\0';
			/* delete orphaned sidecar files */
			if(stat(mailname, &st) < 0) {
				if(errno == ENOENT) {
					*ch = '.';
					unlink(mailname);
				}
			}
			myfree(mailname);
			continue;
		}

		fromname = concatstr(2, mailname, ".mailfrom");
		toname = concatstr(2, mailname, ".reciptto");
		reptoname = concatstr(2, mailname, ".reply-to");

		fromfd = open(fromname, O_RDONLY);
		if(fromfd < 0)
			err = errno;
		tofd = open(toname, O_RDONLY);

		if((fromfd < 0 && err == ENOENT) ||
				(tofd < 0 && errno == ENOENT)) {
			/* only delete old files to avoid deleting
			   mail currently being sent */
			t = time(NULL);
			if(stat(mailname, &st) == 0) {
				if(t - st.st_mtime > (time_t)36000) {
					unlink(mailname);
					/* avoid leaving orphans */
					unlink(fromname);
					unlink(toname);
					unlink(reptoname);
				}
			}
			myfree(mailname);
			myfree(fromname);
			myfree(toname);
			myfree(reptoname);
			if(fromfd >= 0)
				close(fromfd);
			if(tofd >= 0)
				close(tofd);
			continue;
		}

		from = mygetline(fromfd);
		chomp(from);
		close(fromfd);
		myfree(fromname);
		to = mygetline(tofd);
		chomp(to);
		close(tofd);
		myfree(toname);
		fd = open(reptoname, O_RDONLY);
		if(fd < 0) {
			myfree(reptoname);
			repto = NULL;
		} else {
			repto = mygetline(fd);
			chomp(repto);
			close(fd);
			myfree(reptoname);
		}

		/* before we try again, check and see if it's old */
		bouncelifestr = ctrlvalue(listdir, "bouncelife");
		if(bouncelifestr) {
			bouncelife = atol(bouncelifestr);
			myfree(bouncelifestr);
		}
		if(bouncelife == 0)
			bouncelife = BOUNCELIFE;

		t = time(NULL);
		if(t - st.st_mtime > bouncelife) {
			unlink(mailname);
			myfree(mailname);
			myfree(from);
			myfree(to);
			myfree(repto);
			continue;
		}

		childpid = fork();

		if(childpid < 0) {
			myfree(mailname);
			myfree(from);
			myfree(to);
			myfree(repto);
			log_error(LOG_ARGS, "Could not fork");
			continue;
		}

		if(childpid > 0) {
			myfree(mailname);
			myfree(from);
			myfree(to);
			myfree(repto);
			do /* Parent waits for the child */
			      pid = waitpid(childpid, &status, 0);
			while(pid == -1 && errno == EINTR);
		} else {
			if(repto) {
				execlp(mlmmjsend, mlmmjsend,
						"-l", "1",
						"-L", listdir,
						"-m", mailname,
						"-F", from,
						"-T", to,
						"-R", repto,
						"-a", (char *)NULL);
			} else {
				execlp(mlmmjsend, mlmmjsend,
						"-l", "1",
						"-L", listdir,
						"-m", mailname,
						"-F", from,
						"-T", to,
						"-a", (char *)NULL);
			}
			log_error(LOG_ARGS, "Could not execlp %s",
						mlmmjsend);
			/* This is the child. Exit on failure. */
			exit(EXIT_FAILURE);
		}
	}

	closedir(queuedir);

	return 0;
}