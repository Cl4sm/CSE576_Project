static int
uev_add_path (struct uevent *uev, struct vectors * vecs)
{
	struct path *pp;
	int ret, i;

	condlog(2, "%s: add path (uevent)", uev->kernel);
	if (strstr(uev->kernel, "..") != NULL) {
		/*
		 * Don't allow relative device names in the pathvec
		 */
		condlog(0, "%s: path name is invalid", uev->kernel);
		return 1;
	}

	pp = find_path_by_dev(vecs->pathvec, uev->kernel);
	if (pp) {
		condlog(0, "%s: spurious uevent, path already in pathvec",
			uev->kernel);
		if (pp->mpp)
			return 0;
		if (!strlen(pp->wwid)) {
			udev_device_unref(pp->udev);
			pp->udev = udev_device_ref(uev->udev);
			ret = pathinfo(pp, conf->hwtable,
				       DI_ALL | DI_BLACKLIST);
			if (ret == 2) {
				i = find_slot(vecs->pathvec, (void *)pp);
				if (i != -1)
					vector_del_slot(vecs->pathvec, i);
				free_path(pp);
				return 0;
			} else if (ret == 1) {
				condlog(0, "%s: failed to reinitialize path",
					uev->kernel);
				return 1;
			}
		}
	} else {
		/*
		 * get path vital state
		 */
		ret = store_pathinfo(vecs->pathvec, conf->hwtable,
				     uev->udev, DI_ALL, &pp);
		if (!pp) {
			if (ret == 2)
				return 0;
			condlog(0, "%s: failed to store path info",
				uev->kernel);
			return 1;
		}
		pp->checkint = conf->checkint;
	}

	return ev_add_path(pp, vecs);
}
