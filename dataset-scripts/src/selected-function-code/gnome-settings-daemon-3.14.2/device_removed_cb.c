static void
device_removed_cb (GdkDeviceManager *device_manager,
                   GdkDevice        *device,
                   GsdMouseManager  *manager)
{
	int id;

	/* Remove the device from the hash table so that
	 * device_is_ignored () doesn't check for blacklisted devices */
	g_object_get (G_OBJECT (device), "device-id", &id, NULL);
	g_hash_table_remove (manager->priv->blacklist,
			     GINT_TO_POINTER (id));

        if (device_is_ignored (manager, device) == FALSE) {
                run_custom_command (device, COMMAND_DEVICE_REMOVED);

                /* If a touchpad was to disappear... */
                set_disable_w_typing (manager, g_settings_get_boolean (manager->priv->touchpad_settings, KEY_TOUCHPAD_DISABLE_W_TYPING));

                ensure_touchpad_active (manager);
        }
}