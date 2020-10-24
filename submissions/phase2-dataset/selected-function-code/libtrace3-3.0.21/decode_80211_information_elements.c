static void decode_80211_information_elements(const char *pkt, unsigned len) {
	ieee80211_ie *ie;
	int i = 0;
	const uint8_t * data;
	uint8_t bmap_offset;
	while (len >= sizeof(ieee80211_ie)) {
		ie = (ieee80211_ie *) pkt;
		
		if ( len < ( sizeof(ieee80211_ie) + ie->length)) {
			printf("  [Truncated]\n");
			return;
		}
		
		data = (( const unsigned char *)pkt + sizeof (ieee80211_ie));
		
		switch (ie->id) {
			case 0:
				printf("  SSID = ");
				for (i = 0; i < ie->length; i++) 
					printf("%c", data[i]);
				printf("\n");
				break;
			case 1:
				printf("  Supported Rates (Kbit/s):\n   ");
				/* NB: the MSB of each field will be set
				 * if the rate it describes is part of the
				 * basic rate set, hence the AND */
				for (i = 0; i < ie->length; i++) {
					printf("%u, ", 
						( (data[i]&0x7F) * 500));

				}
				printf("%c%c\n", 0x8, 0x8);
				break;
			case 3:
				printf("  DSSS Channel = ");
				printf("%u\n", *data);
				break;
			case 5:
				printf("  Traffic Indication Message:\n");
				printf("   DTIM Count = %u, ", *data);
				data++;
				printf("DTIM Period = %u\n", *data);
				data++;
				printf("   Broadcast/Multicast waiting = %s\n", 
					(*data) & 0x01 ? "Yes\0" : "No\0");
				bmap_offset = ((*data) & 0xFE) >> 1;
				data++;
				if ((ie->length == 4) && ( *data == 0)) {
					printf("   No traffic waiting for stations\n");
					break;
				}
				
				printf("   Traffic waiting for AssocIDs: ");
				for (i = 0; i < (ie->length - 3); i++) {
					int j;
					for (j = 0; j < 8; j++) {
						if (data[i] & (1 << j)) {
							printf("%u ", (bmap_offset + i + 1) * 8 + j);
						}
					}
				}		
				printf("\n");
						
				break;
			case 7:
				printf("  802.11d Country Information:\n");
				printf("   ISO 3166 Country Code: %c%c\n", data[0], data[1]);
				printf("   Regulatory Operating Environment: ");
				if (data[2] == ' ') printf("Indoor/Outdoor\n");
				else if (data[2] == 'O') printf("Outdoor only\n");
				else if (data[2] == 'I') printf("Indoor only\n");
				else printf("Unknown, code = %c\n", data[2]);
				data += sizeof(uint8_t) * 3;
				for (i = 0; i < ((ie->length - 3) / 3); i++) {
					printf("   First Channel: %u, Num Channels: %u, Max Tx Power %idBm\n",
							data[0], data[1], (int8_t) data[2]);
					data += sizeof(uint8_t) * 3;
				}
				
				break;
			case 11:
				printf("  802.11e QBSS Load\n");
				break;
			case 12:
				printf("  802.11e EDCA Parameter\n");
				break;
			case 13:
				printf("  802.11e TSPEC\n");
				break;
			case 14:
				printf("  802.11e TCLAS\n");
				break;
			case 15:
				printf("  802.11e Schedule\n");
				break;
			case 16:
				printf("  Authentication Challenge Text\n");
				break;
			case 32:
				printf("  802.11h Power Contraint\n");
				printf("   Local Power Contraint = %udB\n", data[0]);
				break;
			case 33:
				printf("  802.11h Power Capability\n");
				printf("   Minimum Transmit Power Capability = %idBm\n", (int8_t)data[0]);
				printf("   Maximum Transmit Power Capability = %idBm\n", (int8_t)data[1]);
				break;
			case 34:
				printf("  802.11h Transmit Power Control Request\n");
				break;
			case 35:
				printf("  802.11h Transmit Power Control Report\n");
				printf("   Transmit Power = %idBm\n", (int8_t)data[0]);
				printf("   Link Margin = %idB\n", (int8_t)data[1]);
				break;
			case 36:
				printf("  802.11h Supported Channels\n");
				for(i = 0; i < (ie->length / 2); i++) {
					printf("   First Channel = %u, Num Channels = %u\n", data[0], data[1]);
					data += 2;
				}
				break;
			case 37:
				printf("  802.11h Channel Switch Announcement\n");
				printf("   New Channel Number = %u\n", data[1]);
				printf("   Target Beacon Transmission Times untill switch = %u\n", data[2]);
				if (data[0]) printf("   Don't transmit more frames until switch occurs\n");
				break;
			case 38:
				printf("  802.11h Measurement Request\n");
				break;
			case 39:
				printf("  802.11h Measurement Report\n");
				break;
			case 40:
				printf("  802.11h Quiet\n");
				break;
			case 41:
				printf("  802.11h IBSS DFS\n");
				break;
			case 42:
				printf("  802.11g ERP Information\n");
				if(data[0] & 0x80) printf("   NonERP STAs are present in this BSS\n");
				if(data[0] & 0x40) printf("   Use Protection Mechanism\n");
				if(data[0] & 0x20) printf("   Do not use short preamble\n");
				break;
			case 43:
				printf("  802.11e TS Delay\n");
				break;
			case 44:
				printf("  802.11e TCLAS Processing\n");
				break;
			case 46:
				printf("  802.11e QoS Capability\n");
				break;
			case 48:
				printf("  802.11i RSN:\n");
				break;
			case 50:
				printf("  802.11g Extended Supported Rates (Kbit/s)\n   ");
				for(i = 0; i < ie->length; i++) 
					printf("%u, ", data[i] * 500);
				printf("%c%c\n", (char) 8, (char) 8);		
				break;
				
			case 221:
				decode_80211_vendor_ie(ie);
				break;
			default:
				printf("  Unknown IE Element ID, 0x%02x\n", ie->id);
		}
		len -= sizeof(ieee80211_ie) + ie->length;
		pkt = ((char *)pkt + sizeof(ieee80211_ie) + ie->length);
	}
}