static void
_pa_context_state_cb (pa_context *context,
                      void       *userdata)
{
        GvcMixerControl *control = GVC_MIXER_CONTROL (userdata);

        switch (pa_context_get_state (context)) {
        case PA_CONTEXT_UNCONNECTED:
        case PA_CONTEXT_CONNECTING:
        case PA_CONTEXT_AUTHORIZING:
        case PA_CONTEXT_SETTING_NAME:
                break;

        case PA_CONTEXT_READY:
                gvc_mixer_control_ready (control);
                break;

        case PA_CONTEXT_FAILED:
                control->priv->state = GVC_STATE_FAILED;
                g_signal_emit (control, signals[STATE_CHANGED], 0, GVC_STATE_FAILED);
                if (control->priv->reconnect_id == 0)
                        control->priv->reconnect_id = g_timeout_add_seconds (RECONNECT_DELAY, idle_reconnect, control);
                break;

        case PA_CONTEXT_TERMINATED:
        default:
                /* FIXME: */
                break;
        }
}