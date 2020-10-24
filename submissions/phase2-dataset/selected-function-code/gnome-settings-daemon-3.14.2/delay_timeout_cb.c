static gboolean
delay_timeout_cb (gpointer data)
{
        guint i;
        gboolean ret;
        GString *string;
        GsdUpdatesFirmware *firmware = GSD_UPDATES_FIRMWARE (data);
        NotifyNotification *notification;
        GPtrArray *array;
        GError *error = NULL;
        PkPackage *item = NULL;
        const GsdUpdatesFirmwareRequest *req;
        gboolean has_data = FALSE;

        /* message string */
        string = g_string_new ("");

        /* try to find each firmware file in an available package */
        array = firmware->priv->array_requested;
        for (i=0; i<array->len; i++) {
                req = g_ptr_array_index (array, i);
                /* save to new array if we found one package for this file */
                item = check_available (firmware, req->filename);
                if (item != NULL) {
                        g_ptr_array_add (firmware->priv->packages_found, item);
                        g_object_unref (item);
                }
        }

        /* nothing to do */
        if (firmware->priv->packages_found->len == 0) {
                g_debug ("no packages providing any of the missing firmware");
                goto out;
        }

        /* check we don't want the same package more than once */
        remove_duplicate (firmware->priv->packages_found);

        /* have we got any models to array */
        for (i=0; i<array->len; i++) {
                req = g_ptr_array_index (array, i);
                if (req->model != NULL) {
                        has_data = TRUE;
                        break;
                }
        }

        /* TRANSLATORS: we need another package to keep udev quiet */
        g_string_append (string, _("Additional firmware is required to make hardware in this computer function correctly."));

        /* sdd what information we have */
        if (has_data) {
                g_string_append (string, "\n");
                for (i=0; i<array->len; i++) {
                        req = g_ptr_array_index (array, i);
                        if (req->model != NULL)
                                g_string_append_printf (string, "\n• %s", req->model);
                }
                g_string_append (string, "\n");
        }

        /* TRANSLATORS: title of libnotify bubble */
        notification = notify_notification_new (_("Additional firmware required"), string->str, NULL);
        notify_notification_set_hint_string (notification, "desktop-entry", "gpk-update-viewer");
        notify_notification_set_app_name (notification, _("Software Updates"));
        notify_notification_set_timeout (notification, NOTIFY_EXPIRES_NEVER);
        notify_notification_set_urgency (notification, NOTIFY_URGENCY_LOW);
        notify_notification_add_action (notification, "install-firmware",
                                        /* TRANSLATORS: button label */
                                        _("Install firmware"), libnotify_cb, firmware, NULL);
        notify_notification_add_action (notification, "ignore-devices",
                                        /* TRANSLATORS: we should ignore this device and not ask anymore */
                                        _("Ignore devices"), libnotify_cb, firmware, NULL);
        g_signal_connect (notification, "closed",
                          G_CALLBACK (on_notification_closed), NULL);

        ret = notify_notification_show (notification, &error);
        if (!ret) {
                g_warning ("error: %s", error->message);
                g_error_free (error);
        }

out:
        g_string_free (string, TRUE);
        /* never repeat */
        return FALSE;
}