sysfs_set_session_tmo(struct multipath *mpp, struct path *pp)
{
	struct udev_device *session_dev = NULL;
	char session_id[64];
	char value[11];

	sprintf(session_id, "session%d", pp->sg_id.transport_id);
	session_dev = udev_device_new_from_subsystem_sysname(conf->udev,
				"iscsi_session", session_id);
	if (!session_dev) {
		condlog(1, "%s: No iscsi session for '%s'", pp->dev,
			session_id);
		return;
	}
	condlog(4, "target%d:%d:%d -> %s", pp->sg_id.host_no,
		pp->sg_id.channel, pp->sg_id.scsi_id, session_id);

	if (mpp->dev_loss) {
		condlog(3, "%s: ignoring dev_loss_tmo on iSCSI", pp->dev);
	}
	if (mpp->fast_io_fail != MP_FAST_IO_FAIL_UNSET) {
		if (mpp->fast_io_fail == MP_FAST_IO_FAIL_OFF) {
			condlog(3, "%s: can't switch off fast_io_fail_tmo "
				"on iSCSI", pp->dev);
		} else if (mpp->fast_io_fail == MP_FAST_IO_FAIL_ZERO) {
			condlog(3, "%s: can't set fast_io_fail_tmo to '0'"
				"on iSCSI", pp->dev);
		} else {
			snprintf(value, 11, "%u", mpp->fast_io_fail);
			if (sysfs_attr_set_value(session_dev, "recovery_tmo",
						 value, 11) <= 0) {
				condlog(3, "%s: Failed to set recovery_tmo, "
					" error %d", pp->dev, errno);
			}
		}
	}
	udev_device_unref(session_dev);
	return;
}
