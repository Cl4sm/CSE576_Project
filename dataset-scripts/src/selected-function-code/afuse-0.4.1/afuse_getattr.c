static int afuse_getattr(const char *path, struct stat *stbuf)
{
	char *root_name = alloca(strlen(path));
	char *real_path = alloca(max_path_out_len(path));
	int retval;
	mount_list_t *mount;
	BLOCK_SIGALRM;

	fprintf(stderr, "> GetAttr\n");

	switch (process_path(path, real_path, root_name, 0, &mount)) {
	case PROC_PATH_FAILED:
		retval = -ENXIO;
		break;

	case PROC_PATH_ROOT_DIR:
		fprintf(stderr, "Getattr on: (%s) - %s\n", path, root_name);
		stbuf->st_mode = S_IFDIR | 0700;
		stbuf->st_nlink = 1;
		stbuf->st_uid = getuid();
		stbuf->st_gid = getgid();
		stbuf->st_size = 0;
		stbuf->st_blksize = 0;
		stbuf->st_blocks = 0;
		stbuf->st_atime = 0;
		stbuf->st_mtime = 0;
		stbuf->st_ctime = 0;
		retval = 0;
		break;
	case PROC_PATH_ROOT_SUBDIR:
		if (user_options.exact_getattr)
			/* try to mount it */
			process_path(path, real_path, root_name, 1, &mount);
		if (!mount) {
			stbuf->st_mode = S_IFDIR | 0000;
			if (!user_options.exact_getattr)
				stbuf->st_mode = S_IFDIR | 0750;
			stbuf->st_nlink = 1;
			stbuf->st_uid = getuid();
			stbuf->st_gid = getgid();
			stbuf->st_size = 0;
			stbuf->st_blksize = 0;
			stbuf->st_blocks = 0;
			stbuf->st_atime = 0;
			stbuf->st_mtime = 0;
			stbuf->st_ctime = 0;
			retval = 0;
			break;
		}

	case PROC_PATH_PROXY_DIR:
		retval = get_retval(lstat(real_path, stbuf));
		break;

	default:
		DEFAULT_CASE_INVALID_ENUM;
	}
	if (mount)
		update_auto_unmount(mount);
	UNBLOCK_SIGALRM;
	return retval;
}