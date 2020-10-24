void getaddrandtype(const char *listdir, const char *modstr,
		char **addrptr, enum subtype *subtypeptr)
{
	int fd;
	char *readaddr, *readtype, *modfilename;

	if (strncmp(modstr, "subscribe", 9) == 0)
			modstr += 9;

	modfilename = concatstr(3, listdir, "/moderation/subscribe", modstr);

	fd = open(modfilename, O_RDONLY);
	if(fd < 0) {
		log_error(LOG_ARGS, "Could not open %s", modfilename);
		exit(EXIT_FAILURE);
	}

	readaddr = mygetline(fd);
	readtype = mygetline(fd);

	close(fd);

	if(readaddr == NULL || readtype == NULL) {
		log_error(LOG_ARGS, "Could not parse %s", modfilename);
		exit(EXIT_FAILURE);
	}
	
	chomp(readaddr);
	*addrptr = readaddr;

	if(strncmp(readtype, "SUB_NORMAL", 10) == 0) {
		*subtypeptr = SUB_NORMAL;
		goto freedone;
	}

	if(strncmp(readtype, "SUB_DIGEST", 10) == 0) {
		*subtypeptr = SUB_DIGEST;
		goto freedone;
	}

	if(strncmp(readtype, "SUB_NOMAIL", 10) == 0) {
		*subtypeptr = SUB_NOMAIL;
		goto freedone;
	}

	if(strncmp(readtype, "SUB_BOTH", 8) == 0) {
		*subtypeptr = SUB_BOTH;
		goto freedone;
	}

	log_error(LOG_ARGS, "Type %s not valid in %s", readtype,
			modfilename);

freedone:
	myfree(readtype);
	unlink(modfilename);
	myfree(modfilename);
}