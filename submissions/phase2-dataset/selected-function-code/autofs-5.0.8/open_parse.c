struct parse_mod *open_parse(const char *name, const char *err_prefix,
			     int argc, const char *const *argv)
{
	struct parse_mod *mod;
	char buf[MAX_ERR_BUF];
	char fnbuf[PATH_MAX];
	size_t size;
	void *dh;
	int *ver;


	mod = malloc(sizeof(struct parse_mod));
	if (!mod) {
		if (err_prefix) {
			char *estr = strerror_r(errno, buf, MAX_ERR_BUF);
			logerr("%s%s", err_prefix, estr);
		}
		return NULL;
	}

	size = snprintf(fnbuf, sizeof(fnbuf),
			"%s/parse_%s.so", AUTOFS_LIB_DIR, name);
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
			logerr("%scannot open parse module %s (%s)",
			     err_prefix, name, dlerror());
		free(mod);
		return NULL;
	}

	if (!(ver = (int *) dlsym(dh, "parse_version"))
	    || *ver != AUTOFS_PARSE_VERSION) {
		if (err_prefix)
			logerr("%sparse module %s version mismatch",
			     err_prefix, name);
		dlclose(dh);
		free(mod);
		return NULL;
	}

	if (!(mod->parse_init = (parse_init_t) dlsym(dh, "parse_init")) ||
	    !(mod->parse_mount = (parse_mount_t) dlsym(dh, "parse_mount")) ||
	    !(mod->parse_done = (parse_done_t) dlsym(dh, "parse_done"))) {
		if (err_prefix)
			logerr("%sparse module %s corrupt",
			     err_prefix, name);
		dlclose(dh);
		free(mod);
		return NULL;
	}

	if (mod->parse_init(argc, argv, &mod->context)) {
		dlclose(dh);
		free(mod);
		return NULL;
	}
	mod->dlhandle = dh;
	return mod;
}