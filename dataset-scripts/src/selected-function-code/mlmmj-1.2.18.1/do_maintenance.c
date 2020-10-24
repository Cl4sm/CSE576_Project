void do_maintenance(const char *listdir, const char *mlmmjsend,
		    const char *mlmmjbounce, const char *mlmmjunsub)
{
	char *random, *logname, *logstr;
	char timenow[64];
	struct stat st;
	int maintdlogfd;
	uid_t uid = getuid();
	time_t t;

	if(!listdir)
		return;
	
	if(stat(listdir, &st) < 0) {
		log_error(LOG_ARGS, "Could not stat(%s) "
				    "No maintenance run performed.", listdir);
		return;
	}
	
	if(uid == 0) { /* We're root. Do something about it.*/
		if(setuid(st.st_uid) < 0) {
			log_error(LOG_ARGS, "Could not setuid listdir owner.");
			return;
		}
	} else if(uid != st.st_uid) {
		log_error(LOG_ARGS,
				"User ID not equal to the ID of %s. No "
				"maintenance run performed.", listdir);
		return;
	}

	if(chdir(listdir) < 0) {
		log_error(LOG_ARGS, "Could not chdir(%s). "
				    "No maintenance run performed.", listdir);
		return;
	}

	random = random_str();
	logname = concatstr(3, listdir, "/maintdlog-", random);
	myfree(random);

	maintdlogfd = open(logname, O_WRONLY|O_EXCL|O_CREAT, S_IRUSR|S_IWUSR);
	if(maintdlogfd < 0) {
		log_error(LOG_ARGS, "Could not open %s", logname);
		myfree(logname);
		return;
	}

	t = time(NULL);
	if(ctime_r(&t, timenow))
		WRITEMAINTLOG4(3, "Starting maintenance run at ",
				timenow, "\n");


	WRITEMAINTLOG4(3, "clean_moderation(", listdir, ");\n");
	clean_moderation(listdir);

	WRITEMAINTLOG4(3, "clean_discarded(", listdir, ");\n");
	clean_discarded(listdir);

	WRITEMAINTLOG4(3, "clean_subconf(", listdir, ");\n");
	clean_subconf(listdir);

	WRITEMAINTLOG4(3, "clean_unsubconf(", listdir, ");\n");
	clean_unsubconf(listdir);

	WRITEMAINTLOG6(5, "resend_queue(", listdir, ", ", mlmmjsend,
			");\n");
	resend_queue(listdir, mlmmjsend);

	WRITEMAINTLOG6(5, "resend_requeue(", listdir, ", ", mlmmjsend,
			");\n");
	resend_requeue(listdir, mlmmjsend);

	WRITEMAINTLOG4(3, "clean_nolongerbouncing(", listdir, ");\n");
	clean_nolongerbouncing(listdir);

	WRITEMAINTLOG6(5, "unsub_bouncers(", listdir, ", ",
			mlmmjunsub, ");\n");
	unsub_bouncers(listdir, mlmmjunsub);

	WRITEMAINTLOG6(5, "probe_bouncers(", listdir, ", ",
			mlmmjbounce, ");\n");
	probe_bouncers(listdir, mlmmjbounce);

	WRITEMAINTLOG6(5, "run_digests(", listdir, ", ", mlmmjsend,
			");\n");
	run_digests(listdir, mlmmjsend);

	close(maintdlogfd);

	logstr = concatstr(3, listdir, "/", MAINTD_LOGFILE);

	if(rename(logname, logstr) < 0)
		log_error(LOG_ARGS, "Could not rename(%s,%s)",
				logname, logstr);

	myfree(logname);
	myfree(logstr);
}