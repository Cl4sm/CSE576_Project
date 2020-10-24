static int fabric_populate_ports(IN ftree_fabric_t * p_ftree)
{
	ftree_hca_t *p_hca;
	ftree_hca_t *p_next_hca;
	ftree_sw_t *p_sw;
	ftree_sw_t *p_next_sw;
	int res = 0;

	OSM_LOG_ENTER(&p_ftree->p_osm->log);

	p_next_hca = (ftree_hca_t *) cl_qmap_head(&p_ftree->hca_tbl);
	while (p_next_hca != (ftree_hca_t *) cl_qmap_end(&p_ftree->hca_tbl)) {
		p_hca = p_next_hca;
		p_next_hca = (ftree_hca_t *) cl_qmap_next(&p_hca->map_item);
		if (fabric_construct_hca_ports(p_ftree, p_hca) != 0) {
			res = -1;
			goto Exit;
		}
	}

	p_next_sw = (ftree_sw_t *) cl_qmap_head(&p_ftree->sw_tbl);
	while (p_next_sw != (ftree_sw_t *) cl_qmap_end(&p_ftree->sw_tbl)) {
		p_sw = p_next_sw;
		p_next_sw = (ftree_sw_t *) cl_qmap_next(&p_sw->map_item);
		if (fabric_construct_sw_ports(p_ftree, p_sw) != 0) {
			res = -1;
			goto Exit;
		}
	}
Exit:
	OSM_LOG_EXIT(&p_ftree->p_osm->log);
	return res;
}				/* fabric_populate_ports() */