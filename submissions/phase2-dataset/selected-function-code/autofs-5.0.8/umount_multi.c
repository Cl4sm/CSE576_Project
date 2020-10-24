int umount_multi(struct autofs_point *ap, const char *path, int incl)
{
	int is_autofs_fs;
	int left;

	debug(ap->logopt, "path %s incl %d", path, incl);

	is_autofs_fs = 0;
	if (master_find_submount(ap, path))
		is_autofs_fs = 1;

	left = 0;

	/*
	 * If we are a submount we need to umount any offsets our
	 * parent may have mounted over top of us.
	 */
	if (ap->submount)
		left += umount_subtree_mounts(ap->parent, path, is_autofs_fs);

	left += umount_subtree_mounts(ap, path, is_autofs_fs);

	/* Delete detritus like unwanted mountpoints and symlinks */
	if (left == 0 && ap->state != ST_READMAP) {
		update_map_cache(ap, path);
		check_rm_dirs(ap, path, incl);
	}

	return left;
}