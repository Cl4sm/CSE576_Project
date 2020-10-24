static void encrypt_ips(struct libtrace_ip *ip,bool enc_source,bool enc_dest)
{
	struct libtrace_tcp *tcp;
	struct libtrace_udp *udp;
	struct libtrace_icmp *icmp;

	tcp=trace_get_tcp_from_ip(ip,NULL);
	udp=trace_get_udp_from_ip(ip,NULL);
	icmp=trace_get_icmp_from_ip(ip,NULL);

	if (enc_source) {
		uint32_t old_ip=ip->ip_src.s_addr;
		uint32_t new_ip=htonl(enc_ip(
					htonl(ip->ip_src.s_addr)
					));
		update_in_cksum32(&ip->ip_sum,old_ip,new_ip);
		if (tcp) update_in_cksum32(&tcp->check,old_ip,new_ip);
		if (udp) update_in_cksum32(&udp->check,old_ip,new_ip);
		ip->ip_src.s_addr = new_ip;
	}

	if (enc_dest) {
		uint32_t old_ip=ip->ip_dst.s_addr;
		uint32_t new_ip=htonl(enc_ip(
					htonl(ip->ip_dst.s_addr)
					));
		update_in_cksum32(&ip->ip_sum,old_ip,new_ip);
		if (tcp) update_in_cksum32(&tcp->check,old_ip,new_ip);
		if (udp) update_in_cksum32(&udp->check,old_ip,new_ip);
		ip->ip_dst.s_addr = new_ip;
	}

	if (icmp) {
		/* These are error codes that return the IP packet
		 * internally 
		 */
		
		if (icmp->type == 3 
				|| icmp->type == 5 
				|| icmp->type == 11) {
			char *ptr = (char *)icmp;
			encrypt_ips(
				(struct libtrace_ip*)(ptr+
					sizeof(struct libtrace_icmp)),
				enc_dest,
				enc_source);
		}

		if (enc_source || enc_dest)
			icmp->checksum = 0;
	}
}