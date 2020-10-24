DLLEXPORT bool trace_get_wireless_tx_power_dbm(void *link,
		libtrace_linktype_t linktype, int8_t *txpower)
{
	int8_t *p;
	void *l;
	uint16_t type;

	if (link == NULL || txpower == NULL) return false;
	switch (linktype) {
		case TRACE_TYPE_80211_RADIO:
			if ((p=(int8_t *) trace_get_radiotap_field(link,
							TRACE_RADIOTAP_DBM_TX_POWER))) {
				*txpower = *p;
				return true;
			} else break;
		case TRACE_TYPE_LINUX_SLL:
			l = trace_get_payload_from_linux_sll(link, &type, NULL, NULL);
			return trace_get_wireless_tx_power_dbm(l, arphrd_type_to_libtrace(type), txpower);
		default:
			return false;
	}
	return false;
}