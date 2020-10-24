static void _sc_link_spinf_class_init(ScLinkSpinFClass *klass) {

   GtkObjectClass *object_class = (GtkObjectClass *)klass;

   parent_class = g_type_class_peek(gtk_spin_button_get_type());

   _sc_link_spinf_signals[MODIFIED_SIGNAL] =
      g_signal_new("modified",                     /* Signal name */
                   G_TYPE_FROM_CLASS(object_class),/* Type of object the signal applies to */
                   G_SIGNAL_RUN_LAST,              /* Signal flags; run this signal last */
                   offsetof(ScLinkSpinFClass, modified),
                                                   /* Offset to signal handler in class */
                   NULL,                           /* Signal accumulator function */
                   NULL,                           /* Signal accumulator data */
                   g_cclosure_marshal_VOID__VOID,  /* Marshal function for this signal */
                   G_TYPE_NONE,                    /* Return type for the marshaller */
                   0                               /* Number of extra parametres to pass */
                  );

   klass->modified = NULL;

}
