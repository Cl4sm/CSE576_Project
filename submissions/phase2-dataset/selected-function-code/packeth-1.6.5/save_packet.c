int save_packet(GtkButton *button, gpointer user_data, FILE *file_p) {

	if (make_packet(button, user_data) == -1) {
		return -1;
	}

	struct pcap_hdr fh;
	fh.magic = PCAP_MAGIC;
	fh.version_major = 2;
	fh.version_minor = 4;
	fh.thiszone = 0;
	fh.sigfigs = 0;
	fh.snaplen = 102400;
	fh.network = pcap_link_type;
	fwrite(&fh, sizeof(fh), 1, file_p);
    
	struct pcaprec_hdr ph;
	ph.ts_sec = 0;
        ph.ts_usec = 0;
        ph.incl_len = number;
        ph.orig_len = number;
        fwrite(&ph, sizeof(ph), 1, file_p);

	fwrite(packet, number, 1, file_p);

	return 1;
}