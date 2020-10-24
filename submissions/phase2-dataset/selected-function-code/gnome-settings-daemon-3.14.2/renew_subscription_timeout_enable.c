static void
renew_subscription_timeout_enable (GsdPrintNotificationsManager *manager,
                                   gboolean                      enable,
                                   gboolean                      with_connection_test)
{
        if (manager->priv->renew_source_id > 0)
                g_source_remove (manager->priv->renew_source_id);

        if (enable) {
                renew_subscription (manager);
                if (with_connection_test) {
                        manager->priv->renew_source_id =
                                g_timeout_add_seconds (RENEW_INTERVAL,
                                                       renew_subscription_with_connection_test,
                                                       manager);
                        g_source_set_name_by_id (manager->priv->renew_source_id, "[gnome-settings-daemon] renew_subscription_with_connection_test");
                } else {
                        manager->priv->renew_source_id =
                                g_timeout_add_seconds (RENEW_INTERVAL,
                                                       renew_subscription,
                                                       manager);
                        g_source_set_name_by_id (manager->priv->renew_source_id, "[gnome-settings-daemon] renew_subscription");
                }
        } else {
                manager->priv->renew_source_id = 0;
        }
}