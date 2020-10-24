int master_show_mounts(struct master *master)
{
	struct list_head *p, *head;

	printf("\nautofs dump map information\n"
		 "===========================\n\n");

	printf("global options: ");
	if (!global_options)
		printf("none configured\n");
	else {
		printf("%s\n", global_options);
		unsigned int append_options = defaults_get_append_options();
		const char *append = append_options ? "will" : "will not";
		printf("global options %s be appended to map entries\n", append);
	}

	if (list_empty(&master->mounts)) {
		printf("no master map entries found\n\n");
		return 1;
	}

	head = &master->mounts;
	p = head->next;
	while (p != head) {
		struct map_source *source;
		struct master_mapent *this;
		struct autofs_point *ap;
		time_t now = time(NULL);
		unsigned int count = 0;

		this = list_entry(p, struct master_mapent, list);
		p = p->next;

		ap = this->ap;

		printf("\nMount point: %s\n", ap->path);

		printf("\nsource(s):\n");

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
			printf("  failed to read map\n\n");
			continue;
		}

		if (!this->maps) {
			printf("  no map sources found\n\n");
			continue;
		}

		source = this->maps;
		while (source) {
			struct mapent *me;

			if (source->type)
				printf("\n  type: %s\n", source->type);
			else {
				printf("\n  instance type(s): ");
				list_source_instances(source, source->instance);
				printf("\n");
			}

			if (source->argc >= 1) {
				print_map_info(source);
				if (count && ap->type == LKP_INDIRECT)
					printf("  duplicate indirect map entry"
					       " will be ignored at run time\n");
			}

			printf("\n");

			me = cache_lookup_first(source->mc);
			if (!me)
				printf("  no keys found in map\n");
			else {
				do {
					printf("  %s | %s\n", me->key, me->mapent);
				} while ((me = cache_lookup_next(source->mc, me)));
			}

			count++;

			source = source->next;
		}

		lookup_close_lookup(ap);

		printf("\n");
	}

	return 1;
}