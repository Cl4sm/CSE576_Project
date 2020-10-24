static char *clean_val(char *val)
{
	char *p = val;
	/* clean leading spaces */
	while (isspace(*p))
		p++;
	val = p;
	if (!*val)
		return val;
	/* clean trailing spaces */
	p = val + strlen(val) - 1;
	while (p > val && isspace(*p))
		p--;
	p[1] = '\0';
	/* clean quotas */
	if ((*val == '\"' && *p == '\"') || (*val == '\'' && *p == '\'')) {
		val++;
		*p-- = '\0';
	}
	return val;
}