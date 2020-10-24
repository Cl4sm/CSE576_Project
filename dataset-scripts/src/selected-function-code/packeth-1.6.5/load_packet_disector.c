int load_packet_disector(GtkButton *button, char *fieldek, int whocalled, struct clist_hdr *clptr, int dolpaketa) {

	int c;

	ptrf = fieldek;
	ptrt = temp;

	//printf("\n:\n%s\n", fieldek);
	//convert8field(ptrt, ptrf);    insint(button, "entry179", ptrt, 8); ptrt = temp; ptrf = ptrf-8;
	//convert8field(ptrt, ptrf+8);  insint(button, "entry180", ptrt, 8); ptrt = temp; ptrf = ptrf-8;
	//convert8field(ptrt, ptrf+16); insint(button, "entry181", ptrt, 8); ptrt = temp; ptrf = ptrf-8;
	//convert8field(ptrt, ptrf+24); insint(button, "entry182", ptrt, 8); ptrt = temp; ptrf = ptrf-8;
	remain = dolpaketa;
	ptrf = ptrf + 32;

	/* what is the shortest length we still allow?
	 * we don't care if the packet is shorter than actually allowed to go on ethernet
	 * maybe the user just wanted to save packet even if it is to short, so why not load it?
	 * what we do demand is, that every layer must be completed 
	 * ok, here at least 14 bytes: 6 dest mac, 6 source mac and 2 for type or length*/
	if (remain < 14) {
		error("Can't load packet: Ethernet header is not long enough!");
		return -1;
	}

	/* first there is destination mac */
	w1 = lookup_widget(GTK_WIDGET(button), "L_dst_mac");
	for (i=1; i<=18; i++, ptrt++) {
		if (i%3 == 0) 
			*ptrt = ':';
		else {
			*ptrt = *ptrf++;
		}
	}
	*(ptrt-1) = '\0';

	if (whocalled == 1)
		gtk_entry_set_text(GTK_ENTRY(w1), temp);
	else 
		memcpy(clptr->dst, temp, 20);

	/* and source mac */
	ptrt = temp;
	w2 = lookup_widget(GTK_WIDGET(button), "L_src_mac");
	for (i=1; i<=18; i++, ptrt++) {
		if (i%3 == 0) 
			*ptrt = ':';
		else {
			*ptrt = *ptrf++;
		}
	}
	*(ptrt-1) = '\0';

	if (whocalled == 1)
		gtk_entry_set_text(GTK_ENTRY(w2), temp);
	else 
		memcpy(clptr->src, temp, 20);

	/* next there is type or length field or 802.1q or QinQ! */
	i = char2x(ptrf)*256 + char2x(ptrf+2);

	remain = remain - 14;

	/* in case of a vlan tag 0x8100 == 33024) */
	w1 = lookup_widget(GTK_WIDGET(button), "bt_8021q");
	w2 = lookup_widget(GTK_WIDGET(button), "frame6");

	if ((i == 33024) || (i==34984) || (i==37120) || (i==37376)) {
		w3 = lookup_widget(GTK_WIDGET(button), "L_optmenu2_bt");
		w4 = lookup_widget(GTK_WIDGET(button), "checkbutton39");
		w5 = lookup_widget(GTK_WIDGET(button), "checkbutton40");
		w6 = lookup_widget(GTK_WIDGET(button), "L_vlan_id");
		w7 = lookup_widget(GTK_WIDGET(button), "entry165");
		w8 = lookup_widget(GTK_WIDGET(button), "optionmenu21");
		
	        //if we have 8100 after the next 4 bytes we do QinQ	
		if ( ((char2x(ptrf+8)*256 + char2x(ptrf+10))==33024) && remain>=8) 
		{
			ptrf = ptrf + 4;
			
			if (whocalled == 1) {
				gtk_widget_set_sensitive (w7, TRUE);
				gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w5), TRUE);
				if (i==33024)
					gtk_option_menu_set_history (GTK_OPTION_MENU (w8), 0);
				else if (i==34984)
					gtk_option_menu_set_history (GTK_OPTION_MENU (w8), 3);
				else if (i==37120)
					gtk_option_menu_set_history (GTK_OPTION_MENU (w8), 1);
				else if (i==37376)
					gtk_option_menu_set_history (GTK_OPTION_MENU (w8), 2);

				inspar(button, "entry165", ptrf, 4);
			}
			else
				ptrf = ptrf +3;

			i = char2x(ptrf)*256 + char2x(ptrf+2);
		}	
		else {
			if (whocalled == 1) {
				gtk_widget_set_sensitive (w7, FALSE);
				gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w5), FALSE);
			}	

		}

		if (remain < 4) {
			error("Can't load packet: Ethernet VLAN field is not long enough!");
			return -1;
		}
		remain = remain -4;

		ptrf = ptrf + 4;
		*ptrt++ = '0';
		*ptrt-- = *ptrf++;
		i = char2x(ptrt);

		if (whocalled == 1) {
			gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w1), TRUE);
			gtk_widget_set_sensitive (w2, TRUE);
		
			gtk_option_menu_set_history (GTK_OPTION_MENU (w3), (i>>1));

			if ( (i%2) == 0)
				gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w4), FALSE);
			else
				gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w4), TRUE);

			inspar(button, "L_vlan_id", ptrf, 3);
		}
		else
			ptrf = ptrf+3;

		i = char2x(ptrf)*256 + char2x(ptrf+2);

	}
	else {
		if (whocalled == 1) {
			gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w1), FALSE);

			gtk_widget_set_sensitive (w2, FALSE);
		}	
	}
	
	/* c will tell us which ethernet type we have */
	c = i;

	/* ok, from now one, we split the dissection in different routines, depending on what values */
	/* now if length is <= 1500, we have 802.3 ethernet and this value means length of ethernet packet */
	if (i <= 1500) 
		next_prot = ethernet_8023(button, whocalled);
	/* Values between 1500 and 1536 are forbidden */
	else if ( (i>1500) && (i<1536) ) {
		error("Can't load packet: Wrong ethernet length/type field");
		return -1;
	}
	/* if i >= 1536 - ethernet ver II */
	else
		next_prot = ethernet_verII(button, whocalled);


	/* ok, so we have dissected the ethernet layer and now move on two the next layer.
	 * if the ethernet dissector returns -1, this means an error and we quit
	 * otherwise, the return value can be 2048 == 0x0800 and this means the ipv4
	 * so we try to dissect ipv4 header. in case it is ok, we activate the ippkt_radibt
	 * and this one then calls the callback which fills in ethernet ver II type field
	 * and PID field in 802.3 LLC/SNAP field. It is the same for arp packets
	 * for other packets we will try to open the userdefined window */

	/* we got an error? */
	if (next_prot == -1) 
		return -1;

	/* ipv4 */
	else if (next_prot == 2048) {
		/* ok, ipv4 should follow, so we call the routine for parsing ipv4 header. */
		next_prot = ipv4_header(button, whocalled, clptr);
		if (next_prot == -1)
			return -1;

		/* if the return value from parsing ipv4 header was != 0, then the header parameters 
		 * are ok and we can open ipv4 notebook page activate toggle button (button calls 
		 * the callback then!!! */
		w1 = lookup_widget(GTK_WIDGET(button), "ippkt_radibt");
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w1), TRUE);

		/* here we do the further parsing: tcp, udp, icmp, ...*/
		if (next_prot == 1) {
			/* try to parse icmp header */
			next_prot = icmp_header(button, whocalled);
			/* not ok, return an error */
			if (next_prot == -1)
				return -1;
			/* ok, lets activate the icmp notebook */
			else {
				w1 = lookup_widget(GTK_WIDGET(button), "icmp_bt");
				gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w1), TRUE);
			}
		}
		else if (next_prot == 2) {
			/* try to parse igmp header */
			next_prot = igmp_header(button, whocalled);
			/* not ok, return an error */
			if (next_prot == -1)
				return -1;
			/* ok, lets activate the igmp notebook */
			else {
				w1 = lookup_widget(GTK_WIDGET(button), "igmp_bt");
				gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w1), TRUE);
			}
		}
		else if (next_prot == 6) {
			/* try to parse tcp header */
			next_prot = tcp_header(button, whocalled);
			/* not ok, return an error */
			if (next_prot == -1)
				return -1;
			/* ok, lets activate the tcp notebook */
			else {
				w1 = lookup_widget(GTK_WIDGET(button), "tcp_bt");
				gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w1), TRUE);
			}

			/* protocols on top of tcp would follow here */

		}	
		else if (next_prot == 17) {
			/* try to parse udp header */
			next_prot = udp_header(button, whocalled);
			/* not ok, return an error */
			if (next_prot == -1)
				return -1;
			/* ok, lets activate the udp notebook */
			else {
				w1 = lookup_widget(GTK_WIDGET(button), "udp_bt");
				gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w1), TRUE);
			}

			/* protocols on top of udp would follow here */
		}	
		/* protocol we do not support yet; user defined window */
		else {
			next_prot = usedef_insert(button, "text2", whocalled);
			w1 = lookup_widget(GTK_WIDGET(button), "ip_user_data_bt");
			gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w1), TRUE);
		}
	}
	/*arp */
	else if (next_prot == 2054) {
		/* ok, arp header follows */
		next_prot = arp_header(button, whocalled);
		if (next_prot == -1)
			return -1;

		w1 = lookup_widget(GTK_WIDGET(button), "arppkt_radiobt");
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w1), TRUE);
	}

	/* when ipv6 will be added, activate ipv6 button instead of userdef button */
	else if (next_prot == 34525) {

		/* ok, ipv6 should follow, so we call the routine for parsing ipv6 header. */
                next_prot = ipv6_header(button, whocalled, clptr);
                if (next_prot == -1)
                        return -1;

		w1 = lookup_widget(GTK_WIDGET(button), "IPv6_rdbt");
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w1), TRUE);

                /* here we do the further parsing: tcp, udp, icmp, ...*/
                if (next_prot == 1) {
                        /* try to parse icmpv6 header */
                        next_prot = icmpv6_header(button, whocalled);
                        /* not ok, return an error */
                        if (next_prot == -1)
                                return -1;
                        /* ok, lets activate the icmpv6 notebook */
                        else {
                                w1 = lookup_widget(GTK_WIDGET(button), "radiobutton69");
                                gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w1), TRUE);
                        }
                }
                else if (next_prot == 6) {
                        /* try to parse tcp header */
                        next_prot = tcp_header(button, whocalled);
                        /* not ok, return an error */
                        if (next_prot == -1)
                                return -1;
                        /* ok, lets activate the tcp notebook */
                        else {
                                w1 = lookup_widget(GTK_WIDGET(button), "radiobutton68");
                                gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w1), TRUE);
                        }
		}
		else if (next_prot == 17) {
                        /* try to parse udp header */
                        next_prot = udp_header(button, whocalled);
                        /* not ok, return an error */
                        if (next_prot == -1)
                                return -1;
                        /* ok, lets activate the udp notebook */
                        else {
                                w1 = lookup_widget(GTK_WIDGET(button), "radiobutton67");
                                gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w1), TRUE);
                        }
                }
                /* protocol we do not support yet; user defined window */
                else {
                        next_prot = usedef_insert(button, "text2", whocalled);
                        w1 = lookup_widget(GTK_WIDGET(button), "radiobutton71");
                        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w1), TRUE);
                }

	}
	/* anything else - user defined */
	else {
		/* setting "usedef2_radibt" toggle button to true will call the callback which will clear 
		eth II type field and 802.3 pid field, so we have to fill this later */
		w1 = lookup_widget(GTK_WIDGET(button), "usedef2_radibt");
		gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w1), TRUE);
		
		/* we still have c to distinguish between ver II and 802.3 */
		/* ver II */
		if (c >= 1536) {
			ptrf = ptrf - 4;
   			inspar(button, "L_ethtype", ptrf, 4);
		}
		/* 802.3 and with LLC SNAP */
		else if (next_prot != -2) {
			ptrf = ptrf - 4;
			inspar(button, "L_pid", ptrf, 4);
		}		

		next_prot = usedef_insert(button, "text1", whocalled);
	}

	return 1;
}