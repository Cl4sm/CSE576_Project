static void
req_update_card (GvcMixerControl *control,
                 int              index)
{
        pa_operation *o;

        if (index < 0) {
                o = pa_context_get_card_info_list (control->priv->pa_context,
                                                   _pa_context_get_card_info_by_index_cb,
                                                   control);
        } else {
                o = pa_context_get_card_info_by_index (control->priv->pa_context,
                                                       index,
                                                       _pa_context_get_card_info_by_index_cb,
                                                       control);
        }

        if (o == NULL) {
                g_warning ("pa_context_get_card_info_by_index() failed");
                return;
        }
        pa_operation_unref (o);
}