static void
gsd_ldsm_dialog_get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
        GsdLdsmDialog *self;
	
        g_return_if_fail (GSD_IS_LDSM_DIALOG (object));
	
        self = GSD_LDSM_DIALOG (object);

        switch (prop_id)
        {
        case PROP_OTHER_USABLE_PARTITIONS:
                g_value_set_boolean (value, self->priv->other_usable_partitions);
                break;
        case PROP_OTHER_PARTITIONS:
                g_value_set_boolean (value, self->priv->other_partitions);
                break;
        case PROP_HAS_TRASH:
                g_value_set_boolean (value, self->priv->has_trash);
                break;
        case PROP_SPACE_REMAINING:
                g_value_set_int64 (value, self->priv->space_remaining);
                break;
        case PROP_PARTITION_NAME:
                g_value_set_string (value, self->priv->partition_name);
                break;
        case PROP_MOUNT_PATH:
                g_value_set_string (value, self->priv->mount_path);
                break;
        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
                break;
        }
}