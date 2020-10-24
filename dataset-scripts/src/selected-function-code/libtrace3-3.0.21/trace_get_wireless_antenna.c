DLLEXPORT bool trace_get_wireless_antenna(void *link,
		libtrace_linktype_t linktype, uint8_t *antenna)
{
	uint8_t *p;
	void *l;
	uint16_t type;

	if (link == NULL || antenna == NULL) return false;
	switch (linktype) {
		case TRACE_TYPE_80211_RADIO:
			if ((p = (uint8_t *) trace_get_radiotap_field(link,
							TRACE_RADIOTAP_ANTENNA))) {
				*antenna = *p;
				return true;
			} else break;
		case TRACE_TYPE_LINUX_SLL:
			l = trace_get_payload_from_linux_sll(link, &type, NULL, NULL);
			return trace_get_wireless_antenna(l, arphrd_type_to_libtrace(type), antenna);
		default:
			return false;
	}
	return false;
}