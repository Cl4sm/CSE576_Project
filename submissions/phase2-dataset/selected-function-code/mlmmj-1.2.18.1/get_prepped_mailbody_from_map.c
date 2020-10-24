char *get_prepped_mailbody_from_map(char *mapstart, size_t size, size_t *blen)
{
	char *cur, *next, *endhdrs, *retstr, *r;
	char newlinebuf[3];
	size_t bodylen, len, n = 0;

	endhdrs = strstr(mapstart, "\n\n");
	if(endhdrs == NULL)
		return NULL; /* The map doesn't map a file with a mail */

	endhdrs++; /* Skip the first newline, it's in hdrs */

	bodylen = size - (endhdrs - mapstart);
	
	for(next = cur = endhdrs; next < mapstart + size; next++) {
		if(*next == '\n') {
			n++;
			if((next < mapstart + size - 1) && *(next+1) == '.')
				n++;
		}
	}

	retstr = mymalloc(bodylen + n);
	*blen = bodylen + n;
	r = retstr;	
	
	for(next = cur = endhdrs; next < mapstart + size; next++) {
		if(*next == '\n') {
			strncpy(r, cur, next - cur);
			r += next - cur;
			newlinebuf[0] = '\r';
			newlinebuf[1] = '\n';
			len = 2;
			if((next < mapstart + size - 1) && *(next+1) == '.') {
				newlinebuf[2] = '.';
				len = 3;
			}
			strncpy(r, newlinebuf, len);
			r += len;
			cur = next + 1;
		}
	}

	return retstr;
}