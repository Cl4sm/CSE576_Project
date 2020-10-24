static void
scp_handler (GsdPrintNotificationsManager *manager,
             gboolean                      start)
{
        if (start) {
                GError *error = NULL;
                char *args[2];

                if (manager->priv->scp_handler_spawned)
                        return;

                args[0] = LIBEXECDIR "/gsd-printer";
                args[1] = NULL;

                g_spawn_async (NULL, args, NULL,
                               0, NULL, NULL,
                               &manager->priv->scp_handler_pid, &error);

                manager->priv->scp_handler_spawned = (error == NULL);

                if (error) {
                        g_warning ("Could not execute system-config-printer-udev handler: %s",
                                   error->message);
                        g_error_free (error);
                }
        } else if (manager->priv->scp_handler_spawned) {
                kill (manager->priv->scp_handler_pid, SIGHUP);
                g_spawn_close_pid (manager->priv->scp_handler_pid);
                manager->priv->scp_handler_spawned = FALSE;
        }
}