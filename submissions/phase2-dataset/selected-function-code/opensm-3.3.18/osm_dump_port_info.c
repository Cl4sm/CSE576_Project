void osm_dump_port_info(IN osm_log_t * p_log, IN ib_net64_t node_guid,
			IN ib_net64_t port_guid, IN uint8_t port_num,
			IN const ib_port_info_t * p_pi,
			IN osm_log_level_t log_level)
{
	if (osm_log_is_active(p_log, log_level)) {
		char buf[BUF_SIZE];

		osm_dump_port_info_to_buf(node_guid, port_guid,
					  port_num, p_pi, buf);

		osm_log(p_log, log_level, "%s", buf);

		/*  show the capabilities mask */
		if (p_pi->capability_mask) {
			dbg_get_capabilities_str(buf, BUF_SIZE, "\t\t\t\t",
						 p_pi);
			osm_log(p_log, log_level, "%s", buf);
		}
	}
}