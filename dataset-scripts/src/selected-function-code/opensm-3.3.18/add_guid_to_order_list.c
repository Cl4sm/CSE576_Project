static int add_guid_to_order_list(void *ctx, uint64_t guid, char *p)
{
	osm_ucast_mgr_t *m = ctx;
	osm_port_t *port = osm_get_port_by_guid(m->p_subn, cl_hton64(guid));

	if (!port) {
		OSM_LOG(m->p_log, OSM_LOG_DEBUG,
			"port guid not found: 0x%016" PRIx64 "\n", guid);
		return 0;
	}

	if (port->flag) {
		OSM_LOG(m->p_log, OSM_LOG_DEBUG,
			"port guid specified multiple times 0x%016" PRIx64 "\n",
			guid);
		return 0;
	}

	cl_qlist_insert_tail(&m->port_order_list, &port->list_item);
	port->flag = 1;
	port->use_scatter =  (m->p_subn->opt.guid_routing_order_no_scatter == TRUE) ? 0 : m->p_subn->opt.scatter_ports;

	return 0;
}