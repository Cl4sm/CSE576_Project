pvmcopyenv(anyep)
	char **anyep;		/* old environment to copy, or NULL */
{
	char **newep;
	int i, n;

	if (anyep) {
		for (n = 0; anyep[n]; n++) ;
		if (newep = TALLOC(n + 1, char *, "env")) {
			newep[n] = 0;
			for (i = 0; i < n; i++) {
				if (!(newep[i] = STRALLOC(anyep[i]))) {
					while (i-- > 0)
						PVM_FREE(newep[i]);
					PVM_FREE(newep);
					newep = 0;
					break;
				}
			}
		}

	} else {
		if (newep = TALLOC(1, char *, "env"))
			newep[0] = 0;
	}
	return newep;
}
