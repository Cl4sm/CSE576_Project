int do_mount(struct autofs_point *ap, const char *root, const char *name, int name_len,
	     const char *what, const char *fstype, const char *options)
{
	struct mount_mod *mod;
	const char *modstr;
	size_t root_len = root ? strlen(root) : 0;
	char **ngp;
	int rv;

	/* Initially look for a mount module but don't issue an error on fail */
	mod = open_mount(modstr = fstype, NULL);
	if (!mod) {
		for (ngp = not_generic; *ngp; ngp++) {
			if (!strcmp(fstype, *ngp))
				break;
		}
		/*
		 * If there's not a known mount module use the generic module,
		 * otherwise redo the fs mount module with error reporting
		 */
		if (!*ngp)
			mod = open_mount(modstr = "generic", ERR_PREFIX);
		else
			mod = open_mount(modstr = fstype, ERR_PREFIX);
		if (!mod) {
			error(ap->logopt,
			      "cannot find mount method for filesystem %s",
			      fstype);
			return -1;
		}
	}

	if (*name == '/')
		debug(ap->logopt,
		      "%s %s type %s options %s using module %s",
		      what, name, fstype, options, modstr);
	else if (root_len > 1 && root[root_len - 1] == '/')
		debug(ap->logopt,
		      "%s %s type %s options %s using module %s",
		      what, root, fstype, options, modstr);
	else
		debug(ap->logopt,
		      "%s %s/%s type %s options %s using module %s",
		      what, root, name, fstype, options, modstr);

	rv = mod->mount_mount(ap, root, name, name_len, what, fstype, options, mod->context);
	close_mount(mod);

	return rv;
}