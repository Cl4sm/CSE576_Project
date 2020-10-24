static void cleanup_multi_triggers(struct autofs_point *ap,
			    struct mapent *me, const char *root, int start,
			    const char *base)
{
	char path[PATH_MAX + 1];
	char offset[PATH_MAX + 1];
	char *poffset = offset;
	struct mapent *oe;
	struct list_head *mm_root, *pos;
	const char o_root[] = "/";
	const char *mm_base;

	mm_root = &me->multi->multi_list;

	if (!base)
		mm_base = o_root;
	else
		mm_base = base;

	pos = NULL;

	/* Make sure "none" of the offsets have an active mount. */
	while ((poffset = cache_get_offset(mm_base, poffset, start, mm_root, &pos))) {
		oe = cache_lookup_offset(mm_base, poffset, start, &me->multi_list);
		/* root offset is a special case */
		if (!oe || !oe->mapent || (strlen(oe->key) - start) == 1)
			continue;

		strcpy(path, root);
		strcat(path, poffset);
		if (umount(path)) {
			error(ap->logopt, "error recovering from mount fail");
			error(ap->logopt, "cannot umount offset %s", path);
		}
	}

	return;
}