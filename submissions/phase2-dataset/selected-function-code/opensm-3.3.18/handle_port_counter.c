static void handle_port_counter(_log_events_t * log, osm_epi_pe_event_t * pc)
{
	if (pc->symbol_err_cnt > 0
	    || pc->link_err_recover > 0
	    || pc->link_downed > 0
	    || pc->rcv_err > 0
	    || pc->rcv_rem_phys_err > 0
	    || pc->rcv_switch_relay_err > 0
	    || pc->xmit_discards > 0
	    || pc->xmit_constraint_err > 0
	    || pc->rcv_constraint_err > 0
	    || pc->link_integrity > 0
	    || pc->buffer_overrun > 0
	    || pc->vl15_dropped > 0
	    || pc->xmit_wait > 0) {
		fprintf(log->log_file,
			"Port counter errors for node 0x%" PRIx64
			" (%s) port %d\n", pc->port_id.node_guid,
			pc->port_id.node_name, pc->port_id.port_num);
	}
}