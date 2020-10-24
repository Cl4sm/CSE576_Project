static void
restart_and_install_updates (void)
{
        GDBusConnection *bus;
        const gchar *command;
        GError *error = NULL;

        command = "pkexec /usr/libexec/pk-trigger-offline-update";
        g_debug ("calling '%s'", command);
        if (!g_spawn_command_line_sync (command, NULL, NULL, NULL, &error)) {
                g_warning ("Failed to call '%s': %s\n", command, error->message);
                g_error_free (error);
                return;
        }

        g_debug ("calling org.gnome.SessionManager.Reboot");
        bus = g_bus_get_sync (G_BUS_TYPE_SESSION, NULL, NULL);
        g_dbus_connection_call (bus,
                                "org.gnome.SessionManager",
                                "/org/gnome/SessionManager",
                                "org.gnome.SessionManager",
                                "Reboot",
                                NULL,
                                NULL,
                                G_DBUS_CALL_FLAGS_NONE,
                                G_MAXINT,
                                NULL,
                                reboot_failed,
                                NULL);
        g_object_unref (bus);
}