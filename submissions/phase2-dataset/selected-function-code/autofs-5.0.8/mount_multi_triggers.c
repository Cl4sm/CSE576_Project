int mount_multi_triggers(struct autofs_point *ap, struct mapent *me,
			 const char *root, unsigned int start, const char *base)
{
	char path[PATH_MAX + 1];
	char *offset = path;
	struct mapent *oe;
	struct list_head *pos = NULL;
	unsigned int fs_path_len;
	int mounted;

	fs_path_len = start + strlen(base);
	if (fs_path_len > PATH_MAX)
		return -1;

	mounted = 0;
	offset = cache_get_offset(base, offset, start, &me->multi_list, &pos);
	while (offset) {
		int plen = fs_path_len + strlen(offset);

		if (plen > PATH_MAX) {
			warn(ap->logopt, "path loo long");
			goto cont;
		}

		oe = cache_lookup_offset(base, offset, start, &me->multi_list);
		if (!oe || !oe->mapent)
			goto cont;

		mounted += do_mount_autofs_offset(ap, oe, root, offset);

		/*
		 * If re-constructing a multi-mount it's necessary to walk
		 * into nested mounts, unlike the usual "mount only what's
		 * needed as you go" behavior.
		 */
		if (ap->state == ST_READMAP && ap->flags & MOUNT_FLAG_REMOUNT) {
			if (oe->ioctlfd != -1 ||
			    is_mounted(_PROC_MOUNTS, oe->key, MNTS_REAL)) {
				char oe_root[PATH_MAX + 1];
				strcpy(oe_root, root);
				strcat(oe_root, offset); 
				mount_multi_triggers(ap, oe, oe_root, strlen(oe_root), base);
			}
		}
cont:
		offset = cache_get_offset(base,
				offset, start, &me->multi_list, &pos);
	}

	return mounted;
}