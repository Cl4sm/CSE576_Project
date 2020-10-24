{
	char libname[LIB_PRIO_NAMELEN];
	struct stat stbuf;
	struct prio * p;
	char *errstr;

	p = alloc_prio();
	if (!p)
		return NULL;
	snprintf(p->name, PRIO_NAME_LEN, "%s", name);
	snprintf(libname, LIB_PRIO_NAMELEN, "%s/libprio%s.so",
		 conf->multipath_dir, name);
	if (stat(libname,&stbuf) < 0) {
		condlog(0,"Prioritizer '%s' not found in %s",
			name, conf->multipath_dir);
		goto out;
	}
	condlog(3, "loading %s prioritizer", libname);
	p->handle = dlopen(libname, RTLD_NOW);
	if (!p->handle) {
		if ((errstr = dlerror()) != NULL)
			condlog(0, "A dynamic linking error occurred: (%s)",
				errstr);
		goto out;
	}
	p->getprio = (int (*)(struct path *, char *)) dlsym(p->handle, "getprio");
	errstr = dlerror();
	if (errstr != NULL)
		condlog(0, "A dynamic linking error occurred: (%s)", errstr);
	if (!p->getprio)
		goto out;
	list_add(&p->node, &prioritizers);
	return p;
out:
	free_prio(p);
	return NULL;
}
