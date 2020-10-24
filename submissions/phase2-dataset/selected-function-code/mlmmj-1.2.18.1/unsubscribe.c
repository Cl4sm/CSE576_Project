ssize_t unsubscribe(int subreadfd, int subwritefd, const char *address)
{
	off_t suboff = find_subscriber(subreadfd, address);
	struct stat st;
	char *inmap;
	size_t len = strlen(address) + 1; /* + 1 for the '\n' */
	ssize_t writeres = 0, written = 0;
	
	if(suboff == -1)
		return -1; /* Did not find subscriber */

	if(fstat(subreadfd, &st) < 0) {
		log_error(LOG_ARGS, "Could not stat subreadfd");
		return -1;
	}

	if((inmap = mmap(0, st.st_size, PROT_READ, MAP_SHARED,
		       subreadfd, 0)) == MAP_FAILED) {
		log_error(LOG_ARGS, "Could not mmap subreadfd");
		return -1;
	}

	if(suboff > 0) {
		writeres = writen(subwritefd, inmap, suboff);
		if(writeres < 0)
			return -1;
	}
	written += writeres;
	
	writeres = writen(subwritefd, inmap + len + suboff,
				st.st_size - len - suboff);
	if(writeres < 0)
		return -1;

	written += writeres;

	munmap(inmap, st.st_size);

	return written;
}