static void decode_80211_ctrl(const char *pkt, unsigned len) {
	ieee80211_ctrl_frame_1addr *ctrl1 = (ieee80211_ctrl_frame_1addr *) pkt;
	ieee80211_ctrl_frame_2addr *ctrl2 = (ieee80211_ctrl_frame_2addr *) pkt;
	printf(" 802.11MAC: Control frame: ");
	
	if (len < sizeof(ieee80211_ctrl_frame_1addr)) {
		printf("[Truncated]\n");
		return;
	}
	
	switch (ctrl1->ctl.subtype) {
		case 8: 
			printf("BlockAckReq\n"); 
			break;
		case 9: 
			printf("BlockAck\n"); 
			break;
		case 10: 
			printf("PS-Poll\n"); 
			printf(" 802.11MAC: AID = 0x%04x\n", ntohs(ctrl1->duration));
			printf(" 802.11MAC: BSSID = %s\n", macaddr(ctrl1->addr1));
			break;
		case 11:
			printf("RTS\n");
 
			if (len < sizeof(ieee80211_ctrl_frame_2addr)) {
				printf("[Truncated]\n");
				return;
			}

			printf(" 802.11MAC: RA = %s\n", macaddr(ctrl2->addr1));
			printf(" 802.11MAC: TA = %s\n", macaddr(ctrl2->addr2));
			break;
		case 12: 
			printf("CTS\n"); 
			printf(" 802.11MAC: RA = %s\n", macaddr(ctrl1->addr1));
			break;
		case 13:
			printf("ACK\n"); 
			printf(" 802.11MAC: RA = %s\n", macaddr(ctrl1->addr1));
			break;
		case 14:
			printf("CF-End\n"); 

			if (len < sizeof(ieee80211_ctrl_frame_2addr)) {
				printf("[Truncated]\n");
				return;
			}

			printf(" 802.11MAC: RA = %s\n", macaddr(ctrl2->addr1));
			printf(" 802.11MAC: BSSID = %s\n", macaddr(ctrl2->addr2));
			break;
		case 15:
			printf("CF-End + CF-Ack\n"); 

			if (len < sizeof(ieee80211_ctrl_frame_2addr)) {
				printf("[Truncated]\n");
				return;
			}

			printf(" 802.11MAC: RA = %s\n", macaddr(ctrl2->addr1));
			printf(" 802.11MAC: BSSID = %s\n", macaddr(ctrl2->addr2));
			break;
		default:
			printf("RESERVED"); 
			break;
	}

}