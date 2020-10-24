int macro_parse_globalvar(const char *define)
{
	char buf[MAX_MACRO_STRING];
	char *pbuf, *value;

	if (strlen(define) >= MAX_MACRO_STRING)
		return 0;

	strcpy(buf, define);

	pbuf = buf;
	while (pbuf) {
		if (*pbuf == '=') {
			*pbuf = '\0';
			value = pbuf + 1;
			break;
		}
		pbuf++;
	}

	/* Macro must have value */
	if (!pbuf)
		return 0;

	return macro_global_addvar(buf, strlen(buf), value);
}