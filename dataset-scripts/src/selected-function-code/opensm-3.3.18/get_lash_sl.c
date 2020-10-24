static uint8_t get_lash_sl(void *context, uint8_t path_sl_hint,
			   const ib_net16_t slid, const ib_net16_t dlid)
{
	unsigned dst_id;
	unsigned src_id;
	osm_port_t *p_src_port, *p_dst_port;
	osm_switch_t *p_sw;
	lash_t *p_lash = context;
	osm_opensm_t *p_osm = p_lash->p_osm;

	if (!(p_osm->routing_engine_used &&
	      p_osm->routing_engine_used->type == OSM_ROUTING_ENGINE_TYPE_LASH))
		return OSM_DEFAULT_SL;

	p_src_port = osm_get_port_by_lid(&p_osm->subn, slid);
	if (!p_src_port)
		return OSM_DEFAULT_SL;

	p_dst_port = osm_get_port_by_lid(&p_osm->subn, dlid);
	if (!p_dst_port)
		return OSM_DEFAULT_SL;

	p_sw = get_osm_switch_from_port(p_dst_port);
	if (!p_sw || !p_sw->priv)
		return OSM_DEFAULT_SL;
	dst_id = get_lash_id(p_sw);

	p_sw = get_osm_switch_from_port(p_src_port);
	if (!p_sw || !p_sw->priv)
		return OSM_DEFAULT_SL;

	src_id = get_lash_id(p_sw);
	if (src_id == dst_id)
		return p_osm->subn.opt.lash_start_vl;

	return (uint8_t) ((switch_t *) p_sw->priv)->routing_table[dst_id].lane;
}