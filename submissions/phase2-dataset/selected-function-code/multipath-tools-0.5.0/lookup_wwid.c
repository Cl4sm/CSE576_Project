lookup_wwid(FILE *f, char *wwid) {
	int c;
	char buf[LINE_MAX];
	int count;

	while ((c = fgetc(f)) != EOF){
		if (c != '/') {
			if (fgets(buf, LINE_MAX, f) == NULL)
				return 0;
			else
				continue;
		}
		count = 0;
		while ((c = fgetc(f)) != '/') {
			if (c == EOF)
				return 0;
			if (count >= WWID_SIZE - 1)
				goto next;
			if (wwid[count] == '\0')
				goto next;
			if (c != wwid[count++])
				goto next;
		}
		if (wwid[count] == '\0')
			return 1;
next:
		if (fgets(buf, LINE_MAX, f) == NULL)
			return 0;
	}
	return 0;
}
