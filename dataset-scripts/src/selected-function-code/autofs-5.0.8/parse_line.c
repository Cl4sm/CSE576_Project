static int parse_line(char *line, char **res, char **value)
{
	char *key, *val, *trailer;
	int len;

	key = line;

	if (*key == '#' || !isalpha(*key))
		return 0;

	while (*key && *key == ' ')
		key++;

	if (!*key)
		return 0;

	if (!(val = strchr(key, '=')))
		return 0;

	*val++ = '\0';

	while (*val && (*val == '"' || isblank(*val)))
		val++;

	len = strlen(val);

	if (val[len - 1] == '\n') {
		val[len - 1] = '\0';
		len--;
	}

	trailer = strchr(val, '#');
	if (!trailer)
		trailer = val + len - 1;
	else
		trailer--;

	while (*trailer && (*trailer == '"' || isblank(*trailer)))
		*(trailer--) = '\0';;

	*res = key;
	*value = val;

	return 1;
}