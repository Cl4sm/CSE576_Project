sysfs_set_rport_tmo(struct multipath *mpp, struct path *pp)
{
	struct udev_device *rport_dev = NULL;
	char value[16];
	char rport_id[32];
	unsigned long long tmo = 0;
	int ret;

	sprintf(rport_id, "rport-%d:%d-%d",
		pp->sg_id.host_no, pp->sg_id.channel, pp->sg_id.transport_id);
	rport_dev = udev_device_new_from_subsystem_sysname(conf->udev,
				"fc_remote_ports", rport_id);
	if (!rport_dev) {
		condlog(1, "%s: No fc_remote_port device for '%s'", pp->dev,
			rport_id);
		return;
	}
	condlog(4, "target%d:%d:%d -> %s", pp->sg_id.host_no,
		pp->sg_id.channel, pp->sg_id.scsi_id, rport_id);

	/*
	 * This is tricky.
	 * dev_loss_tmo will be limited to 600 if fast_io_fail
	 * is _not_ set.
	 * fast_io_fail will be limited by the current dev_loss_tmo
	 * setting.
	 * So to get everything right we first need to increase
	 * dev_loss_tmo to the fast_io_fail setting (if present),
	 * then set fast_io_fail, and _then_ set dev_loss_tmo
	 * to the correct value.
	 */
	memset(value, 0, 16);
	if (mpp->fast_io_fail != MP_FAST_IO_FAIL_UNSET &&
	    mpp->fast_io_fail != MP_FAST_IO_FAIL_ZERO &&
	    mpp->fast_io_fail != MP_FAST_IO_FAIL_OFF) {
		/* Check if we need to temporarily increase dev_loss_tmo */
		ret = sysfs_attr_get_value(rport_dev, "dev_loss_tmo",
					   value, 16);
		if (ret <= 0) {
			condlog(0, "%s: failed to read dev_loss_tmo value, "
				"error %d", rport_id, -ret);
			goto out;
		}
		if (sscanf(value, "%llu\n", &tmo) != 1) {
			condlog(0, "%s: Cannot parse dev_loss_tmo "
				"attribute '%s'", rport_id, value);
			goto out;
		}
		if (mpp->fast_io_fail >= tmo) {
			snprintf(value, 16, "%u", mpp->fast_io_fail + 1);
		}
	} else if (mpp->dev_loss > 600) {
		condlog(3, "%s: limiting dev_loss_tmo to 600, since "
			"fast_io_fail is not set", rport_id);
		snprintf(value, 16, "%u", 600);
	} else {
		snprintf(value, 16, "%u", mpp->dev_loss);
	}
	if (strlen(value)) {
		ret = sysfs_attr_set_value(rport_dev, "dev_loss_tmo",
					   value, strlen(value));
		if (ret <= 0) {
			if (ret == -EBUSY)
				condlog(3, "%s: rport blocked", rport_id);
			else
				condlog(0, "%s: failed to set dev_loss_tmo to %s, error %d",
					rport_id, value, -ret);
			goto out;
		}
	}
	if (mpp->fast_io_fail != MP_FAST_IO_FAIL_UNSET) {
		if (mpp->fast_io_fail == MP_FAST_IO_FAIL_OFF)
			sprintf(value, "off");
		else if (mpp->fast_io_fail == MP_FAST_IO_FAIL_ZERO)
			sprintf(value, "0");
		else
			snprintf(value, 16, "%u", mpp->fast_io_fail);
		ret = sysfs_attr_set_value(rport_dev, "fast_io_fail_tmo",
					   value, strlen(value));
		if (ret <= 0) {
			if (ret == -EBUSY)
				condlog(3, "%s: rport blocked", rport_id);
			else
				condlog(0, "%s: failed to set fast_io_fail_tmo to %s, error %d",
					rport_id, value, -ret);
		}
	}
	if (tmo > 0) {
		snprintf(value, 16, "%u", mpp->dev_loss);
		ret = sysfs_attr_set_value(rport_dev, "dev_loss_tmo",
					   value, strlen(value));
		if (ret <= 0) {
			if (ret == -EBUSY)
				condlog(3, "%s: rport blocked", rport_id);
			else
				condlog(0, "%s: failed to set dev_loss_tmo to %s, error %d",
					rport_id, value, -ret);
		}
	}
out:
	udev_device_unref(rport_dev);
}
