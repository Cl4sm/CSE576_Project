static void osm_dump_mlnx_ext_port_info_to_buf(IN ib_net64_t node_guid,
					       IN ib_net64_t port_guid, IN uint8_t port_num,
					       IN const ib_mlnx_ext_port_info_t * p_pi,
					       OUT char * buf)
{
	if (!buf || !p_pi)
		return;
	else {
		sprintf(buf,
                        "MLNX ExtendedPortInfo dump:\n"
                        "\t\t\t\tport number..............%u\n"
                        "\t\t\t\tnode_guid................0x%016" PRIx64 "\n"
                        "\t\t\t\tport_guid................0x%016" PRIx64 "\n"
                        "\t\t\t\tStateChangeEnable........0x%X\n"
                        "\t\t\t\tLinkSpeedSupported.......0x%X\n"
                        "\t\t\t\tLinkSpeedEnabled.........0x%X\n"
                        "\t\t\t\tLinkSpeedActive..........0x%X\n",
                        port_num, cl_ntoh64(node_guid), cl_ntoh64(port_guid),
                        p_pi->state_change_enable, p_pi->link_speed_supported,
                        p_pi->link_speed_enabled, p_pi->link_speed_active);
	}
}