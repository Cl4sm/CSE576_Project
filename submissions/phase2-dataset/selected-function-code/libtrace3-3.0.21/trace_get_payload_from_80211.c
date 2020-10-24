static void *trace_get_payload_from_80211(void *link, uint16_t *type, uint32_t *remaining)
{
	libtrace_80211_t *wifi;
	uint16_t *eth; /* ethertype */
	int8_t extra = 0; /* how many QoS bytes to skip */
	
	if (remaining && *remaining < sizeof(libtrace_80211_t)) {
		*remaining = 0;
		return NULL;
	}

	wifi=(libtrace_80211_t*)link;

	/* Data packet? */
	if (wifi->type != 2) {
		return NULL;
	}

	/* If FromDS and ToDS are both set then we have a four-address
	 * frame. Otherwise we have a three-address frame */
	if (!(wifi->to_ds && wifi->from_ds)) 
		extra -= 6; 
	
	/* Indicates QoS field present, see IEEE802.11e-2005 pg 21 */
	if (wifi->subtype & 0x8) 
		extra += 2;

	if (remaining && *remaining < sizeof(*eth)) {
		*remaining = 0;
		return NULL;
	}

	eth=(uint16_t *)((char*)wifi+sizeof(*wifi)+extra);
	
	if (*eth == 0xaaaa)
		/* Payload contains an 802.2 LLC/SNAP frame */
		return trace_get_payload_from_llcsnap((void *)eth, type, remaining);
			
	/* Otherwise we assume an Ethernet II frame */
	if (type) *type=ntohs(*eth);
	if (remaining) *remaining = *remaining - sizeof(libtrace_80211_t) - extra - sizeof(*eth);
	
	return (void*)((char*)eth+sizeof(*eth));
}