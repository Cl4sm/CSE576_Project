const char *get_memory_line(void *state)
{
	memory_lines_state *s = (memory_lines_state *)state;
	char *line, *pos;

	if (s == NULL) return NULL;

	if (s->pos != NULL) *s->pos++ = '\n';
	else s->pos = s->lines;

	line = s->pos;
	pos = line;

	if (*pos == '\0') {
		s->pos = NULL;
		return NULL;
	}

	while (*pos != '\n') pos++;
	*pos = '\0';

	s->pos = pos;
	return line;
}