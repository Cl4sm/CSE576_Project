static boolean_t mgrp_request_is_realizable(IN osm_sa_t * sa,
					    IN ib_net64_t comp_mask,
					    IN ib_member_rec_t * p_mcm_rec,
					    IN const osm_physp_t * p_physp)
{
	uint8_t mtu_sel = 2;	/* exactly */
	uint8_t mtu_required, mtu, port_mtu;
	uint8_t rate_sel = 2;	/* exactly */
	uint8_t rate_required, rate, port_rate;
	const ib_port_info_t *p_pi;
	osm_log_t *p_log = sa->p_log;
	int extended;

	OSM_LOG_ENTER(sa->p_log);

	/*
	 * End of o15-0.2.3 specifies:
	 * ....
	 * The entity may also supply the other components such as HopLimit,
	 * MTU, etc. during group creation time. If these components are not
	 * provided during group creation time, SA will provide them for the
	 * group. The values chosen are vendor-dependent and beyond the scope
	 * of the specification.
	 *
	 * so we might also need to assign RATE/MTU if they are not comp
	 * masked in.
	 */

	p_pi = &p_physp->port_info;
	port_mtu = p_physp ? ib_port_info_get_mtu_cap(p_pi) : 0;
	if (!(comp_mask & IB_MCR_COMPMASK_MTU) ||
	    !(comp_mask & IB_MCR_COMPMASK_MTU_SEL) ||
	    (mtu_sel = (p_mcm_rec->mtu >> 6)) == 3)
		mtu = port_mtu ? port_mtu : sa->p_subn->min_ca_mtu;
	else {
		mtu_required = (uint8_t) (p_mcm_rec->mtu & 0x3F);
		mtu = mtu_required;
		switch (mtu_sel) {
		case 0:	/* Greater than MTU specified */
			if (port_mtu && mtu_required >= port_mtu) {
				OSM_LOG(p_log, OSM_LOG_VERBOSE,
					"Requested MTU %x >= the port\'s mtu:%x\n",
					mtu_required, port_mtu);
				return FALSE;
			}
			/* we provide the largest MTU possible if we can */
			if (port_mtu)
				mtu = port_mtu;
			else if (mtu_required < sa->p_subn->min_ca_mtu)
				mtu = sa->p_subn->min_ca_mtu;
			else
				mtu++;
			break;
		case 1:	/* Less than MTU specified */
			/* use the smaller of the two:
			   a. one lower then the required
			   b. the mtu of the requesting port (if exists) */
			if (port_mtu && mtu_required > port_mtu)
				mtu = port_mtu;
			else
				mtu--;
			break;
		case 2:	/* Exactly MTU specified */
		default:
			break;
		}
		/* make sure it still is in the range */
		if (mtu < IB_MIN_MTU || mtu > IB_MAX_MTU) {
			OSM_LOG(p_log, OSM_LOG_VERBOSE,
				"Calculated MTU %x is out of range\n", mtu);
			return FALSE;
		}
	}
	p_mcm_rec->mtu = (mtu_sel << 6) | mtu;

	if (p_physp) {
		extended = p_pi->capability_mask & IB_PORT_CAP_HAS_EXT_SPEEDS;
		port_rate = ib_port_info_compute_rate(p_pi, extended);
	} else
		port_rate = 0;

	if (!(comp_mask & IB_MCR_COMPMASK_RATE)
	    || !(comp_mask & IB_MCR_COMPMASK_RATE_SEL)
	    || (rate_sel = (p_mcm_rec->rate >> 6)) == 3)
		rate = port_rate ? port_rate : sa->p_subn->min_ca_rate;
	else {
		rate_required = (uint8_t) (p_mcm_rec->rate & 0x3F);
		rate = rate_required;
		switch (rate_sel) {
		case 0:	/* Greater than RATE specified */
			if (ib_path_compare_rates(rate_required, port_rate) >= 0) {
				OSM_LOG(p_log, OSM_LOG_VERBOSE,
					"Requested RATE %x >= the port\'s rate:%x\n",
					rate_required, port_rate);
				return FALSE;
			}
			/* we provide the largest RATE possible if we can */
			if (port_rate)
				rate = port_rate;
			else if (ib_path_compare_rates(rate_required,
						       sa->p_subn->min_ca_rate) < 0)
				rate = sa->p_subn->min_ca_rate;
			else
				rate = ib_path_rate_get_next(rate);
			break;
		case 1:	/* Less than RATE specified */
			/* use the smaller of the two:
			   a. one lower then the required
			   b. the rate of the requesting port (if exists) */
			if (ib_path_compare_rates(rate_required, port_rate) > 0)
				rate = port_rate;
			else
				rate = ib_path_rate_get_prev(rate);
			break;
		case 2:	/* Exactly RATE specified */
		default:
			break;
		}
		/* make sure it still is in the range */
		if (rate < IB_MIN_RATE || rate > IB_MAX_RATE) {
			OSM_LOG(p_log, OSM_LOG_VERBOSE,
				"Calculated RATE %x is out of range\n", rate);
			return FALSE;
		}
	}
	p_mcm_rec->rate = (rate_sel << 6) | rate;

	OSM_LOG_EXIT(sa->p_log);
	return TRUE;
}