int
sysfs_pathinfo(struct path * pp)
{
	if (common_sysfs_pathinfo(pp))
		return 1;

	pp->bus = SYSFS_BUS_UNDEF;
	if (!strncmp(pp->dev,"cciss",5))
		pp->bus = SYSFS_BUS_CCISS;
	if (!strncmp(pp->dev,"dasd", 4))
		pp->bus = SYSFS_BUS_CCW;
	if (!strncmp(pp->dev,"sd", 2))
		pp->bus = SYSFS_BUS_SCSI;

	if (pp->bus == SYSFS_BUS_UNDEF)
		return 0;
	else if (pp->bus == SYSFS_BUS_SCSI) {
		if (scsi_sysfs_pathinfo(pp))
			return 1;
	} else if (pp->bus == SYSFS_BUS_CCW) {
		if (ccw_sysfs_pathinfo(pp))
			return 1;
	} else if (pp->bus == SYSFS_BUS_CCISS) {
		if (cciss_sysfs_pathinfo(pp))
			return 1;
	}
	return 0;
}
