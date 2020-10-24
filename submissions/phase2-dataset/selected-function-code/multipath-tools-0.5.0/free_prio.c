{
	if (!p)
		return;
	p->refcount--;
	if (p->refcount) {
		condlog(3, "%s prioritizer refcount %d",
			p->name, p->refcount);
		return;
	}
	condlog(3, "unloading %s prioritizer", p->name);
	list_del(&p->node);
	if (p->handle) {
		if (dlclose(p->handle) != 0) {
			condlog(0, "Cannot unload prioritizer %s: %s",
				p->name, dlerror());
		}
	}
	FREE(p);
}
