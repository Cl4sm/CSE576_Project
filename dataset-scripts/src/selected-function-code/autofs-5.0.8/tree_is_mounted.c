int tree_is_mounted(struct mnt_list *mnts, const char *path, unsigned int type)
{
	struct ioctl_ops *ops = get_ioctl_ops();
	struct list_head *p;
	struct list_head list;
	int mounted = 0;

	if (ops->ismountpoint)
		return ioctl_is_mounted(path, type);

	INIT_LIST_HEAD(&list);

	if (!tree_find_mnt_ents(mnts, &list, path))
		return 0;

	list_for_each(p, &list) {
		struct mnt_list *mptr;

		mptr = list_entry(p, struct mnt_list, entries);

		if (type) {
			unsigned int autofs_fs;

			autofs_fs = !strcmp(mptr->fs_type, "autofs");

			if (type & MNTS_REAL) {
				if (!autofs_fs) {
					mounted = 1;
					break;
				}
			} else if (type & MNTS_AUTOFS) {
				if (autofs_fs) {
					mounted = 1;
					break;
				}
			} else {
				mounted = 1;
				break;
			}
		}
	}
	return mounted;
}