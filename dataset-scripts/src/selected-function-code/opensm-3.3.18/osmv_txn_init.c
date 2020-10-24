ib_api_status_t
osmv_txn_init(IN osm_bind_handle_t h_bind,
	      IN uint64_t tid, IN uint64_t key, OUT osmv_txn_ctx_t ** pp_txn)
{
	ib_api_status_t st;
	osmv_txn_ctx_t *p_txn;
	osmv_bind_obj_t *p_bo = (osmv_bind_obj_t *) h_bind;

	OSM_LOG_ENTER(p_bo->p_vendor->p_log);

	CL_ASSERT(NULL != h_bind && NULL != pp_txn);

	osm_log(p_bo->p_vendor->p_log, OSM_LOG_DEBUG,
		"Starting transaction 0x%016" PRIx64
		" (key=0x%016" PRIx64 ")\n", tid, key);

	p_txn = malloc(sizeof(osmv_txn_ctx_t));
	if (!p_txn) {
		return IB_INSUFFICIENT_MEMORY;
	}

	memset(p_txn, 0, sizeof(osmv_txn_ctx_t));
	p_txn->p_log = p_bo->txn_mgr.p_log;
	p_txn->tid = tid;
	p_txn->key = key;
	p_txn->p_madw = NULL;
	p_txn->rmpp_txfr.rmpp_state = OSMV_TXN_RMPP_NONE;

	/* insert into transaction manager DB */
	st = __osmv_txnmgr_insert_txn(&p_bo->txn_mgr, p_txn, key);
	if (IB_SUCCESS != st) {
		osm_log(p_bo->p_vendor->p_log, OSM_LOG_ERROR,
			"osmv_txn_init: ERR 6703: "
			"Failed to insert to transaction 0x%016" PRIx64
			" (key=0x%016" PRIx64 ") to manager DB\n",
			tid, key);
		goto insert_txn_failed;
	}

	*pp_txn = p_txn;
	OSM_LOG_EXIT(p_bo->p_vendor->p_log);
	return IB_SUCCESS;

insert_txn_failed:
	free(p_txn);

	OSM_LOG_EXIT(p_bo->p_vendor->p_log);
	return st;
}