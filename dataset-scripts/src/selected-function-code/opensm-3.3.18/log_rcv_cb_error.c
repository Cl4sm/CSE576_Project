static void log_rcv_cb_error(osm_log_t *p_log, ib_smp_t *p_smp, ib_net16_t status)
{
	char buf[BUF_SIZE];
	uint32_t i;

	if (p_smp->mgmt_class == IB_MCLASS_SUBN_DIR) {
		char ipath[BUF_SIZE], rpath[BUF_SIZE];
		int ni = sprintf(ipath, "%d", p_smp->initial_path[0]);
		int nr = sprintf(rpath, "%d", p_smp->return_path[0]);
		for (i = 1; i <= p_smp->hop_count; i++) {
			ni += sprintf(ipath + ni, ",%d", p_smp->initial_path[i]);
			nr += sprintf(rpath + nr, ",%d", p_smp->return_path[i]);
		}
		snprintf(buf, sizeof(buf),
			 "\n\t\t\tInitial path: %s Return path: %s",
			 ipath, rpath);
	}

	OSM_LOG(p_log, OSM_LOG_ERROR, "ERR 3111: "
		"Received MAD with error status = 0x%X\n"
		"\t\t\t%s(%s), attr_mod 0x%x, TID 0x%" PRIx64 "%s\n",
		cl_ntoh16(status), ib_get_sm_method_str(p_smp->method),
		ib_get_sm_attr_str(p_smp->attr_id), cl_ntoh32(p_smp->attr_mod),
		cl_ntoh64(p_smp->trans_id),
		p_smp->mgmt_class == IB_MCLASS_SUBN_DIR ? buf : "");

	osm_dump_dr_smp_v2(p_log, p_smp, FILE_ID, OSM_LOG_VERBOSE);
}