
   static GType sc_link_entry_type = 0;

   if(sc_link_entry_type == 0) {
      static const GTypeInfo sc_link_entry_info = {
         sizeof(ScLinkEntryClass),        /* Size of the class object */
         NULL,                            /* Base initializer */
         NULL,                            /* Base finalizer */
         (GClassInitFunc)_sc_link_entry_class_init,
                                          /* Class initializer */
         NULL,                            /* Class finalizer */
         NULL,                            /* Class data pointer */
         sizeof(ScLinkEntry),             /* Size of an instance object */
         0,                               /* Number of preallocs */
         (GInstanceInitFunc)_sc_link_entry_init_obj,
                                          /* Instance initializer */
         NULL                             /* Value table */
      };
      sc_link_entry_type = g_type_register_static(gtk_entry_get_type(), "ScLinkEntry",
                                                  &sc_link_entry_info, 0);
   }

   return(sc_link_entry_type);

}
