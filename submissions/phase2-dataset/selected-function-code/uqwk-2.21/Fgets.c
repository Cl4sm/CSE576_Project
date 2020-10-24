char *Fgets (c, n, fd)
char *c;
int n;
FILE *fd;
/*
 *  Same as fgets, but changes trailing linefeed to a null
 */
{
	int i;

	char  *bufptr = c;
	size_t bufsiz = n;

	/*
	 * This bit of code reallocs dynamically -- it will fail
	 * if it is tries to reallocate non-reallocatable memory
	 */
	if (NULL == fgets (c, n-1, fd)) return (NULL);
	i = strlen (c);
	if ( i == n-1 ) {	/* n-1 bytes read! more? */
		i = n;
		while (i == n) {
			if((c = (char *)realloc(c, bufsiz + n )) == NULL)
				OutOfMemory();
			bufptr = c + bufsiz - 1; bufsiz += n;
			fgets (bufptr, n+1, fd);
			i = strlen (c);
		}
	}
	i = strlen (c);
	if ( (i > 0) && (c[i-1]=='\n') ) c[i-1] = 0;
	if ( (i > 1) && (c[i-2]=='\r') ) c[i-2] = 0;

	return (c);
}
