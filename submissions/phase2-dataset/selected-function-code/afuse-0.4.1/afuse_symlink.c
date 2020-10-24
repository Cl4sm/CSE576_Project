static int afuse_symlink(const char *from, const char *to)
{
	char *root_name_to = alloca(strlen(to));
	char *real_to_path = alloca(max_path_out_len(to));
	mount_list_t *mount;
	int retval;
	BLOCK_SIGALRM;

	switch (process_path(to, real_to_path, root_name_to, 0, &mount)) {
	case PROC_PATH_FAILED:
		retval = -ENXIO;
		break;
	case PROC_PATH_ROOT_DIR:
	case PROC_PATH_ROOT_SUBDIR:
		retval = -ENOTSUP;
		break;
	case PROC_PATH_PROXY_DIR:
		retval = get_retval(symlink(from, real_to_path));
		break;

	default:
		DEFAULT_CASE_INVALID_ENUM;
	}
	if (mount)
		update_auto_unmount(mount);
	UNBLOCK_SIGALRM;
	return retval;
}