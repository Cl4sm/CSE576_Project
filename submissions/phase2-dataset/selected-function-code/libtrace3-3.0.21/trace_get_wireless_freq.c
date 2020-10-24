DLLEXPORT bool trace_get_wireless_freq(void *link, 
		libtrace_linktype_t linktype, uint16_t *freq)
{
	uint16_t *p;
	void *l;
	uint16_t type;

	if (link == NULL || freq == NULL) return false;
	switch (linktype) {
		case TRACE_TYPE_80211_RADIO:
			/* NB: The channel field is actually two 16 bit fields. 
			 * The chan_freq field is the first of those two, so we
			 * just cast it to a uint16_t.
			 */
			if (( p = (uint16_t *) trace_get_radiotap_field(link,
							TRACE_RADIOTAP_CHANNEL))) {
				*freq = bswap_le_to_host16(*p);
				return true;
			} else break;
		case TRACE_TYPE_LINUX_SLL:
			l = trace_get_payload_from_linux_sll(link, &type, NULL, NULL);
			return trace_get_wireless_freq(l, arphrd_type_to_libtrace(type), freq);
		default:
			return false;
	}
	return false;
}