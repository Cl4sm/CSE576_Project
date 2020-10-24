static void
libnotify_action_cb (NotifyNotification *notification,
                     gchar *action,
                     gpointer user_data)
{
        gboolean ret;
        GError *error = NULL;
        GsdUpdatesManager *manager = GSD_UPDATES_MANAGER (user_data);

        notify_notification_close (notification, NULL);
        if (g_strcmp0 (action, "ignore") == 0)
                goto out;
        if (g_strcmp0 (action, "distro-upgrade-info") == 0) {
                ret = g_spawn_command_line_async (DATADIR "/PackageKit/pk-upgrade-distro.sh",
                                                  &error);
                if (!ret) {
                        g_warning ("Failure launching pk-upgrade-distro.sh: %s",
                                   error->message);
                        g_error_free (error);
                }
                goto out;
        }
        if (g_strcmp0 (action, "show-update-viewer") == 0) {
                ret = g_spawn_command_line_async (BINDIR "/gpk-update-viewer",
                                                  &error);
                if (!ret) {
                        g_warning ("Failure launching update viewer: %s",
                                   error->message);
                        g_error_free (error);
                }
                goto out;
        }
        if (g_strcmp0 (action, "clear-offline-updates") == 0) {
                clear_offline_updates_message ();
                goto out;
        }
        if (g_strcmp0 (action, "error-offline-updates") == 0) {
                show_offline_updates_error (manager);
                goto out;
        }
        if (g_strcmp0 (action, "install") == 0) {
                restart_and_install_updates ();
                goto out;
        }
        if (g_strcmp0 (action, "cancel") == 0) {
                /* try to cancel */
                g_cancellable_cancel (manager->priv->cancellable);
                goto out;
        }
        g_warning ("unknown action id: %s", action);
out:
        return;
}