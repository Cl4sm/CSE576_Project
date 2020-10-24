				int head, int sectors)
{
	int n,m;
	if (lseek(fd->fd, (cylinder * heads + head) * sectors * 512,
		  SEEK_SET) < 0) {
		perror("lseek");
		return -1;
	}
	m = sectors * 512;
	while(m>0) {
		/* read until we have read everything we should */
		n=read(fd->fd, data, m);
		if ( n < 0 ) {
			perror("read");
			return -1;
		}
		if(n== 0) {
			fprintf(stderr, "Error, %d bytes remaining\n", m);
			return -1;
		}
		m -= n;
	}
	return 0;
}
