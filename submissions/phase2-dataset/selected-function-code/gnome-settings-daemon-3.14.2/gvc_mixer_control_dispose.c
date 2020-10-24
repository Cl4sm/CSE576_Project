static void
gvc_mixer_control_dispose (GObject *object)
{
        GvcMixerControl *control = GVC_MIXER_CONTROL (object);

        if (control->priv->reconnect_id != 0) {
                g_source_remove (control->priv->reconnect_id);
                control->priv->reconnect_id = 0;
        }

        if (control->priv->pa_context != NULL) {
                pa_context_unref (control->priv->pa_context);
                control->priv->pa_context = NULL;
        }

        if (control->priv->default_source_name != NULL) {
                g_free (control->priv->default_source_name);
                control->priv->default_source_name = NULL;
        }
        if (control->priv->default_sink_name != NULL) {
                g_free (control->priv->default_sink_name);
                control->priv->default_sink_name = NULL;
        }

        if (control->priv->pa_mainloop != NULL) {
                pa_glib_mainloop_free (control->priv->pa_mainloop);
                control->priv->pa_mainloop = NULL;
        }

        if (control->priv->all_streams != NULL) {
                g_hash_table_destroy (control->priv->all_streams);
                control->priv->all_streams = NULL;
        }

        if (control->priv->sinks != NULL) {
                g_hash_table_destroy (control->priv->sinks);
                control->priv->sinks = NULL;
        }
        if (control->priv->sources != NULL) {
                g_hash_table_destroy (control->priv->sources);
                control->priv->sources = NULL;
        }
        if (control->priv->sink_inputs != NULL) {
                g_hash_table_destroy (control->priv->sink_inputs);
                control->priv->sink_inputs = NULL;
        }
        if (control->priv->source_outputs != NULL) {
                g_hash_table_destroy (control->priv->source_outputs);
                control->priv->source_outputs = NULL;
        }
        if (control->priv->clients != NULL) {
                g_hash_table_destroy (control->priv->clients);
                control->priv->clients = NULL;
        }
        if (control->priv->cards != NULL) {
                g_hash_table_destroy (control->priv->cards);
                control->priv->cards = NULL;
        }
        if (control->priv->ui_outputs != NULL) {
                g_hash_table_destroy (control->priv->ui_outputs);
                control->priv->ui_outputs = NULL;
        }
        if (control->priv->ui_inputs != NULL) {
                g_hash_table_destroy (control->priv->ui_inputs);
                control->priv->ui_inputs = NULL;
        }

        G_OBJECT_CLASS (gvc_mixer_control_parent_class)->dispose (object);
}