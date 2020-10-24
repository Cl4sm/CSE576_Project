static void sa_pir_check_physp(IN osm_sa_t * sa, IN const osm_physp_t * p_physp,
			       osm_pir_search_ctxt_t * p_ctxt)
{
	const ib_portinfo_record_t *p_rcvd_rec;
	ib_net64_t comp_mask;
	const ib_port_info_t *p_comp_pi;
	const ib_port_info_t *p_pi;
	const osm_physp_t * p_physp0;
	ib_net32_t cap_mask;

	OSM_LOG_ENTER(sa->p_log);

	p_rcvd_rec = p_ctxt->p_rcvd_rec;
	comp_mask = p_ctxt->comp_mask;
	p_comp_pi = &p_rcvd_rec->port_info;
	p_pi = &p_physp->port_info;

	osm_dump_port_info_v2(sa->p_log, osm_node_get_node_guid(p_physp->p_node),
			      p_physp->port_guid, p_physp->port_num,
			      &p_physp->port_info, FILE_ID, OSM_LOG_DEBUG);

	/* We have to re-check the base_lid, since if the given
	   base_lid in p_pi is zero - we are comparing on all ports. */
	if (comp_mask & IB_PIR_COMPMASK_BASELID) {
		if (p_comp_pi->base_lid != p_pi->base_lid)
			goto Exit;
	}
	if (comp_mask & IB_PIR_COMPMASK_MKEY) {
		if (p_comp_pi->m_key != p_pi->m_key)
			goto Exit;
	}
	if (comp_mask & IB_PIR_COMPMASK_GIDPRE) {
		if (p_comp_pi->subnet_prefix != p_pi->subnet_prefix)
			goto Exit;
	}
	if (comp_mask & IB_PIR_COMPMASK_SMLID) {
		if (p_comp_pi->master_sm_base_lid != p_pi->master_sm_base_lid)
			goto Exit;
	}

	/* IBTA 1.2 errata provides support for bitwise compare if the bit 31
	   of the attribute modifier of the Get/GetTable is set */
	if (comp_mask & IB_PIR_COMPMASK_CAPMASK) {
		if (p_ctxt->is_enhanced_comp_mask) {
			if ((p_comp_pi->capability_mask & p_pi->
			     capability_mask) != p_comp_pi->capability_mask)
				goto Exit;
		} else {
			if (p_comp_pi->capability_mask != p_pi->capability_mask)
				goto Exit;
		}
	}

	if (comp_mask & IB_PIR_COMPMASK_DIAGCODE) {
		if (p_comp_pi->diag_code != p_pi->diag_code)
			goto Exit;
	}
	if (comp_mask & IB_PIR_COMPMASK_MKEYLEASEPRD) {
		if (p_comp_pi->m_key_lease_period != p_pi->m_key_lease_period)
			goto Exit;
	}
	if (comp_mask & IB_PIR_COMPMASK_LOCALPORTNUM) {
		if (p_comp_pi->local_port_num != p_pi->local_port_num)
			goto Exit;
	}
	if (comp_mask & IB_PIR_COMPMASK_LNKWIDTHSUPPORT) {
		if (p_comp_pi->link_width_supported !=
		    p_pi->link_width_supported)
			goto Exit;
	}
	if (comp_mask & IB_PIR_COMPMASK_LNKWIDTHACTIVE) {
		if (p_comp_pi->link_width_active != p_pi->link_width_active)
			goto Exit;
	}
	if (comp_mask & IB_PIR_COMPMASK_LINKWIDTHENABLED) {
		if (p_comp_pi->link_width_enabled != p_pi->link_width_enabled)
			goto Exit;
	}
	if (comp_mask & IB_PIR_COMPMASK_LNKSPEEDSUPPORT) {
		if (ib_port_info_get_link_speed_sup(p_comp_pi) !=
		    ib_port_info_get_link_speed_sup(p_pi))
			goto Exit;
	}
	if (comp_mask & IB_PIR_COMPMASK_PORTSTATE) {
		if (ib_port_info_get_port_state(p_comp_pi) !=
		    ib_port_info_get_port_state(p_pi))
			goto Exit;
	}
	if (comp_mask & IB_PIR_COMPMASK_PORTPHYSTATE) {
		if (ib_port_info_get_port_phys_state(p_comp_pi) !=
		    ib_port_info_get_port_phys_state(p_pi))
			goto Exit;
	}
	if (comp_mask & IB_PIR_COMPMASK_LINKDWNDFLTSTATE) {
		if (ib_port_info_get_link_down_def_state(p_comp_pi) !=
		    ib_port_info_get_link_down_def_state(p_pi))
			goto Exit;
	}
	if (comp_mask & IB_PIR_COMPMASK_MKEYPROTBITS) {
		if (ib_port_info_get_mpb(p_comp_pi) !=
		    ib_port_info_get_mpb(p_pi))
			goto Exit;
	}
	if (comp_mask & IB_PIR_COMPMASK_LMC) {
		if (ib_port_info_get_lmc(p_comp_pi) !=
		    ib_port_info_get_lmc(p_pi))
			goto Exit;
	}
	if (comp_mask & IB_PIR_COMPMASK_LINKSPEEDACTIVE) {
		if (ib_port_info_get_link_speed_active(p_comp_pi) !=
		    ib_port_info_get_link_speed_active(p_pi))
			goto Exit;
	}
	if (comp_mask & IB_PIR_COMPMASK_LINKSPEEDENABLE) {
		if (ib_port_info_get_link_speed_enabled(p_comp_pi) !=
		    ib_port_info_get_link_speed_enabled(p_pi))
			goto Exit;
	}
	if (comp_mask & IB_PIR_COMPMASK_NEIGHBORMTU) {
		if (ib_port_info_get_neighbor_mtu(p_comp_pi) !=
		    ib_port_info_get_neighbor_mtu(p_pi))
			goto Exit;
	}
	if (comp_mask & IB_PIR_COMPMASK_MASTERSMSL) {
		if (ib_port_info_get_master_smsl(p_comp_pi) !=
		    ib_port_info_get_master_smsl(p_pi))
			goto Exit;
	}
	if (comp_mask & IB_PIR_COMPMASK_VLCAP) {
		if (ib_port_info_get_vl_cap(p_comp_pi) !=
		    ib_port_info_get_vl_cap(p_pi))
			goto Exit;
	}
	if (comp_mask & IB_PIR_COMPMASK_INITTYPE) {
		if (ib_port_info_get_init_type(p_comp_pi) !=
		    ib_port_info_get_init_type(p_pi))
			goto Exit;
	}
	if (comp_mask & IB_PIR_COMPMASK_VLHIGHLIMIT) {
		if (p_comp_pi->vl_high_limit != p_pi->vl_high_limit)
			goto Exit;
	}
	if (comp_mask & IB_PIR_COMPMASK_VLARBHIGHCAP) {
		if (p_comp_pi->vl_arb_high_cap != p_pi->vl_arb_high_cap)
			goto Exit;
	}
	if (comp_mask & IB_PIR_COMPMASK_VLARBLOWCAP) {
		if (p_comp_pi->vl_arb_low_cap != p_pi->vl_arb_low_cap)
			goto Exit;
	}
	if (comp_mask & IB_PIR_COMPMASK_MTUCAP) {
		if (ib_port_info_get_mtu_cap(p_comp_pi) !=
		    ib_port_info_get_mtu_cap(p_pi))
			goto Exit;
	}
	if (comp_mask & IB_PIR_COMPMASK_VLSTALLCNT) {
		if (ib_port_info_get_vl_stall_count(p_comp_pi) !=
		    ib_port_info_get_vl_stall_count(p_pi))
			goto Exit;
	}
	if (comp_mask & IB_PIR_COMPMASK_HOQLIFE) {
		if ((p_comp_pi->vl_stall_life & 0x1F) !=
		    (p_pi->vl_stall_life & 0x1F))
			goto Exit;
	}
	if (comp_mask & IB_PIR_COMPMASK_OPVLS) {
		if ((p_comp_pi->vl_enforce & 0xF0) != (p_pi->vl_enforce & 0xF0))
			goto Exit;
	}
	if (comp_mask & IB_PIR_COMPMASK_PARENFIN) {
		if ((p_comp_pi->vl_enforce & 0x08) != (p_pi->vl_enforce & 0x08))
			goto Exit;
	}
	if (comp_mask & IB_PIR_COMPMASK_PARENFOUT) {
		if ((p_comp_pi->vl_enforce & 0x04) != (p_pi->vl_enforce & 0x04))
			goto Exit;
	}
	if (comp_mask & IB_PIR_COMPMASK_FILTERRAWIN) {
		if ((p_comp_pi->vl_enforce & 0x02) != (p_pi->vl_enforce & 0x02))
			goto Exit;
	}
	if (comp_mask & IB_PIR_COMPMASK_FILTERRAWOUT) {
		if ((p_comp_pi->vl_enforce & 0x01) != (p_pi->vl_enforce & 0x01))
			goto Exit;
	}
	if (comp_mask & IB_PIR_COMPMASK_MKEYVIO) {
		if (p_comp_pi->m_key_violations != p_pi->m_key_violations)
			goto Exit;
	}
	if (comp_mask & IB_PIR_COMPMASK_PKEYVIO) {
		if (p_comp_pi->p_key_violations != p_pi->p_key_violations)
			goto Exit;
	}
	if (comp_mask & IB_PIR_COMPMASK_QKEYVIO) {
		if (p_comp_pi->q_key_violations != p_pi->q_key_violations)
			goto Exit;
	}
	if (comp_mask & IB_PIR_COMPMASK_GUIDCAP) {
		if (p_comp_pi->guid_cap != p_pi->guid_cap)
			goto Exit;
	}
	if (comp_mask & IB_PIR_COMPMASK_SUBNTO) {
		if (ib_port_info_get_timeout(p_comp_pi) !=
		    ib_port_info_get_timeout(p_pi))
			goto Exit;
	}
	if (comp_mask & IB_PIR_COMPMASK_RESPTIME) {
		if ((p_comp_pi->resp_time_value & 0x1F) !=
		    (p_pi->resp_time_value & 0x1F))
			goto Exit;
	}
	if (comp_mask & IB_PIR_COMPMASK_LOCALPHYERR) {
		if (ib_port_info_get_local_phy_err_thd(p_comp_pi) !=
		    ib_port_info_get_local_phy_err_thd(p_pi))
			goto Exit;
	}
	if (comp_mask & IB_PIR_COMPMASK_OVERRUNERR) {
		if (ib_port_info_get_overrun_err_thd(p_comp_pi) !=
		    ib_port_info_get_overrun_err_thd(p_pi))
			goto Exit;
	}

	/* IBTA 1.2 errata provides support for bitwise compare if the bit 31
	   of the attribute modifier of the Get/GetTable is set */
	if (comp_mask & IB_PIR_COMPMASK_CAPMASK2) {
		if (p_ctxt->is_enhanced_comp_mask) {
			if ((cl_ntoh16(p_comp_pi->capability_mask2) &
			     cl_ntoh16(p_pi->capability_mask2)) !=
			     cl_ntoh16(p_comp_pi->capability_mask2))
				goto Exit;
		} else {
			if (cl_ntoh16(p_comp_pi->capability_mask2) !=
			    cl_ntoh16(p_pi->capability_mask2))
				goto Exit;
		}
	}
	if (osm_node_get_type(p_physp->p_node) == IB_NODE_TYPE_SWITCH) {
		p_physp0 = osm_node_get_physp_ptr(p_physp->p_node, 0);
		cap_mask = p_physp0->port_info.capability_mask;
	} else
		cap_mask = p_pi->capability_mask;
	if (comp_mask & IB_PIR_COMPMASK_LINKSPDEXTACT) {
		if (((cap_mask & IB_PORT_CAP_HAS_EXT_SPEEDS) > 0) &&
		    (ib_port_info_get_link_speed_ext_active(p_comp_pi) !=
		     ib_port_info_get_link_speed_ext_active(p_pi)))
			goto Exit;
	}
	if (comp_mask & IB_PIR_COMPMASK_LINKSPDEXTSUPP) {
		if (((cap_mask & IB_PORT_CAP_HAS_EXT_SPEEDS) > 0) &&
		    (ib_port_info_get_link_speed_ext_sup(p_comp_pi) !=
		     ib_port_info_get_link_speed_ext_sup(p_pi)))
			goto Exit;
	}
	if (comp_mask & IB_PIR_COMPMASK_LINKSPDEXTENAB) {
		if (((cap_mask & IB_PORT_CAP_HAS_EXT_SPEEDS) > 0) &&
		    (ib_port_info_get_link_speed_ext_enabled(p_comp_pi) !=
		     ib_port_info_get_link_speed_ext_enabled(p_pi)))
			goto Exit;
	}
	sa_pir_create(sa, p_physp, p_ctxt);

Exit:
	OSM_LOG_EXIT(sa->p_log);
}