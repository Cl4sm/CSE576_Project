iow_t *stdio_wopen(const char *filename,int flags)
{
	iow_t *iow = malloc(sizeof(iow_t));
	iow->source = &stdio_wsource;
	iow->data = malloc(sizeof(struct stdiow_t));

	if (strcmp(filename,"-") == 0) 
		DATA(iow)->fd = 1; /* STDOUT */
	else {
		DATA(iow)->fd = safe_open(filename, flags);
	}

	if (DATA(iow)->fd == -1) {
		free(iow);
		return NULL;
	}

	DATA(iow)->offset = 0;

	return iow;
}