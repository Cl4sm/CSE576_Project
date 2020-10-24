static int afuse_rename(const char *from, const char *to)
{
	char *root_name_from = alloca(strlen(from));
	char *root_name_to = alloca(strlen(to));
	char *real_from_path = alloca(max_path_out_len(from));
	char *real_to_path = alloca(max_path_out_len(to));
	mount_list_t *mount_from, *mount_to = NULL;
	int retval;
	BLOCK_SIGALRM;

	switch (process_path
		(from, real_from_path, root_name_from, 0, &mount_from)) {

	case PROC_PATH_FAILED:
		retval = -ENXIO;
		break;

	case PROC_PATH_ROOT_DIR:
	case PROC_PATH_ROOT_SUBDIR:
		retval = -ENOTSUP;
		break;

	case PROC_PATH_PROXY_DIR:
		switch (process_path
			(to, real_to_path, root_name_to, 0, &mount_to)) {

		case PROC_PATH_FAILED:
			retval = -ENXIO;
			break;

		case PROC_PATH_ROOT_DIR:
		case PROC_PATH_ROOT_SUBDIR:
			retval = -ENOTSUP;
			break;

		case PROC_PATH_PROXY_DIR:
			retval =
			    get_retval(rename(real_from_path, real_to_path));
			break;

		default:
			DEFAULT_CASE_INVALID_ENUM;
		}
		break;

	default:
		DEFAULT_CASE_INVALID_ENUM;
	}
	if (mount_to)
		update_auto_unmount(mount_to);
	if (mount_from && mount_from != mount_to)
		update_auto_unmount(mount_from);
	UNBLOCK_SIGALRM;
	return retval;
}