pathinfo (struct path *pp, vector hwtable, int mask)
{
	int path_state;

	if (!pp)
		return 1;

	condlog(3, "%s: mask = 0x%x", pp->dev, mask);

	/*
	 * fetch info available in sysfs
	 */
	if (mask & DI_SYSFS && sysfs_pathinfo(pp))
		return 1;

	if (mask & DI_BLACKLIST && mask & DI_SYSFS) {
		if (filter_device(conf->blist_device, conf->elist_device,
				  pp->vendor_id, pp->product_id) > 0) {
			return 2;
		}
	}

	path_state = path_offline(pp);
	if (path_state == PATH_REMOVED)
		goto blank;

	/*
	 * fetch info not available through sysfs
	 */
	if (pp->fd < 0)
		pp->fd = open(udev_device_get_devnode(pp->udev), O_RDONLY);

	if (pp->fd < 0) {
		condlog(4, "Couldn't open node for %s: %s",
			pp->dev, strerror(errno));
		goto blank;
	}

	if (mask & DI_SERIAL)
		get_geometry(pp);

	if (path_state == PATH_UP && pp->bus == SYSFS_BUS_SCSI &&
	    scsi_ioctl_pathinfo(pp, mask))
		goto blank;

	if (pp->bus == SYSFS_BUS_CCISS &&
	    cciss_ioctl_pathinfo(pp, mask))
		goto blank;

	if (mask & DI_CHECKER) {
		if (path_state == PATH_UP) {
			pp->chkrstate = pp->state = get_state(pp, 0);
			if (pp->state == PATH_UNCHECKED ||
			    pp->state == PATH_WILD)
				goto blank;
			if (pp->state == PATH_TIMEOUT)
				pp->state = PATH_DOWN;
		} else {
			condlog(3, "%s: path inaccessible", pp->dev);
			pp->chkrstate = pp->state = path_state;
			if (path_state == PATH_PENDING ||
			    path_state == PATH_DOWN)
				pp->priority = 0;
		}
	}

	if ((mask & DI_WWID) && !strlen(pp->wwid))
		get_uid(pp);
	if (mask & DI_BLACKLIST && mask & DI_WWID) {
		if (filter_wwid(conf->blist_wwid, conf->elist_wwid,
				pp->wwid) > 0) {
			return 2;
		}
	}

	 /*
	  * Retrieve path priority, even for PATH_DOWN paths if it has never
	  * been successfully obtained before.
	  */
	if ((mask & DI_PRIO) && path_state == PATH_UP) {
		if (pp->state != PATH_DOWN || pp->priority == PRIO_UNDEF) {
			if (!strlen(pp->wwid))
				get_uid(pp);
			get_prio(pp);
		}
	}

	return 0;

blank:
	/*
	 * Recoverable error, for example faulty or offline path
	 */
	memset(pp->wwid, 0, WWID_SIZE);
	pp->chkrstate = pp->state = PATH_DOWN;

	return 0;
}
