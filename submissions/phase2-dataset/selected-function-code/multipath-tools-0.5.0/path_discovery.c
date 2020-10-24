path_discovery (vector pathvec, struct config * conf, int flag)
{
	struct udev_enumerate *udev_iter;
	struct udev_list_entry *entry;
	struct udev_device *udevice;
	const char *devpath;
	int r = 0;

	udev_iter = udev_enumerate_new(conf->udev);
	if (!udev_iter)
		return 1;

	udev_enumerate_add_match_subsystem(udev_iter, "block");
	udev_enumerate_scan_devices(udev_iter);

	udev_list_entry_foreach(entry,
				udev_enumerate_get_list_entry(udev_iter)) {
		const char *devtype;
		devpath = udev_list_entry_get_name(entry);
		condlog(4, "Discover device %s", devpath);
		udevice = udev_device_new_from_syspath(conf->udev, devpath);
		if (!udevice) {
			condlog(4, "%s: no udev information", devpath);
			r++;
			continue;
		}
		devtype = udev_device_get_devtype(udevice);
		if(devtype && !strncmp(devtype, "disk", 4)) {
			r += path_discover(pathvec, conf,
						   udevice, flag);
		}
		udev_device_unref(udevice);
	}
	udev_enumerate_unref(udev_iter);
	condlog(4, "Discovery status %d", r);
	return r;
}
