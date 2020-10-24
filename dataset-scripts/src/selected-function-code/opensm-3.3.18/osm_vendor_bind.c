osm_bind_handle_t
osm_vendor_bind(IN osm_vendor_t * const p_vend,
		IN osm_bind_info_t * const p_bind_info,
		IN osm_mad_pool_t * const p_mad_pool,
		IN osm_vend_mad_recv_callback_t mad_recv_callback,
		IN osm_vend_mad_send_err_callback_t send_err_callback,
		IN void *context)
{
	osmv_bind_obj_t *p_bo;
	ib_api_status_t status;
	char hca_id[32];
	cl_status_t cl_st;
	cl_list_obj_t *p_obj;
	uint8_t hca_index;

	if (NULL == p_vend || NULL == p_bind_info || NULL == p_mad_pool
	    || NULL == mad_recv_callback || NULL == send_err_callback) {
		osm_log(p_vend->p_log, OSM_LOG_ERROR,
			"osm_vendor_bind: ERR 7302: "
			"NULL parameter passed in: p_vend=%p p_bind_info=%p p_mad_pool=%p recv_cb=%p send_err_cb=%p\n",
			p_vend, p_bind_info, p_mad_pool, mad_recv_callback,
			send_err_callback);

		return OSM_BIND_INVALID_HANDLE;
	}

	p_bo = malloc(sizeof(osmv_bind_obj_t));
	if (NULL == p_bo) {
		osm_log(p_vend->p_log, OSM_LOG_ERROR,
			"osm_vendor_bind: ERR 7303: could not allocate the bind object\n");
		return OSM_BIND_INVALID_HANDLE;
	}

	memset(p_bo, 0, sizeof(osmv_bind_obj_t));
	p_bo->p_vendor = p_vend;
	p_bo->recv_cb = mad_recv_callback;
	p_bo->send_err_cb = send_err_callback;
	p_bo->cb_context = context;
	p_bo->p_osm_pool = p_mad_pool;

	/* obtain the hca name and port num from the guid */
	osm_log(p_bo->p_vendor->p_log, OSM_LOG_DEBUG,
		"osm_vendor_bind: "
		"Finding CA and Port that owns port guid 0x%" PRIx64 ".\n",
		cl_ntoh64(p_bind_info->port_guid));

	status = osm_vendor_get_guid_ca_and_port(p_bo->p_vendor,
						 p_bind_info->port_guid,
						 &(p_bo->hca_hndl),
						 hca_id,
						 &hca_index, &(p_bo->port_num));
	if (status != IB_SUCCESS) {
		osm_log(p_bo->p_vendor->p_log, OSM_LOG_ERROR,
			"osm_vendor_bind: ERR 7304: "
			"Fail to find port number of port guid:0x%016" PRIx64
			"\n", p_bind_info->port_guid);
		free(p_bo);
		return OSM_BIND_INVALID_HANDLE;
	}

	/* Initialize the magic_ptr to the pointer of the p_bo info.
	   This will be used to signal when the object is being destroyed, so no
	   real action will be done then. */
	p_bo->magic_ptr = p_bo;

	p_bo->is_closing = FALSE;

	cl_spinlock_construct(&(p_bo->lock));
	cl_st = cl_spinlock_init(&(p_bo->lock));
	if (cl_st != CL_SUCCESS) {
		osm_log(p_bo->p_vendor->p_log, OSM_LOG_ERROR,
			"osm_vendor_bind: ERR 7305: "
			"could not initialize the spinlock ...\n");
		free(p_bo);
		return OSM_BIND_INVALID_HANDLE;
	}

	osm_log(p_bo->p_vendor->p_log, OSM_LOG_DEBUG,
		"osm_vendor_bind: osmv_txnmgr_init ... \n");
	if (osmv_txnmgr_init(&p_bo->txn_mgr, p_vend->p_log, &(p_bo->lock)) !=
	    IB_SUCCESS) {
		osm_log(p_bo->p_vendor->p_log, OSM_LOG_ERROR,
			"osm_vendor_bind: ERR 7306: "
			"osmv_txnmgr_init failed \n");
		cl_spinlock_destroy(&p_bo->lock);
		free(p_bo);
		return OSM_BIND_INVALID_HANDLE;
	}

	/* Do the real job! (Transport-dependent) */
	if (IB_SUCCESS !=
	    osmv_transport_init(p_bind_info, hca_id, hca_index, p_bo)) {
		osm_log(p_bo->p_vendor->p_log, OSM_LOG_ERROR,
			"osm_vendor_bind: ERR 7307: "
			"osmv_transport_init failed \n");
		osmv_txnmgr_done((osm_bind_handle_t) p_bo);
		cl_spinlock_destroy(&p_bo->lock);
		free(p_bo);
		return OSM_BIND_INVALID_HANDLE;
	}

	/* insert bind handle into db */
	p_obj = malloc(sizeof(cl_list_obj_t));
	if (NULL == p_obj) {

		osm_log(p_bo->p_vendor->p_log, OSM_LOG_ERROR,
			"osm_vendor_bind: ERR 7308: "
			"osm_vendor_bind: could not allocate the list object\n");

		osmv_transport_done(p_bo->p_transp_mgr);
		osmv_txnmgr_done((osm_bind_handle_t) p_bo);
		cl_spinlock_destroy(&p_bo->lock);
		free(p_bo);
		return OSM_BIND_INVALID_HANDLE;
	}
	memset(p_obj, 0, sizeof(cl_list_obj_t));
	cl_qlist_set_obj(p_obj, p_bo);

	cl_qlist_insert_head(&p_vend->bind_handles, &p_obj->list_item);

	return (osm_bind_handle_t) p_bo;
}