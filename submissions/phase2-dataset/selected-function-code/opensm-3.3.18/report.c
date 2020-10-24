static void report(void *_log, osm_epi_event_id_t event_id, void *event_data)
{
	_log_events_t *log = (_log_events_t *) _log;

	switch (event_id) {
	case OSM_EVENT_ID_PORT_ERRORS:
		handle_port_counter(log, (osm_epi_pe_event_t *) event_data);
		break;
	case OSM_EVENT_ID_PORT_DATA_COUNTERS:
		handle_port_counter_ext(log, (osm_epi_dc_event_t *) event_data);
		break;
	case OSM_EVENT_ID_PORT_SELECT:
		handle_port_select(log, (osm_epi_ps_event_t *) event_data);
		break;
	case OSM_EVENT_ID_TRAP:
		handle_trap_event(log, (ib_mad_notice_attr_t *) event_data);
		break;
	case OSM_EVENT_ID_SUBNET_UP:
		fprintf(log->log_file, "Subnet up reported\n");
		break;
	case OSM_EVENT_ID_HEAVY_SWEEP_START:
		fprintf(log->log_file, "Heavy sweep started\n");
		break;
	case OSM_EVENT_ID_HEAVY_SWEEP_DONE:
		fprintf(log->log_file, "Heavy sweep completed\n");
		break;
	case OSM_EVENT_ID_UCAST_ROUTING_DONE:
		fprintf(log->log_file, "Unicast routing completed %d\n",
			(osm_epi_ucast_routing_flags_t) event_data);
		break;
	case OSM_EVENT_ID_STATE_CHANGE:
		fprintf(log->log_file, "SM state changed\n");
		break;
	case OSM_EVENT_ID_SA_DB_DUMPED:
		fprintf(log->log_file, "SA DB dump file updated\n");
		break;
	case OSM_EVENT_ID_LFT_CHANGE:
		handle_lft_change_event(log, (osm_epi_lft_change_event_t *) event_data);
		break;
	case OSM_EVENT_ID_MAX:
	default:
		osm_log(log->osmlog, OSM_LOG_ERROR,
			"Unknown event (%d) reported to plugin\n", event_id);
	}
	fflush(log->log_file);
}