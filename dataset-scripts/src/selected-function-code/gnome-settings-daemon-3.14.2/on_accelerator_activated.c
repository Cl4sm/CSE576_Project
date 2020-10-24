static void
on_accelerator_activated (ShellKeyGrabber     *grabber,
                          guint                accel_id,
                          guint                deviceid,
                          guint                timestamp,
                          GsdMediaKeysManager *manager)
{
        guint i;

        for (i = 0; i < manager->priv->keys->len; i++) {
                MediaKey *key;

                key = g_ptr_array_index (manager->priv->keys, i);

                if (key->accel_id != accel_id)
                        continue;

                if (key->key_type == CUSTOM_KEY)
                        do_custom_action (manager, deviceid, key, timestamp);
                else
                        do_action (manager, deviceid, key->key_type, timestamp);
                return;
        }
}