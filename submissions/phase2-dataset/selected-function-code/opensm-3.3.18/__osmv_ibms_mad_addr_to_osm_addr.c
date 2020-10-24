static void
__osmv_ibms_mad_addr_to_osm_addr(IN osm_vendor_t const *p_vend,
				 IN struct _ibms_mad_addr *p_ibms_addr,
				 IN uint8_t is_smi,
				 OUT osm_mad_addr_t * p_osm_addr)
{
	memset(p_osm_addr, 0, sizeof(osm_mad_addr_t));
	p_osm_addr->dest_lid = cl_hton16(p_ibms_addr->slid);
	p_osm_addr->static_rate = 0;
	p_osm_addr->path_bits = 0;
	if (is_smi) {
		/* SMI */
		p_osm_addr->addr_type.smi.source_lid =
		    cl_hton16(p_ibms_addr->slid);
		p_osm_addr->addr_type.smi.port_num = 1;	/* TODO add if required p_ibms_addr->port; */
	} else {
		/* GSI */
		p_osm_addr->addr_type.gsi.remote_qp =
		    cl_ntoh32(p_ibms_addr->sqpn);
		p_osm_addr->addr_type.gsi.remote_qkey = IB_QP1_WELL_KNOWN_Q_KEY;
		p_osm_addr->addr_type.gsi.pkey_ix = p_ibms_addr->pkey_index;
		p_osm_addr->addr_type.gsi.service_level = p_ibms_addr->sl;

		p_osm_addr->addr_type.gsi.global_route = FALSE;
		/* copy the GRH data if relevant - TopSpin imp doesnt relate to GRH!!! */
		/*
		   if (p_osm_addr->addr_type.gsi.global_route)
		   {
		   p_osm_addr->addr_type.gsi.grh_info.ver_class_flow =
		   ib_grh_set_ver_class_flow(p_rcv_desc->grh.IP_version,
		   p_rcv_desc->grh.traffic_class,
		   p_rcv_desc->grh.flow_label);
		   p_osm_addr->addr_type.gsi.grh_info.hop_limit =  p_rcv_desc->grh.hop_limit;
		   memcpy(&p_osm_addr->addr_type.gsi.grh_info.src_gid.raw,
		   &p_rcv_desc->grh.sgid, sizeof(ib_net64_t));
		   memcpy(&p_osm_addr->addr_type.gsi.grh_info.dest_gid.raw,
		   p_rcv_desc->grh.dgid,  sizeof(ib_net64_t));
		   }
		 */
	}
}