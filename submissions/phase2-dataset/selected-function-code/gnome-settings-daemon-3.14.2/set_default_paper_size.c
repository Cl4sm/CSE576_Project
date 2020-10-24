static void
set_default_paper_size (const gchar *printer_name,
                        const gchar *ppd_file_name)
{
        GDBusProxy  *proxy;
        GVariant    *output;
        GError      *error = NULL;
        GVariantBuilder *builder;

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
                return;
        }

        /* Set default media size according to the locale
         * FIXME: Handle more than A4 and Letter:
         * https://bugzilla.gnome.org/show_bug.cgi?id=660769 */
        builder = g_variant_builder_new (G_VARIANT_TYPE ("as"));
        g_variant_builder_add (builder, "s", get_paper_size_from_locale ());

        output = g_dbus_proxy_call_sync (proxy,
                                         "PrinterAddOption",
                                         g_variant_new ("(ssas)",
                                                        printer_name ? printer_name : "",
                                                        "media",
                                                        builder),
                                         G_DBUS_CALL_FLAGS_NONE,
                                         DBUS_TIMEOUT,
                                         NULL,
                                         &error);

        if (output) {
                g_variant_unref (output);
        } else {
                if (!(error->domain == G_DBUS_ERROR &&
                      (error->code == G_DBUS_ERROR_SERVICE_UNKNOWN ||
                       error->code == G_DBUS_ERROR_UNKNOWN_METHOD)))
                        g_warning ("%s", error->message);
                g_error_free (error);
        }

        g_object_unref (proxy);
}