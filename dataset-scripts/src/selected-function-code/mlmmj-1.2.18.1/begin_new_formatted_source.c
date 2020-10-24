static void begin_new_formatted_source(text *txt, char **line_p, char **pos_p,
		int *width_p, char *suffix, formatted *fmt, int transparent) {
	char *line = *line_p;
	char *pos = *pos_p;
	const char *str;
	source *src;

	/* Save any later lines for use after finishing the source */
	while (*pos != '\0' && *pos != '\r' && *pos != '\n') pos++;
	if (*pos == '\r') pos++;
	if (*pos == '\n') pos++;
	if (*pos != '\0') {
		txt->src->upcoming = mystrdup(pos);
		txt->src->processedlen = 0;
		txt->src->processedwidth = 0;
	}

	(*fmt->rew)(fmt->state);

	src = mymalloc(sizeof(source));
	src->prev = txt->src;
	src->upcoming = NULL;
	if (*line == '\0') {
		src->prefix = NULL;
	} else {
		src->prefix = mystrdup(line);
	}
	src->prefixlen = strlen(line);
	src->prefixwidth = *width_p;
	if (*suffix == '\0' || *suffix == '\r' || *suffix == '\n') {
		src->suffix = NULL;
	} else {
		src->suffix = mystrdup(suffix);
	}
	src->fd = -1;
	src->fmt = fmt;
	src->transparent = transparent;
	src->limit = -1;
	txt->src = src;
	str = (*fmt->get)(fmt->state);
	if (str == NULL) {
		close_source(txt);
		**line_p = '\0';
		*pos_p = *line_p;
		*width_p = 0;
		return;
	}
	if (!transparent) str = unistr_escaped_to_utf8(str);
	line = concatstr(2, line, str);
	/* The suffix will be added back in get_processed_text_line() */
	*pos_p = line + strlen(*line_p);
	myfree(*line_p);
	*line_p = line;
}