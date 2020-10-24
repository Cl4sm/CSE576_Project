snprint_blacklist_devgroup (char *buff, int len, int *fwd, vector *vec)
{
	int threshold = MAX_LINE_LEN;
	struct blentry_device * bled;
	int pos;
	int i;

	pos = *fwd;
	if (!VECTOR_SIZE(*vec)) {
		if ((len - pos - threshold) <= 0)
			return 0;
		pos += snprintf(buff + pos, len - pos, "        <empty>\n");
	} else vector_foreach_slot (*vec, bled, i) {
		if ((len - pos - threshold) <= 0)
			return 0;
		if (bled->origin == ORIGIN_CONFIG)
			pos += snprintf(buff + pos, len - pos, "        (config file rule) ");
		else if (bled->origin == ORIGIN_DEFAULT)
			pos += snprintf(buff + pos, len - pos, "        (default rule)     ");
		pos += snprintf(buff + pos, len - pos, "%s:%s\n", bled->vendor, bled->product);
	}

	*fwd = pos;
	return pos;
}
