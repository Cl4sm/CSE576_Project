static void
grab_media_key (MediaKey            *key,
		GsdMediaKeysManager *manager)
{
	GrabData *data;
	char *tmp;

	ungrab_media_key (key, manager);

	tmp = get_key_string (manager, key);

	data = g_slice_new0 (GrabData);
	data->manager = manager;
	data->key = key;

	shell_key_grabber_call_grab_accelerator (manager->priv->key_grabber,
	                                         tmp, key->modes,
	                                         manager->priv->grab_cancellable,
	                                         grab_accelerator_complete,
	                                         data);

	g_free (tmp);
}