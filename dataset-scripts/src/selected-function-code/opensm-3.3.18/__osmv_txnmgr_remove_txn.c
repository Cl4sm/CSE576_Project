ib_api_status_t
__osmv_txnmgr_remove_txn(IN osmv_txn_mgr_t * p_tx_mgr,
			 IN uint64_t key, OUT osmv_txn_ctx_t ** pp_txn)
{
	cl_map_obj_t *p_obj;
	cl_map_item_t *p_item;

	OSM_LOG_ENTER(p_tx_mgr->p_log);

	CL_ASSERT(p_tx_mgr);
	CL_ASSERT(pp_txn);

	p_item = cl_qmap_remove(p_tx_mgr->p_txn_map, key);

	if (p_item == cl_qmap_end(p_tx_mgr->p_txn_map)) {

		osm_log(p_tx_mgr->p_log, OSM_LOG_ERROR,
			"__osmv_txnmgr_remove_txn: ERR 6701: "
			"Could not remove the transaction 0x%016" PRIx64 " - "
			"something is really wrong!\n", key);
		OSM_LOG_EXIT(p_tx_mgr->p_log);
		return IB_NOT_FOUND;
	}

	p_obj = PARENT_STRUCT(p_item, cl_map_obj_t, item);
	*pp_txn = cl_qmap_obj(p_obj);

	free(p_obj);

	OSM_LOG_EXIT(p_tx_mgr->p_log);
	return IB_SUCCESS;
}