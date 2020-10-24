int
sysfs_set_scsi_tmo (struct multipath *mpp)
{
	struct path *pp;
	int i;
	int dev_loss_tmo = mpp->dev_loss;

	if (mpp->no_path_retry > 0) {
		int no_path_retry_tmo = mpp->no_path_retry * conf->checkint;

		if (no_path_retry_tmo > MAX_DEV_LOSS_TMO)
			no_path_retry_tmo = MAX_DEV_LOSS_TMO;
		if (no_path_retry_tmo > dev_loss_tmo)
			dev_loss_tmo = no_path_retry_tmo;
		condlog(3, "%s: update dev_loss_tmo to %d",
			mpp->alias, dev_loss_tmo);
	} else if (mpp->no_path_retry == NO_PATH_RETRY_QUEUE) {
		dev_loss_tmo = MAX_DEV_LOSS_TMO;
		condlog(3, "%s: update dev_loss_tmo to %d",
			mpp->alias, dev_loss_tmo);
	}
	mpp->dev_loss = dev_loss_tmo;
	if (mpp->dev_loss && mpp->fast_io_fail >= (int)mpp->dev_loss) {
		condlog(3, "%s: turning off fast_io_fail (%d is not smaller than dev_loss_tmo)",
			mpp->alias, mpp->fast_io_fail);
		mpp->fast_io_fail = MP_FAST_IO_FAIL_OFF;
	}
	if (!mpp->dev_loss && mpp->fast_io_fail == MP_FAST_IO_FAIL_UNSET)
		return 0;

	vector_foreach_slot(mpp->paths, pp, i) {
		if (pp->sg_id.proto_id == SCSI_PROTOCOL_FCP)
			sysfs_set_rport_tmo(mpp, pp);
		if (pp->sg_id.proto_id == SCSI_PROTOCOL_ISCSI)
			sysfs_set_session_tmo(mpp, pp);
		if (pp->sg_id.proto_id == SCSI_PROTOCOL_SAS)
			sysfs_set_nexus_loss_tmo(mpp, pp);
	}
	return 0;
}
