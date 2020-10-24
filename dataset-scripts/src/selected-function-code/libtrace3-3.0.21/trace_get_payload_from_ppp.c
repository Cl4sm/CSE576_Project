static void *trace_get_payload_from_ppp(void *link, 
		uint16_t *type, uint32_t *remaining)
{
	/* 64 byte capture. */
	libtrace_ppp_t *ppp = (libtrace_ppp_t*)link;

	if (remaining) {
		if (*remaining < sizeof(libtrace_ppp_t)) {
			*remaining = 0;
			return NULL;
		}
		*remaining-=sizeof(libtrace_ppp_t);
	}

	if (type) {
		switch(ntohs(ppp->protocol)) {
			case 0x0021: *type = TRACE_ETHERTYPE_IP; break;
			/* If it isn't IP, then it is probably PPP control and
			 * I can't imagine anyone caring about that too much
			 */
			default: *type = 0; break;
		}
	}


	return (void*)((char *)ppp+sizeof(*ppp));
}