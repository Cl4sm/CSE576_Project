static void
__osmv_ibms_osm_addr_to_mad_addr(IN const osm_mad_addr_t * p_osm_addr,
				 IN uint8_t is_smi,
				 OUT struct _ibms_mad_addr *p_ibms_addr)
{

	/* For global destination or Multicast address: */
	p_ibms_addr->dlid = cl_ntoh16(p_osm_addr->dest_lid);
	p_ibms_addr->sl = p_osm_addr->addr_type.gsi.service_level;
	if (is_smi) {
		p_ibms_addr->sqpn = 0;
		p_ibms_addr->dqpn = 0;
	} else {
		p_ibms_addr->sqpn = 1;
		p_ibms_addr->dqpn =
		    cl_ntoh32(p_osm_addr->addr_type.gsi.remote_qp);
	}
	/*
	   HACK we limit to the first PKey Index assuming it will
	   always be the default PKey
	 */
	p_ibms_addr->pkey_index = 0;
}