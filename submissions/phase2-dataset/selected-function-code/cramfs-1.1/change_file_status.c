{
	struct utimbuf epoch = { 0, 0 };

	if (euid == 0) {
		if (lchown(path, i->uid, i->gid) < 0) {
			die(FSCK_ERROR, 1, "lchown failed: %s", path);
		}
		if (S_ISLNK(i->mode))
			return;
		if ((S_ISUID | S_ISGID) & i->mode) {
			if (chmod(path, i->mode) < 0) {
				die(FSCK_ERROR, 1, "chown failed: %s", path);
			}
		}
	}
	if (S_ISLNK(i->mode))
		return;
	if (utime(path, &epoch) < 0) {
		die(FSCK_ERROR, 1, "utime failed: %s", path);
	}
}
