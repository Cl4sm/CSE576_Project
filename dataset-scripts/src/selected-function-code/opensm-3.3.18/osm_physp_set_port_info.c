void osm_physp_set_port_info(IN osm_physp_t * p_physp,
					   IN const ib_port_info_t * p_pi,
					   IN const struct osm_sm * p_sm)
{
	CL_ASSERT(p_pi);
	CL_ASSERT(osm_physp_is_valid(p_physp));

	if (ib_port_info_get_port_state(p_pi) == IB_LINK_DOWN) {
		/* If PortState is down, only copy PortState */
		/* and PortPhysicalState per C14-24-2.1 */
		ib_port_info_set_port_state(&p_physp->port_info, IB_LINK_DOWN);
		ib_port_info_set_port_phys_state
		    (ib_port_info_get_port_phys_state(p_pi),
		     &p_physp->port_info);
	} else {
		p_physp->port_info = *p_pi;

		/* The MKey in p_pi can only be considered valid if it's
		 * for a HCA/router or switch port 0, and it's either
		 * non-zero or the MKeyProtect bits are also zero.
		 */
		if ((osm_node_get_type(p_physp->p_node) !=
		     IB_NODE_TYPE_SWITCH || p_physp->port_num == 0) &&
		    (p_pi->m_key != 0 || ib_port_info_get_mpb(p_pi) == 0))
			osm_db_guid2mkey_set(p_sm->p_subn->p_g2m,
					     cl_ntoh64(p_physp->port_guid),
					     cl_ntoh64(p_pi->m_key));
	}
}