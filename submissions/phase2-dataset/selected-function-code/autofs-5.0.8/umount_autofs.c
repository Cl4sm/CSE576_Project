int umount_autofs(struct autofs_point *ap, const char *root, int force)
{
	int ret = 0;

	if (ap->state == ST_INIT)
		return -1;

	/*
	 * Since lookup.c is lazy about closing lookup modules
	 * to prevent unneeded opens, we need to clean them up
	 * before umount.
	 */
	lookup_close_lookup(ap);

	if (ap->type == LKP_INDIRECT) {
		if (umount_all(ap, force) && !force)
			return -1;
		ret = umount_autofs_indirect(ap, root);
	} else
		ret = umount_autofs_direct(ap);

	return ret;
}