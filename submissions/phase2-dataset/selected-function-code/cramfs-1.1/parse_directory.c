static unsigned int parse_directory(struct entry *root_entry, const char *name, struct entry **prev, loff_t *fslen_ub)
{
	struct dirent **dirlist;
	int totalsize = 0, dircount, dirindex;
	char *path, *endpath;
	size_t len = strlen(name);

	/* Set up the path. */
	/* TODO: Reuse the parent's buffer to save memcpy'ing and duplication. */
	path = malloc(len + 1 + MAX_INPUT_NAMELEN + 1);
	if (!path) {
		die(MKFS_ERROR, 1, "malloc failed");
	}
	memcpy(path, name, len);
	endpath = path + len;
	*endpath = '/';
	endpath++;

	/* read in the directory and sort */
	dircount = scandir(name, &dirlist, 0, cramsort);

	if (dircount < 0) {
		die(MKFS_ERROR, 1, "scandir failed: %s", name);
	}

	/* process directory */
	for (dirindex = 0; dirindex < dircount; dirindex++) {
		struct dirent *dirent;
		struct entry *entry;
		struct stat st;
		int size;
		size_t namelen;

		dirent = dirlist[dirindex];

		/* Ignore "." and ".." - we won't be adding them to the archive */
		if (dirent->d_name[0] == '.') {
			if (dirent->d_name[1] == '\0')
				continue;
			if (dirent->d_name[1] == '.') {
				if (dirent->d_name[2] == '\0')
					continue;
			}
		}
		namelen = strlen(dirent->d_name);
		if (namelen > MAX_INPUT_NAMELEN) {
			die(MKFS_ERROR, 0,
				"very long (%u bytes) filename found: %s\n"
				"please increase MAX_INPUT_NAMELEN in mkcramfs.c and recompile",
				namelen, dirent->d_name);
		}
		memcpy(endpath, dirent->d_name, namelen + 1);

		if (lstat(path, &st) < 0) {
			warn_skip = 1;
			continue;
		}
		entry = calloc(1, sizeof(struct entry));
		if (!entry) {
			die(MKFS_ERROR, 1, "calloc failed");
		}
		entry->name = strdup(dirent->d_name);
		if (!entry->name) {
			die(MKFS_ERROR, 1, "strdup failed");
		}
		/* truncate multi-byte UTF-8 filenames on character boundary */
		if (namelen > CRAMFS_MAXPATHLEN) {
			namelen = CRAMFS_MAXPATHLEN;
			warn_namelen = 1;
			/* the first lost byte must not be a trail byte */
			while ((entry->name[namelen] & 0xc0) == 0x80) {
				namelen--;
				/* are we reasonably certain it was UTF-8 ? */
				if (entry->name[namelen] < 0x80 || !namelen) {
					die(MKFS_ERROR, 0, "cannot truncate filenames not encoded in UTF-8");
				}
			}
			entry->name[namelen] = '\0';
		}
		entry->mode = st.st_mode;
		entry->size = st.st_size;
		entry->uid = st.st_uid;
		if (entry->uid >= 1 << CRAMFS_UID_WIDTH)
			warn_uid = 1;
		entry->gid = st.st_gid;
		if (entry->gid >= 1 << CRAMFS_GID_WIDTH)
			/* TODO: We ought to replace with a default
			   gid instead of truncating; otherwise there
			   are security problems.  Maybe mode should
			   be &= ~070.  Same goes for uid once Linux
			   supports >16-bit uids. */
			warn_gid = 1;
		size = sizeof(struct cramfs_inode) + ((namelen + 3) & ~3);
		*fslen_ub += size;
		if (S_ISDIR(st.st_mode)) {
			entry->size = parse_directory(root_entry, path, &entry->child, fslen_ub);
		} else if (S_ISREG(st.st_mode)) {
			if (entry->size) {
				if (access(path, R_OK) < 0) {
					warn_skip = 1;
					continue;
				}
				entry->path = strdup(path);
				if (!entry->path) {
					die(MKFS_ERROR, 1, "strdup failed");
				}
				if ((entry->size >= 1 << CRAMFS_SIZE_WIDTH)) {
					warn_size = 1;
					entry->size = (1 << CRAMFS_SIZE_WIDTH) - 1;
				}
			}
		} else if (S_ISLNK(st.st_mode)) {
			int len;
			entry->uncompressed = malloc(entry->size);
			if (!entry->uncompressed) {
				die(MKFS_ERROR, 1, "malloc failed");
			}
			len = readlink(path, entry->uncompressed, entry->size);
			if (len < 0) {
				warn_skip = 1;
				continue;
			}
			entry->size = len;
		} else if (S_ISFIFO(st.st_mode) || S_ISSOCK(st.st_mode)) {
			/* maybe we should skip sockets */
			entry->size = 0;
		} else if (S_ISCHR(st.st_mode) || S_ISBLK(st.st_mode)) {
			entry->size = st.st_rdev;
			if (entry->size & -(1<<CRAMFS_SIZE_WIDTH))
				warn_dev = 1;
		} else {
			die(MKFS_ERROR, 0, "bogus file type: %s", entry->name);
		}

		if (S_ISREG(st.st_mode) || S_ISLNK(st.st_mode)) {
			int blocks = ((entry->size - 1) / blksize + 1);

			/* block pointers & data expansion allowance + data */
			if (entry->size)
				*fslen_ub += (4+26)*blocks + entry->size + 3;
		}

		/* Link it into the list */
		*prev = entry;
		prev = &entry->next;
		totalsize += size;
	}
	free(path);
	free(dirlist);		/* allocated by scandir() with malloc() */
	return totalsize;
}
