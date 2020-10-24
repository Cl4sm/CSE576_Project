uev_discard(char * devpath)
{
	char *tmp;
	char a[11], b[11];

	/*
	 * keep only block devices, discard partitions
	 */
	tmp = strstr(devpath, "/block/");
	if (tmp == NULL){
		condlog(4, "no /block/ in '%s'", devpath);
		return 1;
	}
	if (sscanf(tmp, "/block/%10s", a) != 1 ||
	    sscanf(tmp, "/block/%10[^/]/%10s", a, b) == 2) {
		condlog(4, "discard event on %s", devpath);
		return 1;
	}
	return 0;
}
