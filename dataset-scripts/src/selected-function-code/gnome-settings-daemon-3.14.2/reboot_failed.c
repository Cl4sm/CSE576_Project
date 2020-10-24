static void
reboot_failed (GObject *source, GAsyncResult *res, gpointer data)
{
        GVariant *ret;
        const gchar *command;
        GError *error = NULL;

        ret = g_dbus_connection_call_finish (G_DBUS_CONNECTION (source), res, &error);
        if (ret) {
                g_variant_unref (ret);
                return;
        }

        if (error) {
                g_warning ("Calling org.gnome.SessionManager.Reboot failed: %s\n", error->message);
                g_error_free (error);
                return;
        }

        command = "pkexec /usr/libexec/pk-trigger-offline-update --cancel";
        g_debug ("calling '%s'", command);
        if (!g_spawn_command_line_sync (command, NULL, NULL, NULL, &error)) {
                g_warning ("Failed to call '%s': %s\n", command, error->message);
                g_error_free (error);
        }
}