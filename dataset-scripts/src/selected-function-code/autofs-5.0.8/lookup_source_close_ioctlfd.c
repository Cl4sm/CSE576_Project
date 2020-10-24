int lookup_source_close_ioctlfd(struct autofs_point *ap, const char *key)
{
	struct master_mapent *entry = ap->entry;
	struct map_source *map;
	struct mapent_cache *mc;
	struct mapent *me;
	int ret = 0;

	map = entry->maps;
	while (map) {
		mc = map->mc;
		cache_readlock(mc);
		me = cache_lookup_distinct(mc, key);
		if (me) {
			if (me->ioctlfd != -1) {
				struct ioctl_ops *ops = get_ioctl_ops();
				ops->close(ap->logopt, me->ioctlfd);
				me->ioctlfd = -1;
			}
			cache_unlock(mc);
			ret = 1;
			break;
		}
		cache_unlock(mc);
		map = map->next;
	}

	return ret;
}