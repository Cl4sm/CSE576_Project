void show_config(gpointer object, gpointer user_data)
{
	static GtkWidget *dialog = NULL;
	GtkWidget *box;
	GtkWidget *grid;
	GtkWidget *label;
	int row = 0;
	GtkWidget *hbox;
	time_t now;
	struct tm *ctime;
	guint d, m, y;

	if (stop_date != 0)
		now = stop_date;
	else 
		now = time(NULL);

	ctime = gmtime(&now);
	d = ctime->tm_mday;
	m = ctime->tm_mon;
	y = ctime->tm_year;

	if (dialog != NULL)
		return;

	dialog = gtk_dialog_new_with_buttons(
			_("QuitCount configuration"),
			NULL, GTK_DIALOG_MODAL,
			GTK_STOCK_CLOSE, GTK_RESPONSE_ACCEPT,
			NULL);
			
	box = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	
	grid = gtk_grid_new();
	
	gtk_grid_set_row_spacing(GTK_GRID(grid), 2);
	gtk_grid_set_column_spacing(GTK_GRID(grid), 3);
	
	label = gtk_label_new(_("I quit on"));
	gtk_misc_set_alignment(GTK_MISC(label), 1, 0);
	gtk_grid_attach(GTK_GRID(grid), label, 0, row, 1, 1);
	
	stop_date_calendar = gtk_calendar_new();
	gtk_calendar_select_month(GTK_CALENDAR(stop_date_calendar),
					m,
					y + 1900);
	gtk_calendar_select_day(GTK_CALENDAR(stop_date_calendar),
					d);
	gtk_grid_attach(GTK_GRID(grid), stop_date_calendar, 1, row, 1, 1);
	
	row++;
	
	label = gtk_label_new(_("I smoked"));
	gtk_misc_set_alignment(GTK_MISC(label), 1, 0.5);
	gtk_grid_attach(GTK_GRID(grid), label, 0, row, 1, 1);
	
	num_per_day_adj = GTK_ADJUSTMENT(
		gtk_adjustment_new(num_per_day, 1.0, 100.0, 1.0, 1.0, 0.0));
	num_per_day_spin = gtk_spin_button_new(num_per_day_adj, 1.0, 0);
	gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(num_per_day_spin), TRUE);
	hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
	label = gtk_label_new(_("cigarettes per day"));
	gtk_misc_set_alignment(GTK_MISC(label), 0, 0.5);
	
	gtk_box_pack_start(GTK_BOX(hbox), num_per_day_spin, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(hbox), label, TRUE, TRUE, 0);
	gtk_grid_attach(GTK_GRID(grid), hbox, 1, row, 1, 1);
	
	row++;
	
	label = gtk_label_new(_("Each contained"));
	gtk_misc_set_alignment(GTK_MISC(label), 1, 0.5);
	gtk_grid_attach(GTK_GRID(grid), label, 0, row, 1, 1);
	
	nicotine_per_cig_adj = GTK_ADJUSTMENT(
		gtk_adjustment_new(nicotine_per_cig, 0.1, 3.0, 0.1, 0.1, 0.0));
	nicotine_per_cig_spin = gtk_spin_button_new(nicotine_per_cig_adj, 0.1, 1);
	gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(nicotine_per_cig_spin), TRUE);
	hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
	label = gtk_label_new(_("mg of nicotine"));
	gtk_misc_set_alignment(GTK_MISC(label), 0, 0.5);
	
	gtk_box_pack_start(GTK_BOX(hbox), nicotine_per_cig_spin, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(hbox), label, TRUE, TRUE, 0);
	gtk_grid_attach(GTK_GRID(grid), hbox, 1, row, 1, 1);
	
	row++;
	
	tar_per_cig_adj = GTK_ADJUSTMENT(
		gtk_adjustment_new(tar_per_cig, 1.0, 20.0, 0.5, 0.5, 0.0));
	tar_per_cig_spin = gtk_spin_button_new(tar_per_cig_adj, 0.5, 1);
	gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(tar_per_cig_spin), TRUE);
	hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
	label = gtk_label_new(_("mg of tar"));
	gtk_misc_set_alignment(GTK_MISC(label), 0, 0.5);
	
	gtk_box_pack_start(GTK_BOX(hbox), tar_per_cig_spin, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(hbox), label, TRUE, TRUE, 0);
	gtk_grid_attach(GTK_GRID(grid), hbox, 1, row, 1, 1);
	
	row++;
	
	label = gtk_label_new(_("It cost"));
	gtk_misc_set_alignment(GTK_MISC(label), 1, 0.5);
	gtk_grid_attach(GTK_GRID(grid), label, 0, row, 1, 1);
	
	hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
	price_per_pack_adj = GTK_ADJUSTMENT(
		gtk_adjustment_new(price_per_pack, 0.1, 1000.0, 0.1, 0.1, 0.0));
	price_per_pack_spin = gtk_spin_button_new(price_per_pack_adj, 0.1, 2);
	gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(price_per_pack_spin), TRUE);
	gtk_box_pack_start(GTK_BOX(hbox), price_per_pack_spin, FALSE, FALSE, 0);
	
	currency_entry = gtk_entry_new();
	gtk_entry_set_text(GTK_ENTRY(currency_entry), currency?currency:"€");
	gtk_entry_set_width_chars(GTK_ENTRY(currency_entry), 3);
	gtk_box_pack_start(GTK_BOX(hbox), currency_entry, FALSE, FALSE, 0);

	label = gtk_label_new(_("per pack of"));
	gtk_misc_set_alignment(GTK_MISC(label), 0.5, 0.5);
	gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 0);

	pack_size_adj = GTK_ADJUSTMENT(
		gtk_adjustment_new(pack_size, 1.0, 100.0, 1.0, 1.0, 0.0));
	pack_size_spin = gtk_spin_button_new(pack_size_adj, 1.0, 0);
	gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(pack_size_spin), TRUE);
	
	gtk_box_pack_start(GTK_BOX(hbox), pack_size_spin, TRUE, TRUE, 0);
	gtk_grid_attach(GTK_GRID(grid), hbox, 1, row, 1, 1);
	
	gtk_widget_show_all(grid);
	gtk_box_pack_start(GTK_BOX(box), wrap(grid, _("<span size='x-large'><b>QuitCount - configuration</b></span>")), 
			FALSE, FALSE, 10);
	gtk_widget_set_size_request(dialog, 500, -1);
	gtk_dialog_run(GTK_DIALOG(dialog));
	
	gtk_calendar_get_date(GTK_CALENDAR(stop_date_calendar),
				&y, &m, &d);
	ctime->tm_year = y - 1900;
	ctime->tm_mon  = m;
	ctime->tm_mday = d;
	
	ctime->tm_hour = ctime->tm_min = ctime->tm_sec = 1;
	
	stop_date = mktime(ctime);
	num_per_day = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(num_per_day_spin));
	price_per_pack = gtk_spin_button_get_value(GTK_SPIN_BUTTON(price_per_pack_spin));
	pack_size = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(pack_size_spin));
	unit_price = price_per_pack/pack_size;
	nicotine_per_cig = gtk_spin_button_get_value(GTK_SPIN_BUTTON(nicotine_per_cig_spin));
	tar_per_cig = gtk_spin_button_get_value(GTK_SPIN_BUTTON(tar_per_cig_spin));
	g_free(currency);
	currency = gtk_editable_get_chars(GTK_EDITABLE(currency_entry), 0, -1);
	write_config();
	install_anniversary_watch();

	gtk_widget_destroy(dialog);
	dialog = NULL;
}