DLLEXPORT bool trace_get_wireless_tsft(void *link, 
		libtrace_linktype_t linktype, uint64_t *tsft)
{
	uint64_t *p;
	void *l;
	uint16_t type;
	if (link == NULL || tsft == NULL) return false;

	switch (linktype) {
		case TRACE_TYPE_80211_RADIO:
			if( (p = (uint64_t *) trace_get_radiotap_field(link, 
							TRACE_RADIOTAP_TSFT))) {
				*tsft = bswap_le_to_host64(*p);
				return true;
			} else break;
		case TRACE_TYPE_LINUX_SLL:
			l = trace_get_payload_from_linux_sll(link, &type, NULL, NULL );
			return trace_get_wireless_tsft(l, arphrd_type_to_libtrace(type), tsft);

		case TRACE_TYPE_80211_PRISM:
			return false;
		default:
			return false;
	}
	return false;
}