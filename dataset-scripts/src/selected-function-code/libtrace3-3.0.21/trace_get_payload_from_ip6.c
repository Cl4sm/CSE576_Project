void *trace_get_payload_from_ip6(libtrace_ip6_t *ipptr, uint8_t *prot,
		uint32_t *remaining) 
{
	void *payload = (char*)ipptr+sizeof(libtrace_ip6_t);
	uint8_t nxt;
	uint16_t len;

	assert (ipptr != NULL);
 	nxt = ipptr->nxt;	
	if (remaining) {
		if (*remaining<sizeof(libtrace_ip6_t)) {
			*remaining = 0;
			return NULL;
		}
		*remaining-=sizeof(libtrace_ip6_t);
	}

	while(1) {
		switch (nxt) {
			case 0: /* hop by hop options */
			case TRACE_IPPROTO_ROUTING:
			case TRACE_IPPROTO_AH: 
			case TRACE_IPPROTO_DSTOPTS: 
			{
				/* Length does not include the first 8 bytes */
				len=((libtrace_ip6_ext_t*)payload)->len * 8;
				len += 8;


				if (remaining) {
					if (*remaining < len) {
						/* Snap too short */
						*remaining = 0;
						return NULL;
					}
					*remaining-=len;
				}

				nxt=((libtrace_ip6_ext_t*)payload)->nxt;
				payload=(char*)payload+len;
				continue;
			}
			case TRACE_IPPROTO_ESP: 
			{
				if (prot) *prot=TRACE_IPPROTO_ESP;
				return payload;
			}
			case TRACE_IPPROTO_FRAGMENT:
				{
					len = sizeof(libtrace_ip6_frag_t);
					if (remaining) {
						if (*remaining < len) {
							/* Snap too short */
							*remaining = 0;
							return NULL;
						}
						*remaining-=len;
					}
					nxt=((libtrace_ip6_frag_t*)payload)->nxt;
					payload=(char*)payload+len;
					continue;
				}

			default:
				if (prot) *prot=nxt;
				return payload;
		}
	}
}