static void fabric_remove_unranked_sw(IN ftree_fabric_t *p_ftree)
{
	ftree_sw_t *p_sw = NULL;
	ftree_sw_t *p_next_sw;
	int removed_hca;
	int count = 0;

	p_next_sw = (ftree_sw_t *) cl_qmap_head(&p_ftree->sw_tbl);
	while (p_next_sw != (ftree_sw_t *) cl_qmap_end(&p_ftree->sw_tbl)) {
		p_sw = p_next_sw;
		p_next_sw = (ftree_sw_t *) cl_qmap_next(&p_sw->map_item);
		if (!sw_ranked(p_sw)) {
			cl_qmap_remove_item(&p_ftree->sw_tbl,&p_sw->map_item);
			removed_hca = remove_depended_hca(p_ftree, p_sw);
			OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_VERBOSE,
				"Removing Unranked sw 0x%" PRIx64 " (with %d dependent hca's)\n",
				sw_get_guid_ho(p_sw),removed_hca);
			sw_destroy(p_ftree, p_sw);
			count++;
		}
	}
	OSM_LOG(&p_ftree->p_osm->log, OSM_LOG_DEBUG,
		"Removed %d invalid switches\n", count);
}