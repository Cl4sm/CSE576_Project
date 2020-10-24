static struct timeval legacynzix_get_timeval(const libtrace_packet_t *packet) {
	uint64_t new_ts = DATA(packet->trace)->ts_high;
	uint32_t old_ts = DATA(packet->trace)->ts_old;
	struct timeval tv;
	uint32_t hdr_ts;

	legacy_nzix_t *legacy = (legacy_nzix_t *)packet->header;
		
	hdr_ts = legacy->ts;

	/* Seems we only need 30 bits to represent our timestamp */
	hdr_ts >>=2;
	
	/* Try a sequence number wrap-around comparison */
	if (ts_cmp(hdr_ts, old_ts) > (UINT32_MAX / 2) )
		new_ts += (1LL << 30); /* Wraparound */
	new_ts &= ~((1LL << 30) -1);	/* Mask lower 30 bits */
	new_ts += hdr_ts;		/* Packet ts is the new 30 bits */
	DATA(packet->trace)->ts_old = hdr_ts;

	tv.tv_sec = DATA(packet->trace)->starttime + (new_ts / (1000 * 1000));
	tv.tv_usec = new_ts % (1000 * 1000);
	DATA(packet->trace)->ts_high = new_ts;


	/*dts = tv.tv_sec + (double)tv.tv_usec / 1000 / 1000; */
	return tv;
	
}	