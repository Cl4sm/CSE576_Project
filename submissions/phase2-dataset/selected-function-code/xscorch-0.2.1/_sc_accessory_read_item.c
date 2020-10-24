/* _sc_accessory_read_item
   Read an accessory info and insert it into the registry. */

   int iterator;
   sc_accessory_info *info;
   char desc[SC_INVENTORY_MAX_DESC_LEN];
   char name[SC_INVENTORY_MAX_NAME_LEN];

   assert(ac != NULL);
   assert(reader != NULL);
   assert(item != NULL);

   /* Make sure we're loading an accessory definition here. */
   if(strcomp(reg_get_var_class(item), "accessories_class")) goto out;

   /* Set defaults for this item. */
   if(!reg_set_var_class_defaults(reader, item, NULL, _reg_accessories_class)) goto out;

   /* Allocate the new info struct. */
   info = (sc_accessory_info *)malloc(sizeof(sc_accessory_info));
   if(info == NULL) goto out;

   /* Find out what it'll be named... */
   if(!reg_get_string(reader, item, "accessoryName", name, SC_INVENTORY_MAX_NAME_LEN) || name[0] == '\0')
      goto out_info;

   /* Check the name for duplication */
   if(sc_accessory_lookup_by_name(ac, name, SC_ACCESSORY_LIMIT_NONE))
      goto out_info;

   /* Allocate space for the accessory's name. */
   info->name = (char *)malloc(strlenn(name) + 1);
   if(info->name == NULL) goto out_info;

   /* Copy the name from the temp buffer. */
   strcopyn(info->name, name, strlenn(name));

   /* Ask the registry for a unique ID for this accessory. */
   info->ident = sc_registry_get_next_new_key(ac->registry);
   if(info->ident < 0) goto out_name;

   /* Set in the other fields in the accessory_info struct */
   reg_get_integer(reader, item, "armsLevel", &info->armslevel);
   reg_get_integer(reader, item, "price",     &info->price);
   reg_get_integer(reader, item, "bundle",    &info->bundle);
   reg_get_integer(reader, item, "shielding", &info->shield);
   reg_get_integer(reader, item, "repulsion", &info->repulsion);
   reg_get_integer(reader, item, "fuel",      &info->fuel);

   reg_get_boolean(reader, item, "useless",   &info->useless);
   reg_get_boolean(reader, item, "indirect",  &info->indirect);

   info->state = reg_get_bitmask_from_values(reader, item, "stateFlags", 0,
                                             sc_accessory_state_bit_names(),
                                             sc_accessory_state_bit_items());

   /* Read in the accessory description if there is one. */
   if(!reg_get_string(reader, item, "description", desc, SC_INVENTORY_MAX_DESC_LEN) || desc[0] == '\0') {
      info->description = NULL;
   } else {
      info->description = (char *)malloc(strlenn(desc) + 1);
      /* If the description allocation fails, let it slide... */
      if(info->description != NULL)
         strcopyn(info->description, desc, strlenn(desc));
   }

   /* Clean up minor details such as player inventories. */
   for(iterator = 0; iterator < SC_MAX_PLAYERS; ++iterator) {
      if(SC_ACCESSORY_IS_INFINITE(info))
         info->inventories[iterator] = SC_INVENTORY_MAX_ITEMS;
      else
         info->inventories[iterator] = 0;
   }

   /* Register the accessory with the data registry. */
   if(!sc_registry_add_by_int(ac->registry, info, ac->registryclass, info->ident))
      goto out_desc;

   /* Successful exit, return true! */
   return(true);

   /* Error paths... */
   out_desc:
      free(info->description);
   out_name:
      free(info->name);
   out_info:
      free(info);
   out:
      return(false);

}
