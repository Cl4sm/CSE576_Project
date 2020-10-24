static void
__osmv_IBMGT_rcv_desc_to_osm_addr(IN IB_MGT_mad_rcv_desc_t * p_rcv_desc,
				  IN uint8_t is_smi,
				  OUT osm_mad_addr_t * p_mad_addr)
{
	/*  p_mad_addr->dest_lid = p_osm->subn.sm_base_lid; - for resp we use the dest lid ... */
	p_mad_addr->dest_lid = cl_hton16(p_rcv_desc->remote_lid);
	p_mad_addr->static_rate = 0;	/*  HACK - we do not  know the rate ! */
	p_mad_addr->path_bits = p_rcv_desc->local_path_bits;
	/* Clear the grh any way to avoid unset fields */
	memset(&p_mad_addr->addr_type.gsi.grh_info, 0,
	       sizeof(p_mad_addr->addr_type.gsi.grh_info));

	if (is_smi) {
		/* SMI */
		p_mad_addr->addr_type.smi.source_lid =
		    cl_hton16(p_rcv_desc->remote_lid);
		p_mad_addr->addr_type.smi.port_num = 99;	/*  HACK - if used - should fail */
	} else {
		/* GSI */
		/* seems to me there is a IBMGT bug reversing the QPN ... */
		/* Does IBMGT supposed to provide the QPN is network or HOST ? */
		p_mad_addr->addr_type.gsi.remote_qp = cl_hton32(p_rcv_desc->qp);

		p_mad_addr->addr_type.gsi.remote_qkey = IB_QP1_WELL_KNOWN_Q_KEY;
		/*  we do have the p_mad_addr->pkey_ix but how to get the PKey by index ? */
		/*  the only way seems to be to use VAPI_query_hca_pkey_tbl and obtain */
		/*  the full PKey table - than go by the index. */
		/*  since this does not seem reasonable to me I simply use the default */
		/*  There is a TAVOR limitation that only one P_KEY is supported per  */
		/*  QP - so QP1 must use IB_DEFAULT_PKEY */
		p_mad_addr->addr_type.gsi.pkey_ix = 0;
		p_mad_addr->addr_type.gsi.service_level = p_rcv_desc->sl;

		p_mad_addr->addr_type.gsi.global_route = p_rcv_desc->grh_flag;
		/* copy the GRH data if relevant */
		if (p_mad_addr->addr_type.gsi.global_route) {
			p_mad_addr->addr_type.gsi.grh_info.ver_class_flow =
			    ib_grh_set_ver_class_flow(p_rcv_desc->grh.
						      IP_version,
						      p_rcv_desc->grh.
						      traffic_class,
						      p_rcv_desc->grh.
						      flow_label);
			p_mad_addr->addr_type.gsi.grh_info.hop_limit =
			    p_rcv_desc->grh.hop_limit;
			memcpy(&p_mad_addr->addr_type.gsi.grh_info.src_gid.raw,
			       &p_rcv_desc->grh.sgid, sizeof(ib_net64_t));
			memcpy(&p_mad_addr->addr_type.gsi.grh_info.dest_gid.raw,
			       p_rcv_desc->grh.dgid, sizeof(ib_net64_t));
		}
	}
}