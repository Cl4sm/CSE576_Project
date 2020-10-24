static void decode_80211_mgmt(const char *pkt, unsigned len) {
	ieee80211_mgmt_frame *mgmt = (ieee80211_mgmt_frame *)pkt;
	const char *data;
	
	printf(" 802.11MAC: Management frame: ");
	
	if (len < sizeof(ieee80211_mgmt_frame)) {
		printf("[Truncated]\n");
		return;
	}

	switch (mgmt->ctl.subtype) {
		case 0: printf("association request"); break;
		case 1: printf("association response"); break;
		case 2: printf("reassociation request"); break;
		case 3: printf("reassociation response"); break;
		case 4: printf("probe request"); break;
		case 5: printf("probe response"); break;
		case 8: printf("beacon"); break;
		case 9: printf("ATIM"); break;
		case 10: printf("disassociation"); break;
		case 11: printf("authentication"); break;
		case 12: printf("deauthentication"); break;
		case 13: printf("action"); break;
		default: printf("RESERVED"); break;
	}
	
	printf("\n 802.11MAC: Duration = %u us\n", mgmt->duration);
	printf(" 802.11MAC: DA       = %s\n", macaddr(mgmt->addr1));
	printf(" 802.11MAC: SA       = %s\n", macaddr(mgmt->addr2));
	printf(" 802.11MAC: BSSID    = %s\n", macaddr(mgmt->addr3));
	printf(" 802.11MAC: fragment no. = %u, sequence no. = %u\n",
			(mgmt->seq_ctrl & 0x000F) ,
			(mgmt->seq_ctrl & 0xFFF0) >> 4);

	switch (mgmt->ctl.subtype) {
		case 0:
			decode_80211_assoc_request(pkt, len);
			break;	
		case 1:
			decode_80211_assoc_response(pkt, len);
			break;
		case 2:
			decode_80211_reassoc_request(pkt, len);
			break;
		case 3:
			/* Reassoc response == assoc response */
			decode_80211_assoc_response(pkt, len);
			break;
		case 4:
			decode_80211_information_elements((char *)pkt + sizeof(ieee80211_mgmt_frame), len - sizeof(ieee80211_mgmt_frame));
			break;
		case 5:
			/* Probe response == beacon frame */
			decode_80211_beacon(pkt, len);
			break;
		case 8:
			decode_80211_beacon(pkt, len);
			break;
		case 10:
			data = (pkt + sizeof(ieee80211_mgmt_frame));
			printf(" 802.11MAC: Reason Code = ");
			ieee80211_print_reason_code((uint16_t) ((data[0] << 8) | (data[1])));
			printf("\n");
			break;
						    
		case 11:
			decode_80211_authentication_frame(pkt, len);
			break;
		case 12:
			data = (pkt + sizeof(ieee80211_mgmt_frame));
			printf(" 802.11MAC: Reason Code = ");
			ieee80211_print_reason_code((uint16_t) ((data[0] << 8) | (data[1])));
			printf("\n");
			break;
		default:
			printf(" 802.11MAC: Subtype %u decoder not implemented\n", mgmt->ctl.subtype);
	}

	printf("\n");

}