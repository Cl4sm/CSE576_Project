static void test_fs(int start)
{
	struct cramfs_inode *root;

	root = read_super();
	umask(0);
	euid = geteuid();
	stream.next_in = NULL;
	stream.avail_in = 0;
	inflateInit(&stream);
	expand_fs(extract_dir, root);
	inflateEnd(&stream);
	if (start_data != ~0UL) {
		if (start_data < (sizeof(struct cramfs_super) + start)) {
			die(FSCK_UNCORRECTED, 0, "directory data start (%ld) < sizeof(struct cramfs_super) + start (%ld)", start_data, sizeof(struct cramfs_super) + start);
		}
		if (end_dir != start_data) {
			die(FSCK_UNCORRECTED, 0, "directory data end (%ld) != file data start (%ld)", end_dir, start_data);
		}
	}
	if (super.flags & CRAMFS_FLAG_FSID_VERSION_2) {
		if (end_data > super.size) {
			die(FSCK_UNCORRECTED, 0, "invalid file data offset");
		}
	}
	iput(root);		/* free(root) */
}
