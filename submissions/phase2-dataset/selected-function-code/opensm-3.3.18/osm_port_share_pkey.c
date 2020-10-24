boolean_t osm_port_share_pkey(IN osm_log_t * p_log,
			      IN const osm_port_t * p_port_1,
			      IN const osm_port_t * p_port_2,
			      IN boolean_t allow_both_pkeys)
{

	osm_physp_t *p_physp1, *p_physp2;
	boolean_t ret;

	OSM_LOG_ENTER(p_log);

	if (!p_port_1 || !p_port_2) {
		ret = FALSE;
		goto Exit;
	}

	p_physp1 = p_port_1->p_physp;
	p_physp2 = p_port_2->p_physp;

	if (!p_physp1 || !p_physp2) {
		ret = FALSE;
		goto Exit;
	}

	ret = osm_physp_share_pkey(p_log, p_physp1, p_physp2, allow_both_pkeys);

Exit:
	OSM_LOG_EXIT(p_log);
	return ret;
}