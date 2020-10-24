static int afuse_utime(const char *path, struct utimbuf *buf)
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
		retval = -ENOTSUP;
		break;
	case PROC_PATH_ROOT_SUBDIR:
		if (!mount) {
			retval = -ENOTSUP;
			break;
		}
	case PROC_PATH_PROXY_DIR:
		retval = get_retval(utime(real_path, buf));
		break;

	default:
		DEFAULT_CASE_INVALID_ENUM;
	}
	if (mount)
		update_auto_unmount(mount);
	UNBLOCK_SIGALRM;
	return retval;
}