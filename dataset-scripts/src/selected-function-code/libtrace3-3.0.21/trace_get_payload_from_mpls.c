void *trace_get_payload_from_mpls(void *ethernet, uint16_t *type, 
		uint32_t *remaining)
{
	
	assert(type);
	if ((((char*)ethernet)[2]&0x01)==0) {
		/* The MPLS Stack bit is set */
		*type = TRACE_ETHERTYPE_MPLS;
	}
	else {
		if (!remaining || *remaining>=5) {
			switch (((char*)ethernet)[4]&0xF0) {
				case 0x40:	/* IPv4 */
					*type = TRACE_ETHERTYPE_IP;
					break;
				case 0x60:	/* IPv6 */
					*type = TRACE_ETHERTYPE_IPV6;
					break;
				default:	/* VPLS */
					/* Ethernet */
					*type = 0;
			}
		}
	}
	ethernet=(char*)ethernet+4;
	if (remaining) {
		if (*remaining<4)
			return NULL;
		else
			*remaining-=4;
	}


	return ethernet;
}