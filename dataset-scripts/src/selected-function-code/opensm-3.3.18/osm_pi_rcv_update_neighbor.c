static int osm_pi_rcv_update_neighbor(IN osm_physp_t *p_physp)
{
	osm_physp_t *p_rem_physp = p_physp->p_remote_physp;
	osm_node_t *p_node;

	/*
	 * Our own port - this is the only case where CA port
	 * is discovered before its' neighbor port
	 */
	if (!p_rem_physp)
		return p_physp->need_update;

	p_node = osm_physp_get_node_ptr(p_rem_physp);
	CL_ASSERT(p_node);

	/* CA/RTR to CA/RTR connection */
	if (!p_node->sw)
		return p_physp->need_update;

	return (ib_switch_info_get_state_change(&p_node->sw->switch_info) ? 1 : p_physp->need_update);
}