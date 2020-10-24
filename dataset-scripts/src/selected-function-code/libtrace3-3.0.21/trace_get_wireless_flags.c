bool trace_get_wireless_flags(void *link, 
		libtrace_linktype_t linktype, uint8_t *flags)
{
	uint8_t *p;
	void *l;
	uint16_t type;

	if (link == NULL || flags == NULL) return false;

	switch(linktype) {
		case TRACE_TYPE_80211_RADIO:
			if (( p = (uint8_t *) trace_get_radiotap_field(link,
							TRACE_RADIOTAP_FLAGS))) {
				*flags = *p;
				return true;
			} else break;
		case TRACE_TYPE_LINUX_SLL:
			l = trace_get_payload_from_linux_sll(link, &type, NULL, NULL);
			return trace_get_wireless_flags(l, arphrd_type_to_libtrace(type), flags);
		default:
			return false;
	}
	return false;
}