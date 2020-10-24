static int
fabric_construct_hca_ports(IN ftree_fabric_t * p_ftree, IN ftree_hca_t * p_hca)
{
	ftree_sw_t *p_remote_sw;
	osm_node_t *p_node = p_hca->p_osm_node;
	osm_node_t *p_remote_node;
	uint8_t remote_node_type;
	ib_net64_t remote_node_guid;
	osm_physp_t *p_remote_osm_port;
	uint8_t i;
	uint8_t remote_port_num;
	boolean_t is_cn = TRUE;
	boolean_t is_in_cn_file = FALSE;
	boolean_t is_io = FALSE;
	boolean_t is_cns_file_provided = fabric_cns_provided(p_ftree);
	boolean_t is_ios_file_provided = fabric_ios_provided(p_ftree);
	int res = 0;

	for (i = 0; i < osm_node_get_num_physp(p_node); i++) {
		osm_physp_t *p_osm_port = osm_node_get_physp_ptr(p_node, i);
		if (!p_osm_port || !osm_link_is_healthy(p_osm_port))
			continue;

		p_remote_osm_port = osm_physp_get_remote(p_osm_port);
		p_remote_node =
		    osm_node_get_remote_node(p_node, i, &remote_port_num);

		if (!p_remote_osm_port || !p_remote_node)
			continue;

		remote_node_type = osm_node_get_type(p_remote_node);
		remote_node_guid = osm_node_get_node_guid(p_remote_node);

		switch (remote_node_type) {
		case IB_NODE_TYPE_ROUTER:
			/* leaving this port - proceeding to the next one */
			continue;

		case IB_NODE_TYPE_CA:
			/* HCA connected directly to another HCA - not FatTree */
			OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_ERROR,
				"ERR AB11: "
				"CA conected directly to another CA: " "0x%016"
				PRIx64 " <---> 0x%016" PRIx64 "\n",
				cl_ntoh64(osm_node_get_node_guid(p_node)),
				cl_ntoh64(remote_node_guid));
			res = -1;
			goto Exit;

		case IB_NODE_TYPE_SWITCH:
			/* continue with this port */
			break;

		default:
			OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_ERROR,
				"ERR AB12: Node GUID 0x%016" PRIx64
				" - Unknown node type: %s\n",
				cl_ntoh64(remote_node_guid),
				ib_get_node_type_str(remote_node_type));
			res = -1;
			goto Exit;
		}

		/* remote node is switch */

		p_remote_sw = fabric_get_sw_by_guid(p_ftree, remote_node_guid);
		CL_ASSERT(p_remote_sw);

		/* If CN file is not supplied, then all the CAs considered as Compute Nodes.
		   Otherwise all the CAs are not CNs, and only guids that are present in the
		   CN file will be marked as compute nodes. */
		if (is_cns_file_provided == TRUE) {
			name_map_item_t *p_elem = (name_map_item_t *)
			cl_qmap_get(&p_ftree->cn_guid_tbl,
				    cl_ntoh64(osm_physp_get_port_guid
					     (p_osm_port)));
			if (p_elem == (name_map_item_t *)
				cl_qmap_end(&p_ftree->cn_guid_tbl))
				is_cn = FALSE;
			else
				is_in_cn_file = TRUE;
		}
		if (is_in_cn_file == FALSE && is_ios_file_provided == TRUE) {
			name_map_item_t *p_elem = (name_map_item_t *)
			cl_qmap_get(&p_ftree->io_guid_tbl,
				    cl_ntoh64(osm_physp_get_port_guid
					     (p_osm_port)));
			if (p_elem != (name_map_item_t *)
				cl_qmap_end(&p_ftree->io_guid_tbl)) {
				is_io = TRUE;
				is_cn = FALSE;
			}
		}

		if (is_cn) {
			p_ftree->cn_num++;
			p_hca->cn_num++;
			OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_DEBUG,
				"Marking CN port GUID 0x%016" PRIx64 "\n",
				cl_ntoh64(osm_physp_get_port_guid(p_osm_port)));
		} else if (is_io) {
				OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_DEBUG,
					"Marking I/O port GUID 0x%016" PRIx64
					"\n",
					cl_ntoh64(osm_physp_get_port_guid
						 (p_osm_port)));

		} else {
				OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_DEBUG,
					"Marking non-CN port GUID 0x%016" PRIx64
					"\n",
					cl_ntoh64(osm_physp_get_port_guid
						 (p_osm_port)));
		}
		p_ftree->ca_ports++;

		hca_add_port(p_ftree,
			     p_hca,	/* local ftree_hca object */
			     i,	/* local port number */
			     remote_port_num,	/* remote port number */
			     cl_ntoh16(osm_node_get_base_lid(p_node, i)),	/* local lid */
			     cl_ntoh16(osm_node_get_base_lid(p_remote_node, 0)),	/* remote lid */
			     osm_physp_get_port_guid(p_osm_port),	/* local port guid */
			     osm_physp_get_port_guid(p_remote_osm_port),	/* remote port guid */
			     remote_node_guid,	/* remote node guid */
			     remote_node_type,	/* remote node type */
			     (void *)p_remote_sw,	/* remote ftree_hca/sw object */
			     is_cn, is_io);	/* whether this port is compute node */
	}

Exit:
	return res;
}				/* fabric_construct_hca_ports() */