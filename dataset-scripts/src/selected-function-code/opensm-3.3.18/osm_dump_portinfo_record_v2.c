void osm_dump_portinfo_record_v2(IN osm_log_t * p_log,
				 IN const ib_portinfo_record_t * p_pir,
				 IN const int file_id,
				 IN osm_log_level_t log_level)
{
	if (osm_log_is_active_v2(p_log, log_level, file_id)) {
		char buf[BUF_SIZE];
		const ib_port_info_t *p_pi = &p_pir->port_info;

		osm_dump_portinfo_record_to_buf(p_pir, buf);

		osm_log_v2(p_log, log_level, file_id, "%s", buf);

		/*  show the capabilities mask */
		if (p_pi->capability_mask) {
			dbg_get_capabilities_str(buf, BUF_SIZE, "\t\t\t\t",
						 p_pi);
			osm_log_v2(p_log, log_level, file_id, "%s", buf);
		}
	}
}