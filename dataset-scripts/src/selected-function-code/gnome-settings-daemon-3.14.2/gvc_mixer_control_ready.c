static void
gvc_mixer_control_ready (GvcMixerControl *control)
{
        pa_operation *o;

        pa_context_set_subscribe_callback (control->priv->pa_context,
                                           _pa_context_subscribe_cb,
                                           control);
        o = pa_context_subscribe (control->priv->pa_context,
                                  (pa_subscription_mask_t)
                                  (PA_SUBSCRIPTION_MASK_SINK|
                                   PA_SUBSCRIPTION_MASK_SOURCE|
                                   PA_SUBSCRIPTION_MASK_SINK_INPUT|
                                   PA_SUBSCRIPTION_MASK_SOURCE_OUTPUT|
                                   PA_SUBSCRIPTION_MASK_CLIENT|
                                   PA_SUBSCRIPTION_MASK_SERVER|
                                   PA_SUBSCRIPTION_MASK_CARD),
                                  NULL,
                                  NULL);

        if (o == NULL) {
                g_warning ("pa_context_subscribe() failed");
                return;
        }
        pa_operation_unref (o);

        req_update_server_info (control, -1);
        req_update_card (control, -1);
        req_update_client_info (control, -1);
        req_update_sink_info (control, -1);
        req_update_source_info (control, -1);
        req_update_sink_input_info (control, -1);
        req_update_source_output_info (control, -1);


        control->priv->n_outstanding = 6;

        /* This call is not always supported */
        o = pa_ext_stream_restore_read (control->priv->pa_context,
                                        _pa_ext_stream_restore_read_cb,
                                        control);
        if (o != NULL) {
                pa_operation_unref (o);
                control->priv->n_outstanding++;

                pa_ext_stream_restore_set_subscribe_cb (control->priv->pa_context,
                                                        _pa_ext_stream_restore_subscribe_cb,
                                                        control);

                o = pa_ext_stream_restore_subscribe (control->priv->pa_context,
                                                     1,
                                                     NULL,
                                                     NULL);
                if (o != NULL) {
                        pa_operation_unref (o);
                }

        } else {
                g_debug ("Failed to initialized stream_restore extension: %s",
                         pa_strerror (pa_context_errno (control->priv->pa_context)));
        }
}