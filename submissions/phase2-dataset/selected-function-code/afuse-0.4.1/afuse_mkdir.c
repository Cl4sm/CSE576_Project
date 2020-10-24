static int afuse_mkdir(const char *path, mode_t mode)
{
	char *root_name = alloca(strlen(path));
	char *real_path = alloca(max_path_out_len(path));
	int retval;
	mount_list_t *mount;
	BLOCK_SIGALRM;

	switch (process_path(path, real_path, root_name, 0, &mount)) {
	case PROC_PATH_FAILED:
		retval = -ENXIO;
		break;
	case PROC_PATH_ROOT_DIR:
	case PROC_PATH_ROOT_SUBDIR:
		retval = -ENOTSUP;
		break;
	case PROC_PATH_PROXY_DIR:
		retval = get_retval(mkdir(real_path, mode));
		break;

	default:
		DEFAULT_CASE_INVALID_ENUM;
	}
	if (mount)
		update_auto_unmount(mount);
	UNBLOCK_SIGALRM;
	return retval;
}