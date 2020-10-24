extern int
get_refwwid (char * dev, enum devtypes dev_type, vector pathvec, char **wwid)
{
	int ret = 1;
	struct path * pp;
	char buff[FILE_NAME_SIZE];
	char * refwwid = NULL, tmpwwid[WWID_SIZE];

	if (!wwid)
		return 1;
	*wwid = NULL;

	if (dev_type == DEV_NONE)
		return 1;

	if (dev_type == DEV_DEVNODE) {
		if (basenamecpy(dev, buff, FILE_NAME_SIZE) == 0) {
			condlog(1, "basename failed for '%s' (%s)",
				dev, buff);
			return 1;
		}

		pp = find_path_by_dev(pathvec, buff);
		if (!pp) {
			struct udev_device *udevice = udev_device_new_from_subsystem_sysname(conf->udev, "block", buff);

			if (!udevice) {
				condlog(2, "%s: can't get udev device", buff);
				return 1;
			}
			ret = store_pathinfo(pathvec, conf->hwtable, udevice,
					     DI_SYSFS | DI_WWID, &pp);
			udev_device_unref(udevice);
			if (!pp) {
				if (ret == 1)
					condlog(0, "%s can't store path info",
						buff);
				return ret;
			}
		}
		refwwid = pp->wwid;
		goto out;
	}

	if (dev_type == DEV_DEVT) {
		strchop(dev);
		pp = find_path_by_devt(pathvec, dev);
		if (!pp) {
			struct udev_device *udevice = udev_device_new_from_devnum(conf->udev, 'b', parse_devt(dev));

			if (!udevice) {
				condlog(2, "%s: can't get udev device", dev);
				return 1;
			}
			ret = store_pathinfo(pathvec, conf->hwtable, udevice,
					     DI_SYSFS | DI_WWID, &pp);
			udev_device_unref(udevice);
			if (!pp) {
				if (ret == 1)
					condlog(0, "%s can't store path info",
						buff);
				return ret;
			}
		}
		refwwid = pp->wwid;
		goto out;
	}
	if (dev_type == DEV_DEVMAP) {

		if (((dm_get_uuid(dev, tmpwwid)) == 0) && (strlen(tmpwwid))) {
			refwwid = tmpwwid;
			goto check;
		}

		/*
		 * may be a binding
		 */
		if (get_user_friendly_wwid(dev, tmpwwid,
					   conf->bindings_file) == 0) {
			refwwid = tmpwwid;
			goto check;
		}

		/*
		 * or may be an alias
		 */
		refwwid = get_mpe_wwid(dev);

		/*
		 * or directly a wwid
		 */
		if (!refwwid)
			refwwid = dev;

check:
		if (refwwid && strlen(refwwid)) {
			if (filter_wwid(conf->blist_wwid, conf->elist_wwid,
					refwwid) > 0)
			return 2;
		}
	}
out:
	if (refwwid && strlen(refwwid)) {
		*wwid = STRDUP(refwwid);
		return 0;
	}

	return 1;
}
