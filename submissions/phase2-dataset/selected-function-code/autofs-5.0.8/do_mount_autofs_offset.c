static int do_mount_autofs_offset(struct autofs_point *ap,
				  struct mapent *oe, const char *root,
				  char *offset)

{
	int mounted = 0;
	int ret;

	debug(ap->logopt, "mount offset %s at %s", oe->key, root);

	ret = mount_autofs_offset(ap, oe, root, offset);
	if (ret >= MOUNT_OFFSET_OK)
		mounted++;
	else {
		if (ret != MOUNT_OFFSET_IGNORE)
			warn(ap->logopt, "failed to mount offset");
		else {
			debug(ap->logopt, "ignoring \"nohide\" trigger %s",
			      oe->key);
			free(oe->mapent);
			oe->mapent = NULL;
		}
	}

	return mounted;
}