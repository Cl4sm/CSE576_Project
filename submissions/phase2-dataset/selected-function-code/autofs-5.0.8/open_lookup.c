struct lookup_mod *open_lookup(const char *name, const char *err_prefix,
			       const char *mapfmt, int argc, const char *const *argv)
{
	struct lookup_mod *mod;
	char buf[MAX_ERR_BUF];
	char fnbuf[PATH_MAX];
	size_t size;
	void *dh;
	int *ver;


	mod = malloc(sizeof(struct lookup_mod));
	if (!mod) {
		if (err_prefix) {
			char *estr = strerror_r(errno, buf, MAX_ERR_BUF);
			logerr("%s%s", err_prefix, estr);
		}
		return NULL;
	}

	size = snprintf(fnbuf, sizeof(fnbuf),
			"%s/lookup_%s.so", AUTOFS_LIB_DIR, name);
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
			logerr("%scannot open lookup module %s (%s)",
			       err_prefix, name, dlerror());
		free(mod);
		return NULL;
	}

	if (!(ver = (int *) dlsym(dh, "lookup_version"))
	    || *ver != AUTOFS_LOOKUP_VERSION) {
		if (err_prefix)
			logerr("%slookup module %s version mismatch",
			     err_prefix, name);
		dlclose(dh);
		free(mod);
		return NULL;
	}

	if (!(mod->lookup_init = (lookup_init_t) dlsym(dh, "lookup_init")) ||
	    !(mod->lookup_read_master = (lookup_read_master_t) dlsym(dh, "lookup_read_master")) ||
	    !(mod->lookup_read_map = (lookup_read_map_t) dlsym(dh, "lookup_read_map")) ||
	    !(mod->lookup_mount = (lookup_mount_t) dlsym(dh, "lookup_mount")) ||
	    !(mod->lookup_done = (lookup_done_t) dlsym(dh, "lookup_done"))) {
		if (err_prefix)
			logerr("%slookup module %s corrupt", err_prefix, name);
		dlclose(dh);
		free(mod);
		return NULL;
	}

	if (mod->lookup_init(mapfmt, argc, argv, &mod->context)) {
		dlclose(dh);
		free(mod);
		return NULL;
	}
	mod->dlhandle = dh;
	return mod;
}