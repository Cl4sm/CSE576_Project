ib_api_status_t
__osmv_txnmgr_lookup(IN osmv_txn_mgr_t * p_tx_mgr,
		     IN uint64_t key, OUT osmv_txn_ctx_t ** pp_txn)
{
	ib_api_status_t status = IB_SUCCESS;
	cl_map_item_t *p_item;
	cl_map_obj_t *p_obj;

	uint64_t tmp_key;

	OSM_LOG_ENTER(p_tx_mgr->p_log);

	CL_ASSERT(p_tx_mgr);
	CL_ASSERT(pp_txn);

	osm_log(p_tx_mgr->p_log, OSM_LOG_DEBUG,
		"__osmv_txnmgr_lookup: "
		"Looking for key: 0x%016" PRIx64 " in map ptr:%p\n", key,
		p_tx_mgr->p_txn_map);

	p_item = cl_qmap_head(p_tx_mgr->p_txn_map);
	while (p_item != cl_qmap_end(p_tx_mgr->p_txn_map)) {
		tmp_key = cl_qmap_key(p_item);
		osm_log(p_tx_mgr->p_log, OSM_LOG_DEBUG,
			"__osmv_txnmgr_lookup: "
			"Found key 0x%016" PRIx64 "\n", tmp_key);
		p_item = cl_qmap_next(p_item);
	}

	p_item = cl_qmap_get(p_tx_mgr->p_txn_map, key);
	if (cl_qmap_end(p_tx_mgr->p_txn_map) == p_item) {
		status = IB_NOT_FOUND;
	} else {
		p_obj = PARENT_STRUCT(p_item, cl_map_obj_t, item);
		*pp_txn = cl_qmap_obj(p_obj);
	}

	OSM_LOG_EXIT(p_tx_mgr->p_log);
	return status;
}