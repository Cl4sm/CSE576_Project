static void ucast_cache_validate(osm_ucast_mgr_t * p_mgr)
{
	cache_switch_t *p_cache_sw;
	cache_switch_t *p_remote_cache_sw;
	unsigned port_num;
	unsigned max_ports;
	uint8_t remote_node_type;
	uint16_t lid_ho;
	uint16_t remote_lid_ho;
	osm_switch_t *p_sw;
	osm_switch_t *p_remote_sw;
	osm_node_t *p_node;
	osm_physp_t *p_physp;
	osm_physp_t *p_remote_physp;
	osm_port_t *p_remote_port;
	cl_qmap_t *p_sw_tbl;

	OSM_LOG_ENTER(p_mgr->p_log);
	if (!p_mgr->cache_valid)
		goto Exit;

	/* If there are no switches in the subnet, we are done */
	p_sw_tbl = &p_mgr->p_subn->sw_guid_tbl;
	if (cl_qmap_count(p_sw_tbl) == 0) {
		osm_ucast_cache_invalidate(p_mgr);
		goto Exit;
	}

	/*
	 * Scan all the physical switch ports in the subnet.
	 * If the port need_update flag is on, check whether
	 * it's just some node/port reset or a cached topology
	 * change. Otherwise the cache is invalid.
	 */
	for (p_sw = (osm_switch_t *) cl_qmap_head(p_sw_tbl);
	     p_sw != (osm_switch_t *) cl_qmap_end(p_sw_tbl);
	     p_sw = (osm_switch_t *) cl_qmap_next(&p_sw->map_item)) {

		p_node = p_sw->p_node;

		lid_ho = cl_ntoh16(osm_node_get_base_lid(p_node, 0));
		p_cache_sw = cache_get_sw(p_mgr, lid_ho);

		max_ports = osm_node_get_num_physp(p_node);

		/* skip port 0 */
		for (port_num = 1; port_num < max_ports; port_num++) {

			p_physp = osm_node_get_physp_ptr(p_node, port_num);

			if (!p_physp || !p_physp->p_remote_physp ||
			    !osm_physp_link_exists(p_physp,
						   p_physp->p_remote_physp))
				/* no valid link */
				continue;

			/*
			 * While scanning all the physical ports in the subnet,
			 * mark corresponding leaf switches in the cache.
			 */
			if (p_cache_sw &&
			    !p_cache_sw->dropped &&
			    !cache_sw_is_leaf(p_cache_sw) &&
			    p_physp->p_remote_physp->p_node &&
			    osm_node_get_type(p_physp->p_remote_physp->
					      p_node) != IB_NODE_TYPE_SWITCH)
				cache_sw_set_leaf(p_cache_sw);

			if (!p_physp->need_update)
				continue;

			OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
				"Checking switch lid %u, port %u\n",
				lid_ho, port_num);

			p_remote_physp = osm_physp_get_remote(p_physp);
			remote_node_type =
			    osm_node_get_type(p_remote_physp->p_node);

			if (remote_node_type == IB_NODE_TYPE_SWITCH)
				remote_lid_ho =
				    cl_ntoh16(osm_node_get_base_lid
					      (p_remote_physp->p_node, 0));
			else
				remote_lid_ho =
				    cl_ntoh16(osm_node_get_base_lid
					      (p_remote_physp->p_node,
					       osm_physp_get_port_num
					       (p_remote_physp)));

			if (!p_cache_sw ||
			    port_num >= p_cache_sw->num_ports ||
			    !p_cache_sw->ports[port_num].remote_lid_ho) {
				/*
				 * There is some uncached change on the port.
				 * In general, the reasons might be as follows:
				 *  - switch reset
				 *  - port reset (or port down/up)
				 *  - quick connection location change
				 *  - new link (or new switch)
				 *
				 * First two reasons allow cache usage, while
				 * the last two reasons should invalidate cache.
				 *
				 * In case of quick connection location change,
				 * cache would have been invalidated by
				 * osm_ucast_cache_check_new_link() function.
				 *
				 * In case of new link between two known nodes,
				 * cache also would have been invalidated by
				 * osm_ucast_cache_check_new_link() function.
				 *
				 * Another reason is cached link between two
				 * known switches went back. In this case the
				 * osm_ucast_cache_check_new_link() function would
				 * clear both sides of the link from the cache
				 * during the discovery process, so effectively
				 * this would be equivalent to port reset.
				 *
				 * So three possible reasons remain:
				 *  - switch reset
				 *  - port reset (or port down/up)
				 *  - link of a new switch
				 *
				 * To validate cache, we need to check only the
				 * third reason - link of a new node/switch:
				 *  - If this is the local switch that is new,
				 *    then it should have (p_sw->need_update == 2).
				 *  - If the remote node is switch and it's new,
				 *    then it also should have
				 *    (p_sw->need_update == 2).
				 *  - If the remote node is CA/RTR and it's new,
				 *    then its port should have is_new flag on.
				 */
				if (p_sw->need_update == 2) {
					OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
						"New switch found (lid %u)\n",
						lid_ho);
					osm_ucast_cache_invalidate(p_mgr);
					goto Exit;
				}

				if (remote_node_type == IB_NODE_TYPE_SWITCH) {

					p_remote_sw =
					    p_remote_physp->p_node->sw;
					if (p_remote_sw->need_update == 2) {
						/* this could also be case of
						   switch coming back with an
						   additional link that it
						   didn't have before */
						OSM_LOG(p_mgr->p_log,
							OSM_LOG_DEBUG,
							"New switch/link found (lid %u)\n",
							remote_lid_ho);
						osm_ucast_cache_invalidate
						    (p_mgr);
						goto Exit;
					}
				} else {
					/*
					 * Remote node is CA/RTR.
					 * Get p_port of the remote node and
					 * check its p_port->is_new flag.
					 */
					p_remote_port =
					    osm_get_port_by_guid(p_mgr->p_subn,
								 osm_physp_get_port_guid
								 (p_remote_physp));
					if (!p_remote_port) {
						OSM_LOG(p_mgr->p_log,
							OSM_LOG_ERROR,
							"ERR AD04: No port was found for "
							"port GUID 0x%" PRIx64 "\n",
							cl_ntoh64(osm_physp_get_port_guid
								      (p_remote_physp)));
						osm_ucast_cache_invalidate
						    (p_mgr);
						goto Exit;
					}
					if (p_remote_port->is_new) {
						OSM_LOG(p_mgr->p_log,
							OSM_LOG_DEBUG,
							"New CA/RTR found (lid %u)\n",
							remote_lid_ho);
						osm_ucast_cache_invalidate
						    (p_mgr);
						goto Exit;
					}
				}
			} else {
				/*
				 * The change on the port is cached.
				 * In general, the reasons might be as follows:
				 *  - link between two known nodes went back
				 *  - one or more nodes went back, causing all
				 *    the links to reappear
				 *
				 * If it was link that went back, then this case
				 * would have been taken care of during the
				 * discovery by osm_ucast_cache_check_new_link(),
				 * so it's some node that went back.
				 */
				if ((p_cache_sw->ports[port_num].is_leaf &&
				     remote_node_type == IB_NODE_TYPE_SWITCH) ||
				    (!p_cache_sw->ports[port_num].is_leaf &&
				     remote_node_type != IB_NODE_TYPE_SWITCH)) {
					OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
						"Remote node type change on switch lid %u, port %u\n",
						lid_ho, port_num);
					osm_ucast_cache_invalidate(p_mgr);
					goto Exit;
				}

				if (p_cache_sw->ports[port_num].remote_lid_ho !=
				    remote_lid_ho) {
					OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
						"Remote lid change on switch lid %u, port %u"
						"(was %u, now %u)\n",
						lid_ho, port_num,
						p_cache_sw->ports[port_num].
						remote_lid_ho, remote_lid_ho);
					osm_ucast_cache_invalidate(p_mgr);
					goto Exit;
				}

				/*
				 * We don't care who is the node that has
				 * reappeared in the subnet (local or remote).
				 * What's important that the cached link matches
				 * the real fabrics link.
				 * Just clean it from cache.
				 */

				p_cache_sw->ports[port_num].remote_lid_ho = 0;
				p_cache_sw->ports[port_num].is_leaf = FALSE;
				if (p_cache_sw->dropped) {
					cache_restore_ucast_info(p_mgr,
								 p_cache_sw,
								 p_sw);
					p_cache_sw->dropped = FALSE;
				}

				OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
					"Restored link from cache: lid %u, port %u to lid %u\n",
					lid_ho, port_num, remote_lid_ho);
			}
		}
	}

	/* Remove all the cached switches that
	   have all their ports restored */
	cache_cleanup_switches(p_mgr);

	/*
	 * Done scanning all the physical switch ports in the subnet.
	 * Now we need to check the other side:
	 * Scan all the cached switches and their ports:
	 *  - If the cached switch is missing in the subnet
	 *    (dropped flag is on), check that it's a leaf switch.
	 *    If it's not a leaf, the cache is invalid, because
	 *    cache can tolerate only leaf switch removal.
	 *  - If the cached switch exists in fabric, check all
	 *    its cached ports. These cached ports represent
	 *    missing link in the fabric.
	 *    The missing links that can be tolerated are:
	 *      + link to missing CA/RTR
	 *      + link to missing leaf switch
	 */
	for (p_cache_sw = (cache_switch_t *) cl_qmap_head(&p_mgr->cache_sw_tbl);
	     p_cache_sw != (cache_switch_t *) cl_qmap_end(&p_mgr->cache_sw_tbl);
	     p_cache_sw =
	     (cache_switch_t *) cl_qmap_next(&p_cache_sw->map_item)) {

		if (p_cache_sw->dropped) {
			if (!cache_sw_is_leaf(p_cache_sw)) {
				OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
					"Missing non-leaf switch (lid %u)\n",
					cache_sw_get_base_lid_ho(p_cache_sw));
				osm_ucast_cache_invalidate(p_mgr);
				goto Exit;
			}

			OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
				"Missing leaf switch (lid %u) - "
				"continuing validation\n",
				cache_sw_get_base_lid_ho(p_cache_sw));
			continue;
		}

		for (port_num = 1; port_num < p_cache_sw->num_ports; port_num++) {
			if (!p_cache_sw->ports[port_num].remote_lid_ho)
				continue;

			if (p_cache_sw->ports[port_num].is_leaf) {
				CL_ASSERT(cache_sw_is_leaf(p_cache_sw));
				OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
					"Switch lid %u, port %u: missing link to CA/RTR - "
					"continuing validation\n",
					cache_sw_get_base_lid_ho(p_cache_sw),
					port_num);
				continue;
			}

			p_remote_cache_sw = cache_get_sw(p_mgr,
							 p_cache_sw->
							 ports[port_num].
							 remote_lid_ho);

			if (!p_remote_cache_sw || !p_remote_cache_sw->dropped) {
				OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
					"Switch lid %u, port %u: missing link to existing switch\n",
					cache_sw_get_base_lid_ho(p_cache_sw),
					port_num);
				osm_ucast_cache_invalidate(p_mgr);
				goto Exit;
			}

			if (!cache_sw_is_leaf(p_remote_cache_sw)) {
				OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
					"Switch lid %u, port %u: missing link to non-leaf switch\n",
					cache_sw_get_base_lid_ho(p_cache_sw),
					port_num);
				osm_ucast_cache_invalidate(p_mgr);
				goto Exit;
			}

			/*
			 * At this point we know that the missing link is to
			 * a leaf switch. However, one case deserves a special
			 * treatment. If there was a link between two leaf
			 * switches, then missing leaf switch might break
			 * routing. It is possible that there are routes
			 * that use leaf switches to get from switch to switch
			 * and not just to get to the CAs behind the leaf switch.
			 */
			if (cache_sw_is_leaf(p_cache_sw) &&
			    cache_sw_is_leaf(p_remote_cache_sw)) {
				OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
					"Switch lid %u, port %u: missing leaf-2-leaf link\n",
					cache_sw_get_base_lid_ho(p_cache_sw),
					port_num);
				osm_ucast_cache_invalidate(p_mgr);
				goto Exit;
			}

			OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG,
				"Switch lid %u, port %u: missing remote leaf switch - "
				"continuing validation\n",
				cache_sw_get_base_lid_ho(p_cache_sw),
				port_num);
		}
	}

	OSM_LOG(p_mgr->p_log, OSM_LOG_DEBUG, "Unicast cache is valid\n");
	ucast_cache_dump(p_mgr);
Exit:
	OSM_LOG_EXIT(p_mgr->p_log);
}				/* osm_ucast_cache_validate() */