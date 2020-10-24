bool _sc_scoring_read_item(sc_economy_config *ec, reg *reader, reg_var *item) {
/* _sc_scoring_read_item
   Read an economy scoring info and insert it into the registry. */

   sc_scoring_info *info;
   char desc[SC_ECONOMY_MAX_DESC_LEN];
   char name[SC_ECONOMY_MAX_NAME_LEN];

   assert(ec != NULL);
   assert(reader != NULL);
   assert(item != NULL);

   /* Make sure we're loading an scoring definition here. */
   if(strcomp(reg_get_var_class(item), "scorings_class")) goto out;

   /* Set defaults for this item. */
   if(!reg_set_var_class_defaults(reader, item, NULL, _reg_scorings_class)) goto out;

   /* Allocate the new info struct. */
   info = (sc_scoring_info *)malloc(sizeof(sc_scoring_info));
   if(info == NULL) goto out;

   /* Find out what it'll be named... */
   if(!reg_get_string(reader, item, "scoringName", name, SC_ECONOMY_MAX_NAME_LEN) || name[0] == '\0')
      goto out_info;

   /* Check the name for duplication */
   if(sc_scoring_lookup_by_name(ec, name))
      goto out_info;

   /* Allocate space for the scoring's name. */
   info->name = (char *)malloc(strlenn(name) + 1);
   if(info->name == NULL) goto out_info;

   /* Copy the name from the temp buffer. */
   strcopyn(info->name, name, strlenn(name));

   /* Ask the registry for a unique ID for this economy scoring. */
   info->ident = sc_registry_get_next_new_key(ec->registry);
   if(info->ident < 0) goto out_name;

   /* Set in the other fields in the scoring_info struct. */
   reg_get_integer(reader, item, "survivalBonus", &info->survivalbonus);
   reg_get_integer(reader, item, "damageBonus",   &info->damagebonus);
   reg_get_integer(reader, item, "killBonus",     &info->killbonus);
   reg_get_integer(reader, item, "damageLoss",    &info->damageloss);
   reg_get_integer(reader, item, "deathLoss",     &info->deathloss);
   reg_get_integer(reader, item, "suicideLoss",   &info->suicideloss);
   reg_get_boolean(reader, item, "fixed",         &info->fixed);

   /* Read in the scoring description if there is one. */
   if(!reg_get_string(reader, item, "description", desc, SC_ECONOMY_MAX_DESC_LEN) || desc[0] == '\0') {
      info->description = NULL;
   } else {
      info->description = (char *)malloc(strlenn(desc) + 1);
      /* If the description allocation fails, let it slide... */
      if(info->description != NULL)
         strcopyn(info->description, desc, strlenn(desc));
   }

   /* Register the scoring with the data registry. */
   if(!sc_registry_add_by_int(ec->registry, info, ec->registryclass, info->ident))
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
