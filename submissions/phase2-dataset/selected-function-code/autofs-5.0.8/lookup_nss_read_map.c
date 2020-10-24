int lookup_nss_read_map(struct autofs_point *ap, struct map_source *source, time_t age)
{
	struct master_mapent *entry = ap->entry;
	struct list_head nsslist;
	struct list_head *head, *p;
	struct nss_source *this;
	struct map_source *map;
	enum nsswitch_status status;
	unsigned int at_least_one = 0;
	int result = 0;

	/*
	 * For each map source (ie. each entry for the mount
	 * point in the master map) do the nss lookup to
	 * locate the map and read it.
	 */
	if (source)
		map = source;
	else
		map = entry->maps;
	while (map) {
		/* Is map source up to date or no longer valid */
		if (!map->stale || entry->age > map->age) {
			map = map->next;
			continue;
		}

		if (map->type) {
			if (!strncmp(map->type, "multi", 5))
				debug(ap->logopt, "reading multi map");
			else
				debug(ap->logopt,
				      "reading map %s %s",
				       map->type, map->argv[0]);
			result = do_read_map(ap, map, age);
			map = map->next;
			continue;
		}

		/* If it starts with a '/' it has to be a file or LDAP map */
		if (map->argv && *map->argv[0] == '/') {
			if (*(map->argv[0] + 1) == '/') {
				char *tmp = strdup("ldap");
				if (!tmp) {
					map = map->next;
					continue;
				}
				map->type = tmp;
				debug(ap->logopt,
				      "reading map %s %s", tmp, map->argv[0]);
				result = do_read_map(ap, map, age);
			} else {
				debug(ap->logopt,
				      "reading map file %s", map->argv[0]);
				result = read_file_source_instance(ap, map, age);
			}
			map = map->next;
			continue;
		}

		INIT_LIST_HEAD(&nsslist);

		pthread_cleanup_push(nsslist_cleanup, &nsslist);
		status = nsswitch_parse(&nsslist);
		pthread_cleanup_pop(0);
		if (status) {
			error(ap->logopt,
			      "can't to read name service switch config.");
			result = 1;
			break;
		}

		pthread_cleanup_push(nsslist_cleanup, &nsslist);
		head = &nsslist;
		list_for_each(p, head) {
			this = list_entry(p, struct nss_source, list);

			debug(ap->logopt,
			      "reading map %s %s", this->source, map->argv[0]);

			result = read_map_source(this, ap, map, age);
			if (result == NSS_STATUS_UNKNOWN)
				continue;

			/* Don't try to update the map cache if it's unavailable */
			if (result == NSS_STATUS_UNAVAIL)
				map->stale = 0;

			if (result == NSS_STATUS_SUCCESS) {
				at_least_one = 1;
				result = NSS_STATUS_TRYAGAIN;
			}

			status = check_nss_result(this, result);
			if (status >= 0) {
				map = NULL;
				break;
			}

			result = NSS_STATUS_SUCCESS;
		}
		pthread_cleanup_pop(1);

		if (!map)
			break;

		map = map->next;
	}

	if (!result || at_least_one)
		return 1;

	return 0;
}