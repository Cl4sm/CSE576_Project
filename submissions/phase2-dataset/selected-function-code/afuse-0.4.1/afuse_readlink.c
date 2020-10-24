static int afuse_readlink(const char *path, char *buf, size_t size)
{
	int res;
	char *root_name = alloca(strlen(path));
	char *real_path = alloca(max_path_out_len(path));
	int retval;
	mount_list_t *mount;
	BLOCK_SIGALRM;

	switch (process_path(path, real_path, root_name, 1, &mount)) {
	case PROC_PATH_FAILED:
		retval = -ENXIO;
		break;
	case PROC_PATH_ROOT_DIR:
		retval = -ENOENT;
		break;
	case PROC_PATH_ROOT_SUBDIR:
		if (!mount) {
			retval = -ENOENT;
			break;
		}
	case PROC_PATH_PROXY_DIR:
		res = readlink(real_path, buf, size - 1);
		if (res == -1) {
			retval = -errno;
			break;
		}
		buf[res] = '\0';
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