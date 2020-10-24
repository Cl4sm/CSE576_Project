const char *get_file_line(void *state)
{
	file_lines_state *s = (file_lines_state *)state;
	char *end;
	if (s == NULL) return NULL;
	if (s->line != NULL) {
		myfree(s->line);
		s->line = NULL;
	}
	if (s->fd >= 0) {
		s->line = mygetline(s->fd);
		if (s->line == NULL) return NULL;
		if (s->truncate != '\0') {
			end = strchr(s->line, s->truncate);
			if (end == NULL) return NULL;
			*end = '\0';
		} else {
			chomp(s->line);
		}
		return s->line;
	}
	return NULL;
}