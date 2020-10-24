proc_result_t process_path(const char *path_in, char *path_out, char *root_name,
			   int attempt_mount, mount_list_t ** out_mount)
{
	char *path_out_base;
	int is_child;
	int len;
	mount_list_t *mount = NULL;

	*out_mount = NULL;

	fprintf(stderr, "Path in: %s\n", path_in);
	is_child = extract_root_name(path_in, root_name);
	fprintf(stderr, "root_name is: %s\n", root_name);

	if (is_mount_filtered(root_name))
		return PROC_PATH_FAILED;

	// Mount filesystem if necessary
	// the combination of is_child and attempt_mount prevent inappropriate
	// mounting of a filesystem for example if the user tries to mknod
	// in the afuse root this should cause an error not a mount.
	// !!FIXME!! this is broken on FUSE < 2.5 (?) because a getattr
	// on the root node seems to occur with every single access.
	if ((is_child || attempt_mount) &&
	    strlen(root_name) > 0 &&
	    !(mount = find_mount(root_name)) && !(mount = do_mount(root_name)))
		return PROC_PATH_FAILED;

	if (mount && !check_mount(mount)) {
		do_umount(mount);
		mount = do_mount(root_name);
		if (!mount)
			return PROC_PATH_FAILED;
	}
	// construct path_out (mount_point_directory + '/' + path_in + '\0')
	path_out_base = path_out;
	len = strlen(mount_point_directory);
	memcpy(path_out, mount_point_directory, len);
	path_out += len;
	*path_out++ = '/';
	len = strlen(path_in) - 1;
	memcpy(path_out, path_in + 1, len);
	path_out += len;
	*path_out = '\0';
	fprintf(stderr, "Path out: %s\n", path_out_base);

	*out_mount = mount;

	if (is_child)
		return PROC_PATH_PROXY_DIR;
	else if (strlen(root_name))
		return PROC_PATH_ROOT_SUBDIR;
	else
		return PROC_PATH_ROOT_DIR;
}