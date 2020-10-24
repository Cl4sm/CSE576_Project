static void set_guidinfo(IN osm_sa_t *sa, IN osm_madw_t *p_madw,
			 IN osm_port_t *p_port, IN uint8_t block_num)
{
	uint32_t max_block;
	int i, j, dirty = 0;
	ib_sa_mad_t *p_sa_mad;
	ib_guidinfo_record_t *p_rcvd_rec;
	osm_assigned_guids_t *p_assigned_guids = 0;
	osm_alias_guid_t *p_alias_guid, *p_alias_guid_check;
	cl_map_item_t *p_item;
	ib_net64_t set_alias_guid, del_alias_guid, assigned_guid;
	uint8_t set_mask;

	max_block = (p_port->p_physp->port_info.guid_cap + GUID_TABLE_MAX_ENTRIES - 1) /
		     GUID_TABLE_MAX_ENTRIES;
	if (block_num >= max_block) {
		OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 5118: "
			"block_num %d is higher than Max GUID Cap block %d "
			"for port GUID 0x%" PRIx64 "\n",
			block_num, max_block, cl_ntoh64(p_port->p_physp->port_guid));
		CL_PLOCK_RELEASE(sa->p_lock);
		osm_sa_send_error(sa, p_madw,
				  IB_SA_MAD_STATUS_NO_RECORDS);
		return;
	}
	if (!p_port->p_physp->p_guids) {
		p_port->p_physp->p_guids = calloc(max_block * GUID_TABLE_MAX_ENTRIES,
						  sizeof(ib_net64_t));
		if (!p_port->p_physp->p_guids) {
			OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 5103: "
				"GUID table memory allocation failed for port "
				"GUID 0x%" PRIx64 "\n",
				cl_ntoh64(p_port->p_physp->port_guid));
			CL_PLOCK_RELEASE(sa->p_lock);
			osm_sa_send_error(sa, p_madw,
					  IB_SA_MAD_STATUS_NO_RESOURCES);
			return;
		}
		/* setup base port guid in index 0 */
		(*p_port->p_physp->p_guids)[0] = p_port->p_physp->port_guid;
	}

	p_sa_mad = osm_madw_get_sa_mad_ptr(p_madw);
	p_rcvd_rec = (ib_guidinfo_record_t *) ib_sa_mad_get_payload_ptr(p_sa_mad);

	if (OSM_LOG_IS_ACTIVE_V2(sa->p_log, OSM_LOG_DEBUG)) {
		OSM_LOG(sa->p_log, OSM_LOG_DEBUG, "Dump of incoming record\n");
		osm_dump_guidinfo_record_v2(sa->p_log, p_rcvd_rec, FILE_ID, OSM_LOG_DEBUG);
	}

	set_mask = coalesce_comp_mask(p_madw);

	for (i = block_num * GUID_TABLE_MAX_ENTRIES;
	     (block_num + 1) * GUID_TABLE_MAX_ENTRIES < p_port->p_physp->port_info.guid_cap ? i < (block_num + 1) * GUID_TABLE_MAX_ENTRIES : i < p_port->p_physp->port_info.guid_cap;
	     i++) {
		/* can't set block 0 index 0 (base guid is RO) for alias guid table */
		if (i == 0 && p_sa_mad->comp_mask & IB_GIR_COMPMASK_GID0) {
			CL_PLOCK_RELEASE(sa->p_lock);
			OSM_LOG(sa->p_log, OSM_LOG_DEBUG,
				"Not allowed to set RO GID 0\n");
			osm_sa_send_error(sa, p_madw,
					  IB_SA_MAD_STATUS_REQ_INVALID);
			return;
		}

		if (!(set_mask & 1<<(i % 8)))
			continue;

		set_alias_guid = p_rcvd_rec->guid_info.guid[i % 8];
		if (!set_alias_guid) {
			/* was a GUID previously assigned for this index ? */
			set_alias_guid = (*p_port->p_physp->p_guids)[i];
			if (set_alias_guid) {
				p_rcvd_rec->guid_info.guid[i % 8] = set_alias_guid;
				continue;
			}
			/* Is there a persistent SA assigned guid for this index ? */
			if (!p_assigned_guids)
				p_assigned_guids =
				    osm_get_assigned_guids_by_guid(sa->p_subn,
								   p_port->p_physp->port_guid);
			if (p_assigned_guids) {
				set_alias_guid = p_assigned_guids->assigned_guid[i];
				if (set_alias_guid) {
					p_rcvd_rec->guid_info.guid[i % 8] = set_alias_guid;
					p_item = cl_qmap_get(&sa->sm->p_subn->alias_port_guid_tbl,
							     set_alias_guid);
					if (p_item == cl_qmap_end(&sa->sm->p_subn->alias_port_guid_tbl))
						goto add_alias_guid;
					else {
						p_alias_guid = (osm_alias_guid_t *) p_item;
						if (p_alias_guid->p_base_port != p_port) {
							OSM_LOG(sa->p_log,
								OSM_LOG_ERROR,
								"ERR 5110: "
								" Assigned alias port GUID 0x%" PRIx64
								" index %d base port GUID 0x%" PRIx64
								" now attempted on port GUID 0x%" PRIx64
								"\n",
								cl_ntoh64(p_alias_guid->alias_guid), i,
								cl_ntoh64(p_alias_guid->p_base_port->guid),
								cl_ntoh64(p_port->guid));
							/* clear response guid at index to indicate duplicate */
							p_rcvd_rec->guid_info.guid[i % 8] = 0;
						}
						continue;
					}
				}
			}
		}
		if (!set_alias_guid) {
			for (j = 0; j < 1000; j++) {
				assigned_guid = sm_assigned_guid(sa->p_subn->opt.sm_assigned_guid);
				if (!assigned_guid) {
					CL_PLOCK_RELEASE(sa->p_lock);
					OSM_LOG(sa->p_log, OSM_LOG_ERROR,
						"ERR 510E: No more assigned guids available\n");
					osm_sa_send_error(sa, p_madw,
							  IB_SA_MAD_STATUS_NO_RESOURCES);
					return;
				}
				p_item = cl_qmap_get(&sa->sm->p_subn->alias_port_guid_tbl,
						     assigned_guid);
				if (p_item == cl_qmap_end(&sa->sm->p_subn->alias_port_guid_tbl)) {
					set_alias_guid = assigned_guid;
					p_rcvd_rec->guid_info.guid[i % 8] = assigned_guid;
					if (!p_assigned_guids) {
						p_assigned_guids = osm_assigned_guids_new(p_port->p_physp->port_guid,
											  max_block * GUID_TABLE_MAX_ENTRIES);
						if (p_assigned_guids) {
							cl_qmap_insert(&(sa->p_subn->assigned_guids_tbl),
								       p_assigned_guids->port_guid,
								       &p_assigned_guids->map_item);
						} else {
							OSM_LOG(sa->p_log,
								OSM_LOG_ERROR,
								"ERR 510D: osm_assigned_guids_new failed port GUID 0x%" PRIx64 " index %d\n",
								cl_ntoh64(p_port->p_physp->port_guid), i);
							CL_PLOCK_RELEASE(sa->p_lock);
							osm_sa_send_error(sa, p_madw,
									  IB_SA_MAD_STATUS_NO_RESOURCES);
							return;
						}
					}
					if (p_assigned_guids)
						p_assigned_guids->assigned_guid[i] = assigned_guid;
					break;
				}
			}
			if (!set_alias_guid) {
				OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 510A: "
					"SA assigned GUID %d failed for "
					"port GUID 0x%" PRIx64 "\n", i,
					cl_ntoh64(p_port->p_physp->port_guid));
				continue;
			}
		}

add_alias_guid:
		/* allocate alias guid and add to alias guid table */
		p_alias_guid = osm_alias_guid_new(set_alias_guid, p_port);
		if (!p_alias_guid) {
			OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 5107: "
				"Alias guid %d memory allocation failed"
				" for port GUID 0x%" PRIx64 "\n",
				i, cl_ntoh64(p_port->p_physp->port_guid));
			CL_PLOCK_RELEASE(sa->p_lock);
			return;
		}

		p_alias_guid_check =
			(osm_alias_guid_t *) cl_qmap_insert(&sa->sm->p_subn->alias_port_guid_tbl,
							    p_alias_guid->alias_guid,
							    &p_alias_guid->map_item);
		if (p_alias_guid_check != p_alias_guid) {
			/* alias GUID is a duplicate if it exists on another port or on the same port but at another index */
			if (p_alias_guid_check->p_base_port != p_port ||
			    (*p_port->p_physp->p_guids)[i] != set_alias_guid) {
				OSM_LOG(sa->p_log, OSM_LOG_ERROR, "ERR 5108: "
					"Duplicate alias port GUID 0x%" PRIx64
					" index %d base port GUID 0x%" PRIx64
					", alias GUID already assigned to "
					"base port GUID 0x%" PRIx64 "\n",
					cl_ntoh64(p_alias_guid->alias_guid), i,
					cl_ntoh64(p_alias_guid->p_base_port->guid),
					cl_ntoh64(p_alias_guid_check->p_base_port->guid));
				osm_alias_guid_delete(&p_alias_guid);
				/* clear response guid at index to indicate duplicate */
				p_rcvd_rec->guid_info.guid[i % 8] = 0;
			}
		} else {
			del_alias_guid = (*p_port->p_physp->p_guids)[i];
			if (del_alias_guid) {
				/* remove original from alias guid table */
				p_alias_guid_check = (osm_alias_guid_t *)
					cl_qmap_remove(&sa->p_subn->alias_port_guid_tbl,
						       del_alias_guid);
				if (p_alias_guid_check)
					osm_alias_guid_delete(&p_alias_guid_check);
				else
					OSM_LOG(sa->p_log, OSM_LOG_ERROR,
						"ERR 510C: Original alias GUID "
						"0x%" PRIx64 "at index %u "
						"not found\n",
						cl_ntoh64(del_alias_guid),
						i);
			}

			/* insert or replace guid at index */
			(*p_port->p_physp->p_guids)[i] = set_alias_guid;
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

	CL_PLOCK_RELEASE(sa->p_lock);
	gir_respond(sa, p_madw);
}