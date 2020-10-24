static gboolean
check_offline_update_cb (gpointer user_data)
{
        const gchar *message;
        const gchar *title;
        gboolean ret;
        gboolean success;
        gchar *error_code = NULL;
        gchar *error_details = NULL;
        gchar *packages = NULL;
        GError *error = NULL;
        GKeyFile *key_file = NULL;
        GsdUpdatesManager *manager = (GsdUpdatesManager *) user_data;
        guint i;
        guint num_packages = 1;
        NotifyNotification *notification;
        PkErrorEnum error_enum = PK_ERROR_ENUM_UNKNOWN;

        /* was any offline update attempted */
        if (!g_file_test (PK_OFFLINE_UPDATE_RESULTS_FILENAME, G_FILE_TEST_EXISTS))
                goto out;

        /* open the file and see what happened */
        key_file = g_key_file_new ();
        ret = g_key_file_load_from_file (key_file,
                                         PK_OFFLINE_UPDATE_RESULTS_FILENAME,
                                         G_KEY_FILE_NONE,
                                         &error);
        if (!ret) {
                g_warning ("failed to open %s: %s",
                           PK_OFFLINE_UPDATE_RESULTS_FILENAME,
                           error->message);
                g_error_free (error);
                goto out;
        }
        success = g_key_file_get_boolean (key_file,
                                          PK_OFFLINE_UPDATE_RESULTS_GROUP,
                                          "Success",
                                          NULL);
        if (success) {
                packages = g_key_file_get_string (key_file,
                                                  PK_OFFLINE_UPDATE_RESULTS_GROUP,
                                                  "Packages",
                                                  NULL);
                if (packages == NULL) {
                        g_warning ("No 'Packages' in %s",
                                   PK_OFFLINE_UPDATE_RESULTS_FILENAME);
                        goto out;
                }

                /* count the packages for translators */
                for (i = 0; packages[i] != '\0'; i++) {
                        if (packages[i] == ',')
                                num_packages++;
                }

                /* TRANSLATORS: title in the libnotify popup */
                title = ngettext ("Software Update Installed",
                                  "Software Updates Installed",
                                  num_packages);

                /* TRANSLATORS: message when we've done offline updates */
                message = ngettext ("An important OS update has been installed.",
                                    "Important OS updates have been installed.",
                                    num_packages);
        } else {
                /* get error details */
                manager->priv->offline_update_error = pk_error_new ();

                error_code = g_key_file_get_string (key_file,
                                                    PK_OFFLINE_UPDATE_RESULTS_GROUP,
                                                    "ErrorCode",
                                                    NULL);
                if (error_code != NULL)
                        error_enum = pk_error_enum_from_string (error_code);
                error_details = g_key_file_get_string (key_file,
                                                       PK_OFFLINE_UPDATE_RESULTS_GROUP,
                                                       "ErrorDetails",
                                                       NULL);
                g_object_set (manager->priv->offline_update_error,
                              "code", error_enum,
                              "details", error_details,
                              NULL);

                /* TRANSLATORS: title in the libnotify popup */
                title = _("Software Updates Failed");

                /* TRANSLATORS: message when we've not done offline updates */
                message = _("An important OS update failed to be installed.");
        }

        /* do the bubble */
        g_debug ("title=%s, message=%s", title, message);

        /* close any existing notification */
        if (manager->priv->notification_offline_success != NULL) {
                notify_notification_close (manager->priv->notification_offline_success, NULL);
                manager->priv->notification_offline_success = NULL;
        }

        notification = notify_notification_new (title,
                                                message,
                                                GSD_UPDATES_ICON_URGENT);
        notify_notification_set_hint_string (notification, "desktop-entry", "gpk-update-viewer");
        notify_notification_set_app_name (notification, _("Software Updates"));
        notify_notification_set_timeout (notification, -1);
        notify_notification_set_urgency (notification, NOTIFY_URGENCY_NORMAL);
        if (!success) {
                notify_notification_add_action (notification, "error-offline-updates",
                                                /* TRANSLATORS: button: review the offline update changes */
                                                _("Show details"), libnotify_action_cb, manager, NULL);
        }
        notify_notification_add_action (notification, "clear-offline-updates",
                                        /* TRANSLATORS: button: clear notification */
                                        _("OK"), libnotify_action_cb, manager, NULL);
        g_signal_connect (notification, "closed",
                          G_CALLBACK (on_notification_closed), NULL);
        manager->priv->notification_offline_success = notification;
        g_object_add_weak_pointer (G_OBJECT (manager->priv->notification_offline_success),
                                   (void **) &manager->priv->notification_offline_success);
        ret = notify_notification_show (notification, &error);
        if (!ret) {
                g_warning ("error: %s", error->message);
                g_error_free (error);
        }
out:
        g_free (packages);
        g_free (error_code);
        g_free (error_details);
        if (key_file != NULL)
                g_key_file_free (key_file);
        manager->priv->offline_update_id = 0;
        return FALSE;
}