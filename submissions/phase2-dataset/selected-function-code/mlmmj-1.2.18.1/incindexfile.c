int incindexfile(const char *listdir)
{
	int fd, lock;
	long int index = 0;
	char intbuf[INTBUF_SIZE] = "uninitialized";
	size_t i;
	char *indexfilename;

	indexfilename = concatstr(2, listdir, "/index");
	fd = open(indexfilename, O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);

	if(fd == -1) {
		myfree(indexfilename);
		log_error(LOG_ARGS, "Error opening index file");
		return 0;
	}

	lock = myexcllock(fd);
	
	if(lock) {
		myfree(indexfilename);
		log_error(LOG_ARGS, "Error locking index file");
		close(fd);
		return 0;
	}

	readn(fd, intbuf, INTBUF_SIZE);
	for(i=0; i<sizeof(intbuf); i++) {
		if(intbuf[i] < '0' || intbuf[i] > '9') {
			intbuf[i] = '\0';
			break;
		}
	}
	index = atol(intbuf);
	index++;
	itoa(index, intbuf);
	lseek(fd, 0, SEEK_SET);
	writen(fd, intbuf, strlen(intbuf));

	close(fd); /* Lock is also released */
	myfree(indexfilename);

	return index;
}