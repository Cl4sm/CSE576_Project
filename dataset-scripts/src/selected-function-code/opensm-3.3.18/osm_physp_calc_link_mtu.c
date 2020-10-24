uint8_t osm_physp_calc_link_mtu(IN osm_log_t * p_log,
				IN const osm_physp_t * p_physp,
				IN uint8_t current_mtu)
{
	const osm_physp_t *p_remote_physp;
	uint8_t mtu;
	uint8_t remote_mtu;

	OSM_LOG_ENTER(p_log);

	p_remote_physp = osm_physp_get_remote(p_physp);
	if (p_remote_physp) {
		/* use the available MTU */
		mtu = ib_port_info_get_mtu_cap(&p_physp->port_info);

		remote_mtu =
		    ib_port_info_get_mtu_cap(&p_remote_physp->port_info);

		OSM_LOG(p_log, OSM_LOG_DEBUG,
			"Remote port 0x%016" PRIx64 " port = %u : "
			"MTU = %u. This Port MTU: %u\n",
			cl_ntoh64(osm_physp_get_port_guid(p_remote_physp)),
			osm_physp_get_port_num(p_remote_physp),
			remote_mtu, mtu);

		if (mtu != remote_mtu) {
			if (mtu > remote_mtu)
				mtu = remote_mtu;
			if (mtu != current_mtu)
				OSM_LOG(p_log, OSM_LOG_VERBOSE,
					"MTU mismatch between ports."
					"\n\t\t\t\tPort 0x%016" PRIx64 ", port %u"
					" and port 0x%016" PRIx64 ", port %u."
					"\n\t\t\t\tUsing lower MTU of %u\n",
					cl_ntoh64(osm_physp_get_port_guid(p_physp)),
					osm_physp_get_port_num(p_physp),
					cl_ntoh64(osm_physp_get_port_guid
						  (p_remote_physp)),
					osm_physp_get_port_num(p_remote_physp), mtu);
		}
	} else
		mtu = ib_port_info_get_neighbor_mtu(&p_physp->port_info);

	if (mtu == 0) {
		OSM_LOG(p_log, OSM_LOG_DEBUG, "ERR 4101: "
			"Invalid MTU = 0. Forcing correction to 256\n");
		mtu = 1;
	}

	OSM_LOG_EXIT(p_log);
	return mtu;
}