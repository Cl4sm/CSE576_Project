static int afuse_releasedir(const char *path, struct fuse_file_info *fi)
{
	DIR *dp = get_dirp(fi);
	mount_list_t *mount;
	char *root_name = alloca(strlen(path));
	char *real_path = alloca(max_path_out_len(path));
	int retval;

	BLOCK_SIGALRM;

	switch (process_path(path, real_path, root_name, 1, &mount)) {
	case PROC_PATH_FAILED:
		retval = -ENXIO;
		break;

	case PROC_PATH_ROOT_DIR:
		retval = 0;
		break;

	case PROC_PATH_ROOT_SUBDIR:
	case PROC_PATH_PROXY_DIR:
		if (mount)
			dir_list_remove(&mount->dir_list, dp);
		if (dp)
			closedir(dp);
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