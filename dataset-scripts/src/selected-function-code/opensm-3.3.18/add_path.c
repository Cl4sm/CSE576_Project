static void add_path(osm_opensm_t * p_osm,
		     osm_switch_t * p_sw, uint16_t lid, uint8_t port_num,
		     ib_net64_t port_guid)
{
	uint16_t new_lid;
	uint8_t old_port;

	new_lid = port_guid ? remap_lid(p_osm, lid, port_guid) : lid;
	old_port = osm_switch_get_port_by_lid(p_sw, new_lid);
	if (old_port != OSM_NO_PATH && old_port != port_num) {
		OSM_LOG(&p_osm->log, OSM_LOG_VERBOSE,
			"LID collision is detected on switch "
			"0x016%" PRIx64 ", will overwrite LID %u entry\n",
			cl_ntoh64(osm_node_get_node_guid(p_sw->p_node)),
			new_lid);
	}

	p_sw->new_lft[new_lid] = port_num;
	if (!(p_osm->subn.opt.port_profile_switch_nodes && port_guid &&
	      osm_get_switch_by_guid(&p_osm->subn, port_guid)))
		osm_switch_count_path(p_sw, port_num);

	OSM_LOG(&p_osm->log, OSM_LOG_DEBUG,
		"route 0x%04x(was 0x%04x) %u 0x%016" PRIx64
		" is added to switch 0x%016" PRIx64 "\n",
		new_lid, lid, port_num, cl_ntoh64(port_guid),
		cl_ntoh64(osm_node_get_node_guid(p_sw->p_node)));
}