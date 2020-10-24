ib_api_status_t
__osmv_txnmgr_insert_txn(IN osmv_txn_mgr_t * p_tx_mgr,
			 IN osmv_txn_ctx_t * p_txn, IN uint64_t key)
{
	cl_map_obj_t *p_obj = NULL;
	cl_map_item_t *p_item;
	uint64_t tmp_key;

	CL_ASSERT(p_tx_mgr);
	CL_ASSERT(p_txn);

	key = osmv_txn_get_key(p_txn);
	p_obj = malloc(sizeof(cl_map_obj_t));
	if (NULL == p_obj)
		return IB_INSUFFICIENT_MEMORY;

	osm_log(p_tx_mgr->p_log, OSM_LOG_DEBUG,
		"__osmv_txnmgr_insert_txn: "
		"Inserting key: 0x%016" PRIx64 " to map ptr:%p\n", key,
		p_tx_mgr->p_txn_map);

	memset(p_obj, 0, sizeof(cl_map_obj_t));

	cl_qmap_set_obj(p_obj, p_txn);
	/* assuming lookup with this key was made and the result was IB_NOT_FOUND */
	cl_qmap_insert(p_tx_mgr->p_txn_map, key, &p_obj->item);

	p_item = cl_qmap_head(p_tx_mgr->p_txn_map);
	while (p_item != cl_qmap_end(p_tx_mgr->p_txn_map)) {
		tmp_key = cl_qmap_key(p_item);
		osm_log(p_tx_mgr->p_log, OSM_LOG_DEBUG,
			"__osmv_txnmgr_insert_txn: "
			"Found key 0x%016" PRIx64 "\n", tmp_key);
		p_item = cl_qmap_next(p_item);
	}

	return IB_SUCCESS;
}