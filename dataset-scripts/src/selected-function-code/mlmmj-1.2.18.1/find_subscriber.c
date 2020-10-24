off_t find_subscriber(int fd, const char *address)
{
	char *start, *cur, *next;
	struct stat st;
	size_t len;

	if(fstat(fd, &st) < 0) {
		log_error(LOG_ARGS, "Could not stat fd");
		return (off_t)-1;
	}

	/* No need to check in 0-size file */
	if(st.st_size == 0)
		return (off_t)-1;

	if(!S_ISREG(st.st_mode)) {
		log_error(LOG_ARGS, "Non regular file in subscribers.d/");
		return (off_t)-1;
	}

	if((start = mmap(0, st.st_size, PROT_READ, MAP_SHARED, fd, 0)) ==
			MAP_FAILED) {
		log_error(LOG_ARGS, "Could not mmap fd");
		return (off_t)-1;
	}
	
	for(next = cur = start; next < start + st.st_size; next++) {
		if(*next == '\n') {
			len = next - cur;
			if((strlen(address) == len) &&
			   (strncasecmp(address, cur, len) == 0)) {
				munmap(start, st.st_size);
				return (off_t)(cur - start);
			}
			cur = next + 1;
		}
	}
	
	if(next > cur) {
		len = next - cur;
		if((strlen(address) == len) &&
		   (strncasecmp(address, cur, len) == 0)) {
			munmap(start, st.st_size);
			return (off_t)(cur - start);
		}
	}
	
	munmap(start, st.st_size);
	return (off_t)-1;
}