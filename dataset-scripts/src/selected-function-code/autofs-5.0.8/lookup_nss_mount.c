int lookup_nss_mount(struct autofs_point *ap, struct map_source *source, const char *name, int name_len)
{
	struct master_mapent *entry = ap->entry;
	struct list_head nsslist;
	struct list_head *head, *p;
	struct nss_source *this;
	struct map_source *map;
	enum nsswitch_status status;
	int result = 0;

	/*
	 * For each map source (ie. each entry for the mount
	 * point in the master map) do the nss lookup to
	 * locate the map and lookup the name.
	 */
	pthread_cleanup_push(master_source_lock_cleanup, entry);
	master_source_readlock(entry);
	if (source)
		map = source;
	else
		map = entry->maps;
	while (map) {
		/*
		 * Only consider map sources that have been read since 
		 * the map entry was last updated.
		 */
		if (entry->age > map->age) {
			map = map->next;
			continue;
		}

		sched_yield();

		if (map->type) {
			result = do_lookup_mount(ap, map, name, name_len);

			if (result == NSS_STATUS_SUCCESS)
				break;

			map = map->next;
			continue;
		}

		/* If it starts with a '/' it has to be a file or LDAP map */
		if (*map->argv[0] == '/') {
			if (*(map->argv[0] + 1) == '/') {
				char *tmp = strdup("ldap");
				if (!tmp) {
					map = map->next;
					continue;
				}
				map->type = tmp;
				result = do_lookup_mount(ap, map, name, name_len);
			} else
				result = lookup_name_file_source_instance(ap, map, name, name_len);

			if (result == NSS_STATUS_SUCCESS)
				break;

			map = map->next;
			continue;
		}

		INIT_LIST_HEAD(&nsslist);

		status = nsswitch_parse(&nsslist);
		if (status) {
			error(ap->logopt,
			      "can't to read name service switch config.");
			result = 1;
			break;
		}

		head = &nsslist;
		list_for_each(p, head) {
			this = list_entry(p, struct nss_source, list);

			result = lookup_map_name(this, ap, map, name, name_len);

			if (result == NSS_STATUS_UNKNOWN) {
				map = map->next;
				continue;
			}

			status = check_nss_result(this, result);
			if (status >= 0) {
				map = NULL;
				break;
			}
		}

		if (!list_empty(&nsslist))
			free_sources(&nsslist);

		if (!map)
			break;

		map = map->next;
	}
	if (ap->state != ST_INIT)
		send_map_update_request(ap);
	pthread_cleanup_pop(1);

	/*
	 * The last source lookup will return NSS_STATUS_NOTFOUND if the
	 * map exits and the key has not been found but the map may also
	 * not exist in which case the key is also not found.
	 */
	if (result == NSS_STATUS_NOTFOUND || result == NSS_STATUS_UNAVAIL)
		update_negative_cache(ap, source, name);

	return !result;
}