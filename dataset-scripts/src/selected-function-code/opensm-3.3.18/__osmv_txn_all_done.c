void __osmv_txn_all_done(osm_bind_handle_t h_bind)
{
	osmv_bind_obj_t *p_bo = (osmv_bind_obj_t *) h_bind;
	cl_map_item_t *p_item;
	cl_map_obj_t *p_obj;
	osmv_txn_ctx_t *p_txn;

	OSM_LOG_ENTER(p_bo->p_vendor->p_log);

	p_item = cl_qmap_head(p_bo->txn_mgr.p_txn_map);
	while (p_item != cl_qmap_end(p_bo->txn_mgr.p_txn_map)) {

		p_obj = PARENT_STRUCT(p_item, cl_map_obj_t, item);
		p_txn = (osmv_txn_ctx_t *) cl_qmap_obj(p_obj);
		osmv_txn_done(h_bind, osmv_txn_get_key(p_txn), FALSE);
		free(p_obj);
		/* assuming osmv_txn_done has removed the txn from the map */
		p_item = cl_qmap_head(p_bo->txn_mgr.p_txn_map);
	}

	OSM_LOG_EXIT(p_bo->p_vendor->p_log);
}