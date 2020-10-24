static char *format_pro(const struct arphdr *arp, const char *pro) {
	static char buffer[1024] = {0,};
	int i;
	
	if (!pro) {
		strncpy(buffer, "(Truncated)", sizeof(buffer));
		return buffer;
	}

	switch(ntohs(arp->ar_pro)) {
		case 0x0800:
			snprintf(buffer,sizeof(buffer),"%s",
					inet_ntoa(*(struct in_addr*)pro));
			break;
		default:
			snprintf(buffer, sizeof(buffer), "%s", " (");
			for (i=0;i<arp->ar_pln;i++) {
				snprintf(buffer,sizeof(buffer),"%s %02x",
						buffer,(unsigned char)pro[i]);
			}
			strncat(buffer,")",sizeof(buffer) - strlen(buffer) - 1);
			break;
	}
	return buffer;
	
}