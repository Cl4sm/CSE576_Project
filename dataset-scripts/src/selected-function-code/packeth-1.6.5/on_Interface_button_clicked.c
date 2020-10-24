void
on_Interface_button_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *combo;
	GList *glist=NULL;

	struct ifconf       Ifc;
	struct ifreq        IfcBuf[512];
	struct ifreq        *pIfr;
	int num_ifreq;
	int i, fd, length;
	char buff[100];
	char *ptr;

	statusbar_text(button, "");

	if (interface_dialog_menu != NULL) {
		gdk_window_show(interface_dialog_menu->window);
		gdk_window_raise(interface_dialog_menu->window);
		return;
	}
	interface_dialog_menu = create_interface_dialog();

	combo = lookup_widget(GTK_WIDGET (interface_dialog_menu), "combo1");

	Ifc.ifc_len = sizeof(IfcBuf);
	Ifc.ifc_buf = (char *) IfcBuf;

	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		//printf("socket error\n");
		error("Error: socket error");
		return;
	}

	if ( ioctl(fd, SIOCGIFCONF, &Ifc) < 0) {
		//printf("ioctl SIOCGIFCONF error\n");
		error("Error: ioctl SIOCGIFCONF error");
		return;
	}

	num_ifreq = Ifc.ifc_len / sizeof(struct ifreq);

	ptr = buff;

	for ( pIfr = Ifc.ifc_req, i = 0, length = 0 ; i < num_ifreq; pIfr++, i++ ) {
		if ( (length + strlen(pIfr->ifr_name) + 1) < 100) {
			snprintf(ptr, 100, "%s", pIfr->ifr_name);
			glist = g_list_append(glist, ptr);
			ptr = ptr + strlen(pIfr->ifr_name) + 1;
			length = length + strlen(pIfr->ifr_name) + 1;
		}
		else
			break;
	}

	gtk_combo_set_popdown_strings(GTK_COMBO(combo), glist) ;

	gtk_entry_set_text(GTK_ENTRY(GTK_COMBO(combo)->entry), iftext);

	gtk_widget_show(interface_dialog_menu);

}