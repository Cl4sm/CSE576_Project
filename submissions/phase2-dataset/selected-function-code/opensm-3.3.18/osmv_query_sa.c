ib_api_status_t
osmv_query_sa(IN osm_bind_handle_t h_bind,
	      IN const osmv_query_req_t * const p_query_req)
{
	union {
		ib_service_record_t svc_rec;
		ib_node_record_t node_rec;
		ib_portinfo_record_t port_info;
		ib_path_rec_t path_rec;
#ifdef DUAL_SIDED_RMPP
		ib_multipath_rec_t multipath_rec;
#endif
		ib_class_port_info_t class_port_info;
	} u;
	osmv_sa_mad_data_t sa_mad_data;
	osmv_sa_bind_info_t *p_bind = (osmv_sa_bind_info_t *) h_bind;
	osmv_user_query_t *p_user_query;
#ifdef DUAL_SIDED_RMPP
	osmv_multipath_req_t *p_mpr_req;
	int i, j;
#endif
	osm_log_t *p_log = p_bind->p_log;
	ib_api_status_t status;

	OSM_LOG_ENTER(p_log);

	/* Set the request information. */
	sa_mad_data.method = IB_MAD_METHOD_GETTABLE;
	sa_mad_data.attr_mod = 0;
	sa_mad_data.attr_offset = 0;

	/* Set the MAD attributes and component mask correctly. */
	switch (p_query_req->query_type) {

	case OSMV_QUERY_USER_DEFINED:
		OSM_LOG(p_log, OSM_LOG_DEBUG, "DBG:001 USER_DEFINED\n");
		p_user_query = (osmv_user_query_t *) p_query_req->p_query_input;
		if (p_user_query->method)
			sa_mad_data.method = p_user_query->method;
#ifdef DUAL_SIDED_RMPP
		if (sa_mad_data.method == IB_MAD_METHOD_GETMULTI ||
		    sa_mad_data.method == IB_MAD_METHOD_GETTRACETABLE)
			sa_mad_data.attr_offset = p_user_query->attr_offset;
#endif
		sa_mad_data.attr_id = p_user_query->attr_id;
		sa_mad_data.attr_mod = p_user_query->attr_mod;
		sa_mad_data.comp_mask = p_user_query->comp_mask;
		sa_mad_data.p_attr = p_user_query->p_attr;
		break;

	case OSMV_QUERY_ALL_SVC_RECS:
		OSM_LOG(p_log, OSM_LOG_DEBUG, "DBG:001 SVC_REC_BY_NAME\n");
		sa_mad_data.attr_id = IB_MAD_ATTR_SERVICE_RECORD;
		sa_mad_data.comp_mask = 0;
		sa_mad_data.p_attr = &u.svc_rec;
		break;

	case OSMV_QUERY_SVC_REC_BY_NAME:
		OSM_LOG(p_log, OSM_LOG_DEBUG, "DBG:001 SVC_REC_BY_NAME\n");
		sa_mad_data.method = IB_MAD_METHOD_GET;
		sa_mad_data.attr_id = IB_MAD_ATTR_SERVICE_RECORD;
		sa_mad_data.comp_mask = IB_SR_COMPMASK_SNAME;
		sa_mad_data.p_attr = &u.svc_rec;
		memcpy(u.svc_rec.service_name, p_query_req->p_query_input,
		       sizeof(ib_svc_name_t));
		break;

	case OSMV_QUERY_SVC_REC_BY_ID:
		OSM_LOG(p_log, OSM_LOG_DEBUG, "DBG:001 SVC_REC_BY_ID\n");
		sa_mad_data.attr_id = IB_MAD_ATTR_SERVICE_RECORD;
		sa_mad_data.comp_mask = IB_SR_COMPMASK_SID;
		sa_mad_data.p_attr = &u.svc_rec;
		u.svc_rec.service_id =
		    *(ib_net64_t *) (p_query_req->p_query_input);
		break;

	case OSMV_QUERY_CLASS_PORT_INFO:
		OSM_LOG(p_log, OSM_LOG_DEBUG, "DBG:001 CLASS_PORT_INFO\n");
		sa_mad_data.method = IB_MAD_METHOD_GET;
		sa_mad_data.attr_id = IB_MAD_ATTR_CLASS_PORT_INFO;
		sa_mad_data.comp_mask = 0;
		sa_mad_data.p_attr = &u.class_port_info;
		break;

	case OSMV_QUERY_NODE_REC_BY_NODE_GUID:
		OSM_LOG(p_log, OSM_LOG_DEBUG, "DBG:001 NODE_REC_BY_NODE_GUID\n");
		sa_mad_data.attr_id = IB_MAD_ATTR_NODE_RECORD;
		sa_mad_data.comp_mask = IB_NR_COMPMASK_NODEGUID;
		sa_mad_data.p_attr = &u.node_rec;
		u.node_rec.node_info.node_guid =
		    *(ib_net64_t *) (p_query_req->p_query_input);
		break;

	case OSMV_QUERY_PORT_REC_BY_LID:
		OSM_LOG(p_log, OSM_LOG_DEBUG, "DBG:001 PORT_REC_BY_LID\n");
		sa_mad_data.attr_id = IB_MAD_ATTR_PORTINFO_RECORD;
		sa_mad_data.comp_mask = IB_PIR_COMPMASK_LID;
		sa_mad_data.p_attr = &u.port_info;
		u.port_info.lid = *(ib_net16_t *) (p_query_req->p_query_input);
		break;

	case OSMV_QUERY_PORT_REC_BY_LID_AND_NUM:
		sa_mad_data.method = IB_MAD_METHOD_GET;
		p_user_query = (osmv_user_query_t *) p_query_req->p_query_input;
		OSM_LOG(p_log, OSM_LOG_DEBUG, "DBG:001 PORT_REC_BY_LID_AND_NUM\n");
		sa_mad_data.attr_id = IB_MAD_ATTR_PORTINFO_RECORD;
		sa_mad_data.comp_mask =
		    IB_PIR_COMPMASK_LID | IB_PIR_COMPMASK_PORTNUM;
		sa_mad_data.p_attr = p_user_query->p_attr;
		break;

	case OSMV_QUERY_VLARB_BY_LID_PORT_BLOCK:
		sa_mad_data.method = IB_MAD_METHOD_GET;
		p_user_query = (osmv_user_query_t *) p_query_req->p_query_input;
		OSM_LOG(p_log, OSM_LOG_DEBUG, "DBG:001 OSMV_QUERY_VLARB_BY_LID_PORT_BLOCK\n");
		sa_mad_data.attr_id = IB_MAD_ATTR_VLARB_RECORD;
		sa_mad_data.comp_mask =
		    IB_VLA_COMPMASK_LID | IB_VLA_COMPMASK_OUT_PORT |
		    IB_VLA_COMPMASK_BLOCK;
		sa_mad_data.p_attr = p_user_query->p_attr;
		break;

	case OSMV_QUERY_SLVL_BY_LID_AND_PORTS:
		sa_mad_data.method = IB_MAD_METHOD_GET;
		p_user_query = (osmv_user_query_t *) p_query_req->p_query_input;
		OSM_LOG(p_log, OSM_LOG_DEBUG, "DBG:001 OSMV_QUERY_VLARB_BY_LID_PORT_BLOCK\n");
		sa_mad_data.attr_id = IB_MAD_ATTR_SLVL_RECORD;
		sa_mad_data.comp_mask =
		    IB_SLVL_COMPMASK_LID | IB_SLVL_COMPMASK_OUT_PORT |
		    IB_SLVL_COMPMASK_IN_PORT;
		sa_mad_data.p_attr = p_user_query->p_attr;
		break;

	case OSMV_QUERY_PATH_REC_BY_PORT_GUIDS:
		OSM_LOG(p_log, OSM_LOG_DEBUG, "DBG:001 PATH_REC_BY_PORT_GUIDS\n");
		memset(&u.path_rec, 0, sizeof(ib_path_rec_t));
		sa_mad_data.attr_id = IB_MAD_ATTR_PATH_RECORD;
		sa_mad_data.comp_mask =
		    (IB_PR_COMPMASK_DGID | IB_PR_COMPMASK_SGID | IB_PR_COMPMASK_NUMBPATH);
		u.path_rec.num_path = 0x7f;
		sa_mad_data.p_attr = &u.path_rec;
		ib_gid_set_default(&u.path_rec.dgid,
				   ((osmv_guid_pair_t *) (p_query_req->
							  p_query_input))->
				   dest_guid);
		ib_gid_set_default(&u.path_rec.sgid,
				   ((osmv_guid_pair_t *) (p_query_req->
							  p_query_input))->
				   src_guid);
		break;

	case OSMV_QUERY_PATH_REC_BY_GIDS:
		OSM_LOG(p_log, OSM_LOG_DEBUG, "DBG:001 PATH_REC_BY_GIDS\n");
		memset(&u.path_rec, 0, sizeof(ib_path_rec_t));
		sa_mad_data.attr_id = IB_MAD_ATTR_PATH_RECORD;
		sa_mad_data.comp_mask =
		    (IB_PR_COMPMASK_DGID | IB_PR_COMPMASK_SGID | IB_PR_COMPMASK_NUMBPATH);
		u.path_rec.num_path = 0x7f;
		sa_mad_data.p_attr = &u.path_rec;
		memcpy(&u.path_rec.dgid,
		       &((osmv_gid_pair_t *) (p_query_req->p_query_input))->
		       dest_gid, sizeof(ib_gid_t));
		memcpy(&u.path_rec.sgid,
		       &((osmv_gid_pair_t *) (p_query_req->p_query_input))->
		       src_gid, sizeof(ib_gid_t));
		break;

	case OSMV_QUERY_PATH_REC_BY_LIDS:
		OSM_LOG(p_log, OSM_LOG_DEBUG, "DBG:001 PATH_REC_BY_LIDS\n");
		memset(&u.path_rec, 0, sizeof(ib_path_rec_t));
		sa_mad_data.method = IB_MAD_METHOD_GET;
		sa_mad_data.attr_id = IB_MAD_ATTR_PATH_RECORD;
		sa_mad_data.comp_mask =
		    (IB_PR_COMPMASK_DLID | IB_PR_COMPMASK_SLID);
		sa_mad_data.p_attr = &u.path_rec;
		u.path_rec.dlid =
		    ((osmv_lid_pair_t *) (p_query_req->p_query_input))->
		    dest_lid;
		u.path_rec.slid =
		    ((osmv_lid_pair_t *) (p_query_req->p_query_input))->src_lid;
		break;

	case OSMV_QUERY_UD_MULTICAST_SET:
		sa_mad_data.method = IB_MAD_METHOD_SET;
		p_user_query = (osmv_user_query_t *) p_query_req->p_query_input;
		OSM_LOG(p_log, OSM_LOG_DEBUG, "DBG:001 OSMV_QUERY_UD_MULTICAST_SET\n");
		sa_mad_data.attr_id = IB_MAD_ATTR_MCMEMBER_RECORD;
		sa_mad_data.comp_mask = p_user_query->comp_mask;
		sa_mad_data.p_attr = p_user_query->p_attr;
		break;

	case OSMV_QUERY_UD_MULTICAST_DELETE:
		sa_mad_data.method = IB_MAD_METHOD_DELETE;
		p_user_query = (osmv_user_query_t *) p_query_req->p_query_input;
		OSM_LOG(p_log, OSM_LOG_DEBUG, "DBG:001 OSMV_QUERY_UD_MULTICAST_DELETE\n");
		sa_mad_data.attr_id = IB_MAD_ATTR_MCMEMBER_RECORD;
		sa_mad_data.comp_mask = p_user_query->comp_mask;
		sa_mad_data.p_attr = p_user_query->p_attr;
		break;

#ifdef DUAL_SIDED_RMPP
	case OSMV_QUERY_MULTIPATH_REC:
		OSM_LOG(p_log, OSM_LOG_DEBUG, "DBG:001 MULTIPATH_REC\n");
		/* Validate sgid/dgid counts against SA client limit */
		p_mpr_req = (osmv_multipath_req_t *) p_query_req->p_query_input;
		if (p_mpr_req->sgid_count + p_mpr_req->dgid_count >
		    IB_MULTIPATH_MAX_GIDS) {
			OSM_LOG(p_log, OSM_LOG_ERROR, "DBG:001 MULTIPATH_REC "
				"SGID count %d DGID count %d max count %d\n",
				p_mpr_req->sgid_count, p_mpr_req->dgid_count,
				IB_MULTIPATH_MAX_GIDS);
			CL_ASSERT(0);
			return IB_ERROR;
		}
		memset(&u.multipath_rec, 0, sizeof(ib_multipath_rec_t));
		sa_mad_data.method = IB_MAD_METHOD_GETMULTI;
		sa_mad_data.attr_id = IB_MAD_ATTR_MULTIPATH_RECORD;
		sa_mad_data.attr_offset =
		    ib_get_attr_offset(sizeof(ib_multipath_rec_t));
		sa_mad_data.p_attr = &u.multipath_rec;
		sa_mad_data.comp_mask = p_mpr_req->comp_mask;
		u.multipath_rec.num_path = p_mpr_req->num_path;
		if (p_mpr_req->reversible)
			u.multipath_rec.num_path |= 0x80;
		else
			u.multipath_rec.num_path &= ~0x80;
		u.multipath_rec.pkey = p_mpr_req->pkey;
		ib_multipath_rec_set_sl(&u.multipath_rec, p_mpr_req->sl);
		ib_multipath_rec_set_qos_class(&u.multipath_rec, 0);
		u.multipath_rec.independence = p_mpr_req->independence;
		u.multipath_rec.sgid_count = p_mpr_req->sgid_count;
		u.multipath_rec.dgid_count = p_mpr_req->dgid_count;
		j = 0;
		for (i = 0; i < p_mpr_req->sgid_count; i++, j++)
			u.multipath_rec.gids[j] = p_mpr_req->gids[j];
		for (i = 0; i < p_mpr_req->dgid_count; i++, j++)
			u.multipath_rec.gids[j] = p_mpr_req->gids[j];
		break;
#endif

	default:
		OSM_LOG(p_log, OSM_LOG_ERROR, "DBG:001 UNKNOWN\n");
		CL_ASSERT(0);
		return IB_ERROR;
	}

	status = __osmv_send_sa_req(h_bind, &sa_mad_data, p_query_req);

	OSM_LOG_EXIT(p_log);
	return status;
}