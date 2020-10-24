void send_map_update_request(struct autofs_point *ap)
{
	struct map_source *map;
	int status, need_update = 0;

	status = pthread_mutex_lock(&instance_mutex);
	if (status)
		fatal(status);

	map = ap->entry->maps;
	while (map) {
		if (check_stale_instances(map))
			map->stale = 1;
		if (map->stale) {
			need_update = 1;
			break;
		}
		map = map->next;
	}

	status = pthread_mutex_unlock(&instance_mutex);
	if (status)
		fatal(status);

	if (!need_update)
		return;

	st_add_task(ap, ST_READMAP);

	return;
}