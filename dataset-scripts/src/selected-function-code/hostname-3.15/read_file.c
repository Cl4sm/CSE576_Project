char *
read_file(char *filename, int boot)
{
	struct stat st;
  	FILE *fp;
	char *buf;

	if ((fp = fopen(filename, "r")) == NULL) {
		if (boot)
			return NULL;
		else
			err(1, NULL);
	}

	if (fstat(fileno(fp), &st) == -1
	   || (buf = (char *) malloc(st.st_size + 1)) == NULL)
		err(1, NULL);

	while (fgets(buf, st.st_size + 1, fp) != NULL) {
		char *p;

		if (buf[0] == '\n' || buf[0] == '#')
			continue;

		if ((p = strchr(buf, '\n')) != NULL)
			*p = '\0';
		break;
	}

	fclose(fp);
	return buf;
}