void
gsd_keyboard_manager_stop (GsdKeyboardManager *manager)
{
        GsdKeyboardManagerPrivate *p = manager->priv;

        g_debug ("Stopping keyboard manager");

        g_cancellable_cancel (p->cancellable);
        g_clear_object (&p->cancellable);

        g_clear_object (&p->settings);
        g_clear_object (&p->input_sources_settings);
        g_clear_object (&p->localed);

        if (p->device_manager != NULL) {
                g_signal_handler_disconnect (p->device_manager, p->device_added_id);
                g_signal_handler_disconnect (p->device_manager, p->device_removed_id);
                p->device_manager = NULL;
        }

	remove_xkb_filter (manager);
}