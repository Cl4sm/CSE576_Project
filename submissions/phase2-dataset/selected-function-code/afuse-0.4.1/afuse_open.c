static int afuse_open(const char *path, struct fuse_file_info *fi)
{
	int fd;
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
	case PROC_PATH_ROOT_SUBDIR:
		retval = -ENOENT;
		break;
	case PROC_PATH_PROXY_DIR:
		fd = open(real_path, fi->flags);
		if (fd == -1) {
			retval = -errno;
			break;
		}

		fi->fh = fd;
		if (mount)
			fd_list_add(&mount->fd_list, fd);
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