sysfs_set_nexus_loss_tmo(struct multipath *mpp, struct path *pp)
{
	struct udev_device *sas_dev = NULL;
	char end_dev_id[64];
	char value[11];

	sprintf(end_dev_id, "end_device-%d:%d",
		pp->sg_id.host_no, pp->sg_id.transport_id);
	sas_dev = udev_device_new_from_subsystem_sysname(conf->udev,
				"sas_end_device", end_dev_id);
	if (!sas_dev) {
		condlog(1, "%s: No SAS end device for '%s'", pp->dev,
			end_dev_id);
		return;
	}
	condlog(4, "target%d:%d:%d -> %s", pp->sg_id.host_no,
		pp->sg_id.channel, pp->sg_id.scsi_id, end_dev_id);

	if (mpp->dev_loss) {
		snprintf(value, 11, "%u", mpp->dev_loss);
		if (sysfs_attr_set_value(sas_dev, "I_T_nexus_loss_timeout",
					 value, 11) <= 0)
			condlog(3, "%s: failed to update "
				"I_T Nexus loss timeout, error %d",
				pp->dev, errno);
	}
	udev_device_unref(sas_dev);
	return;
}
