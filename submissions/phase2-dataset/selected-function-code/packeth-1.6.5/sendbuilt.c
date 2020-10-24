void* sendbuilt (void *parameters)
{
	/* YYY check if li,... are long enough if inifinite number will be sent. Maybe put them into double */
        long li, gap = 0, gap2 = 0, sentnumber = 0, lastnumber = 0, seconds = 0;
        struct timeval nowstr, first, last;
        int c, fd, odd=0, actualnumber;
	unsigned int mbps, pkts, link;
	unsigned long xc=0, yc=0;
        char buff[100];
	struct sockaddr_ll sa;
        struct ifreq ifr;
	guint32 ipcks, pseudo_header, udpcksum, tcpcksum;
	guint32 *stevec32;

        struct params* p = (struct params*) parameters;

        /* do we have the rights to do that? */
        if (getuid() && geteuid()) {
		//printf("Sorry but need the su rights!\n");
		gdk_threads_enter ();
			//gtk_widget_set_sensitive (p->button1, TRUE);
			//gtk_widget_set_sensitive (p->button2, TRUE);
			//gtk_widget_set_sensitive (p->button3, TRUE);
			//gtk_widget_set_sensitive (p->button4, TRUE);
			//gtk_widget_set_sensitive (p->button5, TRUE);
                	//gtk_widget_set_sensitive (p->stopbt, FALSE);
                	
			snprintf(buff, 100, "  Problems with sending");
                	gtk_statusbar_push(GTK_STATUSBAR(p->button), GPOINTER_TO_INT(p->context_id), buff);

                	error("Sorry but need the su rights!");
		gdk_flush();gdk_threads_leave ();
                return NULL;
        }

        /* open socket in raw mode */
        fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
        if (fd == -1) {
                //printf("Error: Could not open socket!\n");
		gdk_threads_enter ();
			//gtk_widget_set_sensitive (p->button1, TRUE);
			//gtk_widget_set_sensitive (p->button2, TRUE);
			//gtk_widget_set_sensitive (p->button3, TRUE);
			//gtk_widget_set_sensitive (p->button4, TRUE);
			//gtk_widget_set_sensitive (p->button5, TRUE);
                        //gtk_widget_set_sensitive (p->stopbt, FALSE);

                        snprintf(buff, 100, "  Problems with sending");
                        gtk_statusbar_push(GTK_STATUSBAR(p->button), GPOINTER_TO_INT(p->context_id), buff);

	                error("Error: Could not open socket!");
		gdk_flush();gdk_threads_leave ();
                return NULL;
        }

	/* which interface would you like to use? */
        memset(&ifr, 0, sizeof(ifr));
        strncpy (ifr.ifr_name, iftext, sizeof(ifr.ifr_name) - 1);
        ifr.ifr_name[sizeof(ifr.ifr_name)-1] = '\0';

	/* does the interface exists? */
        if (ioctl(fd, SIOCGIFINDEX, &ifr) == -1) {
                //printf("No such interface: %s\n", iftext);
		gdk_threads_enter ();
			//gtk_widget_set_sensitive (p->button1, TRUE);
			//gtk_widget_set_sensitive (p->button2, TRUE);
			//gtk_widget_set_sensitive (p->button3, TRUE);
			//gtk_widget_set_sensitive (p->button4, TRUE);
			//gtk_widget_set_sensitive (p->button5, TRUE);
                        //gtk_widget_set_sensitive (p->stopbt, FALSE);

                        snprintf(buff, 100, "  Problems with sending");
                        gtk_statusbar_push(GTK_STATUSBAR(p->button), GPOINTER_TO_INT(p->context_id), buff);

                	snprintf(buff, 100, "No such interface: %s", iftext);
                	error(buff);
		gdk_flush();gdk_threads_leave ();
                close(fd);
                return NULL;
        }

	/* is the interface up? */
        ioctl(fd, SIOCGIFFLAGS, &ifr);
	if ( (ifr.ifr_flags & IFF_UP) == 0) {
                //printf("Interface %s is down\n", iftext);
		gdk_threads_enter ();
			//gtk_widget_set_sensitive (p->button1, TRUE);
			//gtk_widget_set_sensitive (p->button2, TRUE);
			//gtk_widget_set_sensitive (p->button3, TRUE);
			//gtk_widget_set_sensitive (p->button4, TRUE);
			//gtk_widget_set_sensitive (p->button5, TRUE);
                        //gtk_widget_set_sensitive (p->stopbt, FALSE);

                        snprintf(buff, 100, "  Problems with sending");
                        gtk_statusbar_push(GTK_STATUSBAR(p->button), GPOINTER_TO_INT(p->context_id), buff);

                        snprintf(buff, 100, "Interface %s is down", iftext);
                	error(buff);
		gdk_flush();gdk_threads_leave ();
                close(fd);
                return NULL;
        }

	/* just write in the structure again */
        ioctl(fd, SIOCGIFINDEX, &ifr);

        /* well we need this to work, don't ask me what is it about */
        memset(&sa, 0, sizeof (sa));
        sa.sll_family    = AF_PACKET;
        sa.sll_ifindex   = ifr.ifr_ifindex;
        sa.sll_protocol  = htons(ETH_P_ALL);

        /* this is the time we started */
        gettimeofday(&first, NULL);
        gettimeofday(&last, NULL);
        gettimeofday(&nowstr, NULL);

        /* to send first packet immedialtelly */
        gap = p->del;

	/* if packet is shorter than 60 bytes, we need real packet length for calculating checksum,
	 * we use actualnumber for this */
	actualnumber = number;
	if (number < 60)
		number = 60;


	gtk_widget_set_sensitive (p->button1, FALSE);
	gtk_widget_set_sensitive (p->button2, FALSE);
	gtk_widget_set_sensitive (p->button3, FALSE);
	gtk_widget_set_sensitive (p->button4, FALSE);
	gtk_widget_set_sensitive (p->button5, FALSE);
	gtk_widget_set_sensitive (p->button6, FALSE);
//	gtk_widget_set_sensitive (p->stopbt, TRUE);

	/* we check with == -3 if the infinite option was choosed */
        for(li = 0; p->count == -3 ? : li < p->count; li++) {
                while (gap < p->del) {
                        gettimeofday(&nowstr, NULL);

                        if (p->timeflag == 1)
                                gap = (nowstr.tv_sec*1000000 + nowstr.tv_usec) -
                                                (last.tv_sec*1000000 + last.tv_usec);
                        else
                                gap = (nowstr.tv_sec*1000000 + nowstr.tv_usec) -
                                                (first.tv_sec*1000000 + first.tv_usec) - ( (li - 1) * p->del);

                        gap2 = nowstr.tv_sec - first.tv_sec;

                        /* if the flag is set - the user clicked the stop button, we quit */
                        if (stop_flag == 1) {
                                gdk_threads_enter ();

				gtk_widget_set_sensitive (p->button1, TRUE);
				gtk_widget_set_sensitive (p->button2, TRUE);
				gtk_widget_set_sensitive (p->button3, TRUE);
				gtk_widget_set_sensitive (p->button4, TRUE);
				gtk_widget_set_sensitive (p->button5, TRUE);
				gtk_widget_set_sensitive (p->button6, TRUE);
                        //        gtk_widget_set_sensitive (p->stopbt, FALSE);

                                snprintf(buff, 100, "  Sent %ld packets on %s", sentnumber, iftext);
                                gtk_statusbar_push(GTK_STATUSBAR(p->button),
                                                                GPOINTER_TO_INT(p->context_id), buff);

                                gdk_flush();gdk_threads_leave ();
				close(fd);
                                return NULL;
                        }
                }

		c = sendto(fd, packet, number, 0, (struct sockaddr *)&sa, sizeof (sa));

		//printf("There were %d bytes sent on the wire (in case of an error we get -1)\n", c);

                last.tv_sec = nowstr.tv_sec;
                last.tv_usec = nowstr.tv_usec;
                gap = 0;

                if (c > 0)
                        sentnumber++;
//		else
//			break;

		 /* update the status bar every second and display number of sent packets */
                if (gap2 > seconds) {
                        gdk_threads_enter ();
			
			pkts = sentnumber - lastnumber;
			mbps = pkts * number / 128; // 8 bits per byte / 1024 for kbit
			/* +12 bytes for interframe gap time and 12 for preamble, sfd and checksum */
                        link = pkts * (number + 24) / 128;
			lastnumber = sentnumber;

			snprintf(buff, 100, "  Sent %ld packets on %s (%d packets/s, %d kbit/s data rate, %d kbit/s link utilization)", sentnumber, iftext, pkts, mbps, link);
                        gtk_statusbar_push(GTK_STATUSBAR(p->button), GPOINTER_TO_INT(p->context_id), buff);

                        gdk_flush();gdk_threads_leave ();

                        seconds++;
                }

                /* do we need to change any fields */
		switch (p->inc) {
			/* changing source MAC address */
			case 1: {
				/*packet[6] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));*/
				packet[7] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));
				packet[8] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));
				packet[9] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));
				packet[10] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));
				packet[11] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));
				break;
			}
			/* change source IP address - make it random 
			 * and correct IP checksum */
			case 2: {
				packet[p->ipstart] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));
				packet[p->ipstart+1] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));
				packet[p->ipstart+2] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));
				packet[p->ipstart+3] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));
				/* we have to correct the ip checksum 
				 * first we set 0x00 in both fields and then recalculate it */
        		        packet[p->ipstart-2] = 0x00;
                		packet[p->ipstart-1] = 0x00;
				ipcks = ((-1) - get_checksum16(p->ipstart-12, p->ipstart+7) % 0x10000);
        		        packet[p->ipstart-2] = (char)(ipcks/256);
                		packet[p->ipstart-1] =  (char)(ipcks%256);
				break;
			}
			/* source MAC address and source IP address 
			 * and correct IP checksum */
			case 3: {
				/*packet[6] = 1+(int) (16.0*rand()/(RAND_MAX+1.0));*/
				packet[7] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));
				packet[8] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));
				packet[9] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));
				packet[10] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));
				packet[11] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));
				packet[p->ipstart] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));
				packet[p->ipstart+1] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));
				packet[p->ipstart+2] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));
				packet[p->ipstart+3] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));
				/* we have to correct the ip checksum 
				 * first we set 0x00 in both fields and then recalculate it */
        		        packet[p->ipstart-2] = 0x00;
                		packet[p->ipstart-1] = 0x00;
				ipcks = ((-1) - get_checksum16(p->ipstart-12, p->ipstart+7) % 0x10000);
        		        packet[p->ipstart-2] = (char)(ipcks/256);
                		packet[p->ipstart-1] =  (char)(ipcks%256);
				break;
			}
			/* for arp reply messages, change source MAC (ethernet part) *
			 * sender MAC and sender IP (arp part) */
			case 4: {
				//packet[p->ethstart] = 1+(int) (16.0*rand()/(RAND_MAX+1.0));
				packet[p->ethstart+1] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));
				packet[p->ethstart+2] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));
				packet[p->ethstart+3] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));
				packet[p->ethstart+4] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));
				packet[p->ethstart+5] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));
				packet[p->ethstart+6] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));
				packet[p->ethstart+7] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));
				packet[p->ethstart+8] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));
				packet[p->ethstart+9] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));
				//packet[6] = packet[p->ethstart];
				packet[7] = packet[p->ethstart+1];
				packet[8] = packet[p->ethstart+2];
				packet[9] = packet[p->ethstart+3];
				packet[10] = packet[p->ethstart+4];
				packet[11] = packet[p->ethstart+5];
				break;
				
			}
			/* set random source TCP port and IP source address (syn flood) */ 
			case 5: {
				packet[p->ipstart] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));
				packet[p->ipstart+1] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));
				packet[p->ipstart+2] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));
				packet[p->ipstart+3] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));
				packet[p->tcpstart] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));
				packet[p->tcpstart+1] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));
				/* we have to correct the ip checksum 
				 * first we set 0x00 in both fields and then recalculate it */
        		        packet[p->ipstart-2] = 0x00;
                		packet[p->ipstart-1] = 0x00;
				ipcks = ((-1) - get_checksum16(p->ipstart-12, p->ipstart+7) % 0x10000);
        		        packet[p->ipstart-2] = (char)(ipcks/256);
                		packet[p->ipstart-1] =  (char)(ipcks%256);
				break;
			}
			/* increase the udp first payload byte value by one */
			case 6:  {
				packet[p->udpstart]++;
				break;
			}
			/* changing RTP values: seq number++, timestamp for 10ms */
			case 7: {
				packet[p->udpstart+2] = (li+1)/256;
				packet[p->udpstart+3] = (li+1)%256;
				packet[p->udpstart+4] = ((li+1)*80)/16777216;
				packet[p->udpstart+5] = ((li+1)*80)/65536;
				packet[p->udpstart+6] = ((li+1)*80)/256;
				packet[p->udpstart+7] = (signed int)(((li+1)*80)%256);
				break;
			}
			/* changing RTP values: seq number++, timestamp for 20ms */
			case 8: {
				packet[p->udpstart+2] = (li+1)/256;
				packet[p->udpstart+3] = (li+1)%256;
				packet[p->udpstart+4] = ((li+1)*160)/16777216;
				packet[p->udpstart+5] = ((li+1)*160)/65536;
				packet[p->udpstart+6] = ((li+1)*160)/256;
				packet[p->udpstart+7] = (signed int)(((li+1)*160)%256);
				break;
			}
			/* changing RTP values: seq number++, timestamp for 30ms */
			case 9: {
				packet[p->udpstart+2] = (li+1)/256;
				packet[p->udpstart+3] = (li+1)%256;
				packet[p->udpstart+4] = ((li+1)*240)/16777216;
				packet[p->udpstart+5] = ((li+1)*240)/65536;
				packet[p->udpstart+6] = ((li+1)*240)/256;
				packet[p->udpstart+7] = (signed int)(((li+1)*240)%256);
				break;
			}
			/* changing byte x value */
			case 10: {
				/* increment it within specified range */
				if (p->xchange == 1) {
					if (xc < (p->xrange)) {
						stevec32 = (guint32*) &packet[p->xbyte-1];
						(*stevec32)++;
						xc++;
					}
					else	{
						memcpy(&packet[p->xbyte-1], p->xstart, 4);
						xc=0;
					}
				}
				/* decrement it within specified range */
				else if (p->xchange == 2) {
					if (xc < (p->xrange)) {
						stevec32 = (guint32*) &packet[p->xbyte-1];
						(*stevec32)--;
						xc++;
					}
					else	{
						memcpy(&packet[p->xbyte-1], p->xstart, 4);
						xc=0;
					}
				}
				/* set it random */
				else if (p->xchange == 0)
					packet[p->xbyte-1] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));

				else if (p->xchange == 3) {
					packet[p->xbyte-1] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));
					packet[p->xbyte-0] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));
				}	
					
				else if (p->xchange == 4) {
					packet[p->xbyte-1] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));
					packet[p->xbyte] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));
					packet[p->xbyte+1] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));
				}	
					
				else if (p->xchange == 5) {
					packet[p->xbyte-1] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));
					packet[p->xbyte] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));
					packet[p->xbyte+1] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));
					packet[p->xbyte+2] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));
				}	
					
				if(p->type >1) {
					packet[p->ipstart-2] = 0x00;
					packet[p->ipstart-1] = 0x00;
					ipcks = ((-1) - get_checksum16(p->ipstart-12, p->ipstart+7) % 0x10000);
					packet[p->ipstart-2] = (char)(ipcks/256);
					packet[p->ipstart-1] =  (char)(ipcks%256);
				}

				break;
			}
			/* change byte x and y */
			case 11: {
				/* byte x, increment */
				if (p->xchange == 1) {
					if (xc < (p->xrange)) {
						stevec32 = (guint32*) &packet[p->xbyte-1];
						(*stevec32)++;
						xc++;
					}
					else	{
						memcpy(&packet[p->xbyte-1], p->xstart, 4);
						xc=0;
					}
				}
				/* decrement it within specified range */
				else if (p->xchange == 2) {
					if (xc < (p->xrange)) {
						stevec32 = (guint32*) &packet[p->xbyte-1];
						(*stevec32)--;
						xc++;
					}
					else	{
						memcpy(&packet[p->xbyte-1], p->xstart, 4);
						xc=0;
					}
				}
				/* set it random */
				else if (p->xchange == 0)
					packet[p->xbyte-1] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));

				else if (p->xchange == 3) {
					packet[p->xbyte-1] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));
					packet[p->xbyte-0] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));
				}	
					
				else if (p->xchange == 4) {
					packet[p->xbyte-1] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));
					packet[p->xbyte] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));
					packet[p->xbyte+1] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));
				}	
					
				else if (p->xchange == 5) {
					packet[p->xbyte-1] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));
					packet[p->xbyte] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));
					packet[p->xbyte+1] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));
					packet[p->xbyte+2] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));
				}	

				/* byte y increment */
				if (p->ychange == 1) {
					if (yc < (p->yrange)) {
						stevec32 = (guint32*) &packet[p->ybyte-1];
						(*stevec32)++;
						yc++;
					}
					else	{
						memcpy(&packet[p->ybyte-1], p->ystart, 4);
						yc=0;
					}
				}
				/* decrement it within specified range */
				else if (p->ychange == 2) {
					if (yc < (p->yrange)) {
						stevec32 = (guint32*) &packet[p->ybyte-1];
						(*stevec32)--;
						yc++;
					}
					else	{
						memcpy(&packet[p->ybyte-1], p->ystart, 4);
						yc=0;
					}
				}
				/* set it random */
				else if (p->ychange == 0)
					packet[p->ybyte-1] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));

				else if (p->ychange == 3) {
					packet[p->ybyte-1] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));
					packet[p->ybyte-0] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));
				}	
					
				else if (p->ychange == 4) {
					packet[p->ybyte-1] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));
					packet[p->ybyte] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));
					packet[p->ybyte+1] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));
				}	
					
				else if (p->ychange == 5) {
					packet[p->ybyte-1] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));
					packet[p->ybyte] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));
					packet[p->ybyte+1] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));
					packet[p->ybyte+2] = 1+(int) (255.0*rand()/(RAND_MAX+1.0));
				}	

				if(p->type >1) {
					packet[p->ipstart-2] = 0x00;
					packet[p->ipstart-1] = 0x00;
					ipcks = ((-1) - get_checksum16(p->ipstart-12, p->ipstart+7) % 0x10000);
					packet[p->ipstart-2] = (char)(ipcks/256);
					packet[p->ipstart-1] =  (char)(ipcks%256);
				}

				break;
			}
		}

		/* correct the UDP checksum value???
		 * we do it (udp checksum in not mandatory (if set to 0x00)) */
		if (p->type == 4) {
			packet[p->udpstart-2] = (char)(0);
			packet[p->udpstart-1] =  (char)(0);
			pseudo_header = (guint32)(packet[p->ipstart-3]);
			pseudo_header = pseudo_header + get_checksum32(p->ipstart,p->ipstart+7);
			udpcksum = (guint32)(actualnumber - p->udpstart +8);

			/* pseudo header (ip part) + udplength + nr of cicles over guint16 */
			udpcksum = pseudo_header + udpcksum;

			/* what if length is odd */
			if( (actualnumber - p->udpstart)%2 != 0) 
		                odd = 1;
			/* previos value + part from udp checksum */
			udpcksum = udpcksum + get_checksum32(p->udpstart-8, actualnumber+odd);
			while (udpcksum >> 16)
				udpcksum = (udpcksum & 0xFFFF)+(udpcksum >> 16);
	
				/* the one's complement */
			udpcksum = (-1) - udpcksum;
			
			/* let's write it */
			packet[p->udpstart-2] = (char)(udpcksum/256);
			packet[p->udpstart-1] =  (char)(udpcksum%256);
		}
		/* or tcp checksum*/
		else if (p->type == 3) {
			packet[p->tcpstart+16] = (char)(0);
			packet[p->tcpstart+17] =  (char)(0);
			pseudo_header = (guint32)(packet[p->ipstart-3]);
			pseudo_header = pseudo_header + get_checksum32(p->ipstart,p->ipstart+7);
			tcpcksum = (guint32)(actualnumber - p->tcpstart);
			/* pseudo header (ip part) + tcplength + nr of cicles over guint16 */
			tcpcksum = pseudo_header + tcpcksum;
			/* what if length is odd */
			if( (actualnumber - p->tcpstart)%2 != 0) 
		                odd = 1;
			/* previos value + part from tcp checksum */
			tcpcksum = tcpcksum + get_checksum32(p->tcpstart, actualnumber+odd);
			while (tcpcksum >> 16)
				tcpcksum = (tcpcksum & 0xFFFF) + (tcpcksum >> 16);
			/* the one's complement */
			tcpcksum = (-1) - tcpcksum;
			/* let's write it */
			packet[p->tcpstart+16] = (char)(tcpcksum/256);
			packet[p->tcpstart+17] =  (char)(tcpcksum%256);
		}
        }

        /* we sent all the packets, let's activate the toolbar */
        gdk_threads_enter ();

	gtk_widget_set_sensitive (p->button1, TRUE);
	gtk_widget_set_sensitive (p->button2, TRUE);
	gtk_widget_set_sensitive (p->button3, TRUE);
	gtk_widget_set_sensitive (p->button4, TRUE);
	gtk_widget_set_sensitive (p->button5, TRUE);
	gtk_widget_set_sensitive (p->button6, TRUE);
//        gtk_widget_set_sensitive (p->stopbt, FALSE);

        snprintf(buff, 100, "  Sent %ld packets on %s", sentnumber, iftext);
        gtk_statusbar_push(GTK_STATUSBAR(p->button), GPOINTER_TO_INT(p->context_id), buff);

	if (close(fd) != 0) {
		//printf("Warning! close(fd) returned -1!\n");
		error("Warning! close(fd) returned -1!");
	}

        gdk_flush();gdk_threads_leave ();

        return NULL;

}