static void do_symlink(char *path, struct cramfs_inode *i)
{
	unsigned long offset = i->offset << 2;
	unsigned long curr = offset + 4;
	unsigned long next = *(u32 *) romfs_read(offset);
	unsigned long size;

	if (offset == 0) {
		die(FSCK_UNCORRECTED, 0, "symbolic link has zero offset");
	}
	if (i->size == 0) {
		die(FSCK_UNCORRECTED, 0, "symbolic link has zero size");
	}

	if (offset < start_data) {
		start_data = offset;
	}
	if (next > end_data) {
		end_data = next;
	}

	size = uncompress_block(romfs_read(curr), next - curr);
	if (size != i->size) {
		die(FSCK_UNCORRECTED, 0, "size error in symlink: %s", path);
	}
	outbuffer[size] = 0;
	if (opt_verbose) {
		char *str;

		asprintf(&str, "%s -> %s", path, outbuffer);
		print_node('l', i, str);
		if (opt_verbose > 1) {
			printf("  uncompressing block at %ld to %ld (%ld)\n", curr, next, next - curr);
		}
		free(str);
	}
	if (opt_extract) {
		if (symlink(outbuffer, path) < 0) {
			die(FSCK_ERROR, 1, "symlink failed: %s", path);
		}
		change_file_status(path, i);
	}
}
