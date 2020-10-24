static void
gvc_mixer_control_init (GvcMixerControl *control)
{
        control->priv = GVC_MIXER_CONTROL_GET_PRIVATE (control);

        control->priv->pa_mainloop = pa_glib_mainloop_new (g_main_context_default ());
        g_assert (control->priv->pa_mainloop);

        control->priv->pa_api = pa_glib_mainloop_get_api (control->priv->pa_mainloop);
        g_assert (control->priv->pa_api);

        control->priv->all_streams = g_hash_table_new_full (NULL, NULL, NULL, (GDestroyNotify)g_object_unref);
        control->priv->sinks = g_hash_table_new_full (NULL, NULL, NULL, (GDestroyNotify)g_object_unref);
        control->priv->sources = g_hash_table_new_full (NULL, NULL, NULL, (GDestroyNotify)g_object_unref);
        control->priv->sink_inputs = g_hash_table_new_full (NULL, NULL, NULL, (GDestroyNotify)g_object_unref);
        control->priv->source_outputs = g_hash_table_new_full (NULL, NULL, NULL, (GDestroyNotify)g_object_unref);
        control->priv->cards = g_hash_table_new_full (NULL, NULL, NULL, (GDestroyNotify)g_object_unref);
        control->priv->ui_outputs = g_hash_table_new_full (NULL, NULL, NULL, (GDestroyNotify)g_object_unref);
        control->priv->ui_inputs = g_hash_table_new_full (NULL, NULL, NULL, (GDestroyNotify)g_object_unref);

        control->priv->clients = g_hash_table_new_full (NULL, NULL, NULL, (GDestroyNotify)g_free);

        control->priv->state = GVC_STATE_CLOSED;
}