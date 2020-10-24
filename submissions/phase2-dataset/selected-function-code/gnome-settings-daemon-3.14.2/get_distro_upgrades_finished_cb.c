static void
get_distro_upgrades_finished_cb (GObject *object,
                                 GAsyncResult *res,
                                 GsdUpdatesManager *manager)
{
        const gchar *title;
        gboolean ret;
        gchar *name = NULL;
        GError *error = NULL;
        GPtrArray *array = NULL;
        GString *string = NULL;
        guint i;
        NotifyNotification *notification;
        PkClient *client = PK_CLIENT(object);
        PkDistroUpgrade *item;
        PkError *error_code = NULL;
        PkResults *results;
        PkUpdateStateEnum state;

        /* get the results */
        results = pk_client_generic_finish (PK_CLIENT(client), res, &error);
        if (results == NULL) {
                if (g_error_matches (error, G_IO_ERROR, G_IO_ERROR_CANCELLED)) {
                        g_error_free (error);
                        return;
                }
                if (error->domain != PK_CLIENT_ERROR ||
                    error->code != PK_CLIENT_ERROR_NOT_SUPPORTED) {
                        g_warning ("failed to get upgrades: %s",
                                   error->message);
                }
                g_error_free (error);
                goto out;
        }

        /* check error code */
        error_code = pk_results_get_error_code (results);
        if (error_code != NULL) {
                g_warning ("failed to get upgrades: %s, %s",
                           pk_error_enum_to_string (pk_error_get_code (error_code)),
                           pk_error_get_details (error_code));
                goto out;
        }

        /* process results */
        array = pk_results_get_distro_upgrade_array (results);

        /* any updates? */
        if (array->len == 0) {
                g_debug ("no upgrades");
                goto out;
        }

        /* do we do the notification? */
        ret = g_settings_get_boolean (manager->priv->settings_gsd,
                                      GSD_SETTINGS_NOTIFY_DISTRO_UPGRADES);
        if (!ret) {
                g_debug ("ignoring due to GSettings");
                goto out;
        }

        /* find the upgrade string */
        string = g_string_new ("");
        for (i=0; i < array->len; i++) {
                item = (PkDistroUpgrade *) g_ptr_array_index (array, i);
                g_object_get (item,
                              "name", &name,
                              "state", &state,
                              NULL);
                g_string_append_printf (string, "%s (%s)\n",
                                        name,
                                        pk_distro_upgrade_enum_to_string (state));
                g_free (name);
        }
        if (string->len != 0)
                g_string_set_size (string, string->len-1);

        /* TRANSLATORS: a distro update is available, e.g. Fedora 8 to Fedora 9 */
        title = _("Distribution upgrades available");
        notification = notify_notification_new (title,
                                                string->str,
                                                GSD_UPDATES_ICON_NORMAL);
        notify_notification_set_hint_string (notification, "desktop-entry", "gpk-update-viewer");
        notify_notification_set_app_name (notification, _("Software Updates"));
        notify_notification_set_timeout (notification, NOTIFY_EXPIRES_NEVER);
        notify_notification_set_urgency (notification, NOTIFY_URGENCY_NORMAL);
        notify_notification_add_action (notification, "ignore",
                                        /* TRANSLATORS: don't install updates now */
                                        _("Not Now"),
                                        libnotify_action_cb,
                                        manager, NULL);
        notify_notification_add_action (notification, "distro-upgrade-info",
                                        /* TRANSLATORS: provides more information about the upgrade */
                                        _("More information"),
                                        libnotify_action_cb,
                                        manager, NULL);
        g_signal_connect (notification, "closed",
                          G_CALLBACK (on_notification_closed), NULL);
        ret = notify_notification_show (notification, &error);
        if (!ret) {
                g_warning ("error: %s", error->message);
                g_error_free (error);
        }
out:
        if (error_code != NULL)
                g_object_unref (error_code);
        if (array != NULL)
                g_ptr_array_unref (array);
        if (string != NULL)
                g_string_free (string, TRUE);
        if (results != NULL)
                g_object_unref (results);
}