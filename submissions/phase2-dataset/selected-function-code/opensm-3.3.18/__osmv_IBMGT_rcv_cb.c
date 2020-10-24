void
__osmv_IBMGT_rcv_cb(IN IB_MGT_mad_hndl_t mad_hndl,
		    IN void *private_ctx_p,
		    IN void *payload_p,
		    IN IB_MGT_mad_rcv_desc_t * rcv_remote_info_p)
{
	osmv_bind_obj_t *p_bo;
	osm_mad_addr_t mad_addr;
	cl_list_item_t *p_item;
	cl_list_obj_t *p_obj;
	cl_qlist_t *p_list;
	ib_mad_t *p_mad = (ib_mad_t *) payload_p;
	osm_vendor_t *p_vendor;
	osmv_IBMGT_transport_info_t *p_tinfo;

	__osmv_IBMGT_rcv_desc_to_osm_addr(rcv_remote_info_p,
					  ((p_mad->mgmt_class ==
					    IB_MCLASS_SUBN_LID)
					   || (p_mad->mgmt_class ==
					       IB_MCLASS_SUBN_DIR)), &mad_addr);

	/* different handling of SMI and GSI */
	if ((p_mad->mgmt_class == IB_MCLASS_SUBN_DIR) ||
	    (p_mad->mgmt_class == IB_MCLASS_SUBN_LID)) {
		/* SMI CASE */
		p_bo = (osmv_bind_obj_t *) private_ctx_p;
		/* Make sure the p_bo object is still relevant */
		if ((p_bo->magic_ptr != p_bo) || p_bo->is_closing)
			return;

		p_vendor = p_bo->p_vendor;
		p_tinfo =
		    (osmv_IBMGT_transport_info_t *) p_vendor->p_transport_info;
		p_list = p_tinfo->p_smi_list;
	} else {
		/* GSI CASE */
		p_bo = (osmv_bind_obj_t *) private_ctx_p;
		/* Make sure the p_bo object is still relevant */
		if ((p_bo->magic_ptr != p_bo) || p_bo->is_closing)
			return;

		p_vendor = p_bo->p_vendor;
		p_tinfo =
		    (osmv_IBMGT_transport_info_t *) p_vendor->p_transport_info;
		p_list = p_tinfo->gsi_mgmt_lists[p_mad->mgmt_class];
	}

	/* go over the bindings list and send the mad, one of them will accept it,
	   the others will drope
	 */
	p_item = cl_qlist_head(p_list);
	while (p_item != cl_qlist_end(p_list)) {
		p_obj = PARENT_STRUCT(p_item, cl_list_obj_t, list_item);
		p_bo = cl_qlist_obj(p_obj);
		/* give upper layer the mad */
		osmv_dispatch_mad((osm_bind_handle_t) p_bo, payload_p,
				  &mad_addr);
		/* Make sure the p_bo object is still relevant */
		if ((p_bo->magic_ptr != p_bo) || p_bo->is_closing)
			return;

		p_item = cl_qlist_next(p_item);
	}
}