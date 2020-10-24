static unsigned int write_superblock(struct entry *root, char *base, int size)
{
	struct cramfs_super *super = (struct cramfs_super *) base;
	unsigned int offset = sizeof(struct cramfs_super) + image_length;

	offset += opt_pad;	/* 0 if no padding */

	super->magic = CRAMFS_MAGIC;
	super->flags = CRAMFS_FLAG_FSID_VERSION_2 | CRAMFS_FLAG_SORTED_DIRS;
	if (opt_holes)
		super->flags |= CRAMFS_FLAG_HOLES;
	if (image_length > 0)
		super->flags |= CRAMFS_FLAG_SHIFTED_ROOT_OFFSET;
	super->size = size;
	memcpy(super->signature, CRAMFS_SIGNATURE, sizeof(super->signature));

	super->fsid.crc = crc32(0L, Z_NULL, 0);
	super->fsid.edition = opt_edition;
	super->fsid.blocks = total_blocks;
	super->fsid.files = total_nodes;

	memset(super->name, 0x00, sizeof(super->name));
	if (opt_name)
		strncpy(super->name, opt_name, sizeof(super->name));
	else
		strncpy(super->name, "Compressed", sizeof(super->name));

	super->root.mode = root->mode;
	super->root.uid = root->uid;
	super->root.gid = root->gid;
	super->root.size = root->size;
	super->root.offset = offset >> 2;

	return offset;
}
