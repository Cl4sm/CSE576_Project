static int afuse_mknod(const char *path, mode_t mode, dev_t rdev)
{
	char *root_name = alloca(strlen(path));
	char *real_path = alloca(max_path_out_len(path));
	mount_list_t *mount;
	int retval;
	BLOCK_SIGALRM;
	fprintf(stderr, "> Mknod\n");

	switch (process_path(path, real_path, root_name, 0, &mount)) {
	case PROC_PATH_FAILED:
		retval = -ENXIO;
		break;

	case PROC_PATH_ROOT_DIR:
	case PROC_PATH_ROOT_SUBDIR:
		retval = -ENOTSUP;
		break;

	case PROC_PATH_PROXY_DIR:
		if (S_ISFIFO(mode))
			retval = get_retval(mkfifo(real_path, mode));
		else
			retval = get_retval(mknod(real_path, mode, rdev));
		break;

	default:
		DEFAULT_CASE_INVALID_ENUM;
	}
	if (mount)
		update_auto_unmount(mount);
	UNBLOCK_SIGALRM;
	return retval;
}