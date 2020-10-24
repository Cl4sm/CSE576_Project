DLLEXPORT void *trace_get_payload_from_ip(libtrace_ip_t *ipptr, uint8_t *prot,
		uint32_t *remaining) 
{
        void *trans_ptr = 0;

        assert(ipptr != NULL);
	
	/* Er? IPv5? */
	if (ipptr->ip_v != 4)
		return NULL;

	if ((ntohs(ipptr->ip_off) & SW_IP_OFFMASK) != 0) {
		if (remaining)
			*remaining = 0;		
		return NULL;
	}

	if (remaining) {
		if (*remaining<(ipptr->ip_hl*4U)) {
			*remaining = 0;
			return NULL;
		}
		/* If the packet features extra "padding", we probably
		 * don't want that counting as possible payload, e.g. for
		 * payload length calculations */
		//if (*remaining > ntohs(ipptr->ip_len))
		//	*remaining = ntohs(ipptr->ip_len);

		*remaining-=(ipptr->ip_hl * 4);
	}

        trans_ptr = (void *)((char *)ipptr + (ipptr->ip_hl * 4));

	if (prot) *prot = ipptr->ip_p;

        return trans_ptr;
}