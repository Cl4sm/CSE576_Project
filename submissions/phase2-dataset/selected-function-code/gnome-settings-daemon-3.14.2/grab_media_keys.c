static void
grab_media_keys (GsdMediaKeysManager *manager)
{
        GVariantBuilder builder;
        int i;

        g_variant_builder_init (&builder, G_VARIANT_TYPE ("a(su)"));

        for (i = 0; i < manager->priv->keys->len; i++) {
                MediaKey *key;
                char *tmp;

                key = g_ptr_array_index (manager->priv->keys, i);
                tmp = get_key_string (manager, key);
                g_variant_builder_add (&builder, "(su)", tmp, key->modes);
                g_free (tmp);
        }

	shell_key_grabber_call_grab_accelerators (manager->priv->key_grabber,
	                                          g_variant_builder_end (&builder),
	                                          manager->priv->grab_cancellable,
	                                          grab_accelerators_complete,
	                                          manager);
}