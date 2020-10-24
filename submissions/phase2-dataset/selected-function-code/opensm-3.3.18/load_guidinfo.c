static int load_guidinfo(osm_opensm_t * p_osm, ib_net64_t base_guid,
			 ib_guidinfo_record_t *gir)
{
	osm_port_t *p_port;
	uint32_t max_block;
	int i, ret = 0;
	osm_alias_guid_t *p_alias_guid, *p_alias_guid_check;

	cl_plock_excl_acquire(&p_osm->lock);

	p_port = osm_get_port_by_guid(&p_osm->subn, base_guid);
	if (!p_port)
		goto _out;

	if (!p_port->p_physp->p_guids) {
		max_block = (p_port->p_physp->port_info.guid_cap + GUID_TABLE_MAX_ENTRIES - 1) /
			     GUID_TABLE_MAX_ENTRIES;
		p_port->p_physp->p_guids = calloc(max_block * GUID_TABLE_MAX_ENTRIES,
						  sizeof(ib_net64_t));
		if (!p_port->p_physp->p_guids) {
			OSM_LOG(&p_osm->log, OSM_LOG_ERROR,
				"cannot allocate GUID table for port "
				"GUID 0x%" PRIx64 "\n",
				cl_ntoh64(p_port->p_physp->port_guid));
			goto _out;
		}
	}

	for (i = 0; i < GUID_TABLE_MAX_ENTRIES; i++) {
		if (!gir->guid_info.guid[i])
			continue;
		/* skip block 0 index 0 */
		if (gir->block_num == 0 && i == 0)
			continue;
		if (gir->block_num * GUID_TABLE_MAX_ENTRIES + i >
		    p_port->p_physp->port_info.guid_cap)
			break;

		p_alias_guid = osm_alias_guid_new(gir->guid_info.guid[i],
						  p_port);
		if (!p_alias_guid) {
			OSM_LOG(&p_osm->log, OSM_LOG_ERROR,
				"Alias guid %d memory allocation failed"
				" for port GUID 0x%" PRIx64 "\n",
				gir->block_num * GUID_TABLE_MAX_ENTRIES + i,
				cl_ntoh64(p_port->p_physp->port_guid));
			goto _out;
		}

		p_alias_guid_check =
			(osm_alias_guid_t *) cl_qmap_insert(&p_osm->subn.alias_port_guid_tbl,
							    p_alias_guid->alias_guid,
							    &p_alias_guid->map_item);
		if (p_alias_guid_check != p_alias_guid) {
			/* alias GUID is a duplicate */
			OSM_LOG(&p_osm->log, OSM_LOG_ERROR,
				"Duplicate alias port GUID 0x%" PRIx64
				" index %d base port GUID 0x%" PRIx64 "\n",
				cl_ntoh64(p_alias_guid->alias_guid),
				gir->block_num * GUID_TABLE_MAX_ENTRIES + i,
				cl_ntoh64(p_alias_guid->p_base_port->guid));
			osm_alias_guid_delete(&p_alias_guid);
			goto _out;
		}
	}

	memcpy(&(*p_port->p_physp->p_guids)[gir->block_num * GUID_TABLE_MAX_ENTRIES],
	       &gir->guid_info, sizeof(ib_guid_info_t));

	osm_queue_guidinfo(&p_osm->sa, p_port, gir->block_num);

_out:
	cl_plock_release(&p_osm->lock);

	return ret;
}