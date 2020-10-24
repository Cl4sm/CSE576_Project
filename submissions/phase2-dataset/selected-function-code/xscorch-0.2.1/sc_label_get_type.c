
   static GType sc_label_type = 0;

   if(sc_label_type == 0) {
      static const GTypeInfo sc_label_info = {
         sizeof(ScLabelClass),            /* Size of the class object */
         NULL,                            /* Base initializer */
         NULL,                            /* Base finalizer */
         NULL,                            /* Class initializer */
         NULL,                            /* Class finalizer */
         NULL,                            /* Class data pointer */
         sizeof(ScLabel),                 /* Size of an instance object */
         0,                               /* Number of preallocs */
         NULL,                            /* Instance initializer */
         NULL                             /* Value table */
      };
      sc_label_type = g_type_register_static(gtk_label_get_type(), "ScLabel",
                                             &sc_label_info, 0);
   }

   return(sc_label_type);

}
