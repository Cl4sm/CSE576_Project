int osm_qos_setup(osm_opensm_t * p_osm)
{
	struct qos_config ca_config, sw0_config, swe_config, rtr_config;
	struct qos_config *cfg;
	cl_qmap_t *p_tbl;
	cl_map_item_t *p_next;
	osm_port_t *p_port;
	osm_node_t *p_node;
	int ret = 0;
	int vlarb_only;
	qos_mad_list_t *p_list, *p_list_next;
	qos_mad_item_t *p_port_mad;
	cl_qlist_t qos_mad_list;

	if (!p_osm->subn.opt.qos)
		return 0;

	OSM_LOG_ENTER(&p_osm->log);

	qos_build_config(&ca_config, &p_osm->subn.opt.qos_ca_options,
			 &p_osm->subn.opt.qos_options);
	qos_build_config(&sw0_config, &p_osm->subn.opt.qos_sw0_options,
			 &p_osm->subn.opt.qos_options);
	qos_build_config(&swe_config, &p_osm->subn.opt.qos_swe_options,
			 &p_osm->subn.opt.qos_options);
	qos_build_config(&rtr_config, &p_osm->subn.opt.qos_rtr_options,
			 &p_osm->subn.opt.qos_options);

	cl_qlist_init(&qos_mad_list);

	cl_plock_excl_acquire(&p_osm->lock);

	/* read QoS policy config file */
	osm_qos_parse_policy_file(&p_osm->subn);
	p_tbl = &p_osm->subn.port_guid_tbl;
	p_next = cl_qmap_head(p_tbl);
	while (p_next != cl_qmap_end(p_tbl)) {
		vlarb_only = 0;
		p_port = (osm_port_t *) p_next;
		p_next = cl_qmap_next(p_next);

		p_list = (qos_mad_list_t *) malloc(sizeof(*p_list));
		if (!p_list)
			return -1;

		memset(p_list, 0, sizeof(*p_list));

		cl_qlist_init(&p_list->port_mad_list);

		p_node = p_port->p_node;
		if (p_node->sw) {
			if (qos_extports_setup(&p_osm->sm, p_node, &swe_config,
					       &p_list->port_mad_list))
				ret = -1;

			/* skip base port 0 */
			if (!ib_switch_info_is_enhanced_port0
			    (&p_node->sw->switch_info))
				goto Continue;

			if (ib_switch_info_get_opt_sl2vlmapping(&p_node->sw->switch_info) &&
			    p_osm->sm.p_subn->opt.use_optimized_slvl &&
			    !memcmp(&swe_config.sl2vl, &sw0_config.sl2vl,
				    sizeof(swe_config.sl2vl)))
				vlarb_only = 1;

			cfg = &sw0_config;
		} else if (osm_node_get_type(p_node) == IB_NODE_TYPE_ROUTER)
			cfg = &rtr_config;
		else
			cfg = &ca_config;

		if (qos_endport_setup(&p_osm->sm, p_port->p_physp, cfg,
				      vlarb_only, &p_list->port_mad_list))

			ret = -1;
Continue:
		/* if MAD list is not empty, add it to the global MAD list */
		if (cl_qlist_count(&p_list->port_mad_list)) {
			cl_qlist_insert_tail(&qos_mad_list, &p_list->list_item);
		} else {
			free(p_list);
		}
	}
	while (cl_qlist_count(&qos_mad_list)) {
		p_list_next = (qos_mad_list_t *) cl_qlist_head(&qos_mad_list);
		while (p_list_next !=
			(qos_mad_list_t *) cl_qlist_end(&qos_mad_list)) {
			p_list = p_list_next;
			p_list_next = (qos_mad_list_t *)
				      cl_qlist_next(&p_list->list_item);
			/* next MAD to send*/
			p_port_mad = (qos_mad_item_t *)
				     cl_qlist_remove_head(&p_list->port_mad_list);
			osm_send_req_mad(&p_osm->sm, p_port_mad->p_madw);
			osm_qos_mad_delete(&p_port_mad);
			/* remove the QoS MAD from global MAD list */
			if (cl_qlist_count(&p_list->port_mad_list) == 0) {
				cl_qlist_remove_item(&qos_mad_list, &p_list->list_item);
				free(p_list);
			}
		}
	}

	cl_plock_release(&p_osm->lock);
	OSM_LOG_EXIT(&p_osm->log);

	return ret;
}