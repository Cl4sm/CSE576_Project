    GtkWidget* windowFormat;
    GtkWidget* label1;
    GtkWidget* vbox1;

    windowFormat = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gchar * winTitle;
    winTitle = g_strconcat(PACKAGE_NAME, NULL);
    gtk_window_set_title(GTK_WINDOW(windowFormat), winTitle);
    gtk_window_set_position(GTK_WINDOW(windowFormat), GTK_WIN_POS_CENTER_ON_PARENT);
    gtk_window_set_modal(GTK_WINDOW(windowFormat), TRUE);
    gtk_window_set_transient_for(GTK_WINDOW(windowFormat), GTK_WINDOW(windowMain));
    gtk_window_set_destroy_with_parent(GTK_WINDOW(windowFormat), TRUE);
    gtk_window_set_type_hint(GTK_WINDOW(windowFormat), GDK_WINDOW_TYPE_HINT_DIALOG);
    gtk_window_set_default_size(GTK_WINDOW(windowFormat), 200, 60);
    g_free(winTitle);
#if HAVE_GTK3 == 0
    vbox1 = gtk_vbox_new(FALSE, 0);
#else
    vbox1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
#endif
    gtk_widget_show(vbox1);
    gtk_container_add(GTK_CONTAINER(windowFormat), vbox1);
    gtk_container_set_border_width(GTK_CONTAINER(vbox1), 10);
    gtk_box_set_spacing(GTK_BOX(vbox1), 5);

    label1 = gtk_label_new("Formatting...");
    gtk_widget_show(label1);
    gtk_box_pack_start(GTK_BOX(vbox1), label1, TRUE, TRUE, 0);
    gtk_misc_set_padding(GTK_MISC(label1), 0, 5);
    gtk_misc_set_alignment(GTK_MISC(label1), 0, 0);

    formatDialog_progressBar = gtk_progress_bar_new();
    gtk_progress_bar_set_pulse_step(GTK_PROGRESS_BAR(formatDialog_progressBar), 0.05);
    gtk_widget_show(formatDialog_progressBar);
    gtk_box_pack_start(GTK_BOX(vbox1), formatDialog_progressBar, TRUE, TRUE, 0);

    return windowFormat;
}
