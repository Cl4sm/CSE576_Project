int dumpcount(const char *filename, int *count)
{
	int fd;
	char *start, *next, *cur;
	struct stat st;
	size_t len;
	
	if((fd = open(filename, O_RDONLY)) < 0)
		return -1;

	if(stat(filename, &st) < 0)
		return -1;
	
	if(!S_ISREG(st.st_mode))
		return -1;
	
	/* Nobody there */
	if(st.st_size == 0) {
		return 0;
	}

	start = mmap(0, st.st_size, PROT_READ, MAP_SHARED, fd, 0);
	if(start == MAP_FAILED)
		return -1;

	for(next = cur = start; next < start + st.st_size; next++) {
		if(*next == '\n' || next == start + st.st_size - 1) {
			len = next - cur;
			if(next == start + st.st_size - 1 && *next != '\n')
				len++;
			if(count)
				(*count)++;
			else {
				writen(STDOUT_FILENO, cur, len);
				writen(STDOUT_FILENO, "\n", 1);
			}
			cur = next + 1;
		}
	}
	munmap(start, st.st_size);
	close(fd);

	return 0;
}