static void
gvc_mixer_stream_finalize (GObject *object)
{
        GvcMixerStream *mixer_stream;

        g_return_if_fail (object != NULL);
        g_return_if_fail (GVC_IS_MIXER_STREAM (object));

        mixer_stream = GVC_MIXER_STREAM (object);

        g_return_if_fail (mixer_stream->priv != NULL);

        g_object_unref (mixer_stream->priv->channel_map);
        mixer_stream->priv->channel_map = NULL;

        g_free (mixer_stream->priv->name);
        mixer_stream->priv->name = NULL;

        g_free (mixer_stream->priv->description);
        mixer_stream->priv->description = NULL;

        g_free (mixer_stream->priv->application_id);
        mixer_stream->priv->application_id = NULL;

        g_free (mixer_stream->priv->icon_name);
        mixer_stream->priv->icon_name = NULL;

        g_free (mixer_stream->priv->form_factor);
        mixer_stream->priv->form_factor = NULL;

        g_free (mixer_stream->priv->sysfs_path);
        mixer_stream->priv->sysfs_path = NULL;

        g_free (mixer_stream->priv->port);
        mixer_stream->priv->port = NULL;

        g_free (mixer_stream->priv->human_port);
        mixer_stream->priv->human_port = NULL;

        g_list_foreach (mixer_stream->priv->ports, (GFunc) free_port, NULL);
        g_list_free (mixer_stream->priv->ports);
        mixer_stream->priv->ports = NULL;

       if (mixer_stream->priv->change_volume_op) {
               pa_operation_unref(mixer_stream->priv->change_volume_op);
               mixer_stream->priv->change_volume_op = NULL;
       }

        G_OBJECT_CLASS (gvc_mixer_stream_parent_class)->finalize (object);
}