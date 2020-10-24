struct map_source *
master_add_map_source(struct master_mapent *entry,
		      char *type, char *format, time_t age,
		      int argc, const char **argv)
{
	struct map_source *source;
	char *ntype, *nformat;
	const char **tmpargv;

	source = malloc(sizeof(struct map_source));
	if (!source)
		return NULL;
	memset(source, 0, sizeof(struct map_source));

	if (type) {
		ntype = strdup(type);
		if (!ntype) {
			master_free_map_source(source, 0);
			return NULL;
		}
		source->type = ntype;
	}

	if (format) {
		nformat = strdup(format);
		if (!nformat) {
			master_free_map_source(source, 0);
			return NULL;
		}
		source->format = nformat;
	}

	source->age = age;
	source->stale = 1;

	tmpargv = copy_argv(argc, argv);
	if (!tmpargv) {
		master_free_map_source(source, 0);
		return NULL;
	}
	source->argc = argc;
	source->argv = tmpargv;

	master_source_writelock(entry);

	if (!entry->maps) {
		source->mc = cache_init(entry->ap, source);
		if (!source->mc) {
			master_free_map_source(source, 0);
			master_source_unlock(entry);
			return NULL;
		}
		entry->maps = source;
	} else {
		struct map_source *this, *last, *next;

		/* Typically there only a few map sources */

		this = __master_find_map_source(entry, type, format, argc, tmpargv);
		if (this) {
			this->age = age;
			master_free_map_source(source, 0);
			master_source_unlock(entry);
			return this;
		}

		source->mc = cache_init(entry->ap, source);
		if (!source->mc) {
			master_free_map_source(source, 0);
			master_source_unlock(entry);
			return NULL;
		}

		last = NULL;
		next = entry->maps;
		while (next) {
			last = next;
			next = next->next;
		}
		if (last)
			last->next = source;
		else
			entry->maps = source;
	}

	master_source_unlock(entry);

	return source;
}