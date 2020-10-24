mount_list_t *do_mount(const char *root_name)
{
	char *mount_point;
	mount_list_t *mount;

	fprintf(stderr, "Mounting: %s\n", root_name);

	if (!(mount_point = make_mount_point(root_name))) {
		fprintf(stderr,
			"Failed to create mount point directory: %s/%s\n",
			mount_point_directory, root_name);
		return NULL;
	}

	if (!run_template(user_options.mount_command_template,
			  mount_point, root_name)) {
		// remove the now unused directory
		if (rmdir(mount_point) == -1)
			fprintf(stderr,
				"Failed to remove mount point dir: %s (%s)",
				mount_point, strerror(errno));

		free(mount_point);
		return NULL;
	}

	mount = add_mount(root_name, mount_point);
	return mount;
}