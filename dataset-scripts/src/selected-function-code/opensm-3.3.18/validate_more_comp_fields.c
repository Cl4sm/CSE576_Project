static boolean_t validate_more_comp_fields(osm_log_t * p_log,
					   const osm_mgrp_t * p_mgrp,
					   const ib_member_rec_t *
					   p_recvd_mcmember_rec,
					   ib_net64_t comp_mask)
{
	uint8_t mtu_sel;
	uint8_t mtu_required;
	uint8_t mtu_mgrp;
	uint8_t rate_sel;
	uint8_t rate_required;
	uint8_t rate_mgrp;

	if (comp_mask & IB_MCR_COMPMASK_MTU_SEL) {
		mtu_sel = (uint8_t) (p_recvd_mcmember_rec->mtu >> 6);
		/* Clearing last 2 bits */
		mtu_required = (uint8_t) (p_recvd_mcmember_rec->mtu & 0x3F);
		mtu_mgrp = (uint8_t) (p_mgrp->mcmember_rec.mtu & 0x3F);
		switch (mtu_sel) {
		case 0:	/* Greater than MTU specified */
			if (mtu_mgrp <= mtu_required) {
				OSM_LOG(p_log, OSM_LOG_VERBOSE,
					"Requested mcast group has MTU %x, "
					"which is not greater than %x\n",
					mtu_mgrp, mtu_required);
				return FALSE;
			}
			break;
		case 1:	/* Less than MTU specified */
			if (mtu_mgrp >= mtu_required) {
				OSM_LOG(p_log, OSM_LOG_VERBOSE,
					"Requested mcast group has MTU %x, "
					"which is not less than %x\n",
					mtu_mgrp, mtu_required);
				return FALSE;
			}
			break;
		case 2:	/* Exactly MTU specified */
			if (mtu_mgrp != mtu_required) {
				OSM_LOG(p_log, OSM_LOG_VERBOSE,
					"Requested mcast group has MTU %x, "
					"which is not equal to %x\n",
					mtu_mgrp, mtu_required);
				return FALSE;
			}
			break;
		default:
			break;
		}
	}

	/* what about rate ? */
	if (comp_mask & IB_MCR_COMPMASK_RATE_SEL) {
		rate_sel = (uint8_t) (p_recvd_mcmember_rec->rate >> 6);
		/* Clearing last 2 bits */
		rate_required = (uint8_t) (p_recvd_mcmember_rec->rate & 0x3F);
		rate_mgrp = (uint8_t) (p_mgrp->mcmember_rec.rate & 0x3F);
		switch (rate_sel) {
		case 0:	/* Greater than RATE specified */
			if (ib_path_compare_rates(rate_mgrp, rate_required) <= 0) {
				OSM_LOG(p_log, OSM_LOG_VERBOSE,
					"Requested mcast group has RATE %x, "
					"which is not greater than %x\n",
					rate_mgrp, rate_required);
				return FALSE;
			}
			break;
		case 1:	/* Less than RATE specified */
			if (ib_path_compare_rates(rate_mgrp, rate_required) >= 0) {
				OSM_LOG(p_log, OSM_LOG_VERBOSE,
					"Requested mcast group has RATE %x, "
					"which is not less than %x\n",
					rate_mgrp, rate_required);
				return FALSE;
			}
			break;
		case 2:	/* Exactly RATE specified */
			if (ib_path_compare_rates(rate_mgrp, rate_required)) {
				OSM_LOG(p_log, OSM_LOG_VERBOSE,
					"Requested mcast group has RATE %x, "
					"which is not equal to %x\n",
					rate_mgrp, rate_required);
				return FALSE;
			}
			break;
		default:
			break;
		}
	}

	return TRUE;
}