int fileprogress(const uint64_t sent, const uint64_t total, void const * const data) {
    gchar* tmp_string;
    gchar* tmp_sent;
    gchar* tmp_total;
    gint percent = (sent * 100) / total;

    // See if our dialog box was killed, and if so, just return which also kill our download/upload...
    if (progressDialog_killed == TRUE)
        return TRUE;

    // Now update the progress dialog.
    if (progressDialog != NULL) {
        tmp_sent = calculateFriendlySize(sent);
        tmp_total = calculateFriendlySize(total);

        if (progressDialog_filename != NULL) {
            tmp_string = g_strdup_printf(_("%s\n%s of %s (%d%%)"), progressDialog_filename,
                    tmp_sent, tmp_total, percent);
        } else {
            tmp_string = g_strdup_printf(_("%s of %s (%d%%)"),
                    tmp_sent, tmp_total, percent);
        }
        gtk_label_set_text(GTK_LABEL(progressDialog_Text), tmp_string);
        gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progressDialog_Bar), (double) percent / 100.00);

        while (gtk_events_pending()) {
            gtk_main_iteration();
        }
        g_free(tmp_string);
        g_free(tmp_sent);
        g_free(tmp_total);
    }
    return 0;
}
