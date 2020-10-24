{
	int stack_entries = 0;
	int stack_size = 64;
	struct entry **entry_stack;

	entry_stack = malloc(stack_size * sizeof(struct entry *));
	if (!entry_stack) {
		die(MKFS_ERROR, 1, "malloc failed");
	}

	if (opt_verbose) {
		printf("root:\n");
	}

	for (;;) {
		int dir_start = stack_entries;
		while (entry) {
			struct cramfs_inode *inode = (struct cramfs_inode *) (base + offset);
			size_t len = strlen(entry->name);

			entry->dir_offset = offset;

			inode->mode = entry->mode;
			inode->uid = entry->uid;
			inode->gid = entry->gid;
			inode->size = entry->size;
			inode->offset = 0;
			/* Non-empty directories, regfiles and symlinks will
			   write over inode->offset later. */

			offset += sizeof(struct cramfs_inode);
			total_nodes++;	/* another node */
			memcpy(base + offset, entry->name, len);
			/* Pad up the name to a 4-byte boundary */
			while (len & 3) {
				*(base + offset + len) = '\0';
				len++;
			}
			inode->namelen = len >> 2;
			offset += len;

			if (opt_verbose)
				print_node(entry);

			if (entry->child) {
				if (stack_entries >= stack_size) {
					stack_size *= 2;
					entry_stack = realloc(entry_stack, stack_size * sizeof(struct entry *));
					if (!entry_stack) {
						die(MKFS_ERROR, 1, "realloc failed");
					}
				}
				entry_stack[stack_entries] = entry;
				stack_entries++;
			}
			entry = entry->next;
		}

		/*
		 * Reverse the order the stack entries pushed during
		 * this directory, for a small optimization of disk
		 * access in the created fs.  This change makes things
		 * `ls -UR' order.
		 */
		{
			struct entry **lo = entry_stack + dir_start;
			struct entry **hi = entry_stack + stack_entries;
			struct entry *tmp;

			while (lo < --hi) {
				tmp = *lo;
				*lo++ = *hi;
				*hi = tmp;
			}
		}

		/* Pop a subdirectory entry from the stack, and recurse. */
		if (!stack_entries)
			break;
		stack_entries--;
		entry = entry_stack[stack_entries];

		set_data_offset(entry, base, offset);
		if (opt_verbose) {
			printf("%s:\n", entry->name);
		}
		entry = entry->child;
	}
	free(entry_stack);
	return offset;
}
