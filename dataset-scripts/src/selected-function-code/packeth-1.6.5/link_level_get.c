int link_level_get(GtkButton *button, gpointer user_data)
{
	GtkWidget *ver2_tbt, *_801q_cbt, *_8023_tbt;
	GtkWidget *ethtype_e;
	gchar *ethtype_t;

	ver2_tbt = lookup_widget(GTK_WIDGET (button), "bt_ver2");
	_8023_tbt = lookup_widget(GTK_WIDGET (button), "bt_8023");
	_801q_cbt = lookup_widget(GTK_WIDGET (button), "bt_8021q");

	/* always we need first the dest and source mac address */
	if (get_mac_from_string(button) == -1) {
		//printf("Error: mac address field\n");
		error("Error: mac address field");
		return -1;
	}
        number = 12;

	/* is 802.1q active - do we need to add 4 or 8 bytes? */
	if (GTK_TOGGLE_BUTTON(_801q_cbt)->active) {
		if (get_8021q(button) == -1) {
			//printf("Error: 802.1q field\n");
			return -1;
		}
	}
	if (GTK_TOGGLE_BUTTON(_8023_tbt)->active) { /* uporabimo ethernet vezije 802.3 */
		if (get_8023(button) == -1) {
			//printf("Error: 802.3 field");
			return -1;
		}
	}

	else if (GTK_TOGGLE_BUTTON(ver2_tbt)->active){ /* pol pa verzijo 2 */ 
		autolength = 0;
		ethtype_e = lookup_widget(GTK_WIDGET (button), "L_ethtype");
		ethtype_t = (char *)gtk_entry_get_text(GTK_ENTRY(ethtype_e));
		if (char2x(ethtype_t) == -1) {
			//printf("Error: ethernet type field\n");
			error("Error: ethernet type field");
	                return -1;
		}
		packet[number] = (unsigned char)char2x(ethtype_t);
		ethtype_t++; ethtype_t++; number++;
		if (char2x(ethtype_t) == -1) {
			//printf("Error: ethernet type field\n");
			error("Error: ethernet type field");
	                return -1;
		}
		packet[number] = (unsigned char)char2x(ethtype_t);
		number++;
	}
	else {/* kva a je mogoce token ring??? */
		//printf("Error: in ethernet field\n");
		error("Error: in ethernet field");
		return -1;
	}


	return 1;
}