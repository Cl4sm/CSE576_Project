static int
path_discover (vector pathvec, struct config * conf,
	       struct udev_device *udevice, int flag)
{
	struct path * pp;
	const char * devname;

	devname = udev_device_get_sysname(udevice);
	if (!devname)
		return 0;

	if (filter_property(conf, udevice) > 0)
		return 0;

	if (filter_devnode(conf->blist_devnode, conf->elist_devnode,
			   (char *)devname) > 0)
		return 0;

	pp = find_path_by_dev(pathvec, (char *)devname);
	if (!pp) {
		if (store_pathinfo(pathvec, conf->hwtable,
				   udevice, flag, NULL) != 1)
			return 0;
		else
			return 1;
	}
	return pathinfo(pp, conf->hwtable, flag);
}
