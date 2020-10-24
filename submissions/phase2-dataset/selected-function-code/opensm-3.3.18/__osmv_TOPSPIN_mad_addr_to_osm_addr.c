static void
__osmv_TOPSPIN_mad_addr_to_osm_addr(IN osm_vendor_t const *p_vend,
				    IN struct ib_mad *p_mad,
				    IN uint8_t is_smi,
				    OUT osm_mad_addr_t * p_mad_addr)
{
	p_mad_addr->dest_lid = cl_hton16(p_mad->slid);
	p_mad_addr->static_rate = 0;
	p_mad_addr->path_bits = 0;
	if (is_smi) {
		/* SMI */
		p_mad_addr->addr_type.smi.source_lid = cl_hton16(p_mad->slid);
		p_mad_addr->addr_type.smi.port_num = p_mad->port;
	} else {
		/* GSI */
		p_mad_addr->addr_type.gsi.remote_qp = cl_ntoh32(p_mad->sqpn);
		p_mad_addr->addr_type.gsi.remote_qkey = IB_QP1_WELL_KNOWN_Q_KEY;
		/*  There is a TAVOR limitation that only one P_KEY is supported per */
		/*  QP - so QP1 must use IB_DEFAULT_PKEY */
		p_mad_addr->addr_type.gsi.pkey_ix = p_mad->pkey_index;
		p_mad_addr->addr_type.gsi.service_level = p_mad->sl;

		p_mad_addr->addr_type.gsi.global_route = FALSE;
		/* copy the GRH data if relevant - TopSpin imp doesnt relate to GRH!!! */
		/*
		   if (p_mad_addr->addr_type.gsi.global_route)
		   {
		   p_mad_addr->addr_type.gsi.grh_info.ver_class_flow =
		   ib_grh_set_ver_class_flow(p_rcv_desc->grh.IP_version,
		   p_rcv_desc->grh.traffic_class,
		   p_rcv_desc->grh.flow_label);
		   p_mad_addr->addr_type.gsi.grh_info.hop_limit =  p_rcv_desc->grh.hop_limit;
		   memcpy(&p_mad_addr->addr_type.gsi.grh_info.src_gid.raw,
		   &p_rcv_desc->grh.sgid, sizeof(ib_net64_t));
		   memcpy(&p_mad_addr->addr_type.gsi.grh_info.dest_gid.raw,
		   p_rcv_desc->grh.dgid,  sizeof(ib_net64_t));
		   }
		 */
	}
}