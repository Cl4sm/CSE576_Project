static int
snprint_blacklist_group (char *buff, int len, int *fwd, vector *vec)
{
	int threshold = MAX_LINE_LEN;
	struct blentry * ble;
	int pos;
	int i;

	pos = *fwd;
	if (!VECTOR_SIZE(*vec)) {
		if ((len - pos - threshold) <= 0)
			return 0;
		pos += snprintf(buff + pos, len - pos, "        <empty>\n");
	} else vector_foreach_slot (*vec, ble, i) {
		if ((len - pos - threshold) <= 0)
			return 0;
		if (ble->origin == ORIGIN_CONFIG)
			pos += snprintf(buff + pos, len - pos, "        (config file rule) ");
		else if (ble->origin == ORIGIN_DEFAULT)
			pos += snprintf(buff + pos, len - pos, "        (default rule)     ");
		pos += snprintf(buff + pos, len - pos, "%s\n", ble->str);
	}

	*fwd = pos;
	return pos;
}
