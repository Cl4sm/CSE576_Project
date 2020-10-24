int
cli_add_path (void * v, char ** reply, int * len, void * data)
{
	struct vectors * vecs = (struct vectors *)data;
	char * param = get_keyparam(v, PATH);
	struct path *pp;
	int r;

	param = convert_dev(param, 1);
	condlog(2, "%s: add path (operator)", param);

	if (filter_devnode(conf->blist_devnode, conf->elist_devnode,
			   param) > 0)
		goto blacklisted;

	pp = find_path_by_dev(vecs->pathvec, param);
	if (pp) {
		condlog(2, "%s: path already in pathvec", param);
		if (pp->mpp)
			return 0;
	} else {
		struct udev_device *udevice;

		udevice = udev_device_new_from_subsystem_sysname(conf->udev,
								 "block",
								 param);
		r = store_pathinfo(vecs->pathvec, conf->hwtable,
				   udevice, DI_ALL, &pp);
		udev_device_unref(udevice);
		if (!pp) {
			if (r == 2)
				goto blacklisted;
			condlog(0, "%s: failed to store path info", param);
			return 1;
		}
		pp->checkint = conf->checkint;
	}
	return ev_add_path(pp, vecs);
blacklisted:
	*reply = strdup("blacklisted\n");
	*len = strlen(*reply) + 1;
	condlog(2, "%s: path blacklisted", param);
	return 0;
}
