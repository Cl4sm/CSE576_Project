
   static GType sc_link_combo_type = 0;

   if(sc_link_combo_type == 0) {
      static const GTypeInfo sc_link_combo_info = {
         sizeof(ScLinkComboClass),        /* Size of the class object */
         NULL,                            /* Base initializer */
         NULL,                            /* Base finalizer */
         (GClassInitFunc)_sc_link_combo_class_init,
                                          /* Class initializer */
         NULL,                            /* Class finalizer */
         NULL,                            /* Class data pointer */
         sizeof(ScLinkCombo),             /* Size of an instance object */
         0,                               /* Number of preallocs */
         (GInstanceInitFunc)_sc_link_combo_init_obj,
                                          /* Instance initializer */
         NULL                             /* Value table */
      };
      sc_link_combo_type = g_type_register_static(gtk_combo_box_get_type(), "ScLinkCombo",
                                                  &sc_link_combo_info, 0);
   }

   return(sc_link_combo_type);

}
