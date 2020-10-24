static void decode_80211_data(const char *pkt, unsigned len) {
	ieee80211_data_frame *data = (ieee80211_data_frame *) pkt;
	ieee80211_qos_data_frame *qos = (ieee80211_qos_data_frame *)pkt;
	ieee80211_payload *pld; 
	uint32_t hdrlen = 0;
	
	printf(" 802.11MAC: Data frame: ");
	
	if (len < sizeof(ieee80211_data_frame_3)) {
		printf("[Truncated]\n");
		return;
	}

	switch (data->ctl.subtype) {
		case 0: printf("Data"); break;
		case 1: printf("Data + CF-Ack"); break;
		case 2: printf("Data + CF-Poll"); break;
		case 3: printf("Data + CF-Ack + CF-Poll"); break;
		case 4: printf("Null (no data)"); break;
		case 5: printf("CF-Ack (no data)"); break;
		case 6: printf("CF-Poll (no data)"); break;
		case 7: printf("CF-Ack + CF-Poll (no data)"); break;
		case 8: printf("QoS Data"); break;
		case 9: printf("QoS Data + CF-Ack"); break;
		case 10: printf("QoS Data + CF-Poll"); break;
		case 11: printf("QoS Data + CF-Ack + CF-Poll"); break;
		case 12: printf("QoS Null (no data)"); break;
			 /* subtype 13 is reserved */
		case 14: printf("QoS CF-Poll (no data)"); break;
		case 15: printf("Qos CF-Ack + CF-Poll (no data)"); break;

		default: printf("RESERVED"); break;
	}

	printf("\n 802.11MAC: duration = %u us\n", data->duration);
	printf(" 802.11MAC: fragment no. = %u, sequence no. = %u\n",
			(data->seq_ctrl & 0x000F) ,
			(data->seq_ctrl & 0xFFF0) >> 4);

	hdrlen = sizeof(ieee80211_data_frame_3);
	
	if (! data->ctl.from_ds && ! data->ctl.to_ds) {
		printf(" 802.11MAC: DA      = %s\n", macaddr(data->addr1));
		printf(" 802.11MAC: SA      = %s\n", macaddr(data->addr2));
		printf(" 802.11MAC: BSSID   = %s\n", macaddr(data->addr3));
	} else if ( ! data->ctl.from_ds && data->ctl.to_ds) {
		printf(" 802.11MAC: DA      = %s\n", macaddr(data->addr3));
		printf(" 802.11MAC: SA      = %s\n", macaddr(data->addr2));
		printf(" 802.11MAC: BSSID   = %s\n", macaddr(data->addr1));
	} else if ( data->ctl.from_ds && ! data->ctl.to_ds) {
		printf(" 802.11MAC: DA      = %s\n", macaddr(data->addr1));
		printf(" 802.11MAC: SA      = %s\n", macaddr(data->addr3));
		printf(" 802.11MAC: BSSID   = %s\n", macaddr(data->addr2));
	} else {
		/* Check to make sure we have a four-address frame first */
		if (len < sizeof(ieee80211_data_frame)) {
			printf(" 802.11MAC: [Truncated]\n");
			return;
		}
		printf(" 802.11MAC: DA      = %s\n", macaddr(data->addr3));
		printf(" 802.11MAC: SA      = %s\n", macaddr(data->addr4));
		printf(" 802.11MAC: TA      = %s\n", macaddr(data->addr2));
		printf(" 802.11MAC: RA      = %s\n", macaddr(data->addr1));
		hdrlen = sizeof(ieee80211_data_frame); /* 4 addr header */
	}


	if (data->ctl.subtype >= 8) { 
		printf(" 802.11e: QoS = 0x%04x\n", qos->qos);
		if (len > sizeof(ieee80211_qos_data_frame)) 
			hdrlen = sizeof(ieee80211_qos_data_frame);
	}
	
	if (len > hdrlen) {
		int payload_offset = 0;
		uint16_t ethertype = 0;
		pld = (ieee80211_payload *) ((char *)pkt + hdrlen) ;
		if (ntohs(pld->ethertype) == 0xaaaa) {
			/* 802.11 payload contains an 802.2 LLC/SNAP header */
			libtrace_llcsnap_t *llcsnap = (libtrace_llcsnap_t *) pld;
			printf(" 802.2: DSAP = 0x%x, SSAP = 0x%x, OUI = 0x%x, Type = 0x%x\n", 
					llcsnap->dsap, llcsnap->ssap, llcsnap->oui, ntohs(llcsnap->type));
			payload_offset = sizeof(libtrace_llcsnap_t);
			ethertype = ntohs(llcsnap->type);
		} else {
			/* 802.11 payload contains an Ethernet II frame */
			printf(" 802.11MAC: Payload ethertype = 0x%04x\n", ntohs(pld->ethertype));
			payload_offset = sizeof(pld->ethertype);
			ethertype = ntohs(pld->ethertype);
		}
		decode_next((char *) pkt + hdrlen + payload_offset, 
				len - hdrlen - payload_offset, "eth", ethertype);
	}

	
}