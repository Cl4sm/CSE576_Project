static int afuse_truncate(const char *path, off_t size)
{
	char *root_name = alloca(strlen(path));
	char *real_path = alloca(max_path_out_len(path));
	mount_list_t *mount;
	int retval;
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
		retval = get_retval(truncate(real_path, size));
		break;

	default:
		DEFAULT_CASE_INVALID_ENUM;
	}
	if (mount)
		update_auto_unmount(mount);
	UNBLOCK_SIGALRM;
	return retval;
}