static gboolean
idle_reconnect (gpointer data)
{
        GvcMixerControl *control = GVC_MIXER_CONTROL (data);
        GHashTableIter iter;
        gpointer key, value;

        g_return_val_if_fail (control, FALSE);

        if (control->priv->pa_context) {
                pa_context_unref (control->priv->pa_context);
                control->priv->pa_context = NULL;
                gvc_mixer_new_pa_context (control);
        }

        remove_all_streams (control, control->priv->sinks);
        remove_all_streams (control, control->priv->sources);
        remove_all_streams (control, control->priv->sink_inputs);
        remove_all_streams (control, control->priv->source_outputs);

        g_hash_table_iter_init (&iter, control->priv->clients);
        while (g_hash_table_iter_next (&iter, &key, &value))
                g_hash_table_iter_remove (&iter);

        gvc_mixer_control_open (control); /* cannot fail */

        control->priv->reconnect_id = 0;
        return FALSE;
}