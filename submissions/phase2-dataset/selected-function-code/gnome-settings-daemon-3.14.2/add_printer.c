static gboolean
add_printer (gchar *printer_name,
             gchar *device_uri,
             gchar *ppd_name,
             gchar *info,
             gchar *location)
{
        cups_dest_t *dests;
        GDBusProxy  *proxy;
        gboolean     success = FALSE;
        GVariant    *output;
        GError      *error = NULL;
        gint         num_dests;
        gint         i;

        if (!printer_name || !device_uri || !ppd_name)
                return FALSE;

        proxy = g_dbus_proxy_new_for_bus_sync (G_BUS_TYPE_SYSTEM,
                                               G_DBUS_PROXY_FLAGS_NONE,
                                               NULL,
                                               MECHANISM_BUS,
                                               "/",
                                               MECHANISM_BUS,
                                               NULL,
                                               &error);

        if (!proxy) {
                g_warning ("%s", error->message);
                g_error_free (error);
                return FALSE;
        }

        output = g_dbus_proxy_call_sync (proxy,
                                         "PrinterAdd",
                                         g_variant_new ("(sssss)",
                                                        printer_name,
                                                        device_uri,
                                                        ppd_name,
                                                        info ? info : "",
                                                        location ? location : ""),
                                         G_DBUS_CALL_FLAGS_NONE,
                                         DBUS_TIMEOUT,
                                         NULL,
                                         &error);

        if (output) {
                g_variant_unref (output);
        } else {
                g_warning ("%s", error->message);
                g_error_free (error);
        }

        g_object_unref (proxy);

        num_dests = cupsGetDests (&dests);
        for (i = 0; i < num_dests; i++)
                if (g_strcmp0 (dests[i].name, printer_name) == 0)
                        success = TRUE;
        cupsFreeDests (num_dests, dests);

        return success;
}