{
	int rc;
	int aas;
	int priopath;

	if (pp->fd < 0)
		return -ALUA_PRIO_NO_INFORMATION;

	rc = get_alua_info(pp->fd);
	if (rc >= 0) {
		aas = (rc & 0x0f);
		priopath = (rc & 0x80);
		switch(aas) {
			case AAS_OPTIMIZED:
				rc = 50;
				break;
			case AAS_NON_OPTIMIZED:
				rc = 10;
				break;
			case AAS_LBA_DEPENDENT:
				rc = 5;
				break;
			case AAS_STANDBY:
				rc = 1;
				break;
			default:
				rc = 0;
		}
		if (priopath && aas != AAS_OPTIMIZED)
			rc += 80;
	} else {
		switch(-rc) {
			case ALUA_PRIO_NOT_SUPPORTED:
				condlog(0, "%s: alua not supported", pp->dev);
				break;
			case ALUA_PRIO_RTPG_FAILED:
				condlog(0, "%s: couldn't get target port group", pp->dev);
				break;
			case ALUA_PRIO_GETAAS_FAILED:
				condlog(0, "%s: couln't get asymmetric access state", pp->dev);
				break;
			case ALUA_PRIO_TPGS_FAILED:
				condlog(3, "%s: couln't get supported alua states", pp->dev);
				break;
		}
	}
	return rc;
}
