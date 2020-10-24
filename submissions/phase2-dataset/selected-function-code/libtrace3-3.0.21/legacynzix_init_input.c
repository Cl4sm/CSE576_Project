static int legacynzix_init_input(libtrace_t *libtrace) {

	int retval;
	char *filename = libtrace->uridata;
	regex_t reg;
	regmatch_t match;


	legacy_init_format_data(libtrace);	
	
	/* Check that the filename appears to contain a suitable timestamp.
	 * Without it, we have no way of determining the actual timestamps
	 * for each packet */
	if((retval = regcomp(&reg, "[0-9]{8}-[0-9]{6}", REG_EXTENDED)) != 0) {
		trace_set_err(libtrace, errno, "Failed to compile regex");
		return -1;
	}
	if ((retval = regexec(&reg, filename, 1, &match, 0)) !=0) {
		trace_set_err(libtrace, errno, "Failed to exec regex");
		return -1;
	}
	DATA(libtrace)->starttime = trtime(&filename[match.rm_so]);
	return 0;
}