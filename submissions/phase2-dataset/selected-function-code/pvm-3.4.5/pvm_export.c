int
pvm_export(name)
	char *name;
{
	char *vn = "PVM_EXPORT";
	char *e;
	char *p, *q;
	int l;

	if (!name[0])
		goto done;

	if (e = getenv(vn)) {
		p = e;
		while (*p) {
			while (*p == ':')
				p++;
			q = p;
			while (*q && *q != ':')
				q++;
			l = q - p;
			if (strlen(name) == l && !strncmp(name, p, l))
				goto done;
			p = q;
		}
		p = TALLOC(strlen(vn) + strlen(e) + strlen(name) + 3, char, "str");
		strcpy(p, vn);
		strcat(p, "=");
		strcat(p, e);
		if (e[0])
			strcat(p, ":");
		strcat(p, name);
		pvmputenv(p);

	} else {
		e = TALLOC(strlen(vn) + strlen(name) + 2, char, "str");
		strcpy(e, vn);
		strcat(e, "=");
		strcat(e, name);
		pvmputenv(e);
	}

done:
/*
	e = getenv(vn);
	pvmlogprintf("pvm_export() %s=%s\n", vn, e);
*/
	return 0;
}
