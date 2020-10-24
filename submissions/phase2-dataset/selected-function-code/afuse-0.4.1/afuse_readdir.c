static int afuse_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
			 off_t offset, struct fuse_file_info *fi)
{
	DIR *dp = get_dirp(fi);
	struct dirent *de;
	char *root_name = alloca(strlen(path));
	char *real_path = alloca(max_path_out_len(path));
	struct list_t *dir_entry_list = NULL;
	mount_list_t *mount, *next;
	int retval;
	BLOCK_SIGALRM;

	switch (process_path(path, real_path, root_name, 1, &mount)) {
	case PROC_PATH_FAILED:
		retval = -ENXIO;
		break;

	case PROC_PATH_ROOT_DIR:
		filler(buf, ".", NULL, 0);
		filler(buf, "..", NULL, 0);
		insert_sorted_if_unique(&dir_entry_list, ".");
		insert_sorted_if_unique(&dir_entry_list, "..");
		for (mount = mount_list; mount; mount = next) {
			next = mount->next;
			/* Check for dead mounts. */
			if (!check_mount(mount)) {
				do_umount(mount);
			} else {
				if (insert_sorted_if_unique
				    (&dir_entry_list, mount->root_name))
					retval = -1;
				filler(buf, mount->root_name, NULL, 0);
			}
		}
		populate_root_dir(user_options.populate_root_command,
				  &dir_entry_list, filler, buf);
		destroy_list(&dir_entry_list);
		mount = NULL;
		retval = 0;
		break;

	case PROC_PATH_ROOT_SUBDIR:
		if (!mount) {
			retval = (!dp) ? -EBADF : -EACCES;
			break;
		}
	case PROC_PATH_PROXY_DIR:
		seekdir(dp, offset);
		while ((de = readdir(dp)) != NULL) {
			struct stat st;
			memset(&st, 0, sizeof(st));
			st.st_ino = de->d_ino;
			st.st_mode = de->d_type << 12;
			if (filler(buf, de->d_name, &st, telldir(dp)))
				break;
		}
		retval = 0;
		break;

	default:
		DEFAULT_CASE_INVALID_ENUM;
	}
	if (mount)
		update_auto_unmount(mount);
	UNBLOCK_SIGALRM;
	return retval;
}