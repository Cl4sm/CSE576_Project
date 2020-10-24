void osmtest_destroy(IN osmtest_t * const p_osmt)
{
	cl_map_item_t *p_item, *p_next_item;

	/* Currently there is a problem with IBAL exit flow - memory overrun,
	   so bypass vendor deletion - it will be cleaned by the Windows OS */
#ifndef __WIN__
	if (p_osmt->p_vendor)
		osm_vendor_delete(&p_osmt->p_vendor);
#endif

	cl_qpool_destroy(&p_osmt->port_pool);
	cl_qpool_destroy(&p_osmt->node_pool);

	/* destroy the qmap tables */
	p_next_item = cl_qmap_head(&p_osmt->exp_subn.link_tbl);
	while (p_next_item != cl_qmap_end(&p_osmt->exp_subn.link_tbl)) {
		p_item = p_next_item;
		p_next_item = cl_qmap_next(p_item);
		free(p_item);
	}
	p_next_item = cl_qmap_head(&p_osmt->exp_subn.mgrp_mlid_tbl);
	while (p_next_item != cl_qmap_end(&p_osmt->exp_subn.mgrp_mlid_tbl)) {
		p_item = p_next_item;
		p_next_item = cl_qmap_next(p_item);
		free(p_item);
	}
	p_next_item = cl_qmap_head(&p_osmt->exp_subn.node_guid_tbl);
	while (p_next_item != cl_qmap_end(&p_osmt->exp_subn.node_guid_tbl)) {
		p_item = p_next_item;
		p_next_item = cl_qmap_next(p_item);
		free(p_item);
	}

	p_next_item = cl_qmap_head(&p_osmt->exp_subn.node_lid_tbl);
	while (p_next_item != cl_qmap_end(&p_osmt->exp_subn.node_lid_tbl)) {
		p_item = p_next_item;
		p_next_item = cl_qmap_next(p_item);
		free(p_item);
	}

	p_next_item = cl_qmap_head(&p_osmt->exp_subn.path_tbl);
	while (p_next_item != cl_qmap_end(&p_osmt->exp_subn.path_tbl)) {
		p_item = p_next_item;
		p_next_item = cl_qmap_next(p_item);
		free(p_item);
	}
	p_next_item = cl_qmap_head(&p_osmt->exp_subn.port_key_tbl);
	while (p_next_item != cl_qmap_end(&p_osmt->exp_subn.port_key_tbl)) {
		p_item = p_next_item;
		p_next_item = cl_qmap_next(p_item);
		free(p_item);
	}

	osm_log_destroy(&p_osmt->log);
}