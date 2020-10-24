static void del_guidinfo(IN osm_sa_t *sa, IN osm_madw_t *p_madw,
			 IN osm_port_t *p_port, IN uint8_t block_num)
{
	int i;
	uint32_t max_block;
	ib_sa_mad_t *p_sa_mad;
	ib_guidinfo_record_t *p_rcvd_rec;
	ib_net64_t del_alias_guid;
	osm_alias_guid_t *p_alias_guid;
	cl_list_item_t *p_list_item;
	osm_mcm_port_t *p_mcm_port;
	osm_mcm_alias_guid_t *p_mcm_alias_guid;
	uint8_t del_mask;
	int dirty = 0;

	if (!p_port->p_physp->p_guids)
		goto Exit;

	max_block = (p_port->p_physp->port_info.guid_cap + GUID_TABLE_MAX_ENTRIES - 1) /
		     GUID_TABLE_MAX_ENTRIES;

	if (block_num >= max_block) {
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 5116: "
			"block_num %d is higher than Max GUID Cap block %d "
			"for port GUID 0x%" PRIx64 "\n",
			block_num, max_block, cl_ntoh64(p_port->p_physp->port_guid));
		CL_PLOCK_RELEASE(sa->p_lock);
		osm_sa_send_error(sa, p_madw,
				  IB_SA_MAD_STATUS_NO_RECORDS);
		return;
	}

	p_sa_mad = osm_madw_get_sa_mad_ptr(p_madw);
	p_rcvd_rec =
		(ib_guidinfo_record_t *) ib_sa_mad_get_payload_ptr(p_sa_mad);

	del_mask = coalesce_comp_mask(p_madw);

	for (i = block_num * GUID_TABLE_MAX_ENTRIES;
	     (block_num + 1) * GUID_TABLE_MAX_ENTRIES < p_port->p_physp->port_info.guid_cap ? i < (block_num + 1) * GUID_TABLE_MAX_ENTRIES : i < p_port->p_physp->port_info.guid_cap;
	     i++) {
		/* can't delete block 0 index 0 (base guid is RO) for alias guid table */
		if (i == 0 && p_sa_mad->comp_mask & IB_GIR_COMPMASK_GID0) {
			CL_PLOCK_RELEASE(sa->p_lock);
			OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
				"Not allowed to delete RO GID 0\n");
			osm_sa_send_error(sa, p_madw,
					  IB_SA_MAD_STATUS_REQ_INVALID);
			return;
		}
		if (!(del_mask & 1<<(i % 8)))
			continue;

		del_alias_guid = (*p_port->p_physp->p_guids)[i];
		if (del_alias_guid) {
			/* Search all of port's multicast groups for alias */
			p_list_item = cl_qlist_head(&p_port->mcm_list);
			while (p_list_item != cl_qlist_end(&p_port->mcm_list)) {
				p_mcm_port = cl_item_obj(p_list_item,
							 p_mcm_port, list_item);
				p_list_item = cl_qlist_next(p_list_item);
				p_mcm_alias_guid = osm_mgrp_get_mcm_alias_guid(p_mcm_port->mgrp, del_alias_guid);
				if (p_mcm_alias_guid) {
					CL_PLOCK_RELEASE(sa->p_lock);
					osm_sa_send_error(sa, p_madw,
							  IB_SA_MAD_STATUS_DENIED);
					return;
				}
			}
		}
	}

	for (i = block_num * GUID_TABLE_MAX_ENTRIES;
	     (block_num + 1) * GUID_TABLE_MAX_ENTRIES < p_port->p_physp->port_info.guid_cap ? i < (block_num + 1) * GUID_TABLE_MAX_ENTRIES : i < p_port->p_physp->port_info.guid_cap;
	     i++) {
		if (!(del_mask & 1<<(i % 8)))
			continue;

		del_alias_guid = (*p_port->p_physp->p_guids)[i];
		if (del_alias_guid) {
			/* remove original from alias guid table */
			p_alias_guid = (osm_alias_guid_t *)
				cl_qmap_remove(&sa->p_subn->alias_port_guid_tbl,
					       del_alias_guid);
			if (p_alias_guid != (osm_alias_guid_t *)
						cl_qmap_end(&sa->p_subn->alias_port_guid_tbl))
				osm_alias_guid_delete(&p_alias_guid);
			else
				OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 510B: "
					"Original alias GUID 0x%" PRIx64
					" at index %u not found\n",
					cl_ntoh64(del_alias_guid), i);
			/* clear guid at index */
			(*p_port->p_physp->p_guids)[i] = 0;
			dirty = 1;
		}
	}

	if (dirty) {
		if (osm_queue_guidinfo(sa, p_port, block_num))
			osm_sm_signal(sa->sm, OSM_SIGNAL_GUID_PROCESS_REQUEST);
		sa->dirty = TRUE;
	}

	memcpy(&p_rcvd_rec->guid_info,
	       &((*p_port->p_physp->p_guids)[block_num * GUID_TABLE_MAX_ENTRIES]),
	       sizeof(ib_guid_info_t));

Exit:
	CL_PLOCK_RELEASE(sa->p_lock);
	gir_respond(sa, p_madw);
}