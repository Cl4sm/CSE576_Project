{
	if (!c)
		return;
	c->refcount--;
	if (c->refcount) {
		condlog(3, "%s checker refcount %d",
			c->name, c->refcount);
		return;
	}
	condlog(3, "unloading %s checker", c->name);
	list_del(&c->node);
	if (c->handle) {
		if (dlclose(c->handle) != 0) {
			condlog(0, "Cannot unload checker %s: %s",
				c->name, dlerror());
		}
	}
	FREE(c);
}
