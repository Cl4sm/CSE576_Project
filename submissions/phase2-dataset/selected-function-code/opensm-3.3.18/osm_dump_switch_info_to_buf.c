static void osm_dump_switch_info_to_buf(IN const ib_switch_info_t * p_si,
					OUT char * buf)
{
	if (!buf || !p_si)
		return;
	else {
		sprintf(buf,
			"SwitchInfo dump:\n"
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
			"\t\t\t\tflags...................0x%X\n"
			"\t\t\t\tmcast_top...............0x%X\n",
			cl_ntoh16(p_si->lin_cap), cl_ntoh16(p_si->rand_cap),
			cl_ntoh16(p_si->mcast_cap), cl_ntoh16(p_si->lin_top),
			p_si->def_port, p_si->def_mcast_pri_port,
			p_si->def_mcast_not_port, p_si->life_state,
			cl_ntoh16(p_si->lids_per_port),
			cl_ntoh16(p_si->enforce_cap), p_si->flags,
			cl_ntoh16(p_si->mcast_top));
	}
}