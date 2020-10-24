int save_gen_b(GtkButton *button, FILE *file_p) {

	GtkWidget *bt1, *bt2, *bt3, *bt4, *bt5, *bt6, *bt7, *bt8;
	gchar *bt1_t, *bt2_t;
	long count = 0, del = 0;
        int timeflag = 0, index = 0;

	bt1 = lookup_widget(GTK_WIDGET (button), "radiobutton34");
	bt2 = lookup_widget(GTK_WIDGET (button), "optionmenu9");
	bt3 = lookup_widget(GTK_WIDGET (button), "entry109");
	bt4 = lookup_widget(GTK_WIDGET (button), "entry110");
	bt5 = lookup_widget(GTK_WIDGET(button), "checkbutton35");
	bt6 = lookup_widget(GTK_WIDGET(button), "checkbutton37");

	bt1_t = (char *) gtk_entry_get_text(GTK_ENTRY(bt3));
	bt2_t = (char *) gtk_entry_get_text(GTK_ENTRY(bt4));

	bt7 = GTK_OPTION_MENU(bt2)->menu;
	bt8 = gtk_menu_get_active (GTK_MENU (bt7));
	/* YYY we don't save this value, since we don't know how to load it. We don't know what packet this is */
	index = g_list_index (GTK_MENU_SHELL (bt7)->children, bt8);

	if (GTK_TOGGLE_BUTTON(bt5)->active) {
		count = -3;
	}
	else {
		/* there can be rubbish in this field */
		if (check_digit(bt1_t, strlen(bt1_t), "Error: Number of packets to send field") == -1)
			return -1;

		count = strtol(bt1_t, (char **)NULL, 10);
		/* we allow to send 9999999 max */
		if ( (count > 9999999) || (count < 1) ) {
			//printf("Error: Packets send number value\n");
			error("Error: Packets send number value (1 - 9999999)");
			return -1;
		}
	}

	if (GTK_TOGGLE_BUTTON(bt6)->active) {
		del = 0;
	}
	else {
		/* there can be rubbish in this field */
		if (check_digit(bt2_t, strlen(bt2_t), "Error: Delay between packets field") == -1)
			return -1;

		del = strtol(bt2_t, (char **)NULL, 10);
		/* max delay 999,999999 s */
		if ( (del > 999999999) || (del < 1) ) {
			//printf("Error: Delay between packets value\n");
			error("Error: Delay between packets value (1 - 999999999");
			return -1;
		}
	}

	if (GTK_TOGGLE_BUTTON(bt1)->active)
		timeflag = 1;
	else
		timeflag = 0;


	fprintf(file_p, "#configuration parameters for send built generator\n");
	fprintf(file_p, "#absolute/relative timing (1/0)\n%d\n"
			/*"#adjust parameters while sending\n%d\n"*/
			"#number of packets to send (-3 for infinite)\n%ld\n"
			"#delay between packets (0 for max speed)\n%ld\n", 
			timeflag /*,index*/, count, del);

	return 1;
}