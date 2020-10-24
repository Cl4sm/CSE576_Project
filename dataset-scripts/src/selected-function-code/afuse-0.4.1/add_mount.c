mount_list_t *add_mount(const char *root_name, char *mount_point)
{
	mount_list_t *new_mount;

	new_mount = (mount_list_t *) my_malloc(sizeof(mount_list_t));
	new_mount->root_name = my_strdup(root_name);
	new_mount->mount_point = mount_point;

	new_mount->next = mount_list;
	new_mount->prev = NULL;
	new_mount->fd_list = NULL;
	new_mount->dir_list = NULL;
	new_mount->auto_unmount_time = INT64_MAX;
	if (mount_list)
		mount_list->prev = new_mount;

	mount_list = new_mount;

	update_auto_unmount(new_mount);

	return new_mount;
}