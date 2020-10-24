ccw_sysfs_pathinfo (struct path * pp)
{
	struct udev_device *parent;
	char attr_buff[NAME_SIZE];
	const char *attr_path;

	parent = pp->udev;
	while (parent) {
		const char *subsys = udev_device_get_subsystem(parent);
		if (subsys && !strncmp(subsys, "ccw", 3))
			break;
		parent = udev_device_get_parent(parent);
	}
	if (!parent)
		return 1;

	sprintf(pp->vendor_id, "IBM");

	condlog(3, "%s: vendor = %s", pp->dev, pp->vendor_id);

	if (sysfs_get_devtype(parent, attr_buff, FILE_NAME_SIZE) <= 0)
		return 1;

	if (!strncmp(attr_buff, "3370", 4)) {
		sprintf(pp->product_id,"S/390 DASD FBA");
	} else if (!strncmp(attr_buff, "9336", 4)) {
		sprintf(pp->product_id,"S/390 DASD FBA");
	} else {
		sprintf(pp->product_id,"S/390 DASD ECKD");
	}

	condlog(3, "%s: product = %s", pp->dev, pp->product_id);

	/*
	 * set the hwe configlet pointer
	 */
	pp->hwe = find_hwe(conf->hwtable, pp->vendor_id, pp->product_id, NULL);

	/*
	 * host / bus / target / lun
	 */
	attr_path = udev_device_get_sysname(parent);
	pp->sg_id.lun = 0;
	sscanf(attr_path, "%i.%i.%x",
			&pp->sg_id.host_no,
			&pp->sg_id.channel,
			&pp->sg_id.scsi_id);
	condlog(3, "%s: h:b:t:l = %i:%i:%i:%i",
			pp->dev,
			pp->sg_id.host_no,
			pp->sg_id.channel,
			pp->sg_id.scsi_id,
			pp->sg_id.lun);

	return 0;
}
