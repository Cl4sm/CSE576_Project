static void
process_cups_notification (GsdPrintNotificationsManager *manager,
                           const char                   *notify_subscribed_event,
                           const char                   *notify_text,
                           const char                   *notify_printer_uri,
                           const char                   *printer_name,
                           gint                          printer_state,
                           const char                   *printer_state_reasons,
                           gboolean                      printer_is_accepting_jobs,
                           guint                         notify_job_id,
                           gint                          job_state,
                           const char                   *job_state_reasons,
                           const char                   *job_name,
                           gint                          job_impressions_completed)
{
        ipp_attribute_t *attr;
        gboolean         my_job = FALSE;
        gboolean         known_reason;
        http_t          *http;
        gchar           *primary_text = NULL;
        gchar           *secondary_text = NULL;
        gchar           *job_uri = NULL;
        ipp_t           *request, *response;
        static const char * const reasons[] = {
                "toner-low",
                "toner-empty",
                "connecting-to-device",
                "cover-open",
                "cups-missing-filter",
                "door-open",
                "marker-supply-low",
                "marker-supply-empty",
                "media-low",
                "media-empty",
                "offline",
                "other"};

        static const char * statuses_first[] = {
                /* Translators: The printer is low on toner (same as in system-config-printer) */
                N_("Toner low"),
                /* Translators: The printer has no toner left (same as in system-config-printer) */
                N_("Toner empty"),
                /* Translators: The printer is in the process of connecting to a shared network output device (same as in system-config-printer) */
                N_("Not connected?"),
                /* Translators: One or more covers on the printer are open (same as in system-config-printer) */
                N_("Cover open"),
                /* Translators: A filter or backend is not installed (same as in system-config-printer) */
                N_("Printer configuration error"),
                /* Translators: One or more doors on the printer are open (same as in system-config-printer) */
                N_("Door open"),
                /* Translators: "marker" is one color bin of the printer */
                N_("Marker supply low"),
                /* Translators: "marker" is one color bin of the printer */
                N_("Out of a marker supply"),
                /* Translators: At least one input tray is low on media (same as in system-config-printer) */
                N_("Paper low"),
                /* Translators: At least one input tray is empty (same as in system-config-printer) */
                N_("Out of paper"),
                /* Translators: The printer is offline (same as in system-config-printer) */
                N_("Printer off-line"),
                /* Translators: The printer has detected an error (same as in system-config-printer) */
                N_("Printer error") };

        static const char * statuses_second[] = {
                /* Translators: The printer is low on toner (same as in system-config-printer) */
                N_("Printer '%s' is low on toner."),
                /* Translators: The printer has no toner left (same as in system-config-printer) */
                N_("Printer '%s' has no toner left."),
                /* Translators: The printer is in the process of connecting to a shared network output device (same as in system-config-printer) */
                N_("Printer '%s' may not be connected."),
                /* Translators: One or more covers on the printer are open (same as in system-config-printer) */
                N_("The cover is open on printer '%s'."),
                /* Translators: A filter or backend is not installed (same as in system-config-printer) */
                N_("There is a missing print filter for "
                   "printer '%s'."),
                /* Translators: One or more doors on the printer are open (same as in system-config-printer) */
                N_("The door is open on printer '%s'."),
                /* Translators: "marker" is one color bin of the printer */
                N_("Printer '%s' is low on a marker supply."),
                /* Translators: "marker" is one color bin of the printer */
                N_("Printer '%s' is out of a marker supply."),
                /* Translators: At least one input tray is low on media (same as in system-config-printer) */
                N_("Printer '%s' is low on paper."),
                /* Translators: At least one input tray is empty (same as in system-config-printer) */
                N_("Printer '%s' is out of paper."),
                /* Translators: The printer is offline (same as in system-config-printer) */
                N_("Printer '%s' is currently off-line."),
                /* Translators: The printer has detected an error (same as in system-config-printer) */
                N_("There is a problem on printer '%s'.") };

        if (g_strcmp0 (notify_subscribed_event, "printer-added") != 0 &&
            g_strcmp0 (notify_subscribed_event, "printer-deleted") != 0 &&
            g_strcmp0 (notify_subscribed_event, "printer-state-changed") != 0 &&
            g_strcmp0 (notify_subscribed_event, "job-completed") != 0 &&
            g_strcmp0 (notify_subscribed_event, "job-state-changed") != 0 &&
            g_strcmp0 (notify_subscribed_event, "job-created") != 0)
                return;

        if (notify_job_id > 0) {
                if ((http = httpConnectEncrypt (cupsServer (), ippPort (),
                                                cupsEncryption ())) == NULL) {
                        g_debug ("Connection to CUPS server \'%s\' failed.", cupsServer ());
                } else {
                        job_uri = g_strdup_printf ("ipp://localhost/jobs/%d", notify_job_id);

                        request = ippNewRequest (IPP_GET_JOB_ATTRIBUTES);
                        ippAddString (request, IPP_TAG_OPERATION, IPP_TAG_URI,
                                      "job-uri", NULL, job_uri);
                        ippAddString (request, IPP_TAG_OPERATION, IPP_TAG_NAME,
                                     "requesting-user-name", NULL, cupsUser ());
                        ippAddString (request, IPP_TAG_OPERATION, IPP_TAG_KEYWORD,
                                     "requested-attributes", NULL, "job-originating-user-name");
                        response = cupsDoRequest (http, request, "/");

                        if (response) {
                                if (ippGetStatusCode (response) <= IPP_OK_CONFLICT &&
                                    (attr = ippFindAttribute(response, "job-originating-user-name",
                                                             IPP_TAG_NAME))) {
                                        if (g_strcmp0 (ippGetString (attr, 0, NULL), cupsUser ()) == 0)
                                                my_job = TRUE;
                                }
                                ippDelete(response);
                        }
                        g_free (job_uri);
                }
        }

        if (g_strcmp0 (notify_subscribed_event, "printer-added") == 0) {
                cupsFreeDests (manager->priv->num_dests, manager->priv->dests);
                manager->priv->num_dests = cupsGetDests (&manager->priv->dests);

                if (is_local_dest (printer_name,
                                   manager->priv->dests,
                                   manager->priv->num_dests)) {
                        /* Translators: New printer has been added */
                        primary_text = g_strdup (_("Printer added"));
                        secondary_text = g_strdup (printer_name);
                }
        } else if (g_strcmp0 (notify_subscribed_event, "printer-deleted") == 0) {
                if (is_local_dest (printer_name,
                                   manager->priv->dests,
                                   manager->priv->num_dests)) {
                        /* Translators: A printer has been removed */
                        primary_text = g_strdup (_("Printer removed"));
                        secondary_text = g_strdup (printer_name);
                }

                cupsFreeDests (manager->priv->num_dests, manager->priv->dests);
                manager->priv->num_dests = cupsGetDests (&manager->priv->dests);
        } else if (g_strcmp0 (notify_subscribed_event, "job-completed") == 0 && my_job) {
                g_hash_table_remove (manager->priv->printing_printers,
                                     printer_name);

                switch (job_state) {
                        case IPP_JOB_PENDING:
                        case IPP_JOB_HELD:
                        case IPP_JOB_PROCESSING:
                                break;
                        case IPP_JOB_STOPPED:
                                /* Translators: A print job has been stopped */
                                primary_text = g_strdup (C_("print job state", "Printing stopped"));
                                /* Translators: "print-job xy" on a printer */
                                secondary_text = g_strdup_printf (C_("print job", "\"%s\" on %s"), job_name, printer_name);
                                break;
                        case IPP_JOB_CANCELED:
                                /* Translators: A print job has been canceled */
                                primary_text = g_strdup (C_("print job state", "Printing canceled"));
                                /* Translators: "print-job xy" on a printer */
                                secondary_text = g_strdup_printf (C_("print job", "\"%s\" on %s"), job_name, printer_name);
                                break;
                        case IPP_JOB_ABORTED:
                                /* Translators: A print job has been aborted */
                                primary_text = g_strdup (C_("print job state", "Printing aborted"));
                                /* Translators: "print-job xy" on a printer */
                                secondary_text = g_strdup_printf (C_("print job", "\"%s\" on %s"), job_name, printer_name);
                                break;
                        case IPP_JOB_COMPLETED:
                                /* Translators: A print job has been completed */
                                primary_text = g_strdup (C_("print job state", "Printing completed"));
                                /* Translators: "print-job xy" on a printer */
                                secondary_text = g_strdup_printf (C_("print job", "\"%s\" on %s"), job_name, printer_name);
                                break;
                }
        } else if (g_strcmp0 (notify_subscribed_event, "job-state-changed") == 0 && my_job) {
                switch (job_state) {
                        case IPP_JOB_PROCESSING:
                                g_hash_table_insert (manager->priv->printing_printers,
                                                     g_strdup (printer_name), NULL);

                                /* Translators: A job is printing */
                                primary_text = g_strdup (C_("print job state", "Printing"));
                                /* Translators: "print-job xy" on a printer */
                                secondary_text = g_strdup_printf (C_("print job", "\"%s\" on %s"), job_name, printer_name);
                                break;
                        case IPP_JOB_STOPPED:
                                g_hash_table_remove (manager->priv->printing_printers,
                                                     printer_name);
                                /* Translators: A print job has been stopped */
                                primary_text = g_strdup (C_("print job state", "Printing stopped"));
                                /* Translators: "print-job xy" on a printer */
                                secondary_text = g_strdup_printf (C_("print job", "\"%s\" on %s"), job_name, printer_name);
                                break;
                        case IPP_JOB_CANCELED:
                                g_hash_table_remove (manager->priv->printing_printers,
                                                     printer_name);
                                /* Translators: A print job has been canceled */
                                primary_text = g_strdup (C_("print job state", "Printing canceled"));
                                /* Translators: "print-job xy" on a printer */
                                secondary_text = g_strdup_printf (C_("print job", "\"%s\" on %s"), job_name, printer_name);
                                break;
                        case IPP_JOB_ABORTED:
                                g_hash_table_remove (manager->priv->printing_printers,
                                                     printer_name);
                                /* Translators: A print job has been aborted */
                                primary_text = g_strdup (C_("print job state", "Printing aborted"));
                                /* Translators: "print-job xy" on a printer */
                                secondary_text = g_strdup_printf (C_("print job", "\"%s\" on %s"), job_name, printer_name);
                                break;
                        case IPP_JOB_COMPLETED:
                                g_hash_table_remove (manager->priv->printing_printers,
                                                     printer_name);
                                /* Translators: A print job has been completed */
                                primary_text = g_strdup (C_("print job state", "Printing completed"));
                                /* Translators: "print-job xy" on a printer */
                                secondary_text = g_strdup_printf (C_("print job", "\"%s\" on %s"), job_name, printer_name);
                                break;
                        default:
                                break;
                }
        } else if (g_strcmp0 (notify_subscribed_event, "job-created") == 0 && my_job) {
                if (job_state == IPP_JOB_PROCESSING) {
                        g_hash_table_insert (manager->priv->printing_printers,
                                             g_strdup (printer_name), NULL);

                        /* Translators: A job is printing */
                        primary_text = g_strdup (C_("print job state", "Printing"));
                        /* Translators: "print-job xy" on a printer */
                        secondary_text = g_strdup_printf (C_("print job", "\"%s\" on %s"), job_name, printer_name);
                }
        } else if (g_strcmp0 (notify_subscribed_event, "printer-state-changed") == 0) {
                cups_dest_t  *dest = NULL;
                const gchar  *tmp_printer_state_reasons = NULL;
                GSList       *added_reasons = NULL;
                GSList       *tmp_list = NULL;
                GList        *tmp;
                gchar       **old_state_reasons = NULL;
                gchar       **new_state_reasons = NULL;
                gint          i, j;

                /* Remove timeout which shows notification about possible disconnection of printer
                 * if "connecting-to-device" has vanished.
                 */
                if (printer_state_reasons == NULL ||
                    g_strrstr (printer_state_reasons, "connecting-to-device") == NULL) {
                        TimeoutData *data;

                        for (tmp = manager->priv->timeouts; tmp; tmp = g_list_next (tmp)) {
                                data = (TimeoutData *) tmp->data;
                                if (g_strcmp0 (printer_name, data->printer_name) == 0) {
                                        g_source_remove (data->timeout_id);
                                        manager->priv->timeouts = g_list_remove_link (manager->priv->timeouts, tmp);
                                        g_list_free_full (tmp, free_timeout_data);
                                        break;
                                }
                        }
                }

                for (tmp = manager->priv->active_notifications; tmp; tmp = g_list_next (tmp)) {
                        ReasonData *reason_data = (ReasonData *) tmp->data;
                        GList      *remove_list;

                        if (printer_state_reasons == NULL ||
                            (g_strcmp0 (printer_name, reason_data->printer_name) == 0 &&
                             g_strrstr (printer_state_reasons, reason_data->reason) == NULL)) {

                                if (reason_data->notification_close_id > 0 &&
                                    g_signal_handler_is_connected (reason_data->notification,
                                                                   reason_data->notification_close_id)) {
                                        g_signal_handler_disconnect (reason_data->notification,
                                                                     reason_data->notification_close_id);
                                        reason_data->notification_close_id = 0;
                                }

                                notify_notification_close (reason_data->notification, NULL);

                                remove_list = tmp;
                                tmp = g_list_next (tmp);
                                manager->priv->active_notifications =
                                        g_list_remove_link (manager->priv->active_notifications, remove_list);

                                g_list_free_full (remove_list, free_reason_data);
                        }
                }

                /* Check whether we are printing on this printer right now. */
                if (g_hash_table_lookup_extended (manager->priv->printing_printers, printer_name, NULL, NULL)) {
                        dest = cupsGetDest (printer_name,
                                            NULL,
                                            manager->priv->num_dests,
                                            manager->priv->dests);
                        if (dest)
                                tmp_printer_state_reasons = cupsGetOption ("printer-state-reasons",
                                                                           dest->num_options,
                                                                           dest->options);

                        if (tmp_printer_state_reasons)
                                old_state_reasons = g_strsplit (tmp_printer_state_reasons, ",", -1);

                        cupsFreeDests (manager->priv->num_dests, manager->priv->dests);
                        manager->priv->num_dests = cupsGetDests (&manager->priv->dests);

                        dest = cupsGetDest (printer_name,
                                            NULL,
                                            manager->priv->num_dests,
                                            manager->priv->dests);
                        if (dest)
                                tmp_printer_state_reasons = cupsGetOption ("printer-state-reasons",
                                                                           dest->num_options,
                                                                           dest->options);

                        if (tmp_printer_state_reasons)
                                new_state_reasons = g_strsplit (tmp_printer_state_reasons, ",", -1);

                        if (new_state_reasons)
                                qsort (new_state_reasons,
                                       g_strv_length (new_state_reasons),
                                       sizeof (gchar *),
                                       strcmp0);

                        if (old_state_reasons) {
                                qsort (old_state_reasons,
                                       g_strv_length (old_state_reasons),
                                       sizeof (gchar *),
                                       strcmp0);

                                j = 0;
                                for (i = 0; new_state_reasons && i < g_strv_length (new_state_reasons); i++) {
                                        while (old_state_reasons[j] &&
                                               g_strcmp0 (old_state_reasons[j], new_state_reasons[i]) < 0)
                                                j++;

                                        if (old_state_reasons[j] == NULL ||
                                            g_strcmp0 (old_state_reasons[j], new_state_reasons[i]) != 0)
                                                added_reasons = g_slist_append (added_reasons,
                                                                                new_state_reasons[i]);
                                }
                        } else {
                                for (i = 0; new_state_reasons && i < g_strv_length (new_state_reasons); i++) {
                                        added_reasons = g_slist_append (added_reasons,
                                                                        new_state_reasons[i]);
                                }
                        }

                        for (tmp_list = added_reasons; tmp_list; tmp_list = tmp_list->next) {
                                gchar *data = (gchar *) tmp_list->data;
                                known_reason = FALSE;
                                for (j = 0; j < G_N_ELEMENTS (reasons); j++) {
                                        if (strncmp (data,
                                                     reasons[j],
                                                     strlen (reasons[j])) == 0) {
                                                NotifyNotification *notification;
                                                known_reason = TRUE;

                                                if (g_strcmp0 (reasons[j], "connecting-to-device") == 0) {
                                                        TimeoutData *data;

                                                        data = g_new0 (TimeoutData, 1);
                                                        data->printer_name = g_strdup (printer_name);
                                                        data->primary_text = g_strdup ( _(statuses_first[j]));
                                                        data->secondary_text = g_strdup_printf ( _(statuses_second[j]), printer_name);
                                                        data->manager = manager;

                                                        data->timeout_id = g_timeout_add_seconds (CONNECTING_TIMEOUT, show_notification, data);
                                                        g_source_set_name_by_id (data->timeout_id, "[gnome-settings-daemon] show_notification");
                                                        manager->priv->timeouts = g_list_append (manager->priv->timeouts, data);
                                                } else {
                                                        ReasonData *reason_data;
                                                        gchar *second_row = g_strdup_printf ( _(statuses_second[j]), printer_name);

                                                        notification = notify_notification_new ( _(statuses_first[j]),
                                                                                                second_row,
                                                                                                "printer-symbolic");
                                                        notify_notification_set_app_name (notification, _("Printers"));
                                                        notify_notification_set_hint (notification,
                                                                                      "resident",
                                                                                      g_variant_new_boolean (TRUE));
                                                        notify_notification_set_timeout (notification, REASON_TIMEOUT);

                                                        reason_data = g_new0 (ReasonData, 1);
                                                        reason_data->printer_name = g_strdup (printer_name);
                                                        reason_data->reason = g_strdup (reasons[j]);
                                                        reason_data->notification = notification;
                                                        reason_data->manager = manager;

                                                        reason_data->notification_close_id =
                                                                g_signal_connect (notification,
                                                                                  "closed",
                                                                                  G_CALLBACK (notification_closed_cb),
                                                                                  reason_data);

                                                        manager->priv->active_notifications =
                                                                g_list_append (manager->priv->active_notifications, reason_data);

                                                        notify_notification_show (notification, NULL);

                                                        g_free (second_row);
                                                }
                                        }
                                }

                                if (!known_reason &&
                                    !reason_is_blacklisted (data)) {
                                        NotifyNotification *notification;
                                        ReasonData         *reason_data;
                                        gchar              *first_row;
                                        gchar              *second_row;
                                        gchar              *text = NULL;
                                        gchar              *ppd_file_name;
                                        ppd_file_t         *ppd_file;
                                        char                buffer[8192];

                                        ppd_file_name = g_strdup (cupsGetPPD (printer_name));
                                        if (ppd_file_name) {
                                                ppd_file = ppdOpenFile (ppd_file_name);
                                                if (ppd_file) {
                                                        gchar **tmpv;
                                                        static const char * const schemes[] = {
                                                                "text", "http", "help", "file"
                                                        };

                                                        tmpv = g_new0 (gchar *, G_N_ELEMENTS (schemes) + 1);
                                                        i = 0;
                                                        for (j = 0; j < G_N_ELEMENTS (schemes); j++) {
                                                                if (ppdLocalizeIPPReason (ppd_file, data, schemes[j], buffer, sizeof (buffer))) {
                                                                        tmpv[i++] = g_strdup (buffer);
                                                                }
                                                        }

                                                        if (i > 0)
                                                                text = g_strjoinv (", ", tmpv);
                                                        g_strfreev (tmpv);

                                                        ppdClose (ppd_file);
                                                }

                                                g_unlink (ppd_file_name);
                                                g_free (ppd_file_name);
                                        }


                                        if (g_str_has_suffix (data, "-report"))
                                                /* Translators: This is a title of a report notification for a printer */
                                                first_row = g_strdup (_("Printer report"));
                                        else if (g_str_has_suffix (data, "-warning"))
                                                /* Translators: This is a title of a warning notification for a printer */
                                                first_row = g_strdup (_("Printer warning"));
                                        else
                                                /* Translators: This is a title of an error notification for a printer */
                                                first_row = g_strdup (_("Printer error"));


                                        if (text == NULL)
                                                text = g_strdup (data);

                                        /* Translators: "Printer 'MyPrinterName': 'Description of the report/warning/error from a PPD file'." */
                                        second_row = g_strdup_printf (_("Printer '%s': '%s'."), printer_name, text);
                                        g_free (text);


                                        notification = notify_notification_new (first_row,
                                                                                second_row,
                                                                                "printer-symbolic");
                                        notify_notification_set_app_name (notification, _("Printers"));
                                        notify_notification_set_hint (notification,
                                                                      "resident",
                                                                      g_variant_new_boolean (TRUE));
                                        notify_notification_set_timeout (notification, REASON_TIMEOUT);

                                        reason_data = g_new0 (ReasonData, 1);
                                        reason_data->printer_name = g_strdup (printer_name);
                                        reason_data->reason = g_strdup (data);
                                        reason_data->notification = notification;
                                        reason_data->manager = manager;

                                        reason_data->notification_close_id =
                                                g_signal_connect (notification,
                                                                  "closed",
                                                                  G_CALLBACK (notification_closed_cb),
                                                                  reason_data);

                                        manager->priv->active_notifications =
                                                g_list_append (manager->priv->active_notifications, reason_data);

                                        notify_notification_show (notification, NULL);

                                        g_free (first_row);
                                        g_free (second_row);
                                }
                        }
                        g_slist_free (added_reasons);
                }

                if (new_state_reasons)
                        g_strfreev (new_state_reasons);

                if (old_state_reasons)
                        g_strfreev (old_state_reasons);
        }


        if (primary_text) {
                NotifyNotification *notification;
                notification = notify_notification_new (primary_text,
                                                        secondary_text,
                                                        "printer-symbolic");
                notify_notification_set_app_name (notification, _("Printers"));
                notify_notification_set_hint (notification, "transient", g_variant_new_boolean (TRUE));
                notify_notification_show (notification, NULL);
                g_object_unref (notification);
                g_free (primary_text);
                g_free (secondary_text);
        }
}