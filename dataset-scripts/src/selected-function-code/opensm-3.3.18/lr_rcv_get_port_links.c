static void lr_rcv_get_port_links(IN osm_sa_t * sa,
				  IN const ib_link_record_t * p_lr,
				  IN const osm_port_t * p_src_port,
				  IN const osm_port_t * p_dest_port,
				  IN const ib_net64_t comp_mask,
				  IN cl_qlist_t * p_list,
				  IN const osm_physp_t * p_req_physp)
{
	const osm_physp_t *p_src_physp;
	const osm_physp_t *p_dest_physp;
	const cl_qmap_t *p_node_tbl;
	osm_node_t *p_node;
	uint8_t port_num;
	uint8_t num_ports;
	uint8_t dest_num_ports;
	uint8_t dest_port_num;

	OSM_LOG_ENTER(sa->p_log);

	if (p_src_port) {
		if (p_dest_port) {
			/*
			   Build an LR for every link connected between both ports.
			   The inner function will discard physp combinations
			   that do not actually connect.  Don't bother screening
			   for that here.
			 */
			num_ports = osm_node_get_num_physp(p_src_port->p_node);
			dest_num_ports =
			    osm_node_get_num_physp(p_dest_port->p_node);
			for (port_num = 1; port_num < num_ports; port_num++) {
				p_src_physp =
				    osm_node_get_physp_ptr(p_src_port->p_node,
							   port_num);
				for (dest_port_num = 1;
				     dest_port_num < dest_num_ports;
				     dest_port_num++) {
					p_dest_physp =
					    osm_node_get_physp_ptr(p_dest_port->
								   p_node,
								   dest_port_num);
					/* both physical ports should be with data */
					if (p_src_physp && p_dest_physp)
						lr_rcv_get_physp_link
						    (sa, p_lr, p_src_physp,
						     p_dest_physp, comp_mask,
						     p_list, p_req_physp);
				}
			}
		} else {
			/*
			   Build an LR for every link connected from the source port.
			 */
			if (comp_mask & IB_LR_COMPMASK_FROM_PORT) {
				port_num = p_lr->from_port_num;
				/* If the port number is out of the range of the p_src_port, then
				   this couldn't be a relevant record. */
				if (port_num <
				    p_src_port->p_node->physp_tbl_size) {
					p_src_physp =
					    osm_node_get_physp_ptr(p_src_port->
								   p_node,
								   port_num);
					if (p_src_physp)
						lr_rcv_get_physp_link
						    (sa, p_lr, p_src_physp,
						     NULL, comp_mask, p_list,
						     p_req_physp);
				}
			} else {
				num_ports =
				    osm_node_get_num_physp(p_src_port->p_node);
				for (port_num = 1; port_num < num_ports;
				     port_num++) {
					p_src_physp =
					    osm_node_get_physp_ptr(p_src_port->
								   p_node,
								   port_num);
					if (p_src_physp)
						lr_rcv_get_physp_link
						    (sa, p_lr, p_src_physp,
						     NULL, comp_mask, p_list,
						     p_req_physp);
				}
			}
		}
	} else {
		if (p_dest_port) {
			/*
			   Build an LR for every link connected to the dest port.
			 */
			if (comp_mask & IB_LR_COMPMASK_TO_PORT) {
				port_num = p_lr->to_port_num;
				/* If the port number is out of the range of the p_dest_port, then
				   this couldn't be a relevant record. */
				if (port_num <
				    p_dest_port->p_node->physp_tbl_size) {
					p_dest_physp =
					    osm_node_get_physp_ptr(p_dest_port->
								   p_node,
								   port_num);
					if (p_dest_physp)
						lr_rcv_get_physp_link
						    (sa, p_lr, NULL,
						     p_dest_physp, comp_mask,
						     p_list, p_req_physp);
				}
			} else {
				num_ports =
				    osm_node_get_num_physp(p_dest_port->p_node);
				for (port_num = 1; port_num < num_ports;
				     port_num++) {
					p_dest_physp =
					    osm_node_get_physp_ptr(p_dest_port->
								   p_node,
								   port_num);
					if (p_dest_physp)
						lr_rcv_get_physp_link
						    (sa, p_lr, NULL,
						     p_dest_physp, comp_mask,
						     p_list, p_req_physp);
				}
			}
		} else {
			/*
			   Process the world (recurse once back into this function).
			 */
			p_node_tbl = &sa->p_subn->node_guid_tbl;
			p_node = (osm_node_t *) cl_qmap_head(p_node_tbl);

			while (p_node != (osm_node_t *) cl_qmap_end(p_node_tbl)) {
				num_ports = osm_node_get_num_physp(p_node);
				for (port_num = 1; port_num < num_ports;
				     port_num++) {
					p_src_physp =
					    osm_node_get_physp_ptr(p_node,
								   port_num);
					if (p_src_physp)
						lr_rcv_get_physp_link
						    (sa, p_lr, p_src_physp,
						     NULL, comp_mask, p_list,
						     p_req_physp);
				}
				p_node = (osm_node_t *) cl_qmap_next(&p_node->
								     map_item);
			}
		}
	}

	OSM_LOG_EXIT(sa->p_log);
}