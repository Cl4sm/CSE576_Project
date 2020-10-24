static osm_mgrp_t *load_mcgroup(osm_opensm_t * p_osm, ib_net16_t mlid,
				ib_member_rec_t * p_mcm_rec)
{
	ib_net64_t comp_mask;
	osm_mgrp_t *p_mgrp;

	cl_plock_excl_acquire(&p_osm->lock);

	p_mgrp = osm_get_mgrp_by_mgid(&p_osm->subn, &p_mcm_rec->mgid);
	if (p_mgrp) {
		if (p_mgrp->mlid == mlid) {
			OSM_LOG(&p_osm->log, OSM_LOG_DEBUG,
				"mgrp %04x is already here.", cl_ntoh16(mlid));
			goto _out;
		}
		OSM_LOG(&p_osm->log, OSM_LOG_VERBOSE,
			"mlid %04x is already used by another MC group. Will "
			"request clients reregistration.\n", cl_ntoh16(mlid));
		p_mgrp = NULL;
		goto _out;
	}

	comp_mask = IB_MCR_COMPMASK_MTU | IB_MCR_COMPMASK_MTU_SEL
	    | IB_MCR_COMPMASK_RATE | IB_MCR_COMPMASK_RATE_SEL;
	if (!(p_mgrp = osm_mcmr_rcv_find_or_create_new_mgrp(&p_osm->sa,
							    comp_mask,
							    p_mcm_rec)) ||
	    p_mgrp->mlid != mlid) {
		OSM_LOG(&p_osm->log, OSM_LOG_ERROR,
			"cannot create MC group with mlid 0x%04x and mgid "
			"0x%016" PRIx64 ":0x%016" PRIx64 "\n", cl_ntoh16(mlid),
			cl_ntoh64(p_mcm_rec->mgid.unicast.prefix),
			cl_ntoh64(p_mcm_rec->mgid.unicast.interface_id));
		p_mgrp = NULL;
	}

_out:
	cl_plock_release(&p_osm->lock);

	return p_mgrp;
}