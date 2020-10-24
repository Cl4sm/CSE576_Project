static void ni_rcv_get_node_desc(IN osm_sm_t * sm, IN osm_node_t * p_node,
				 IN const osm_madw_t * p_madw)
{
	ib_node_info_t *p_ni;
	ib_smp_t *p_smp;
	uint8_t port_num;
	osm_physp_t *p_physp = NULL;

	OSM_LOG_ENTER(sm->p_log);

	p_smp = osm_madw_get_smp_ptr(p_madw);
	p_ni = ib_smp_get_payload_ptr(p_smp);
	port_num = ib_node_info_get_local_port_num(p_ni);

	/*
	   Request PortInfo & NodeDescription attributes for the port
	   that responded to the NodeInfo attribute.
	   Because this is a channel adapter or router, we are
	   not allowed to request PortInfo for the other ports.
	   Set the context union properly, so the recipient
	   knows which node & port are relevant.
	 */
	p_physp = osm_node_get_physp_ptr(p_node, port_num);

	osm_req_get_node_desc(sm, p_physp);

	OSM_LOG_EXIT(sm->p_log);
}