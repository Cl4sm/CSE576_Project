static char *mygetuntil(int fd, int eof)
{
	size_t i = 0, res, buf_size = BUFSIZE;  /* initial buffer size */
	char *buf, ch;

	buf = mymalloc(buf_size);
	buf[0] = '\0';
	while(1) {	
		res = read(fd, &ch, 1);
		if(res < 0) {
			if(errno == EINTR)
				continue;
			else {
				myfree(buf);
				return NULL;
			}
		}
		if(res == 0) {
			if(buf[0]) {
				buf[i] = '\0';
				return buf;
			} else {
				myfree(buf);
				return NULL;
			}
		}

		if(i == buf_size - 1) {
			buf_size *= 2;
			buf = myrealloc(buf, buf_size);
		}
		buf[i++] = ch;
		if(eof != EOF && ch == eof) {
			buf[i] = '\0';
			return buf;
		}
	}
}