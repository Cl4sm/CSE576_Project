int
dump(void)
{
	register int fd;
	char oldarpfile[256], newarpfile[256];

	(void)sprintf(oldarpfile, "%s-", arpfile);
	(void)sprintf(newarpfile, "%s.new", arpfile);

	if ((fd = creat(newarpfile, 0644)) < 0) {
		syslog(LOG_ERR, "creat(%s): %m", newarpfile);
		return(0);
	}
	if ((dumpf = fdopen(fd, "w")) == NULL) {
		syslog(LOG_ERR, "fdopen(%s): %m", newarpfile);
		return(0);
	}

	(void)ent_loop(dumpone);
	if (ferror(dumpf)) {
		syslog(LOG_ERR, "ferror %s: %m", newarpfile);
		return(0);
	}

	(void)fclose(dumpf);
	if (rename(arpfile, oldarpfile) < 0) {
		syslog(LOG_ERR, "rename %s -> %s: %m", arpfile, oldarpfile);
		return(0);
	}
	if (rename(newarpfile, arpfile) < 0) {
		syslog(LOG_ERR, "rename %s -> %s: %m", newarpfile, arpfile);
		return(0);
	}
	return(1);
}