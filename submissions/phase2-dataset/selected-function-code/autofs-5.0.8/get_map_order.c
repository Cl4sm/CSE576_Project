static unsigned int get_map_order(const char *domain, const char *map)
{
	char key[] = "YP_LAST_MODIFIED";
	int key_len = strlen(key);
	char *order;
	int order_len;
	char *mapname;
	long last_changed;
	int err;

	mapname = alloca(strlen(map) + 1);
	if (!mapname)
		return 0;

	strcpy(mapname, map);

	err = yp_match(domain, mapname, key, key_len, &order, &order_len);
	if (err != YPERR_SUCCESS) {
		if (err == YPERR_MAP) {
			char *usc;

			while ((usc = strchr(mapname, '_')))
				*usc = '.';

			err = yp_match(domain, mapname,
				       key, key_len, &order, &order_len);

			if (err != YPERR_SUCCESS)
				return 0;

			last_changed = atol(order);
			free(order);

			return (unsigned int) last_changed;
		}
		return 0;
	}

	last_changed = atol(order);
	free(order);

	return (unsigned int) last_changed;
}