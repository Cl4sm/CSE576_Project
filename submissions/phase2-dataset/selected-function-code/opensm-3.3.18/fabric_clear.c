static void fabric_clear(ftree_fabric_t * p_ftree)
{
	ftree_hca_t *p_hca;
	ftree_hca_t *p_next_hca;
	ftree_sw_t *p_sw;
	ftree_sw_t *p_next_sw;
	ftree_sw_tbl_element_t *p_element;
	ftree_sw_tbl_element_t *p_next_element;
	name_map_item_t *p_guid_element, *p_next_guid_element;

	if (!p_ftree)
		return;

	/* remove all the elements of hca_tbl */

	p_next_hca = (ftree_hca_t *) cl_qmap_head(&p_ftree->hca_tbl);
	while (p_next_hca != (ftree_hca_t *) cl_qmap_end(&p_ftree->hca_tbl)) {
		p_hca = p_next_hca;
		p_next_hca = (ftree_hca_t *) cl_qmap_next(&p_hca->map_item);
		hca_destroy(p_hca);
	}
	cl_qmap_remove_all(&p_ftree->hca_tbl);

	/* remove all the elements of sw_tbl */

	p_next_sw = (ftree_sw_t *) cl_qmap_head(&p_ftree->sw_tbl);
	while (p_next_sw != (ftree_sw_t *) cl_qmap_end(&p_ftree->sw_tbl)) {
		p_sw = p_next_sw;
		p_next_sw = (ftree_sw_t *) cl_qmap_next(&p_sw->map_item);
		sw_destroy(p_ftree, p_sw);
	}
	cl_qmap_remove_all(&p_ftree->sw_tbl);

	/* remove all the elements of sw_by_tuple_tbl */

	p_next_element =
	    (ftree_sw_tbl_element_t *) cl_qmap_head(&p_ftree->sw_by_tuple_tbl);
	while (p_next_element != (ftree_sw_tbl_element_t *)
	       cl_qmap_end(&p_ftree->sw_by_tuple_tbl)) {
		p_element = p_next_element;
		p_next_element = (ftree_sw_tbl_element_t *)
		    cl_qmap_next(&p_element->map_item);
		sw_tbl_element_destroy(p_element);
	}
	cl_qmap_remove_all(&p_ftree->sw_by_tuple_tbl);

	/* remove all the elements of cn_guid_tbl */
	p_next_guid_element =
	    (name_map_item_t *) cl_qmap_head(&p_ftree->cn_guid_tbl);
	while (p_next_guid_element !=
	       (name_map_item_t *) cl_qmap_end(&p_ftree->cn_guid_tbl)) {
		p_guid_element = p_next_guid_element;
		p_next_guid_element =
		    (name_map_item_t *) cl_qmap_next(&p_guid_element->item);
		free(p_guid_element);
	}
	cl_qmap_remove_all(&p_ftree->cn_guid_tbl);

	/* remove all the elements of io_guid_tbl */
	p_next_guid_element =
	    (name_map_item_t *) cl_qmap_head(&p_ftree->io_guid_tbl);
	while (p_next_guid_element !=
	       (name_map_item_t *) cl_qmap_end(&p_ftree->io_guid_tbl)) {
		p_guid_element = p_next_guid_element;
		p_next_guid_element =
		    (name_map_item_t *) cl_qmap_next(&p_guid_element->item);
		free(p_guid_element);
	}
	cl_qmap_remove_all(&p_ftree->io_guid_tbl);

	/* free the leaf switches array */
	if ((p_ftree->leaf_switches_num > 0) && (p_ftree->leaf_switches))
		free(p_ftree->leaf_switches);

	p_ftree->leaf_switches_num = 0;
	p_ftree->cn_num = 0;
	p_ftree->ca_ports = 0;
	p_ftree->leaf_switch_rank = 0;
	p_ftree->max_switch_rank = 0;
	p_ftree->max_cn_per_leaf = 0;
	p_ftree->lft_max_lid = 0;
	p_ftree->leaf_switches = NULL;
	p_ftree->fabric_built = FALSE;

}				/* fabric_destroy() */