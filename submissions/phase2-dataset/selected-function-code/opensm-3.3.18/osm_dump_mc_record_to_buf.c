static void osm_dump_mc_record_to_buf(IN const ib_member_rec_t * p_mcmr,
				      OUT char * buf)
{
	if(!buf || !p_mcmr)
		return;
	else {
		char gid_str[INET6_ADDRSTRLEN];
		char gid_str2[INET6_ADDRSTRLEN];

		sprintf(buf,
			"MCMember Record dump:\n"
			"\t\t\t\tMGID....................%s\n"
			"\t\t\t\tPortGid.................%s\n"
			"\t\t\t\tqkey....................0x%X\n"
			"\t\t\t\tmlid....................0x%X\n"
			"\t\t\t\tmtu.....................0x%X\n"
			"\t\t\t\tTClass..................0x%X\n"
			"\t\t\t\tpkey....................0x%X\n"
			"\t\t\t\trate....................0x%X\n"
			"\t\t\t\tpkt_life................0x%X\n"
			"\t\t\t\tSLFlowLabelHopLimit.....0x%X\n"
			"\t\t\t\tScopeState..............0x%X\n"
			"\t\t\t\tProxyJoin...............0x%X\n",
			inet_ntop(AF_INET6, p_mcmr->mgid.raw, gid_str,
				  sizeof gid_str),
			inet_ntop(AF_INET6, p_mcmr->port_gid.raw, gid_str2,
				  sizeof gid_str2),
			cl_ntoh32(p_mcmr->qkey), cl_ntoh16(p_mcmr->mlid),
			p_mcmr->mtu, p_mcmr->tclass, cl_ntoh16(p_mcmr->pkey),
			p_mcmr->rate, p_mcmr->pkt_life,
			cl_ntoh32(p_mcmr->sl_flow_hop),
			p_mcmr->scope_state, p_mcmr->proxy_join);
	}
}