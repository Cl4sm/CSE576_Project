static void dump_ospf_v2_header(libtrace_ospf_v2_t *hdr, unsigned len) {

	DISPLAY(hdr, ospf_v, " OSPF Header: Version %u");
        DISPLAY(hdr, type, " Type %u ");
	switch(hdr->type) {
		case TRACE_OSPF_HELLO:
			printf("(Hello)");
			break;
		case TRACE_OSPF_DATADESC:
			printf("(Database Desc)");
			break;
		case TRACE_OSPF_LSREQ:
			printf("(Link State Request)");
			break;
		case TRACE_OSPF_LSUPDATE:
			printf("(Link State Update)");
			break;
		case TRACE_OSPF_LSACK:
			printf("(Link State Ack.)");
			break;
	}
        printf("\n");

	DISPLAYS(hdr, ospf_len, "OSPF Header: Length %u \n");
        DISPLAYIP(hdr, router, " OSPF Header: Router Id %s ");
        DISPLAYIP(hdr, area, "Area Id %s\n");
	DISPLAYS(hdr, sum, " OSPF Header: Checksum %u ");
        DISPLAYS(hdr, au_type, "Auth Type %u\n");
        DISPLAY(hdr, au_key_id, " OSPF Header: Auth Key ID %u ");
        DISPLAY(hdr, au_data_len, "Auth Data Len %u\n");
        DISPLAYL(hdr, au_seq_num, " OSPF Header: Auth Crypto Seq %u\n");

}