static int unlink_mount_tree(struct autofs_point *ap, struct list_head *list)
{
	struct list_head *p;
	int rv, ret;
	pid_t pgrp = getpgrp();
	char spgrp[20];

	sprintf(spgrp, "pgrp=%d", pgrp);

	ret = 1;
	list_for_each(p, list) {
		struct mnt_list *mnt;

		mnt = list_entry(p, struct mnt_list, list);

		if (strstr(mnt->opts, spgrp))
			continue;

		if (strcmp(mnt->fs_type, "autofs"))
			rv = spawn_umount(ap->logopt, "-l", mnt->path, NULL);
		else
			rv = umount2(mnt->path, MNT_DETACH);
		if (rv == -1) {
			debug(ap->logopt,
			      "can't unlink %s from mount tree", mnt->path);

			switch (errno) {
			case EINVAL:
				warn(ap->logopt,
				      "bad superblock or not mounted");
				break;

			case ENOENT:
			case EFAULT:
				ret = 0;
				warn(ap->logopt, "bad path for mount");
				break;
			}
		}
	}
	return ret;
}