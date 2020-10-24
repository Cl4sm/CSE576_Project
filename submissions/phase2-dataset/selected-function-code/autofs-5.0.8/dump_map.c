int dump_map(struct master *master, const char *type, const char *name)
{
	struct list_head *p, *head;

	if (list_empty(&master->mounts)) {
		printf("no master map entries found\n");
		return 1;
	}

	head = &master->mounts;
	p = head->next;
	while (p != head) {
		struct map_source *source;
		struct master_mapent *this;
		struct autofs_point *ap;
		time_t now = time(NULL);

		this = list_entry(p, struct master_mapent, list);
		p = p->next;

		ap = this->ap;

		/*
		 * Ensure we actually read indirect map entries so we can
		 * list them. The map reads won't read any indirect map
		 * entries (other than those in a file map) unless the
		 * browse option is set.
		 */
		if (ap->type == LKP_INDIRECT)
			ap->flags |= MOUNT_FLAG_GHOST;

		/* Read the map content into the cache */
		if (lookup_nss_read_map(ap, NULL, now))
			lookup_prune_cache(ap, now);
		else {
			printf("failed to read map\n");
			lookup_close_lookup(ap);
			continue;
		}

		if (!this->maps) {
			printf("no map sources found for %s\n", ap->path);
			lookup_close_lookup(ap);
			continue;
		}

		source = this->maps;
		while (source) {
			struct map_source *instance;
			struct mapent *me;

			instance = NULL;
			if (source->type) {
				if (!match_type(source->type, type)) {
					source = source->next;
					continue;
				}
				if (!match_name(source, name)) {
					source = source->next;
					continue;
				}
				instance = source;
			} else {
				struct map_source *map;

				map = source->instance;
				while (map) {
					if (!match_type(map->type, type)) {
						map = map->next;
						continue;
					}
					if (!match_name(map, name)) {
						map = map->next;
						continue;
					}
					instance = map;
					break;
				}
			}

			if (!instance) {
				source = source->next;
				lookup_close_lookup(ap);
				continue;
			}

			me = cache_lookup_first(source->mc);
			if (!me)
				printf("no keys found in map\n");
			else {
				do {
					if (me->source == instance)
						printf("%s\t%s\n", me->key, me->mapent);
				} while ((me = cache_lookup_next(source->mc, me)));
			}

			lookup_close_lookup(ap);
			return 1;
		}
		lookup_close_lookup(ap);
	}

	return 0;
}