static char *format_hrd(const struct arphdr *arp, const char *hrd) {
	static char buffer[1024] = {0,};
	int i;

	if (!hrd) {
		strncpy(buffer, "(Truncated)", sizeof(buffer));
		return buffer;
	}

	switch(ntohs(arp->ar_hrd)) {
		case ARPHRD_ETHER:
			trace_ether_ntoa((const unsigned char *)hrd, buffer);
			break;
		default:
			for (i=0;i<arp->ar_hln;i++) {
				snprintf(buffer,sizeof(buffer),"%s %02x",
						buffer,(unsigned char)hrd[i]);
			}
			break;
	}
	
	return buffer;
}