static void
ax_response_callback (GsdA11yKeyboardManager *manager,
                      const char             *action,
                      guint                   revert_controls_mask,
                      gboolean                enabled)
{
        if (g_strcmp0 (action, "reject") == 0) {
                /* we're reverting, so we invert sense of 'enabled' flag */
                g_debug ("cancelling AccessX request");
                if (revert_controls_mask == XkbStickyKeysMask) {
                        g_settings_set_boolean (manager->priv->settings,
                                                "stickykeys-enable",
                                                !enabled);
                } else if (revert_controls_mask == XkbSlowKeysMask) {
                        g_settings_set_boolean (manager->priv->settings,
                                                "slowkeys-enable",
                                                !enabled);
                }

                set_server_from_gsettings (manager);
        }
}