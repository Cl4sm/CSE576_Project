static int afuse_statfs(const char *path, struct statfs *stbuf)
#endif
{
	char *root_name = alloca(strlen(path));
	char *real_path = alloca(max_path_out_len(path));
	mount_list_t *mount;
	int retval;
	BLOCK_SIGALRM;

	switch (process_path(path, real_path, root_name, 1, &mount)) {
	case PROC_PATH_FAILED:
		retval = -ENXIO;
		break;

	case PROC_PATH_ROOT_DIR:
#if FUSE_VERSION >= 25
		stbuf->f_namemax = 0x7fffffff;
		stbuf->f_frsize = 512;
#else
		stbuf->f_namelen = 0x7fffffff;
#endif
		stbuf->f_bsize = 1024;
		stbuf->f_blocks = 0;
		stbuf->f_bfree = 0;
		stbuf->f_bavail = 0;
		stbuf->f_files = 0;
		stbuf->f_ffree = 0;
		retval = 0;
		break;

	case PROC_PATH_ROOT_SUBDIR:
		if (!mount) {
			retval = -EACCES;
			break;
		}
	case PROC_PATH_PROXY_DIR:
		retval = get_retval(statvfs(real_path, stbuf));
		break;

	default:
		DEFAULT_CASE_INVALID_ENUM;
	}
	if (mount)
		update_auto_unmount(mount);
	UNBLOCK_SIGALRM;
	return retval;
}