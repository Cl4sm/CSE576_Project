static int pcapfile_write_packet(libtrace_out_t *out,
		libtrace_packet_t *packet)
{
	struct libtrace_pcapfile_pkt_hdr_t hdr;
	struct timeval tv = trace_get_timeval(packet);
	int numbytes;
	int ret;
	void *ptr;
	uint32_t remaining;
	libtrace_linktype_t linktype;

	ptr = trace_get_packet_buffer(packet,&linktype,&remaining);
	
	/* Silently discard RT metadata packets and packets with an
	 * unknown linktype. */
	if (linktype == TRACE_TYPE_NONDATA || linktype == TRACE_TYPE_UNKNOWN) {
		return 0;
	}

	/* If this packet cannot be converted to a pcap linktype then
	 * pop off the top header until it can be converted
	 */
	while (libtrace_to_pcap_linktype(linktype)==TRACE_DLT_ERROR) {
		if (!demote_packet(packet)) {
			trace_set_err_out(out, 
				TRACE_ERR_NO_CONVERSION,
				"pcap does not support this format");
			assert(0);
			return -1;
		}

		ptr = trace_get_packet_buffer(packet,&linktype,&remaining);
	}


	/* Now we know the link type write out a header if we've not done
	 * so already
	 */
	if (!DATAOUT(out)->file) {
		struct pcapfile_header_t pcaphdr;

		DATAOUT(out)->file=trace_open_file_out(out,
				DATAOUT(out)->compress_type,
				DATAOUT(out)->level,
				DATAOUT(out)->flag);

		if (!DATAOUT(out)->file) {
			trace_set_err_out(out,errno,"Unable to open file");
			return -1;
		}

		pcaphdr.magic_number = 0xa1b2c3d4;
		pcaphdr.version_major = 2;
		pcaphdr.version_minor = 4;
		pcaphdr.thiszone = 0;
		pcaphdr.sigfigs = 0;
		pcaphdr.snaplen = 65536;
		pcaphdr.network = 
			libtrace_to_pcap_linktype(linktype);

		wandio_wwrite(DATAOUT(out)->file, 
				&pcaphdr, sizeof(pcaphdr));
	}


	hdr.ts_sec = (uint32_t)tv.tv_sec;
	hdr.ts_usec = (uint32_t)tv.tv_usec;
	hdr.caplen = trace_get_capture_length(packet);
	assert(hdr.caplen < LIBTRACE_PACKET_BUFSIZE);
	/* PCAP doesn't include the FCS in its wire length value, but we do */
	if (linktype==TRACE_TYPE_ETH) {
		if (trace_get_wire_length(packet) >= 4) {
			hdr.wirelen = trace_get_wire_length(packet)-4;
		}
		else {
			hdr.wirelen = 0;
		}
	}
	else
		hdr.wirelen = trace_get_wire_length(packet);

	/* Reason for removing this assert:
	 *
	 * There exist some packets, e.g. in IPLS II, where the wire length
	 * is clearly corrupt. When converting to pcap, we *could* try to
	 * adjust the wire length to something sane but for now, I'll just let
	 * the broken length persist through the conversion.
	 *
	 * XXX Is setting the wire length to zero the best solution in such
	 * cases?
	 */

	/* assert(hdr.wirelen < LIBTRACE_PACKET_BUFSIZE); */

	/* Ensure we have a valid capture length, especially if we're going
	 * to "remove" the FCS from the wire length */
	if (hdr.caplen > hdr.wirelen)
		hdr.caplen = hdr.wirelen;

	/* Write the packet header */
	numbytes=wandio_wwrite(DATAOUT(out)->file,
			&hdr, sizeof(hdr));

	if (numbytes!=sizeof(hdr)) 
		return -1;

	/* Write the rest of the packet now */
	ret=wandio_wwrite(DATAOUT(out)->file,
			ptr,
			hdr.caplen);

	if (ret!=(int)hdr.caplen)
		return -1;

	return numbytes+ret;
}