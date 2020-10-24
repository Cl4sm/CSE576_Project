static osmv_disp_route_t
__osmv_dispatch_route(IN osm_bind_handle_t h_bind,
		      IN const ib_mad_t * p_mad, OUT osmv_txn_ctx_t ** pp_txn)
{
	ib_api_status_t ret;
	osmv_bind_obj_t *p_bo = (osmv_bind_obj_t *) h_bind;
	boolean_t is_resp = ib_mad_is_response(p_mad);
	boolean_t is_txn;
	uint64_t key = cl_ntoh64(p_mad->trans_id);

	CL_ASSERT(NULL != pp_txn);

	ret = osmv_txn_lookup(h_bind, key, pp_txn);
	is_txn = (IB_SUCCESS == ret);

	if (FALSE == is_txn && TRUE == is_resp) {
		osm_log(p_bo->p_vendor->p_log, OSM_LOG_DEBUG,
			"Received a response to a non-started/aged-out transaction (tid=0x%" PRIx64 "). "
			"Dropping the MAD.\n", key);
		return OSMV_ROUTE_DROP;
	}

	if (TRUE == osmv_mad_is_rmpp(p_mad)) {
		/* An RMPP transaction. The filtering is more delicate there */
		return OSMV_ROUTE_RMPP;
	}

	if (TRUE == is_txn && FALSE == is_resp) {
		/* Does this MAD try to start a transaction with duplicate tid? */
		osm_log(p_bo->p_vendor->p_log, OSM_LOG_DEBUG,
			"Duplicate TID 0x%" PRIx64 " received (not a response). "
			"Dropping the MAD.\n", key);

		return OSMV_ROUTE_DROP;
	}

	return OSMV_ROUTE_SIMPLE;
}