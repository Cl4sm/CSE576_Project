int message_box(GtkWidget *parent, char *title, char *msg, int centre,
		int type)
{
    GtkWidget *window, *hbox, *text, *button;
    char *titles;
    int i, def, cancel;

    window = gtk_dialog_new();
    text = gtk_label_new(msg);
    gtk_misc_set_alignment(GTK_MISC(text), 0.0, 0.0);
    hbox = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), text, FALSE, FALSE, 20);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(window)->vbox),
                       hbox, FALSE, FALSE, 20);
    gtk_widget_show(text);
    gtk_widget_show(hbox);
    gtk_window_set_title(GTK_WINDOW(window), title);
    gtk_label_set_line_wrap(GTK_LABEL(text), TRUE);

    if (type == MB_OK) {
	titles = GTK_STOCK_OK "\0";
	def = cancel = 0;
    } else {
	assert(type == MB_YESNO);
	titles = GTK_STOCK_NO "\0" GTK_STOCK_YES "\0";
	def = 1;
	cancel = 0;
    }
    i = 0;
    
    while (*titles) {
	button = gtk_button_new_from_stock(titles);
	gtk_box_pack_end(GTK_BOX(GTK_DIALOG(window)->action_area),
			 button, FALSE, FALSE, 0);
	gtk_widget_show(button);
	if (i == def) {
	    GTK_WIDGET_SET_FLAGS(button, GTK_CAN_DEFAULT);
	    gtk_window_set_default(GTK_WINDOW(window), button);
	}
	if (i == cancel) {
	    gtk_signal_connect(GTK_OBJECT(window), "key_press_event",
			       GTK_SIGNAL_FUNC(win_key_press), button);
	}
	gtk_signal_connect(GTK_OBJECT(button), "clicked",
			   GTK_SIGNAL_FUNC(msgbox_button_clicked), window);
	gtk_object_set_data(GTK_OBJECT(button), "user-data",
			    GINT_TO_POINTER(i));
	titles += strlen(titles)+1;
	i++;
    }
    gtk_object_set_data(GTK_OBJECT(window), "user-data",
			GINT_TO_POINTER(&i));
    gtk_signal_connect(GTK_OBJECT(window), "destroy",
                       GTK_SIGNAL_FUNC(window_destroy), NULL);
    gtk_window_set_modal(GTK_WINDOW(window), TRUE);
    gtk_window_set_transient_for(GTK_WINDOW(window), GTK_WINDOW(parent));
    /* set_transient_window_pos(parent, window); */
    gtk_widget_show(window);
    i = -1;
    gtk_main();
    return (type == MB_YESNO ? i == 1 : TRUE);
}