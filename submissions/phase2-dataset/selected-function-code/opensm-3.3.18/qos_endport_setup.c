static int qos_endport_setup(osm_sm_t * sm, osm_physp_t * p,
			     const struct qos_config *qcfg, int vlarb_only,
			     cl_qlist_t *port_mad_list)
{
	unsigned force_update = p->need_update || sm->p_subn->need_update;
	struct osm_routing_engine *re = sm->p_subn->p_osm->routing_engine_used;
	const ib_slvl_table_t *port_sl2vl = &qcfg->sl2vl;
	ib_slvl_table_t routing_sl2vl;

	p->vl_high_limit = qcfg->vl_high_limit;
	if (vlarb_update(sm, p, 0, force_update, qcfg, port_mad_list))
		return -1;
	if (vlarb_only)
		return 0;

	if (!(p->port_info.capability_mask & IB_PORT_CAP_HAS_SL_MAP))
		return 0;

	if (re && re->update_sl2vl) {
		routing_sl2vl = *port_sl2vl;
		re->update_sl2vl(re->context, p, 0, 0, &routing_sl2vl);
		port_sl2vl = &routing_sl2vl;
	}
	if (sl2vl_update_table(sm, p, 0, 0, force_update, port_sl2vl,
			       port_mad_list))
		return -1;

	return 0;
}