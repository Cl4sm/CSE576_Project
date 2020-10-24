void osmt_init_mc_query_rec(IN osmtest_t * const p_osmt,
			    IN OUT ib_member_rec_t * p_mc_req)
{
	/* use default values so we can change only what we want later */
	memset(p_mc_req, 0, sizeof(ib_member_rec_t));

	/* we leave the MGID to the user */
	memcpy(&p_mc_req->port_gid.unicast.interface_id,
	       &p_osmt->local_port.port_guid,
	       sizeof(p_osmt->local_port.port_guid));

	/*  use our own subnet prefix: */
	p_mc_req->port_gid.unicast.prefix = cl_hton64(p_osmt->local_port_gid.unicast.prefix);

	/*  ib_net32_t  qkey; */
	/*  ib_net16_t  mlid; - we keep it zero for upper level to decide. */
	/*  uint8_t     mtu; - keep it zero means - anything you have please. */
	/*  uint8_t     tclass; can leave as zero for now (between subnets) */
	/*  ib_net16_t  pkey; leave as zero */
	p_mc_req->rate = IB_PATH_RECORD_RATE_2_5_GBS;
	/*  uint8_t     pkt_life; zero means greater than zero ... */
	/*  ib_net32_t  sl_flow_hop; keep it all zeros */
	/*  we want to use a link local scope: 0x02 */
	p_mc_req->scope_state = ib_member_set_scope_state(0x02, 0);
}