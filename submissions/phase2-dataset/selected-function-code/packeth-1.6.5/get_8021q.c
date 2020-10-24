int get_8021q(GtkButton *button)
{
	GtkWidget *vlan_e, *priority_m, *cfi1_rbt, *vlanid_e, *menu, *menu_item, *QinQ_bt, *QinQ, *QinQpvid;
	gchar *vlan_t, *vlanid_t, *QinQ_t; 
	gint menu_index, cfi =0;
	char tmp[2];

	QinQ_bt = lookup_widget(GTK_WIDGET (button), "checkbutton40");

	/* what about QinQ field? */
	if (GTK_TOGGLE_BUTTON(QinQ_bt)->active) {
		QinQpvid = lookup_widget(GTK_WIDGET (button), "optionmenu21");
		QinQ = lookup_widget(GTK_WIDGET (button), "entry165");
                
		menu = GTK_OPTION_MENU(QinQpvid)->menu;
        	menu_item = gtk_menu_get_active (GTK_MENU (menu));
        	menu_index = g_list_index (GTK_MENU_SHELL (menu)->children, menu_item);

		switch (menu_index) {
			case 0: {
				packet[number] = (unsigned char)char2x("81");
				number++;
                		packet[number] = (unsigned char)char2x("00");
				number++;
				break;
			}
			case 1: {
				packet[number] = (unsigned char)char2x("91");
				number++;
                		packet[number] = (unsigned char)char2x("00");
				number++;
				break;
			}
			case 2: {
				packet[number] = (unsigned char)char2x("92");
				number++;
                		packet[number] = (unsigned char)char2x("00");
				number++;
				break;
			}
			case 3: {
				packet[number] = (unsigned char)char2x("88");
				number++;
                		packet[number] = (unsigned char)char2x("a8");
				number++;
				break;
			}
		}

		QinQ_t = (char *)gtk_entry_get_text(GTK_ENTRY(QinQ));
                if (char2x(QinQ_t) == -1) {
                        //printf("Error: VLAN QinQ type field\n");
                        error("Error: VLAN QinQ field");
                        return -1;
                }
                packet[number] = (unsigned char)char2x(QinQ_t);
                QinQ_t++; QinQ_t++; number++;
                if (char2x(QinQ_t) == -1) {
                        //printf("Error: VLAN QinQ type field\n");
                        error("Error: VLAN QinQ field");
                        return -1;
                }
                packet[number] = (unsigned char)char2x(QinQ_t);
                number++;

	}

	vlan_e = lookup_widget(GTK_WIDGET (button), "L_tag_id");
	priority_m = lookup_widget(GTK_WIDGET (button), "L_optmenu2_bt");
	cfi1_rbt = lookup_widget(GTK_WIDGET (button), "checkbutton39");
	vlanid_e = lookup_widget(GTK_WIDGET (button), "L_vlan_id");
	vlan_t = (char *)gtk_entry_get_text(GTK_ENTRY(vlan_e));
	vlanid_t = (char *)gtk_entry_get_text(GTK_ENTRY(vlanid_e));
		
	/* first we chech the vlan protocol id */
	if (char2x(vlan_t) == -1) {
		//printf("Error: 802.1q type field\n");
		error("Error: 802.1q type field");
                return -1;
	}
	packet[number] = (unsigned char)char2x(vlan_t);
	vlan_t++; vlan_t++; number++;
	if (char2x(vlan_t) == -1) {
		//printf("Error: 802.1q type field\n");
		error("Error: 802.1q type field");
                return -1;
	}
	packet[number] = (unsigned char)char2x(vlan_t);
	number++;	
	
	/* next we need the priority */
	menu = GTK_OPTION_MENU(priority_m)->menu;
        menu_item = gtk_menu_get_active (GTK_MENU (menu));
        menu_index = g_list_index (GTK_MENU_SHELL (menu)->children, menu_item);

	/* what about CFI bit? */
	if (GTK_TOGGLE_BUTTON(cfi1_rbt)->active)
		cfi = 1;
	else 
		cfi = 0;

	/* in cfi we store the value of priority and cfi */
	tmp[0] = (unsigned char)(menu_index * 2 + cfi);
	snprintf(&(tmp[0]), 2, "%x", tmp[0]);

	/* we need the vlan id */
	tmp[1] = *vlanid_t;

	if (char2x(tmp) == -1) {
		//printf("Error: 802.1q: priority & cfi field & 1 byte vlan id\n");
		error("Error: 802.1q: priority & cfi field & 1 byte vlan id");
                return -1;
	}
	packet[number] = (unsigned char)char2x(tmp);

	vlanid_t++; number++;

	if (char2x(vlanid_t) == -1) {
		//printf("Error: 802.1q vlanid \n");
		error("Error: 802.1q vlanid ");
                return -1;
	}
	packet[number] = (unsigned char)char2x(vlanid_t);
	number++;

	return 1;
}