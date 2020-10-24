static void
update_default_sink_from_name (GvcMixerControl *control,
                               const char      *name)
{
        gboolean changed = FALSE;

        if ((control->priv->default_sink_name == NULL
             && name != NULL)
            || (control->priv->default_sink_name != NULL
                && name == NULL)
            || (name != NULL && strcmp (control->priv->default_sink_name, name) != 0)) {
                changed = TRUE;
        }

        if (changed) {
                GvcMixerStream *stream;
                g_free (control->priv->default_sink_name);
                control->priv->default_sink_name = g_strdup (name);

                stream = find_stream_for_name (control, name);
                _set_default_sink (control, stream);
        }
}