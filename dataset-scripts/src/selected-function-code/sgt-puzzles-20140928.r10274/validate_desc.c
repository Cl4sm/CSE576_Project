static char *validate_desc(const game_params *params, const char *desc)
{
    const char *p;
    char *err;
    int i, area;
    int *used;

    area = params->w * params->h;
    p = desc;
    err = NULL;

    used = snewn(area, int);
    for (i = 0; i < area; i++)
	used[i] = FALSE;

    for (i = 0; i < area; i++) {
	const char *q = p;
	int n;

	if (*p < '0' || *p > '9') {
	    err = "Not enough numbers in string";
	    goto leave;
	}
	while (*p >= '0' && *p <= '9')
	    p++;
	if (i < area-1 && *p != ',') {
	    err = "Expected comma after number";
	    goto leave;
	}
	else if (i == area-1 && *p) {
	    err = "Excess junk at end of string";
	    goto leave;
	}
	n = atoi(q);
	if (n < 0 || n >= area) {
	    err = "Number out of range";
	    goto leave;
	}
	if (used[n]) {
	    err = "Number used twice";
	    goto leave;
	}
	used[n] = TRUE;

	if (*p) p++;		       /* eat comma */
    }

    leave:
    sfree(used);
    return err;
}