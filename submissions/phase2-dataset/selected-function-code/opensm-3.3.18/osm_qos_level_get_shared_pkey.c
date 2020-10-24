ib_net16_t osm_qos_level_get_shared_pkey(IN const osm_qos_level_t * p_qos_level,
					 IN const osm_physp_t * p_src_physp,
					 IN const osm_physp_t * p_dest_physp,
					 IN const boolean_t allow_both_pkeys)
{
	unsigned i;
	uint16_t pkey_ho = 0;

	if (!p_qos_level || !p_qos_level->pkey_range_len)
		return 0;

	/*
	 * ToDo: This approach is not optimal.
	 *       Think how to find shared pkey that also exists
	 *       in QoS level in less runtime.
	 */

	for (i = 0; i < p_qos_level->pkey_range_len; i++) {
		for (pkey_ho = p_qos_level->pkey_range_arr[i][0];
		     pkey_ho <= p_qos_level->pkey_range_arr[i][1]; pkey_ho++) {
			if (osm_physp_share_this_pkey
			    (p_src_physp, p_dest_physp, cl_hton16(pkey_ho),
			     allow_both_pkeys))
				return cl_hton16(pkey_ho);
		}
	}

	return 0;
}