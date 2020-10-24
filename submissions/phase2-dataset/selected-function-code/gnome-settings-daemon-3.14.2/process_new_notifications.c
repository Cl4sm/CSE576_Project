static gboolean
process_new_notifications (gpointer user_data)
{
        GsdPrintNotificationsManager  *manager = (GsdPrintNotificationsManager *) user_data;
        ipp_attribute_t               *attr;
        const gchar                   *notify_subscribed_event = NULL;
        const gchar                   *printer_name = NULL;
        const gchar                   *notify_text = NULL;
        const gchar                   *notify_printer_uri = NULL;
        const gchar                   *job_state_reasons = NULL;
        const gchar                   *job_name = NULL;
        const char                    *attr_name;
        gboolean                       printer_is_accepting_jobs = FALSE;
        gchar                         *printer_state_reasons = NULL;
        gchar                        **reasons;
        guint                          notify_job_id = 0;
        ipp_t                         *request;
        ipp_t                         *response;
        gint                           printer_state = -1;
        gint                           job_state = -1;
        gint                           job_impressions_completed = -1;
        gint                           notify_sequence_number = -1;
        gint                           i;

        request = ippNewRequest (IPP_GET_NOTIFICATIONS);

        ippAddString (request, IPP_TAG_OPERATION, IPP_TAG_NAME,
                      "requesting-user-name", NULL, cupsUser ());

        ippAddInteger (request, IPP_TAG_OPERATION, IPP_TAG_INTEGER,
                       "notify-subscription-ids", manager->priv->subscription_id);

        ippAddString (request, IPP_TAG_OPERATION, IPP_TAG_URI, "printer-uri", NULL,
                      "/printers/");

        ippAddString (request, IPP_TAG_OPERATION, IPP_TAG_URI, "job-uri", NULL,
                      "/jobs/");

        ippAddInteger (request, IPP_TAG_OPERATION, IPP_TAG_INTEGER,
                       "notify-sequence-numbers",
                       manager->priv->last_notify_sequence_number + 1);


        response = cupsDoRequest (CUPS_HTTP_DEFAULT, request, "/");


        for (attr = ippFindAttribute (response, "notify-sequence-number", IPP_TAG_INTEGER);
             attr != NULL;
             attr = ippNextAttribute (response)) {

                attr_name = ippGetName (attr);
                if (g_strcmp0 (attr_name, "notify-sequence-number") == 0) {
                        notify_sequence_number = ippGetInteger (attr, 0);

                        if (notify_sequence_number > manager->priv->last_notify_sequence_number)
                                manager->priv->last_notify_sequence_number = notify_sequence_number;

                        if (notify_subscribed_event != NULL) {
                                process_cups_notification (manager,
                                                           notify_subscribed_event,
                                                           notify_text,
                                                           notify_printer_uri,
                                                           printer_name,
                                                           printer_state,
                                                           printer_state_reasons,
                                                           printer_is_accepting_jobs,
                                                           notify_job_id,
                                                           job_state,
                                                           job_state_reasons,
                                                           job_name,
                                                           job_impressions_completed);

                                g_clear_pointer (&printer_state_reasons, g_free);
                                g_clear_pointer (&job_state_reasons, g_free);
                        }

                        notify_subscribed_event = NULL;
                        notify_text = NULL;
                        notify_printer_uri = NULL;
                        printer_name = NULL;
                        printer_state = -1;
                        printer_state_reasons = NULL;
                        printer_is_accepting_jobs = FALSE;
                        notify_job_id = 0;
                        job_state = -1;
                        job_state_reasons = NULL;
                        job_name = NULL;
                        job_impressions_completed = -1;
                } else if (g_strcmp0 (attr_name, "notify-subscribed-event") == 0) {
                        notify_subscribed_event = ippGetString (attr, 0, NULL);
                } else if (g_strcmp0 (attr_name, "notify-text") == 0) {
                        notify_text = ippGetString (attr, 0, NULL);
                } else if (g_strcmp0 (attr_name, "notify-printer-uri") == 0) {
                        notify_printer_uri = ippGetString (attr, 0, NULL);
                } else if (g_strcmp0 (attr_name, "printer-name") == 0) {
                        printer_name = ippGetString (attr, 0, NULL);
                } else if (g_strcmp0 (attr_name, "printer-state") == 0) {
                        printer_state = ippGetInteger (attr, 0);
                } else if (g_strcmp0 (attr_name, "printer-state-reasons") == 0) {
                        reasons = g_new0 (gchar *, ippGetCount (attr) + 1);
                        for (i = 0; i < ippGetCount (attr); i++)
                                reasons[i] = g_strdup (ippGetString (attr, i, NULL));
                        printer_state_reasons = g_strjoinv (",", reasons);
                        g_strfreev (reasons);
                } else if (g_strcmp0 (attr_name, "printer-is-accepting-jobs") == 0) {
                        printer_is_accepting_jobs = ippGetBoolean (attr, 0);
                } else if (g_strcmp0 (attr_name, "notify-job-id") == 0) {
                        notify_job_id = ippGetInteger (attr, 0);
                } else if (g_strcmp0 (attr_name, "job-state") == 0) {
                        job_state = ippGetInteger (attr, 0);
                } else if (g_strcmp0 (attr_name, "job-state-reasons") == 0) {
                        reasons = g_new0 (gchar *, ippGetCount (attr) + 1);
                        for (i = 0; i < ippGetCount (attr); i++)
                                reasons[i] = g_strdup (ippGetString (attr, i, NULL));
                        job_state_reasons = g_strjoinv (",", reasons);
                        g_strfreev (reasons);
                } else if (g_strcmp0 (attr_name, "job-name") == 0) {
                        job_name = ippGetString (attr, 0, NULL);
                } else if (g_strcmp0 (attr_name, "job-impressions-completed") == 0) {
                        job_impressions_completed = ippGetInteger (attr, 0);
                }
        }

        if (notify_subscribed_event != NULL) {
                process_cups_notification (manager,
                                           notify_subscribed_event,
                                           notify_text,
                                           notify_printer_uri,
                                           printer_name,
                                           printer_state,
                                           printer_state_reasons,
                                           printer_is_accepting_jobs,
                                           notify_job_id,
                                           job_state,
                                           job_state_reasons,
                                           job_name,
                                           job_impressions_completed);

                g_clear_pointer (&printer_state_reasons, g_free);
                g_clear_pointer (&job_state_reasons, g_free);
        }

        if (response != NULL)
                ippDelete (response);

        return TRUE;
}