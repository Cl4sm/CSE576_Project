static struct autofs_dev_ioctl *alloc_dev_ioctl_open(const char *path, dev_t devid)
{
	struct autofs_dev_ioctl *ioctl;
	size_t size, p_len;
	dev_t devno = devid;

	if (!path)
		return NULL;

	p_len = strlen(path);
	size = sizeof(struct autofs_dev_ioctl) + p_len + 1;
	ioctl = malloc(size);
	if (!ioctl) {
		errno = ENOMEM;
		return NULL;
	}

	init_autofs_dev_ioctl(ioctl);
	ioctl->size = size;
	memcpy(ioctl->path, path, p_len);
	ioctl->path[p_len] = '\0';
	ioctl->openmount.devid = devno;

	return ioctl;
}