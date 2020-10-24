/* _sc_economy_init_names_gtk
   Make a list of names, appropriate for the combo box. */

   int count, idx;
   sc_scoring_info *info, *target;

   /* Find the first economy listed. */
   info = sc_registry_find_first(setup->ec->registry, setup->ec->registryclass,
                                 SC_REGISTRY_FORWARD, SC_REGISTRY_TEST_NULL, 0);

   /* There must exist at least one economy scoring! */
   assert(info != NULL);

   /* Try to find the currently selected scoring so we can hilight it. */
   target = sc_scoring_lookup_by_name(setup->ec, setup->ec->scoringname);
   if(target == NULL) target = info;

   /* Build a list of economy names, and find the currently selected one. */
   count = idx = 0;
   setup->enames = NULL;
   while(info != NULL) {
      if(count % 15 == 0)
         setup->enames = (const char **)realloc(setup->enames, (count + 16) * sizeof(char *));
      setup->enames[count] = info->name;
      if(info->ident == target->ident)
         idx = count;
      ++count;
      info = sc_registry_find_next(setup->ec->registry, setup->ec->registryclass, info->ident,
                                   SC_REGISTRY_FORWARD, SC_REGISTRY_TEST_NULL, 0);
   }

   /* The list should be null terminated. */
   if(count)
      setup->enames[count] = NULL;

   return(idx);

}
