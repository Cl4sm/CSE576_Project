int
pvm_unexport(name)
	char *name;
{
	char *vn = "PVM_EXPORT";
	char *e, *enew;
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
			if (strlen(name) == l && !strncmp(name, p, l)) {
				if (*q == ':')
					q++;
				else if (p > e && *(p - 1) == ':')
					p--;
				enew = TALLOC(strlen(vn) + (p - e) + strlen(q) + 2, char, "str");
				strcpy(enew, vn);
				strcat(enew, "=");
				strncat(enew, e, p - e);
				strcat(enew, q);
				pvmputenv(enew);
				goto done;
			}
			p = q;
		}
	}

done:
/*
	e = getenv(vn);
	pvmlogprintf("pvm_unexport() %s=%s\n", vn, e);
*/
	return 0;
}
