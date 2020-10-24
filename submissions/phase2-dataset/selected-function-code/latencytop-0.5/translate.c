char *translate(char *line)
{
	char buffer[4096], *c, *c2;
	int prio = 0;
	char *selected = NULL;
	GList *item;
	struct translate_line *trans;

	memset(buffer, 0, 4096);	
	strncpy(buffer, line, 4096);

	c2 = buffer;
	while (c2[0] == ' ') c2++;

	while (c2 && strlen(c2) > 0) {

		c = strchr(c2, ' ');
		if (c) *c = 0;

		item = g_list_first(translations);
		while (item) {
			trans = item->data;
			item = g_list_next(item);
			if (trans->priority >= prio && strcmp(trans->function, c2)==0) {
				selected = trans->display;
				prio = trans->priority;
			}
		}

		if (c)
			c2 = c+1;
		else
			c2 = NULL;
	}
	if (dump_unknown && prio < 2) {
		FILE *file;
		file = fopen("latencytop.log", "w+");
		if (file) {
			fprintf(file, "%s\n", line);
			fclose(file);
		}
		printf("Unknown: %s\n", line);
	}
	return selected;
}
