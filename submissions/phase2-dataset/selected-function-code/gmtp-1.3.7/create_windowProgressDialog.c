GtkWidget* create_windowProgressDialog(gchar* msg) {
    GtkWidget *window1;
    GtkWidget *vbox1;
    GtkWidget *hbox1;
    GtkWidget *label_FileProgress;
    GtkWidget *label1;
    GtkWidget *cancelButton;
    GtkWidget *progressbar_Main;

    window1 = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gchar * winTitle;
    winTitle = g_strconcat(PACKAGE_NAME, NULL);
    gtk_window_set_title(GTK_WINDOW(window1), winTitle);
    gtk_window_set_position(GTK_WINDOW(window1), GTK_WIN_POS_CENTER_ON_PARENT);
    gtk_window_set_modal(GTK_WINDOW(window1), TRUE);
    gtk_window_set_resizable(GTK_WINDOW(window1), FALSE);
    gtk_window_set_transient_for(GTK_WINDOW(window1), GTK_WINDOW(windowMain));
    gtk_window_set_destroy_with_parent(GTK_WINDOW(window1), TRUE);
    gtk_window_set_type_hint(GTK_WINDOW(window1), GDK_WINDOW_TYPE_HINT_DIALOG);
    g_free(winTitle);
#if HAVE_GTK3 == 0
    vbox1 = gtk_vbox_new(FALSE, 0);
#else
    vbox1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
#endif
    gtk_widget_show(vbox1);
    gtk_container_add(GTK_CONTAINER(window1), vbox1);
    gtk_container_set_border_width(GTK_CONTAINER(vbox1), 10);
    gtk_box_set_spacing(GTK_BOX(vbox1), 5);

    label1 = gtk_label_new(NULL);
    winTitle = g_strconcat("<b><big>", msg, "</big></b>", NULL);
    gtk_label_set_markup(GTK_LABEL(label1), winTitle);
    gtk_widget_show(label1);
    gtk_box_pack_start(GTK_BOX(vbox1), label1, TRUE, TRUE, 0);
    gtk_misc_set_padding(GTK_MISC(label1), 0, 5);
    gtk_misc_set_alignment(GTK_MISC(label1), 0, 0);
    g_free(winTitle);

    label_FileProgress = gtk_label_new(_("file = ( x / x ) x %"));
    gtk_label_set_line_wrap(GTK_LABEL(label_FileProgress), TRUE);
    
#if GTK_CHECK_VERSION(2,10,0)
    // If we can, word wrap the label.
    gtk_label_set_line_wrap_mode(GTK_LABEL(label_FileProgress), PANGO_WRAP_WORD);
#endif
    gtk_widget_set_size_request(label_FileProgress, 320, -1);
    gtk_widget_show(label_FileProgress);
    gtk_box_pack_start(GTK_BOX(vbox1), label_FileProgress, TRUE, TRUE, 0);
    gtk_misc_set_padding(GTK_MISC(label_FileProgress), 0, 5);

    progressbar_Main = gtk_progress_bar_new();
    gtk_widget_show(progressbar_Main);
    gtk_box_pack_start(GTK_BOX(vbox1), progressbar_Main, TRUE, TRUE, 0);

    // Insert a cancel button.
#if HAVE_GTK3 == 0
    hbox1 = gtk_hbox_new(FALSE, 0);
#else
    hbox1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
#endif
    gtk_widget_show(hbox1);
    gtk_box_pack_start(GTK_BOX(vbox1), hbox1, FALSE, FALSE, 0);
    cancelButton = gtk_button_new_with_mnemonic(_("_Cancel"));
    gtk_widget_show(cancelButton);
    gtk_box_pack_end(GTK_BOX(hbox1), cancelButton, FALSE, FALSE, 0);

    progressDialog = window1;
    progressDialog_Text = label_FileProgress;
    progressDialog_Bar = progressbar_Main;

    g_signal_connect((gpointer) cancelButton, "clicked",
            G_CALLBACK(on_progressDialog_Cancel),
            NULL);

    return window1;
}
