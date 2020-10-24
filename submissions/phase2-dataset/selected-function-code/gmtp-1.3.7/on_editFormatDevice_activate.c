    GtkWidget *dialog;
    dialog = gtk_message_dialog_new(GTK_WINDOW(windowMain),
            GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_WARNING,
            GTK_BUTTONS_YES_NO,
            _("Are you sure you want to format this device?"));
    gtk_window_set_title(GTK_WINDOW(dialog), _("Format Device"));
    gint result = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_hide(GTK_WIDGET(dialog));
    gtk_widget_destroy(dialog);

    if (result == GTK_RESPONSE_YES) {
        dialog = create_windowFormat();
        // Show progress dialog.
        gtk_widget_show_all(dialog);
        // Ensure GTK redraws the window.

        formatThreadWorking = TRUE;

#if GLIB_CHECK_VERSION(2,32,0)
	GThread *th = g_thread_new("format", (GThreadFunc) formatDevice_thread, NULL);
#else
        g_thread_create((GThreadFunc) formatDevice_thread, NULL, FALSE, NULL);
#endif
        while (formatThreadWorking) {
            while (gtk_events_pending())
                gtk_main_iteration();

            if (formatDialog_progressBar != NULL) {
                gtk_progress_bar_pulse(GTK_PROGRESS_BAR(formatDialog_progressBar));
                g_usleep(G_USEC_PER_SEC * 0.1);
            }

        }
        // The worker thread has finished so let's continue.
#if GLIB_CHECK_VERSION(2,32,0)
	g_thread_unref(th);
#endif
        // Disconnect and reconnect the device.
        on_deviceConnect_activate(NULL, NULL);
        // Sleep for 2 secs to allow the device to settle itself
        g_usleep(G_USEC_PER_SEC * 2);
        on_deviceConnect_activate(NULL, NULL);
        // Close progress dialog.
        gtk_widget_hide(dialog);
        gtk_widget_destroy(dialog);
        formatDialog_progressBar = NULL;
    }
    //
} // end on_editFormatDevice_activate()
