static void sort_ports_by_switch_load(osm_ucast_mgr_t * m)
{
	int i, num = cl_qmap_count(&m->p_subn->sw_guid_tbl);
	void **s = malloc(num * sizeof(*s));
	if (!s) {
		OSM_LOG(m->p_log, OSM_LOG_ERROR, "ERR 3A0C: "
			"No memory, skip by switch load sorting.\n");
		return;
	}
	s[0] = cl_qmap_head(&m->p_subn->sw_guid_tbl);
	for (i = 1; i < num; i++)
		s[i] = cl_qmap_next(s[i - 1]);

	for (i = 0; i < num; i++)
		sw_count_endport_links(s[i]);

	qsort(s, num, sizeof(*s), compar_sw_load);

	for (i = 0; i < num; i++)
		add_sw_endports_to_order_list(s[i], m);
	free(s);
}