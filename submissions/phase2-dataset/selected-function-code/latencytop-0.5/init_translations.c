{
	char *c, *c2;
	FILE *file;
	
	file = fopen(filename, "r");
	if (!file)
		return;
	while (!feof(file)) {
		struct translate_line *trans;
		size_t dummy;
		char *line = NULL;
		if (getline(&line, &dummy, file) <= 0) {
			free(line);
			break;
		}
		if (line[0]=='#') {
			free(line);
			continue;
		}

		trans = malloc(sizeof(struct translate_line));
		memset(trans, 0, sizeof(trans));

		c2 = line;
		c = strchr(c2, '\t');
		if (c) {
			*c = 0;
			trans->priority = strtoull(c2, NULL, 10);
			c2 = c+1;
		}

		c = strchr(c2, '\t');
		if (c) {
			*c = 0;
			strcpy(trans->function, c2);
			c2 = c+1;
		}
		while (*c2=='\t') c2++;
		strcpy(trans->display, c2);
		translations = g_list_append(translations, trans);
		free(line);
	}
	fclose(file);
}
