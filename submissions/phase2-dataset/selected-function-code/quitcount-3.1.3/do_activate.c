static void do_activate(gpointer object, gpointer user_data)
{
	time_t now = time(NULL);
	gchar *buf;
	int num_day = (now - stop_date)/86400;
	int num_cig = num_day * num_per_day;
	float nicotine, tar, life;
	const char *n_unit, *t_unit, *l_unit;
	gint response;
	GtkWidget *grid, *label;
	gint row = 0;
	GtkWidget *box;
	static GtkWidget *dialog = NULL;

	if (dialog != NULL) {
		gtk_dialog_response(GTK_DIALOG(dialog), GTK_RESPONSE_ACCEPT);
		return;
	}

	dialog = gtk_dialog_new_with_buttons(
			_("QuitCount statistics"),
			NULL, GTK_DIALOG_MODAL,
			GTK_STOCK_PREFERENCES, QUITCOUNT_RESPONSE_PREFERENCES,
			GTK_STOCK_CLOSE, GTK_RESPONSE_ACCEPT,
			NULL);

	box = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

	gtk_dialog_set_default_response(GTK_DIALOG(dialog), GTK_RESPONSE_ACCEPT);

	grid = gtk_grid_new();
	
	gtk_grid_set_row_spacing(GTK_GRID(grid), 2);
	gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
	
	buf = g_strdup_printf("<span size='large'><b>%d</b></span>", num_day);
	label = gtk_label_new(buf);
	gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
	g_free(buf);
	gtk_misc_set_alignment(GTK_MISC(label), 1, 0.5);
	gtk_grid_attach(GTK_GRID(grid), label, 0, row, 1, 1);
	
	label = gtk_label_new(_("<span size='large'>days since I quit</span>"));
	gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
	gtk_misc_set_alignment(GTK_MISC(label), 0, 0.5);
	gtk_grid_attach(GTK_GRID(grid), label, 1, row, 1, 1);
	
	row++;
	
	if (num_day > 7) {
		int num_year, num_mon, num_week;
		gchar *fmt = NULL, *yfmt = NULL, *mfmt = NULL, *wfmt = NULL;
		num_year = num_day/365;
		num_day = (num_day%365);
		num_mon = num_day/30;
		num_day = (num_day%30);
		num_week = num_day/7;
		num_day = (num_day%7);

		if (num_year > 0)
			yfmt = g_strdup_printf(ngettext("%d year", "%d years", num_year), num_year);
		if (num_mon > 0)
			mfmt = g_strdup_printf(ngettext("%d month", "%d months", num_mon), num_mon);
		if (num_week > 0)
			wfmt = g_strdup_printf(ngettext("%d week", "%d weeks", num_week), num_week);

		fmt = g_strdup_printf("%s%s%s%s%s",
			yfmt ? yfmt:"",
			yfmt && mfmt ? ", ":"",
			mfmt ? mfmt:"",
			(yfmt||mfmt) && wfmt ? ", ":"",
			wfmt ? wfmt:""
			);
			
		label = gtk_label_new(fmt);
		gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
		g_free(fmt);
		g_free(yfmt);
		g_free(mfmt);
		g_free(wfmt);
		gtk_misc_set_alignment(GTK_MISC(label), 0, 0.5);
		gtk_grid_attach(GTK_GRID(grid), label, 1, row, 1, 1);

		row++;

	}
	
	
	buf = g_strdup_printf("<span size='large'><b>%d</b></span>", num_cig);
	label = gtk_label_new(buf);
	gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
	g_free(buf);
	gtk_misc_set_alignment(GTK_MISC(label), 1, 0.5);
	gtk_grid_attach(GTK_GRID(grid), label, 0, row, 1, 1);
	
	label = gtk_label_new(_("<span size='large'>cigarettes not smoked</span>"));
	gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
	gtk_misc_set_alignment(GTK_MISC(label), 0, 0.5);
	gtk_grid_attach(GTK_GRID(grid), label, 1, row, 1, 1);
	
	row++;
	
	nicotine = nicotine_per_cig*num_cig;
	tar = tar_per_cig*num_cig;

	if (nicotine > 1000000) {
		n_unit = _("Kg");
		nicotine /= 1000000;
	} else if (nicotine > 1000) {
		n_unit = _("g");
		nicotine /= 1000;
	} else 
		n_unit = _("mg");

	if (tar > 1000000) {
		t_unit = _("Kg");
		tar /= 1000000;
	} else if (tar > 1000) {
		t_unit = _("g");
		tar /= 1000;
	} else 
		t_unit = _("mg");

	buf = g_strdup_printf(_("Nicotine: %.2f %s\nTar: %.2f %s"),
		nicotine, n_unit, tar, t_unit);
	label = gtk_label_new(buf);
	g_free(buf);
	gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
	gtk_misc_set_alignment(GTK_MISC(label), 0, 0.5);
	gtk_grid_attach(GTK_GRID(grid), label, 1, row, 1, 1);
	
	row++;
	
	buf = g_strdup_printf("<span size='large'><b>%.2f</b></span>", num_cig * unit_price);
	label = gtk_label_new(buf);
	gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
	g_free(buf);
	gtk_misc_set_alignment(GTK_MISC(label), 1, 0.5);
	gtk_grid_attach(GTK_GRID(grid), label, 0, row, 1, 1);
	
	buf = g_strdup_printf(_("<span size='large'>%s saved</span>"), currency);
	label = gtk_label_new(buf);
	g_free(buf);
	gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
	gtk_misc_set_alignment(GTK_MISC(label), 0, 0.5);
	gtk_grid_attach(GTK_GRID(grid), label, 1, row, 1, 1);
	
	row++;
	
	life = num_cig*10.0;
	if (life > 60*24*365) {
		life /= (60*24*365);
		l_unit = ngettext("year","years",(int)life);
	} else if (life > 60*24*30) {
		life /= (60*24*30);
		l_unit = ngettext("month","months",(int)life);
	} else if (life > 60*24*7) {
		life /= (60*24*7);
		l_unit = ngettext("week","weeks",(int)life);
	} else if (life > 60*24) {
		life /= (60*24);
		l_unit = ngettext("day","days",(int)life);
	} else if (life > 60) {
		life /= (60);
		l_unit = ngettext("hour","hours",(int)life);
	} else
		l_unit = ngettext("minute","minutes",(int)life);

	buf = g_strdup_printf("<span size='large'><b>%.2f %s</b></span>", life, l_unit);
	label = gtk_label_new(buf);
	gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
	g_free(buf);
	gtk_misc_set_alignment(GTK_MISC(label), 1, 0.5);
	gtk_grid_attach(GTK_GRID(grid), label, 0, row, 1, 1);
	
	label = gtk_label_new(_("<span size='large'>of life expectancy gained</span>"));
	gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
	gtk_misc_set_alignment(GTK_MISC(label), 0, 0.5);
	gtk_grid_attach(GTK_GRID(grid), label, 1, row, 1, 1);
	
	row++;
	
	gtk_widget_show_all(grid);
	gtk_box_pack_start(GTK_BOX(box), wrap(grid, 
		_("<span size='x-large'><b>QuitCount - statistics</b></span>")), FALSE, FALSE, 10);
	gtk_widget_set_size_request(dialog, 500, -1);
	response = gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
	dialog = NULL;

	if (response == QUITCOUNT_RESPONSE_PREFERENCES)
		show_config(NULL, NULL);
}