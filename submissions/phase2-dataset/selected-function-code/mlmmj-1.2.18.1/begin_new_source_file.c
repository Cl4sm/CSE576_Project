static void begin_new_source_file(text *txt, char **line_p, char **pos_p,
		int *width_p, const char *filename, int transparent) {
	char *line = *line_p;
	char *pos = *pos_p;
	char *tmp, *esc;
	source *src;
	int fd;
	int i;

	/* Save any later lines for use after finishing the source */
	while (*pos != '\0' && *pos != '\r' && *pos != '\n') pos++;
	if (*pos == '\r') pos++;
	if (*pos == '\n') pos++;
	if (*pos != '\0') {
		txt->src->upcoming = mystrdup(pos);
		txt->src->processedlen = 0;
		txt->src->processedwidth = 0;
	}

	fd = open(filename, O_RDONLY);
	if (fd < 0) {
		/* Act as if the source were an empty line */
		**pos_p = '\0';
		return;
	}

	src = mymalloc(sizeof(source));
	src->prev = txt->src;
	src->upcoming = NULL;
	src->prefixlen = strlen(line);
	src->prefixwidth = *width_p;
	src->prefix = mymalloc((*width_p + 1) * sizeof(char));
	for (tmp = src->prefix, i = 0; i < *width_p; tmp++, i++) *tmp = ' ';
	*tmp = '\0';
	src->suffix = NULL;
	src->fd = fd;
	src->fmt = NULL;
	src->transparent = transparent;
	src->limit = -1;
	txt->src = src;
	tmp = mygetline(fd);
	if (tmp == NULL) {
		close_source(txt);
		**pos_p = '\0';
		return;
	}
	if (!transparent) {
		esc = unistr_escaped_to_utf8(tmp);
		myfree(tmp);
		tmp = esc;
	}
	line = concatstr(2, line, tmp);
	*pos_p = line + (*pos_p - *line_p);
	myfree(*line_p);
	*line_p = line;
	myfree(tmp);
}