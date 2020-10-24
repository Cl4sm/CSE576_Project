static ib_api_status_t prtn_make_default(osm_log_t * p_log, osm_subn_t * p_subn,
					 boolean_t no_config)
{
	ib_api_status_t status = IB_UNKNOWN_ERROR;
	osm_prtn_t *p;

	p = osm_prtn_make_new(p_log, p_subn, "Default",
			      IB_DEFAULT_PARTIAL_PKEY);
	if (!p)
		goto _err;
	status = osm_prtn_add_all(p_log, p_subn, p, 0, no_config);
	if (status != IB_SUCCESS)
		goto _err;
	cl_map_remove(&p->part_guid_tbl, p_subn->sm_port_guid);
	status =
	    osm_prtn_add_port(p_log, p_subn, p, p_subn->sm_port_guid, TRUE);

	/* ipv4 broadcast group */
	if (no_config)
		osm_prtn_add_mcgroup(p_log, p_subn, p, OSM_DEFAULT_MGRP_RATE,
				     OSM_DEFAULT_MGRP_MTU, OSM_DEFAULT_SL,
				     0, OSM_IPOIB_BROADCAST_MGRP_QKEY, 0, 0,
				     &osm_ipoib_broadcast_mgid);

_err:
	return status;
}