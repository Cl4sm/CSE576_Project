bool _sc_weapon_read_item(sc_weapon_config *wc, reg *reader, reg_var *item) {
/* _sc_weapon_read_item
   Read a weapon info and insert it into the registry. */

   int iterator;
   sc_weapon_info *info;
   sc_weapon_info *child;
   char childname[SC_INVENTORY_MAX_NAME_LEN];
   char desc[SC_INVENTORY_MAX_DESC_LEN];
   char name[SC_INVENTORY_MAX_NAME_LEN];

   assert(wc != NULL);
   assert(reader != NULL);
   assert(item != NULL);

   /* Make sure we're loading a weapon definition here. */
   if(strcomp(reg_get_var_class(item), "weapons_class")) goto out;

   /* Set defaults for this item. */
   if(!reg_set_var_class_defaults(reader, item, NULL, _reg_weapons_class)) goto out;

   /* Allocate the new info struct. */
   info = (sc_weapon_info *)malloc(sizeof(sc_weapon_info));
   if(info == NULL) goto out;

   /* Find out what it'll be named... */
   if(!reg_get_string(reader, item, "weaponName", name, SC_INVENTORY_MAX_NAME_LEN) || name[0] == '\0')
      goto out_info;

   /* Check the name for duplication */
   if(sc_weapon_lookup_by_name(wc, name, SC_WEAPON_LIMIT_NONE))
      goto out_info;

   /* Allocate space for the weapon's name. */
   info->name = (char *)malloc(strlenn(name) + 1);
   if(info->name == NULL) goto out_info;

   /* Copy the name from the temp buffer. */
   strcopyn(info->name, name, strlenn(name));

   /* Ask the registry for a unique ID for this weapon. */
   info->ident = sc_registry_get_next_new_key(wc->registry);
   if(info->ident < 0) goto out_name;

   /* Set in the other fields in the weapon_info struct. */
   reg_get_integer(reader, item, "armsLevel", &info->armslevel);
   reg_get_integer(reader, item, "price",     &info->price);
   reg_get_integer(reader, item, "bundle",    &info->bundle);
   reg_get_integer(reader, item, "radius",    &info->radius);
   reg_get_integer(reader, item, "force",     &info->force);
   reg_get_integer(reader, item, "liquid",    &info->liquid);
   reg_get_integer(reader, item, "scatter",   &info->scatter);
   reg_get_integer(reader, item, "children",  &info->children);

   /* Convert the angle for the explosion from degrees to radians. */
   reg_get_doublev(reader, item, "angularWidth", &info->angular_width);
   info->angular_width *= M_PI / 180.0;

   reg_get_boolean(reader, item, "useless",   &info->useless);
   reg_get_boolean(reader, item, "indirect",  &info->indirect);

   info->state = reg_get_bitmask_from_values(reader, item, "stateFlags",   0,
                                             sc_weapon_state_bit_names(),
                                             sc_weapon_state_bit_items());
   info->ph_fl = reg_get_bitmask_from_values(reader, item, "phoenixFlags", 0,
                                             sc_phoenix_flags_bit_names(),
                                             sc_phoenix_flags_bit_items());

   /* Read in the weapon description if there is one. */
   if(!reg_get_string(reader, item, "description", desc, SC_INVENTORY_MAX_DESC_LEN) || desc[0] == '\0') {
      info->description = NULL;
   } else {
      info->description = (char *)malloc(strlenn(desc) + 1);
      /* If the description allocation fails, let it slide... */
      if(info->description != NULL)
         strcopyn(info->description, desc, strlenn(desc));
   }

   /* Set the child if there is one. */
   if(reg_get_string(reader, item, "phoenixChild", childname, SC_INVENTORY_MAX_NAME_LEN) && childname[0] != '\0') {
      /* Try and find the child. */
      if(strsimilar(info->name, childname))
         child = info;
      else
         child = sc_weapon_lookup_by_name(wc, childname, SC_WEAPON_LIMIT_NONE);

      /* If we found the child, set it.  Otherwise, warn. */
      if(child == NULL)
         printf("saddconf - warning, \"%s\" has missing child \"%s\"\n", name, childname);
      else
         info->ph_ch = child->ident;
   }

   /* Test for weapon acceptance... */
   if(SC_WEAPON_IS_PHOENIX(info) && !sc_phoenix_verify(wc, info)) {
      printf("saddconf - \"%s\" is an invalid phoenix weapon, rejecting it\n", name);
      goto out_desc;
   }

   /* Clean up minor details such as player inventories. */
   for(iterator = 0; iterator < SC_MAX_PLAYERS; ++iterator) {
      if(SC_WEAPON_IS_INFINITE(info))
         info->inventories[iterator] = SC_INVENTORY_MAX_ITEMS;
      else
         info->inventories[iterator] = 0;
   }

   /* Register the weapon with the data registry. */
   if(!sc_registry_add_by_int(wc->registry, info, wc->registryclass, info->ident))
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
