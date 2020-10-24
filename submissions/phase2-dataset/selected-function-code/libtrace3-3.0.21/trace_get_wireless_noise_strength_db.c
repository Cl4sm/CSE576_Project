DLLEXPORT bool trace_get_wireless_noise_strength_db(void *link,
		libtrace_linktype_t linktype, uint8_t *strength)
{
	uint8_t *p;
	void *l;
	uint16_t type;

	if (link == NULL || strength == NULL) return false;
	switch (linktype) {
		case TRACE_TYPE_80211_RADIO:
			if ((p = (uint8_t *) trace_get_radiotap_field(link,
							TRACE_RADIOTAP_DB_ANTNOISE))) {
				*strength = *p;
				return true;
			} else break;
		case TRACE_TYPE_LINUX_SLL:
			l = trace_get_payload_from_linux_sll(link, &type, NULL, NULL);
			return trace_get_wireless_noise_strength_db(l, arphrd_type_to_libtrace(type), strength);
		default:
			return false;
	}
	return false;
}