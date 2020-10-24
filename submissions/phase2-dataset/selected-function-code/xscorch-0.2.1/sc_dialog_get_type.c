GType sc_dialog_get_type(void) {
/* sc_dialog_get_type
   Return the GTK type for a dialog window.  */

   static GType sc_dialog_type = 0;

   if(sc_dialog_type == 0) {
      static const GTypeInfo sc_dialog_info = {
         sizeof(ScDialogClass),           /* Size of the class object */
         NULL,                            /* Base initializer */
         NULL,                            /* Base finalizer */
         (GClassInitFunc)_sc_dialog_class_init,
                                          /* Class initializer */
         NULL,                            /* Class finalizer */
         NULL,                            /* Class data pointer */
         sizeof(ScDialog),                /* Size of an instance object */
         0,                               /* Number of preallocs */
         (GInstanceInitFunc)_sc_dialog_init_obj,
                                          /* Instance initializer */
         NULL                             /* Value table */
      };
      sc_dialog_type = g_type_register_static(gtk_window_get_type(), "ScDialog",
                                              &sc_dialog_info, 0);
   }

   return(sc_dialog_type);

}
