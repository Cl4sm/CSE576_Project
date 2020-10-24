void osmv_txn_abort_rmpp_txns(osm_bind_handle_t h_bind)
{
	osmv_bind_obj_t *p_bo = (osmv_bind_obj_t *) h_bind;
	cl_map_item_t *p_item;
	cl_map_obj_t *p_obj;
	osmv_txn_ctx_t *p_txn;
	osmv_rmpp_send_ctx_t *p_send_ctx;
	cl_qmap_t *p_map = p_bo->txn_mgr.p_txn_map;

	OSM_LOG_ENTER(p_bo->p_vendor->p_log);

	while (FALSE == cl_is_qmap_empty(p_map)) {

		p_item = cl_qmap_head(p_map);
		p_obj = PARENT_STRUCT(p_item, cl_map_obj_t, item);
		p_txn = (osmv_txn_ctx_t *) cl_qmap_obj(p_obj);
		p_send_ctx = osmv_txn_get_rmpp_send_ctx(p_txn);

		if (NULL != p_send_ctx) {

			p_send_ctx->status = IB_INTERRUPTED;

			/* Wake up the sender thread to let it break out */
			cl_event_signal(&p_send_ctx->event);
		}

		cl_qmap_remove_item(p_map, p_item);
	}

	OSM_LOG_EXIT(p_bo->p_vendor->p_log);
}