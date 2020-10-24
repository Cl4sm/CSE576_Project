static void __qos_policy_validate_pkey(
			osm_qos_policy_t * p_qos_policy,
			osm_qos_match_rule_t * p_qos_match_rule,
			osm_prtn_t * p_prtn)
{
	int i = 0;

	if (!p_qos_policy || !p_qos_match_rule || !p_prtn)
		return;

	if (!p_qos_match_rule->p_qos_level->sl_set ||
	    p_prtn->sl == p_qos_match_rule->p_qos_level->sl)
		return;

	/* overriding partition's SL */
	OSM_LOG(&p_qos_policy->p_subn->p_osm->log, OSM_LOG_ERROR,
		"ERR AC15: pkey 0x%04X in match rule - "
		"overriding partition SL (%u) with QoS Level SL (%u)\n",
		cl_ntoh16(p_prtn->pkey), p_prtn->sl,
		p_qos_match_rule->p_qos_level->sl);
	p_prtn->sl = p_qos_match_rule->p_qos_level->sl;


	/* If this partition has default MCast groups.
	 * Fix those group's SL too */
	for (i = 0; i < p_prtn->nmgrps; i++)
	{
		uint8_t sl;
		uint32_t flow;
		uint8_t hop;

		CL_ASSERT((cl_ntoh16(p_prtn->mgrps[i]->mcmember_rec.pkey) & 0x7fff) ==
			  (cl_ntoh16(p_prtn->pkey) & 0x7fff));

		ib_member_get_sl_flow_hop(p_prtn->mgrps[i]->mcmember_rec.sl_flow_hop,
					  &sl, &flow, &hop);
		if (sl != p_prtn->sl) {
			char gid_str[INET6_ADDRSTRLEN];
			OSM_LOG(&p_qos_policy->p_subn->p_osm->log, OSM_LOG_DEBUG,
				"Updating MCGroup (MGID %s) SL to "
				"match partition SL (%u)\n",
				inet_ntop(AF_INET6, p_prtn->mgrps[i]->mcmember_rec.mgid.raw,
					  gid_str, sizeof gid_str),
				p_prtn->sl);
			p_prtn->mgrps[i]->mcmember_rec.sl_flow_hop =
				ib_member_set_sl_flow_hop(p_prtn->sl, flow, hop);
		}
	}
}