static boolean_t validate_port_caps(osm_log_t * p_log,
				    const osm_mgrp_t * p_mgrp,
				    const osm_physp_t * p_physp)
{
	const ib_port_info_t *p_pi;
	uint8_t mtu_required;
	uint8_t mtu_mgrp;
	uint8_t rate_required;
	uint8_t rate_mgrp;
	int extended;

	mtu_required = ib_port_info_get_mtu_cap(&p_physp->port_info);
	mtu_mgrp = (uint8_t) (p_mgrp->mcmember_rec.mtu & 0x3F);
	if (mtu_required < mtu_mgrp) {
		OSM_LOG(p_log, OSM_LOG_VERBOSE,
			"Port's MTU %x is less than %x\n",
			mtu_required, mtu_mgrp);
		return FALSE;
	}

	p_pi = &p_physp->port_info;
	extended = p_pi->capability_mask & IB_PORT_CAP_HAS_EXT_SPEEDS;
	rate_required = ib_port_info_compute_rate(p_pi, extended);
	rate_mgrp = (uint8_t) (p_mgrp->mcmember_rec.rate & 0x3F);
	if (ib_path_compare_rates(rate_required, rate_mgrp) < 0) {
		OSM_LOG(p_log, OSM_LOG_VERBOSE,
			"Port's RATE %x is less than %x\n",
			rate_required, rate_mgrp);
		return FALSE;
	}

	return TRUE;
}