static void get_guidinfo(IN osm_sa_t *sa, IN osm_madw_t *p_madw,
			 IN osm_physp_t *p_req_physp)
{
	const ib_sa_mad_t *p_rcvd_mad;
	const ib_guidinfo_record_t *p_rcvd_rec;
	cl_qlist_t rec_list;
	osm_gir_search_ctxt_t context;

	p_rcvd_mad = osm_madw_get_sa_mad_ptr(p_madw);
	p_rcvd_rec =
	    (ib_guidinfo_record_t *) ib_sa_mad_get_payload_ptr(p_rcvd_mad);

	cl_qlist_init(&rec_list);

	context.p_rcvd_rec = p_rcvd_rec;
	context.p_list = &rec_list;
	context.comp_mask = p_rcvd_mad->comp_mask;
	context.sa = sa;
	context.p_req_physp = p_req_physp;


	cl_qmap_apply_func(&sa->p_subn->node_guid_tbl, sa_gir_by_comp_mask_cb,
			   &context);

	CL_PLOCK_RELEASE(sa->p_lock);

	osm_sa_respond(sa, p_madw, sizeof(ib_guidinfo_record_t), &rec_list);
}