int
store_pathinfo (vector pathvec, vector hwtable, struct udev_device *udevice,
		int flag, struct path **pp_ptr)
{
	int err = 1;
	struct path * pp;
	const char * devname;

	if (pp_ptr)
		*pp_ptr = NULL;

	devname = udev_device_get_sysname(udevice);
	if (!devname)
		return 1;

	pp = alloc_path();

	if (!pp)
		return 1;

	if(safe_sprintf(pp->dev, "%s", devname)) {
		condlog(0, "pp->dev too small");
		goto out;
	}
	pp->udev = udev_device_ref(udevice);
	err = pathinfo(pp, hwtable,
		       (conf->dry_run == 3)? flag : (flag | DI_BLACKLIST));
	if (err)
		goto out;

	err = store_path(pathvec, pp);
	if (err)
		goto out;

out:
	if (err)
		free_path(pp);
	else if (pp_ptr)
		*pp_ptr = pp;
	return err;
}
