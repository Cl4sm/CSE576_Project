int bouncemail(const char *listdir, const char *mlmmjbounce, const char *from)
{
	char *myfrom = mystrdup(from);
	char *listdelim = getlistdelim(listdir);
	char *addr, *num, *c;
	size_t len;
	pid_t pid = 0;

	if((c = strchr(myfrom, '@')) == NULL) {
		myfree(myfrom);
		myfree(listdelim);
		return 0; /* Success when malformed 'from' */
	}
	*c = '\0';
	num = strrchr(myfrom, '-');
	num++;
	c = strstr(myfrom, listdelim);
	myfrom = strchr(c, '-');
	myfrom++;
	len = num - myfrom - 1;
	addr = mymalloc(len + 1);
	addr[len] = '\0';
	strncpy(addr, myfrom, len);

	myfree(listdelim);

	pid = fork();
	
	if(pid < 0) {
		log_error(LOG_ARGS, "fork() failed!");
		return 1;
	}
	
	if(pid > 0)
		return 0;
	
	execlp(mlmmjbounce, mlmmjbounce,
			"-L", listdir,
			"-a", num,
			"-n", addr, (char *)NULL);

	log_error(LOG_ARGS, "execlp() of '%s' failed", mlmmjbounce);

	return 1;
}