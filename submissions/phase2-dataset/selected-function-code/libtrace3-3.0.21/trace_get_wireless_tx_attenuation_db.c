DLLEXPORT bool trace_get_wireless_tx_attenuation_db(void *link,
		libtrace_linktype_t linktype, uint16_t *attenuation)
{
	uint16_t *p;
	void *l;
	uint16_t type;

	if (link == NULL || attenuation == NULL) return false;
	switch (linktype) {
		case TRACE_TYPE_80211_RADIO:
			if ((p = (uint16_t *) trace_get_radiotap_field(link,
							TRACE_RADIOTAP_DB_TX_ATTENUATION))) {
				*attenuation = bswap_le_to_host16(*p);
				return true;
			} else break;
		case TRACE_TYPE_LINUX_SLL:
			l = trace_get_payload_from_linux_sll(link, &type, NULL, NULL);
			return trace_get_wireless_tx_attenuation_db(l, arphrd_type_to_libtrace(type), attenuation);
		default:
			return false;
	}
	return false;
}