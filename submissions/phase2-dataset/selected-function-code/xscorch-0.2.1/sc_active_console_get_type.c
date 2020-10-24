
   static GType sc_active_console_type = 0;

   if(sc_active_console_type == 0) {
      static const GTypeInfo sc_active_console_info = {
         sizeof(ScActiveConsoleClass),    /* Size of the class object */
         NULL,                            /* Base initializer */
         NULL,                            /* Base finalizer */
         (GClassInitFunc)_sc_active_console_class_init,
                                          /* Class initializer */
         NULL,                            /* Class finalizer */
         NULL,                            /* Class data pointer */
         sizeof(ScActiveConsole),         /* Size of an instance object */
         0,                               /* Number of preallocs */
         (GInstanceInitFunc)_sc_active_console_init_obj,
                                          /* Instance initializer */
         NULL                             /* Value table */
      };
      sc_active_console_type = g_type_register_static(sc_console_get_type(), "ScActiveConsole",
                                                      &sc_active_console_info, 0);
   }

   return(sc_active_console_type);

}
