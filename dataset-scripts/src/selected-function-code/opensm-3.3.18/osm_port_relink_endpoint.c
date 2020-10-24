static
struct endpoint *osm_port_relink_endpoint(const osm_port_t *osm_port)
{
	guid_t node_guid;
	uint8_t port_num, r_port_num;
	struct t_switch *sw;
	struct endpoint *ep = NULL;
	osm_switch_t *osm_sw;
	osm_physp_t *osm_physp;
	osm_node_t *osm_node, *r_osm_node;

	/*
	 * We need to find the torus endpoint that has the same GUID as
	 * the osm_port.  Rather than search the entire set of endpoints,
	 * we'll try to follow pointers.
	 */
	osm_physp = osm_port->p_physp;
	osm_node = osm_port->p_node;
	port_num = osm_physp_get_port_num(osm_physp);
	node_guid = osm_node_get_node_guid(osm_node);
	/*
	 * Switch management port?
	 */
	if (port_num == 0 &&
	    osm_node_get_type(osm_node) == IB_NODE_TYPE_SWITCH) {

		osm_sw = osm_node->sw;
		if (osm_sw && osm_sw->priv) {
			sw = osm_sw->priv;
			if (sw->osm_switch == osm_sw &&
			    sw->port[0]->n_id == node_guid) {

				ep = sw->port[0];
				goto relink_priv;
			}
		}
	}
	/*
	 * CA port?  Try other end of link.  This should also catch a
	 * router port if it is connected to a switch.
	 */
	r_osm_node = osm_node_get_remote_node(osm_node, port_num, &r_port_num);
	if (!r_osm_node)
		goto out;

	osm_sw = r_osm_node->sw;
	if (!osm_sw)
		goto out;

	sw = osm_sw->priv;
	if (!(sw && sw->osm_switch == osm_sw))
		goto out;

	ep = sw->port[r_port_num];
	if (!(ep && ep->link))
		goto out;

	if (ep->link->end[0].n_id == node_guid) {
		ep = &ep->link->end[0];
		goto relink_priv;
	}
	if (ep->link->end[1].n_id == node_guid) {
		ep = &ep->link->end[1];
		goto relink_priv;
	}
	ep = NULL;
	goto out;

relink_priv:
	/* FIXME:
	 * Unfortunately, we need to cast away const to rebuild the links
	 * between the torus endpoint and the osm_port_t.
	 *
	 * What is really needed is to check whether pr_rcv_get_path_parms()
	 * needs its port objects to be const.  If so, why, and whether
	 * anything can be done about it.
	 */
	((osm_port_t *)osm_port)->priv = ep;
	ep->osm_port = (osm_port_t *)osm_port;
out:
	return ep;
}