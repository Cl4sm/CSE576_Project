static gboolean
printer_set_accepting_jobs (const gchar *printer_name,
                            gboolean     accepting_jobs,
                            const gchar *reason)
{
        GDBusProxy *proxy;
        gboolean    result = TRUE;
        GVariant   *output;
        GError     *error = NULL;

        if (!printer_name)
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
                                         "PrinterSetAcceptJobs",
                                         g_variant_new ("(sbs)",
                                                        printer_name,
                                                        accepting_jobs,
                                                        reason ? reason : ""),
                                         G_DBUS_CALL_FLAGS_NONE,
                                         DBUS_TIMEOUT,
                                         NULL,
                                         &error);

        if (output) {
                g_variant_unref (output);
        } else {
                g_warning ("%s", error->message);
                g_error_free (error);
                result = FALSE;
        }

        g_object_unref (proxy);

        return result;
}