void parse_rcfile(const char *filename, rckeys *keys) {

	char	*p,*q;
	char	temp[128];
	char	*tokens = " :\t\n";
	FILE	*fp;
	int		i,key;

	fp = fopen(filename, "r");
	if (fp) {
		while (fgets(temp, 128, fp)) {
			key = 0;
			q = strdup(temp);
			q = strtok(q, tokens);
			while (key >= 0 && keys[key].label) {
				if ((!strcmp(q, keys[key].label))) {
					p = strstr(temp, keys[key].label);
					p += strlen(keys[key].label);
					p += strspn(p, tokens);
					if ((i = strcspn(p, "#\n"))) p[i] = 0;
					free(*keys[key].var);
					*keys[key].var = strdup(p);
					key = -1;
				} else key++;
			}
			free(q);
		}
		fclose(fp);
	}
}
