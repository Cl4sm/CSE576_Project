static void check_rm_dirs(struct autofs_point *ap, const char *path, int incl)
{
	/*
	 * If we're a submount the kernel can't know we're trying to
	 * shutdown and so cannot block processes walking into the
	 * mount point directory. If this is the call to umount_multi()
	 * made during shutdown (incl == 0) we have to leave any mount
	 * point directories in place so we can recover if needed. The
	 * umount itself will clean these directories up for us
	 * automagically.
	 */
	if (!incl && ap->submount)
		return;

	if ((!(ap->flags & MOUNT_FLAG_GHOST)) ||
	    (ap->state == ST_SHUTDOWN_PENDING ||
	     ap->state == ST_SHUTDOWN_FORCE ||
	     ap->state == ST_SHUTDOWN))
		rm_unwanted(ap->logopt, path, incl, ap->dev);
	else if ((ap->flags & MOUNT_FLAG_GHOST) && (ap->type == LKP_INDIRECT))
		rm_unwanted(ap->logopt, path, 0, ap->dev);
}