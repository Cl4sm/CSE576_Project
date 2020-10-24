void
dumpHex(const char* str, int len, int log)
{
	const char * p = str;
	unsigned char c;
	char buff[82];
	const int bpstart = 5;
	int bpos = bpstart;
	int  k;

	if (len <= 0)
		return;
	memset(buff, ' ', 80);
	buff[80] = '\0';
	for (k = 0; k < len; k++) {
		c = *p++;
		bpos += 3;
		if (bpos == (bpstart + (9 * 3)))
			bpos++;
		sprintf(&buff[bpos], "%.2x", (int)(unsigned char)c);
		buff[bpos + 2] = ' ';
		if ((k > 0) && (0 == ((k + 1) % 16))) {
			if (log)
				condlog(0, "%.76s" , buff);
			else
				printf("%.76s" , buff);
			bpos = bpstart;
			memset(buff, ' ', 80);
		}
	}
	if (bpos > bpstart) {
		buff[bpos + 2] = '\0';
		if (log)
			condlog(0, "%s", buff);
		else
			printf("%s\n" , buff);
	}
	return;
}
