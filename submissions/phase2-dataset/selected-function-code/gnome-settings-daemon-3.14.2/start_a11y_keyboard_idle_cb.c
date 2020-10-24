static gboolean
start_a11y_keyboard_idle_cb (GsdA11yKeyboardManager *manager)
{
        guint        event_mask;

        g_debug ("Starting a11y_keyboard manager");
        gnome_settings_profile_start (NULL);

        if (!xkb_enabled (manager))
                goto out;

        manager->priv->settings = g_settings_new (KEYBOARD_A11Y_SCHEMA);
        g_signal_connect (G_OBJECT (manager->priv->settings), "changed",
                          G_CALLBACK (keyboard_callback), manager);

        set_devicepresence_handler (manager);

        /* Get the original configuration from the server */
        manager->priv->desc = get_xkb_desc_rec (manager);

        event_mask = XkbControlsNotifyMask;
        event_mask |= XkbAccessXNotifyMask; /* make default when AXN_AXKWarning works */

        /* be sure to init before starting to monitor the server */
        set_server_from_gsettings (manager);

        XkbSelectEvents (GDK_DISPLAY_XDISPLAY (gdk_display_get_default ()),
                         XkbUseCoreKbd,
                         event_mask,
                         event_mask);

        gdk_window_add_filter (NULL,
                               (GdkFilterFunc) cb_xkb_event_filter,
                               manager);

 out:
        gnome_settings_profile_end (NULL);

        manager->priv->start_idle_id = 0;

        return FALSE;
}