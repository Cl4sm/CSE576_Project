void
__osmv_IBMGT_osm_addr_to_ibmgt_addr(IN const osm_mad_addr_t * p_mad_addr,
				    IN uint8_t is_smi, OUT IB_ud_av_t * p_av)
{

	/* For global destination or Multicast address: */
	u_int8_t ver;

	memset(p_av, 0, sizeof(IB_ud_av_t));

	p_av->src_path_bits = p_mad_addr->path_bits;
	p_av->static_rate = p_mad_addr->static_rate;
	p_av->dlid = cl_ntoh16(p_mad_addr->dest_lid);

	if (is_smi) {
		p_av->sl = 0;	/*  Just to note we use 0 here. */
	} else {
		p_av->sl = p_mad_addr->addr_type.gsi.service_level;
		p_av->grh_flag = p_mad_addr->addr_type.gsi.global_route;

		if (p_mad_addr->addr_type.gsi.global_route) {
			ib_grh_get_ver_class_flow(p_mad_addr->addr_type.gsi.
						  grh_info.ver_class_flow, &ver,
						  &p_av->traffic_class,
						  &p_av->flow_label);
			p_av->hop_limit =
			    p_mad_addr->addr_type.gsi.grh_info.hop_limit;
			p_av->sgid_index = 0;	/*  we always use source GID 0 */
			memcpy(&p_av->dgid,
			       &p_mad_addr->addr_type.gsi.grh_info.dest_gid.raw,
			       sizeof(ib_net64_t));

		}
	}
}