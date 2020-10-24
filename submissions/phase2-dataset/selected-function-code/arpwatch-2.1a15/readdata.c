int
readdata(void)
{
	register FILE *f;

	if ((f = fopen(arpfile, "r")) == NULL) {
		syslog(LOG_ERR, "fopen(%s): %m", arpfile);
		return(0);
	}
	if (!file_loop(f, ent_add, arpfile)) {
		(void)fclose(f);
		return(0);
	}
	(void)fclose(f);

	/* It's not fatal if we can't open the ethercodes file */
	if ((f = fopen(ethercodes, "r")) != NULL) {
		(void)ec_loop(f, ec_add, ethercodes);
		(void)fclose(f);
	}

	return(1);
}