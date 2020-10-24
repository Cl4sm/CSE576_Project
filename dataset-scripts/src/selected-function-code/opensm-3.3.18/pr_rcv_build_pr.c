static void pr_rcv_build_pr(IN osm_sa_t * sa,
			    IN const osm_alias_guid_t * p_src_alias_guid,
			    IN const osm_alias_guid_t * p_dest_alias_guid,
			    IN const ib_gid_t * p_sgid,
			    IN const ib_gid_t * p_dgid,
			    IN const uint16_t src_lid_ho,
			    IN const uint16_t dest_lid_ho,
			    IN const uint8_t preference,
			    IN const osm_path_parms_t * p_parms,
			    OUT ib_path_rec_t * p_pr)
{
	const osm_physp_t *p_src_physp, *p_dest_physp;

	OSM_LOG_ENTER(sa->p_log);

	if (p_dgid)
		p_pr->dgid = *p_dgid;
	else {
		p_dest_physp = p_dest_alias_guid->p_base_port->p_physp;

		p_pr->dgid.unicast.prefix =
		    osm_physp_get_subnet_prefix(p_dest_physp);
		p_pr->dgid.unicast.interface_id = p_dest_alias_guid->alias_guid;
	}
	if (p_sgid)
		p_pr->sgid = *p_sgid;
	else {
		p_src_physp = p_src_alias_guid->p_base_port->p_physp;

		p_pr->sgid.unicast.prefix = osm_physp_get_subnet_prefix(p_src_physp);
		p_pr->sgid.unicast.interface_id = p_src_alias_guid->alias_guid;
	}

	p_pr->dlid = cl_hton16(dest_lid_ho);
	p_pr->slid = cl_hton16(src_lid_ho);

	p_pr->hop_flow_raw &= cl_hton32(1 << 31);

	/* Only set HopLimit if going through a router */
	if (p_dgid)
		p_pr->hop_flow_raw |= cl_hton32(IB_HOPLIMIT_MAX);

	p_pr->pkey = p_parms->pkey;
	ib_path_rec_set_sl(p_pr, p_parms->sl);
	ib_path_rec_set_qos_class(p_pr, 0);
	p_pr->mtu = (uint8_t) (p_parms->mtu | 0x80);
	p_pr->rate = (uint8_t) (p_parms->rate | 0x80);

	/* According to 1.2 spec definition Table 205 PacketLifeTime description,
	   for loopback paths, packetLifeTime shall be zero. */
	if (p_src_alias_guid->p_base_port == p_dest_alias_guid->p_base_port)
		p_pr->pkt_life = 0x80;	/* loopback */
	else
		p_pr->pkt_life = (uint8_t) (p_parms->pkt_life | 0x80);

	p_pr->preference = preference;

	/* always return num_path = 0 so this is only the reversible component */
	if (p_parms->reversible)
		p_pr->num_path = 0x80;

	OSM_LOG_EXIT(sa->p_log);
}