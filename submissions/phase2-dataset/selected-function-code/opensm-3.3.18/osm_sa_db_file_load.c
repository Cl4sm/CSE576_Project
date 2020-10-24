int osm_sa_db_file_load(osm_opensm_t * p_osm)
{
	char line[1024];
	char *file_name;
	FILE *file;
	int ret = 0;
	osm_mgrp_t *p_next_mgrp = NULL;
	osm_mgrp_t *p_prev_mgrp = NULL;
	unsigned rereg_clients = 0;
	unsigned lineno;

	if (!p_osm->subn.first_time_master_sweep) {
		OSM_LOG(&p_osm->log, OSM_LOG_VERBOSE,
			"Not first sweep - skip SA DB restore\n");
		return 0;
	}

	file_name = p_osm->subn.opt.sa_db_file;
	if (!file_name) {
		OSM_LOG(&p_osm->log, OSM_LOG_VERBOSE,
			"sa db file name is not specifed. Skip restore\n");
		return 0;
	}

	file = fopen(file_name, "r");
	if (!file) {
		OSM_LOG(&p_osm->log, OSM_LOG_ERROR | OSM_LOG_SYS, "ERR 4C02: "
			"Can't open sa db file \'%s\'. Skip restoring\n",
			file_name);
		return -1;
	}

	OSM_LOG(&p_osm->log, OSM_LOG_VERBOSE,
		"Restoring SA DB from file \'%s\'\n",
		file_name);

	lineno = 0;

	while (fgets(line, sizeof(line) - 1, file) != NULL) {
		char *p;
		uint8_t val;

		lineno++;

		p = line;
		while (isspace(*p))
			p++;

		if (*p == '#')
			continue;

		if (!strncmp(p, "MC Group", 8)) {
			ib_member_rec_t mcm_rec;
			ib_net16_t mlid;

			p_next_mgrp = NULL;
			memset(&mcm_rec, 0, sizeof(mcm_rec));

			PARSE_AHEAD(p, net16, " 0x", &mlid);
			PARSE_AHEAD(p, net64, " mgid=0x",
				    &mcm_rec.mgid.unicast.prefix);
			PARSE_AHEAD(p, net64, ":0x",
				    &mcm_rec.mgid.unicast.interface_id);
			PARSE_AHEAD(p, net64, " port_gid=0x",
				    &mcm_rec.port_gid.unicast.prefix);
			PARSE_AHEAD(p, net64, ":0x",
				    &mcm_rec.port_gid.unicast.interface_id);
			PARSE_AHEAD(p, net32, " qkey=0x", &mcm_rec.qkey);
			PARSE_AHEAD(p, net16, " mlid=0x", &mcm_rec.mlid);
			PARSE_AHEAD(p, net8, " mtu=0x", &mcm_rec.mtu);
			PARSE_AHEAD(p, net8, " tclass=0x", &mcm_rec.tclass);
			PARSE_AHEAD(p, net16, " pkey=0x", &mcm_rec.pkey);
			PARSE_AHEAD(p, net8, " rate=0x", &mcm_rec.rate);
			PARSE_AHEAD(p, net8, " pkt_life=0x", &mcm_rec.pkt_life);
			PARSE_AHEAD(p, net32, " sl_flow_hop=0x",
				    &mcm_rec.sl_flow_hop);
			PARSE_AHEAD(p, net8, " scope_state=0x",
				    &mcm_rec.scope_state);
			PARSE_AHEAD(p, net8, " proxy_join=0x", &val);
			mcm_rec.proxy_join = val;

			p_next_mgrp = load_mcgroup(p_osm, mlid, &mcm_rec);
			if (!p_next_mgrp)
				rereg_clients = 1;
			if (cl_ntoh16(mlid) > p_osm->sm.mlids_init_max)
				p_osm->sm.mlids_init_max = cl_ntoh16(mlid);
		} else if (p_next_mgrp && !strncmp(p, "mcm_port", 8)) {
			ib_member_rec_t mcmr;
			ib_net64_t guid;
			osm_port_t *port;
			boolean_t proxy;

			PARSE_AHEAD(p, net64, " port_gid=0x",
				    &mcmr.port_gid.unicast.prefix);
			PARSE_AHEAD(p, net64, ":0x",
				    &mcmr.port_gid.unicast.interface_id);
			PARSE_AHEAD(p, net8, " scope_state=0x", &mcmr.scope_state);
			PARSE_AHEAD(p, net8, " proxy_join=0x", &val);
			proxy = val;

			guid = mcmr.port_gid.unicast.interface_id;
			port = osm_get_port_by_alias_guid(&p_osm->subn, guid);
			if (port &&
			    cl_qmap_get(&p_next_mgrp->mcm_port_tbl, guid) ==
			    cl_qmap_end(&p_next_mgrp->mcm_port_tbl) &&
			    !osm_mgrp_add_port(&p_osm->subn, &p_osm->log,
						p_next_mgrp, port, &mcmr, proxy))
				rereg_clients = 1;
		} else if (!strncmp(p, "Service Record:", 15)) {
			ib_service_record_t s_rec;
			uint32_t modified_time, lease_period;

			p_next_mgrp = NULL;
			memset(&s_rec, 0, sizeof(s_rec));

			PARSE_AHEAD(p, net64, " id=0x", &s_rec.service_id);
			PARSE_AHEAD(p, net64, " gid=0x",
				    &s_rec.service_gid.unicast.prefix);
			PARSE_AHEAD(p, net64, ":0x",
				    &s_rec.service_gid.unicast.interface_id);
			PARSE_AHEAD(p, net16, " pkey=0x", &s_rec.service_pkey);
			PARSE_AHEAD(p, net32, " lease=0x",
				    &s_rec.service_lease);
			PARSE_AHEAD(p, net64, " key=0x",
				    (ib_net64_t *) (&s_rec.service_key[0]));
			PARSE_AHEAD(p, net64, ":0x",
				    (ib_net64_t *) (&s_rec.service_key[8]));
			PARSE_AHEAD(p, string64, " name=", s_rec.service_name);
			PARSE_AHEAD(p, net64, " data8=0x",
				    (ib_net64_t *) (&s_rec.service_data8[0]));
			PARSE_AHEAD(p, net64, ":0x",
				    (ib_net64_t *) (&s_rec.service_data8[8]));
			PARSE_AHEAD(p, net64, " data16=0x",
				    (ib_net64_t *) (&s_rec.service_data16[0]));
			PARSE_AHEAD(p, net64, ":0x",
				    (ib_net64_t *) (&s_rec.service_data16[4]));
			PARSE_AHEAD(p, net64, " data32=0x",
				    (ib_net64_t *) (&s_rec.service_data32[0]));
			PARSE_AHEAD(p, net64, ":0x",
				    (ib_net64_t *) (&s_rec.service_data32[2]));
			PARSE_AHEAD(p, net64, " data64=0x",
				    &s_rec.service_data64[0]);
			PARSE_AHEAD(p, net64, ":0x", &s_rec.service_data64[1]);
			PARSE_AHEAD(p, net32, " modified_time=0x",
				    &modified_time);
			PARSE_AHEAD(p, net32, " lease_period=0x",
				    &lease_period);

			if (load_svcr(p_osm, &s_rec, cl_ntoh32(modified_time),
				      cl_ntoh32(lease_period)))
				rereg_clients = 1;
		} else if (!strncmp(p, "InformInfo Record:", 18)) {
			ib_inform_info_record_t i_rec;
			osm_mad_addr_t rep_addr;
			ib_net16_t val16;

			p_next_mgrp = NULL;
			memset(&i_rec, 0, sizeof(i_rec));
			memset(&rep_addr, 0, sizeof(rep_addr));

			PARSE_AHEAD(p, net64, " subscriber_gid=0x",
				    &i_rec.subscriber_gid.unicast.prefix);
			PARSE_AHEAD(p, net64, ":0x",
				    &i_rec.subscriber_gid.unicast.interface_id);
			PARSE_AHEAD(p, net16, " subscriber_enum=0x",
				    &i_rec.subscriber_enum);
			PARSE_AHEAD(p, net64, " gid=0x",
				    &i_rec.inform_info.gid.unicast.prefix);
			PARSE_AHEAD(p, net64, ":0x",
				    &i_rec.inform_info.gid.unicast.
				    interface_id);
			PARSE_AHEAD(p, net16, " lid_range_begin=0x",
				    &i_rec.inform_info.lid_range_begin);
			PARSE_AHEAD(p, net16, " lid_range_end=0x",
				    &i_rec.inform_info.lid_range_end);
			PARSE_AHEAD(p, net8, " is_generic=0x",
				    &i_rec.inform_info.is_generic);
			PARSE_AHEAD(p, net8, " subscribe=0x",
				    &i_rec.inform_info.subscribe);
			PARSE_AHEAD(p, net16, " trap_type=0x",
				    &i_rec.inform_info.trap_type);
			PARSE_AHEAD(p, net16, " trap_num=0x",
				    &i_rec.inform_info.g_or_v.generic.trap_num);
			PARSE_AHEAD(p, net32, " qpn_resp_time_val=0x",
				    &i_rec.inform_info.g_or_v.generic.
				    qpn_resp_time_val);
			PARSE_AHEAD(p, net32, " node_type=0x",
				    (uint32_t *) & i_rec.inform_info.g_or_v.
				    generic.reserved2);

			PARSE_AHEAD(p, net16, " rep_addr: lid=0x",
				    &rep_addr.dest_lid);
			PARSE_AHEAD(p, net8, " path_bits=0x",
				    &rep_addr.path_bits);
			PARSE_AHEAD(p, net8, " static_rate=0x",
				    &rep_addr.static_rate);
			PARSE_AHEAD(p, net32, " remote_qp=0x",
				    &rep_addr.addr_type.gsi.remote_qp);
			PARSE_AHEAD(p, net32, " remote_qkey=0x",
				    &rep_addr.addr_type.gsi.remote_qkey);
			PARSE_AHEAD(p, net16, " pkey_ix=0x", &val16);
			rep_addr.addr_type.gsi.pkey_ix = cl_ntoh16(val16);
			PARSE_AHEAD(p, net8, " sl=0x",
				    &rep_addr.addr_type.gsi.service_level);

			if (load_infr(p_osm, &i_rec, &rep_addr))
				rereg_clients = 1;
		} else if (!strncmp(p, "GUIDInfo Record:", 16)) {
			ib_guidinfo_record_t gi_rec;
			ib_net64_t base_guid;

			p_next_mgrp = NULL;
			memset(&gi_rec, 0, sizeof(gi_rec));

			PARSE_AHEAD(p, net64, " base_guid=0x", &base_guid);
			PARSE_AHEAD(p, net16, " lid=0x", &gi_rec.lid);
			PARSE_AHEAD(p, net8, " block_num=0x",
				    &gi_rec.block_num);
			PARSE_AHEAD(p, net64, " guid0=0x",
				    &gi_rec.guid_info.guid[0]);
			PARSE_AHEAD(p, net64, " guid1=0x",
				    &gi_rec.guid_info.guid[1]);
			PARSE_AHEAD(p, net64, " guid2=0x",
				    &gi_rec.guid_info.guid[2]);
			PARSE_AHEAD(p, net64, " guid3=0x",
				    &gi_rec.guid_info.guid[3]);
			PARSE_AHEAD(p, net64, " guid4=0x",
				    &gi_rec.guid_info.guid[4]);
			PARSE_AHEAD(p, net64, " guid5=0x",
				    &gi_rec.guid_info.guid[5]);
			PARSE_AHEAD(p, net64, " guid6=0x",
				    &gi_rec.guid_info.guid[6]);
			PARSE_AHEAD(p, net64, " guid7=0x",
				    &gi_rec.guid_info.guid[7]);

			if (load_guidinfo(p_osm, base_guid, &gi_rec))
				rereg_clients = 1;
		}

		/*
		 * p_next_mgrp points to the multicast group now being parsed.
		 * p_prev_mgrp points to the last multicast group we parsed.
		 * We decide whether to keep or delete each multicast group
		 * only when we finish parsing it's member records. if the
		 * group has full members, or it is a "well known group" we
		 * keep it.
		 */
		if (p_prev_mgrp != p_next_mgrp) {
			if (p_prev_mgrp)
				sa_db_file_load_handle_mgrp(p_osm, p_prev_mgrp);
			p_prev_mgrp = p_next_mgrp;
		}
	}

	if (p_next_mgrp)
		sa_db_file_load_handle_mgrp(p_osm, p_prev_mgrp);

	/*
	 * If loading succeeded, do whatever 'no_clients_rereg' says.
	 * If loading failed at some point, turn off the 'no_clients_rereg'
	 * option (turn on re-registration requests).
	 */
	if (rereg_clients)
		p_osm->subn.opt.no_clients_rereg = FALSE;

	/* We've just finished loading SA DB file - clear the "dirty" flag */
	p_osm->sa.dirty = FALSE;

_error:
	fclose(file);
	return ret;
}