gboolean
gvc_mixer_control_open (GvcMixerControl *control)
{
        int res;

        g_return_val_if_fail (GVC_IS_MIXER_CONTROL (control), FALSE);
        g_return_val_if_fail (control->priv->pa_context != NULL, FALSE);
        g_return_val_if_fail (pa_context_get_state (control->priv->pa_context) == PA_CONTEXT_UNCONNECTED, FALSE);

        pa_context_set_state_callback (control->priv->pa_context,
                                       _pa_context_state_cb,
                                       control);

        control->priv->state = GVC_STATE_CONNECTING;
        g_signal_emit (G_OBJECT (control), signals[STATE_CHANGED], 0, GVC_STATE_CONNECTING);
        res = pa_context_connect (control->priv->pa_context, NULL, (pa_context_flags_t) PA_CONTEXT_NOFAIL, NULL);
        if (res < 0) {
                g_warning ("Failed to connect context: %s",
                           pa_strerror (pa_context_errno (control->priv->pa_context)));
        }

        return res;
}