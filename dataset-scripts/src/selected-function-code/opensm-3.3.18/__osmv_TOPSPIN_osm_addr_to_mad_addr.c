static void
__osmv_TOPSPIN_osm_addr_to_mad_addr(IN const osm_mad_addr_t * p_mad_addr,
				    IN uint8_t is_smi, OUT struct ib_mad *p_mad)
{

	/* For global destination or Multicast address: */
	p_mad->dlid = cl_ntoh16(p_mad_addr->dest_lid);
	p_mad->sl = p_mad_addr->addr_type.gsi.service_level;
	if (is_smi) {
		p_mad->sqpn = 0;
		p_mad->dqpn = 0;
	} else {
		p_mad->sqpn = 1;
		p_mad->dqpn = cl_ntoh32(p_mad_addr->addr_type.gsi.remote_qp);
	}
	/*
	   HACK we limit to the first PKey Index assuming it will
	   always be the default PKey
	 */
	p_mad->pkey_index = 0;
}