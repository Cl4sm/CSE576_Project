static void *trace_get_payload_from_ppp_hdlc(void *link, 
		uint16_t *type, uint32_t *remaining)
{
	libtrace_ppp_hdlc_t *ppp_hdlc = (libtrace_ppp_hdlc_t*)link;

	if (remaining) {
		if (*remaining < sizeof(libtrace_ppp_hdlc_t)) {
			*remaining = 0;
			return NULL;
		}
		*remaining-=sizeof(libtrace_ppp_hdlc_t);
	}

	if (type) {
		/* http://www.iana.org/assignments/ppp-numbers */

		switch(ntohs(ppp_hdlc->protocol)) {
			case 0x0021: /* IP */
				*type = TRACE_ETHERTYPE_IP;
				break;
			case 0xc021: /* Link Control Protocol */
				*type = 0; /* No ethertype for this */
				break;

			default:
				printf("Unknown chdlc type: %04x\n",
						ntohs(ppp_hdlc->protocol));
				*type = 0; /* Unknown */
		}
	}


	return (void*)((char *)ppp_hdlc+sizeof(*ppp_hdlc));
}