static void osm_dump_switch_info_record_to_buf(IN const ib_switch_info_record_t * p_sir,
					       OUT char * buf)
{
	if (!buf || !p_sir)
		return;
	else {
		sprintf(buf,
			"SwitchInfo Record dump:\n"
			"\t\t\t\tRID\n"
			"\t\t\t\tlid.....................%u\n"
			"\t\t\t\tSwitchInfo dump:\n"
			"\t\t\t\tlin_cap.................0x%X\n"
			"\t\t\t\trand_cap................0x%X\n"
			"\t\t\t\tmcast_cap...............0x%X\n"
			"\t\t\t\tlin_top.................0x%X\n"
			"\t\t\t\tdef_port................%u\n"
			"\t\t\t\tdef_mcast_pri_port......%u\n"
			"\t\t\t\tdef_mcast_not_port......%u\n"
			"\t\t\t\tlife_state..............0x%X\n"
			"\t\t\t\tlids_per_port...........%u\n"
			"\t\t\t\tpartition_enf_cap.......0x%X\n"
			"\t\t\t\tflags...................0x%X\n",
			cl_ntoh16(p_sir->lid),
			cl_ntoh16(p_sir->switch_info.lin_cap),
			cl_ntoh16(p_sir->switch_info.rand_cap),
			cl_ntoh16(p_sir->switch_info.mcast_cap),
			cl_ntoh16(p_sir->switch_info.lin_top),
			p_sir->switch_info.def_port,
			p_sir->switch_info.def_mcast_pri_port,
			p_sir->switch_info.def_mcast_not_port,
			p_sir->switch_info.life_state,
			cl_ntoh16(p_sir->switch_info.lids_per_port),
			cl_ntoh16(p_sir->switch_info.enforce_cap),
			p_sir->switch_info.flags);
	}
}