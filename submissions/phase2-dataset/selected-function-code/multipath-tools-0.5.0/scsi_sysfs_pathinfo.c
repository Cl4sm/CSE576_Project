scsi_sysfs_pathinfo (struct path * pp)
{
	struct udev_device *parent;
	const char *attr_path = NULL;

	parent = pp->udev;
	while (parent) {
		const char *subsys = udev_device_get_subsystem(parent);
		if (subsys && !strncmp(subsys, "scsi", 4)) {
			attr_path = udev_device_get_sysname(parent);
			if (!attr_path)
				break;
			if (sscanf(attr_path, "%i:%i:%i:%i",
				   &pp->sg_id.host_no,
				   &pp->sg_id.channel,
				   &pp->sg_id.scsi_id,
				   &pp->sg_id.lun) == 4)
				break;
		}
		parent = udev_device_get_parent(parent);
	}
	if (!attr_path || pp->sg_id.host_no == -1)
		return 1;

	if (sysfs_get_vendor(parent, pp->vendor_id, SCSI_VENDOR_SIZE) <= 0)
		return 1;

	condlog(3, "%s: vendor = %s", pp->dev, pp->vendor_id);

	if (sysfs_get_model(parent, pp->product_id, SCSI_PRODUCT_SIZE) <= 0)
		return 1;

	condlog(3, "%s: product = %s", pp->dev, pp->product_id);

	if (sysfs_get_rev(parent, pp->rev, SCSI_REV_SIZE) < 0)
		return 1;

	condlog(3, "%s: rev = %s", pp->dev, pp->rev);

	/*
	 * set the hwe configlet pointer
	 */
	pp->hwe = find_hwe(conf->hwtable, pp->vendor_id, pp->product_id, pp->rev);

	/*
	 * host / bus / target / lun
	 */
	condlog(3, "%s: h:b:t:l = %i:%i:%i:%i",
			pp->dev,
			pp->sg_id.host_no,
			pp->sg_id.channel,
			pp->sg_id.scsi_id,
			pp->sg_id.lun);

	/*
	 * target node name
	 */
	if(!sysfs_get_tgt_nodename(pp, pp->tgt_node_name)) {
		condlog(3, "%s: tgt_node_name = %s",
			pp->dev, pp->tgt_node_name);
	}

	return 0;
}