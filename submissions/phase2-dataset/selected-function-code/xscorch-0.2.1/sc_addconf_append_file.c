bool sc_addconf_append_file(sc_addconf_type type, const char *filename, void *container) {
/* sc_addconf_append_file
   Append a conf file to the data registry.  Returns true on success. */

   int count = 0;
   reg_var *item;
   reg_var *top;
   reg *reader;

   if(filename == NULL || container == NULL) return(false);

   /* Read the file into the file registry. */
   reader = reg_new(filename);
   if(reader == NULL) return(false);

   /* Inform the file regsitry how to parse the file. */
   switch(type) {
      case SC_ADDCONF_ACCESSORIES:
         reg_class_register_default_list(reader, _reg_class_accessories_parent);
         break;
      case SC_ADDCONF_SCORINGS:
         reg_class_register_default_list(reader, _reg_class_scorings_parent);
         break;
      case SC_ADDCONF_WEAPONS:
         reg_class_register_default_list(reader, _reg_class_weapons_parent);
         break;
   }

   /* Try to parse the conf file using the file registry. */
   if(!reg_load(reader)) {
      /* Failed to read file; signal error */
      printf("saddconf - aborting, can't load \"%s\"\n", filename);
      reg_free(&reader);
      return(false);
   }

   /* To simplify the switch below, we break this out with the preprocessor. */
   #define  iterate_calling(function)  while(item != NULL) { \
                                          if(function(container, reader, item)) ++count; \
                                          item = reg_get_next_var(item); \
                                       }

   /* Read the variables in order. */
   top = reg_get_top(reader);
   item = reg_get_block_head(top);
   switch(type) {
      case SC_ADDCONF_ACCESSORIES:
         iterate_calling(_sc_accessory_read_item);
         break;
      case SC_ADDCONF_SCORINGS:
         iterate_calling(_sc_scoring_read_item);
         break;
      case SC_ADDCONF_WEAPONS:
         iterate_calling(_sc_weapon_read_item);
         break;
   }

   /* Symmetry is Beautiful. */
   #undef  iterate_calling

   /* Release the registry. */
   reg_free(&reader);

   /* Let the next layer up know whether we found some configuration items. */
   if(count)
      return(true);
   else
      return(false);

}
