static void
gsd_ldsm_dialog_class_init (GsdLdsmDialogClass *klass)
{
        GObjectClass* object_class = G_OBJECT_CLASS (klass);

        object_class->finalize = gsd_ldsm_dialog_finalize;
        object_class->set_property = gsd_ldsm_dialog_set_property;
        object_class->get_property = gsd_ldsm_dialog_get_property;

        g_object_class_install_property (object_class,
                                         PROP_OTHER_USABLE_PARTITIONS,
                                         g_param_spec_boolean ("other-usable-partitions",
                                                               "other-usable-partitions",
                                                               "Set to TRUE if there are other usable partitions on the system",
                                                               FALSE,
                                                               G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));

        g_object_class_install_property (object_class,
                                         PROP_OTHER_PARTITIONS,
                                         g_param_spec_boolean ("other-partitions",
                                                               "other-partitions",
                                                               "Set to TRUE if there are other partitions on the system",
                                                               FALSE,
                                                               G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));

        g_object_class_install_property (object_class,
                                         PROP_HAS_TRASH,
                                         g_param_spec_boolean ("has-trash",
                                                               "has-trash",
                                                               "Set to TRUE if the partition has files in it's trash folder that can be deleted",
                                                               FALSE,
                                                               G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));
	                                                       	                                                       
        g_object_class_install_property (object_class,
                                         PROP_SPACE_REMAINING,
                                         g_param_spec_int64 ("space-remaining",
                                                             "space-remaining",
                                                             "Specify how much space is remaining in bytes",
                                                             G_MININT64, G_MAXINT64, 0,
                                                             G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));
	                                                   
        g_object_class_install_property (object_class,
                                         PROP_PARTITION_NAME,
                                         g_param_spec_string ("partition-name",
                                                              "partition-name",
                                                              "Specify the name of the partition",
                                                              "Unknown",
                                                              G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));
	                                                      
        g_object_class_install_property (object_class,
                                         PROP_MOUNT_PATH,
                                         g_param_spec_string ("mount-path",
                                                              "mount-path",
                                                              "Specify the mount path for the partition",
                                                              "Unknown",
                                                              G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY));

        g_type_class_add_private (klass, sizeof (GsdLdsmDialogPrivate));
}