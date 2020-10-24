static int afuse_opendir(const char *path, struct fuse_file_info *fi)
{
	DIR *dp;
	char *root_name = alloca(strlen(path));
	mount_list_t *mount;
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
		if (!mount) {
			retval = -EACCES;
			fi->fh = 0lu;
			break;
		}
	case PROC_PATH_PROXY_DIR:
		dp = opendir(real_path);
		if (dp == NULL) {
			retval = -errno;
			break;
		}
		fi->fh = (unsigned long)dp;
		if (mount)
			dir_list_add(&mount->dir_list, dp);
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