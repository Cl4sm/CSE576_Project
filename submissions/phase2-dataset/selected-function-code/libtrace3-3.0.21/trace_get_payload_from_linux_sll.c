void *trace_get_payload_from_linux_sll(const void *link,
		uint16_t *arphrd_type, uint16_t *next, 
		uint32_t *remaining) 
{
	libtrace_sll_header_t *sll;

	sll = (libtrace_sll_header_t*) link;

	if (remaining) {
		if (*remaining < sizeof(*sll)) {
			*remaining = 0;
			return NULL;
		}
		*remaining-=sizeof(*sll);
	}

	/* The SLL header is actually in place of a link layer header, so
	 * we want to use the protocol field to tell our caller what the
	 * next header is going to be */
	if (next) *next = (libtrace_linktype_t)(ntohs(sll->protocol));
	if (arphrd_type) *arphrd_type = ntohs(sll->hatype);

	return (void*)((char*)sll+sizeof(*sll));

}