char *get_preppedhdrs_from_map(char *mapstart, size_t *hlen)
{
	char *cur, *next, *endhdrs, *retstr, *r;
	const char newlinebuf[] = "\r\n";
	size_t hdrlen, n = 0;

	endhdrs = strstr(mapstart, "\n\n");
	if(endhdrs == NULL)
		return NULL; /* The map doesn't map a file with a mail */

	hdrlen = endhdrs - mapstart + 1;

	for(next = cur = mapstart; next < mapstart + hdrlen; next++)
		if(*next == '\n')
			n++;

	retstr = mymalloc(hdrlen + n);
	*hlen = hdrlen + n;	
	r = retstr;	

	for(next = cur = mapstart; next < mapstart + hdrlen; next++) {
		if(*next == '\n') {
			strncpy(r, cur, next - cur);
			r += next - cur;
			strncpy(r, newlinebuf, 2);
			r += 2;
			cur = next + 1;
		}
	}

	return retstr;
}