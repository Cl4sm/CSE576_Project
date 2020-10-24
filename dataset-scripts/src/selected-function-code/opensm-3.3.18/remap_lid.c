static uint16_t remap_lid(osm_opensm_t * p_osm, uint16_t lid, ib_net64_t guid)
{
	osm_port_t *p_port;
	uint16_t min_lid, max_lid;
	uint8_t lmc;

	p_port = osm_get_port_by_guid(&p_osm->subn, guid);
	if (!p_port) {
		OSM_LOG(&p_osm->log, OSM_LOG_VERBOSE,
			"cannot find port guid 0x%016" PRIx64
			" , will use the same lid\n", cl_ntoh64(guid));
		return lid;
	}

	osm_port_get_lid_range_ho(p_port, &min_lid, &max_lid);
	if (min_lid <= lid && lid <= max_lid)
		return lid;

	lmc = osm_port_get_lmc(p_port);
	return min_lid + (lid & ((1 << lmc) - 1));
}