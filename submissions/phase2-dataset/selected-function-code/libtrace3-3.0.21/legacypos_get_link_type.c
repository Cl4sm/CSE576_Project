static libtrace_linktype_t legacypos_get_link_type(
		const libtrace_packet_t *packet) {
	/* POS can be PPP over HDLC (DLT_PPP_SERIAL), or it can be
 	 * just straight PPP.  Sigh.
 	 *
 	 * Ref: RFC 1549
 	 * 	- Perry Lorier (2008-11-04)
 	 */
	
	/* It can also be CHDLC, which is different again */
	
	/* This check matchs PPP over HDLC, a la RFC 1662 */
	if (((char *)packet->payload)[0] == '\xFF'
		&& ((char*)packet->payload)[1] == '\x03')
		return TRACE_TYPE_POS;
	
	/* This check matches unicast CHDLC */
	else if (((char *)packet->payload)[0] == '\x0F' &&
		((char*)packet->payload)[1] == '\x00')
		return TRACE_TYPE_HDLC_POS;
	
	/* This check matches broadcast CHDLC */
	else if (((char *)packet->payload)[0] == '\x8F' &&
		((char*)packet->payload)[1] == '\x00')
		return TRACE_TYPE_HDLC_POS;

	/* Otherwise just assume raw PPP (?) */
	else 
		return TRACE_TYPE_PPP;
}