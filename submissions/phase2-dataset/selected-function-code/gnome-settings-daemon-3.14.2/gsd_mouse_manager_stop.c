void
gsd_mouse_manager_stop (GsdMouseManager *manager)
{
        GsdMouseManagerPrivate *p = manager->priv;

        g_debug ("Stopping mouse manager");

        if (manager->priv->start_idle_id != 0) {
                g_source_remove (manager->priv->start_idle_id);
                manager->priv->start_idle_id = 0;
        }

        if (p->device_manager != NULL) {
                g_signal_handler_disconnect (p->device_manager, p->device_added_id);
                g_signal_handler_disconnect (p->device_manager, p->device_removed_id);
                p->device_manager = NULL;
        }

        g_clear_object (&p->mouse_a11y_settings);
        g_clear_object (&p->mouse_settings);
        g_clear_object (&p->touchpad_settings);

        set_locate_pointer (manager, FALSE);
}