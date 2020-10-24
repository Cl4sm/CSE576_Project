int do_lookup_mount(struct autofs_point *ap, struct map_source *map, const char *name, int name_len)
{
	struct lookup_mod *lookup;
	int status;

	if (!map->lookup) {
		lookup = open_lookup(map->type, "",
				     map->format, map->argc, map->argv);
		if (!lookup) {
			debug(ap->logopt,
			      "lookup module %s failed", map->type);
			return NSS_STATUS_UNAVAIL;
		}
		map->lookup = lookup;
	}

	lookup = map->lookup;

	master_source_current_wait(ap->entry);
	ap->entry->current = map;

	status = lookup->lookup_mount(ap, name, name_len, lookup->context);

	return status;
}