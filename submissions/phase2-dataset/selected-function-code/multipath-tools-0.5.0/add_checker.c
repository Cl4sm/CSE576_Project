struct checker * add_checker (char * name)
{
	char libname[LIB_CHECKER_NAMELEN];
	struct stat stbuf;
	struct checker * c;
	char *errstr;

	c = alloc_checker();
	if (!c)
		return NULL;
	snprintf(c->name, CHECKER_NAME_LEN, "%s", name);
	snprintf(libname, LIB_CHECKER_NAMELEN, "%s/libcheck%s.so",
		 conf->multipath_dir, name);
	if (stat(libname,&stbuf) < 0) {
		condlog(0,"Checker '%s' not found in %s",
			name, conf->multipath_dir);
		goto out;
	}
	condlog(3, "loading %s checker", libname);
	c->handle = dlopen(libname, RTLD_NOW);
	if (!c->handle) {
		if ((errstr = dlerror()) != NULL)
			condlog(0, "A dynamic linking error occurred: (%s)",
				errstr);
		goto out;
	}
	c->check = (int (*)(struct checker *)) dlsym(c->handle, "libcheck_check");
	errstr = dlerror();
	if (errstr != NULL)
		condlog(0, "A dynamic linking error occurred: (%s)", errstr);
	if (!c->check)
		goto out;

	c->init = (int (*)(struct checker *)) dlsym(c->handle, "libcheck_init");
	errstr = dlerror();
	if (errstr != NULL)
		condlog(0, "A dynamic linking error occurred: (%s)", errstr);
	if (!c->init)
		goto out;

	c->free = (void (*)(struct checker *)) dlsym(c->handle, "libcheck_free");
	errstr = dlerror();
	if (errstr != NULL)
		condlog(0, "A dynamic linking error occurred: (%s)", errstr);
	if (!c->free)
		goto out;

	c->fd = 0;
	c->sync = 1;
	list_add(&c->node, &checkers);
	return c;
out:
	free_checker(c);
	return NULL;
}
