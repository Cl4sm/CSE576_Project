void parse_rcfile2(const char *filename, rckeys2 *keys) {

	char	*p;
	char	temp[128];
	char	*tokens = " :\t\n";
	FILE	*fp;
	int		i,key;
	char	*family = NULL;

	fp = fopen(filename, "r");
	if (fp) {
		while (fgets(temp, 128, fp)) {
			key = 0;
			while (key >= 0 && keys[key].label) {
				if ((p = strstr(temp, keys[key].label))) {
					p += strlen(keys[key].label);
					p += strspn(p, tokens);
					if ((i = strcspn(p, "#\n"))) p[i] = 0;
					free(*keys[key].var);
					*keys[key].var = strdup(p);
					key = -1;
				} else key++;
			}
		}
		fclose(fp);
	}
	free(family);
}
