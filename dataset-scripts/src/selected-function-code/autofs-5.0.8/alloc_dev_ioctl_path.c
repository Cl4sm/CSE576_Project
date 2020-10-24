static struct autofs_dev_ioctl *alloc_dev_ioctl_path(int ioctlfd, const char *path)
{
	struct autofs_dev_ioctl *ioctl;
	size_t size, p_len;

	if (!path) {
		errno = EINVAL;
		return NULL;
	}

	p_len = strlen(path);
	size = sizeof(struct autofs_dev_ioctl) + p_len + 1;
	ioctl = malloc(size);
	if (!ioctl) {
		errno = ENOMEM;
		return NULL;
	}

	init_autofs_dev_ioctl(ioctl);
	ioctl->ioctlfd = ioctlfd;
	ioctl->size = size;
	memcpy(ioctl->path, path, p_len);
	ioctl->path[p_len] = '\0';

	return ioctl;
}