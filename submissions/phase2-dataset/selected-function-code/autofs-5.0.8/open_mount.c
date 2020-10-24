struct mount_mod *open_mount(const char *name, const char *err_prefix)
{
	struct mount_mod *mod;
	char buf[MAX_ERR_BUF];
	char fnbuf[PATH_MAX];
	size_t size;
	void *dh;
	int *ver;


	mod = malloc(sizeof(struct mount_mod));
	if (!mod) {
		if (err_prefix) {
			char *estr = strerror_r(errno, buf, MAX_ERR_BUF);
			logerr("%s%s", err_prefix, estr);
		}
		return NULL;
	}

	size = snprintf(fnbuf, sizeof(fnbuf),
			"%s/mount_%s.so", AUTOFS_LIB_DIR, name);
	if (size >= sizeof(fnbuf)) {
		free(mod);
		if (err_prefix) {
			char *estr = strerror_r(errno, buf, MAX_ERR_BUF);
			logerr("%s%s", err_prefix, estr);
		}
		return NULL;
	}

	if (!(dh = dlopen(fnbuf, RTLD_NOW))) {
		if (err_prefix)
			logerr("%scannot open mount module %s (%s)",
			     err_prefix, name, dlerror());
		free(mod);
		return NULL;
	}

	if (!(ver = (int *) dlsym(dh, "mount_version"))
	    || *ver != AUTOFS_MOUNT_VERSION) {
		if (err_prefix)
			logerr("%smount module %s version mismatch",
			     err_prefix, name);
		dlclose(dh);
		free(mod);
		return NULL;
	}

	if (!(mod->mount_init = (mount_init_t) dlsym(dh, "mount_init")) ||
	    !(mod->mount_mount = (mount_mount_t) dlsym(dh, "mount_mount")) ||
	    !(mod->mount_done = (mount_done_t) dlsym(dh, "mount_done"))) {
		if (err_prefix)
			logerr("%smount module %s corrupt",
			     err_prefix, name);
		dlclose(dh);
		free(mod);
		return NULL;
	}

	if (mod->mount_init(&mod->context)) {
		dlclose(dh);
		free(mod);
		return NULL;
	}
	mod->dlhandle = dh;
	return mod;
}