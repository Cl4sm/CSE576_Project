void
on_button76_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
        GtkWidget *tbt1, *w1, *w2, *w3, *w4, *w5, *en1, *menu, *menu_item;
	gint tos_value;
	gchar *en_t;
	gchar tmp[3];

        tbt1 = lookup_widget(GTK_WIDGET(button), "radiobutton38");

	if (GTK_TOGGLE_BUTTON(tbt1)->active) {
		w1 = lookup_widget(GTK_WIDGET(button), "optionmenu13");
		w2 = lookup_widget(GTK_WIDGET(button), "radiobutton48");
		w3 = lookup_widget(GTK_WIDGET(button), "radiobutton50");
		w4 = lookup_widget(GTK_WIDGET(button), "radiobutton52");
		w5 = lookup_widget(GTK_WIDGET(button), "radiobutton54");

		menu = GTK_OPTION_MENU(w1)->menu;
                menu_item = gtk_menu_get_active (GTK_MENU (menu));
                tos_value = g_list_index (GTK_MENU_SHELL (menu)->children, menu_item);
		tos_value = tos_value << 5;

		if (GTK_TOGGLE_BUTTON(w2)->active) 
			tos_value = tos_value + 16;
		if (GTK_TOGGLE_BUTTON(w3)->active) 
			tos_value = tos_value + 8;
		if (GTK_TOGGLE_BUTTON(w4)->active) 
			tos_value = tos_value + 4;
		if (GTK_TOGGLE_BUTTON(w5)->active) 
			tos_value = tos_value + 2;
        }
        else {
        	en1 = lookup_widget(GTK_WIDGET(button), "entry154");
		en_t = (char *)gtk_entry_get_text(GTK_ENTRY(en1));
		tos_value = (guchar)strtol(en_t, (char **)NULL, 10);
		if ((tos_value<0) || (tos_value>63)) {
			//printf("wrong DSCP value\n");
			error("Error: wrong DSCP value");
			return;
		}
		else 
			tos_value = tos_value << 2;
			
	}

	c8(tmp, tos_value);
	gtk_entry_set_text(GTK_ENTRY(entry_field_tos), tmp);

	gtk_grab_remove(gtk_widget_get_toplevel(GTK_WIDGET(button)));
	gtk_widget_destroy(gtk_widget_get_toplevel(GTK_WIDGET(button)));
}