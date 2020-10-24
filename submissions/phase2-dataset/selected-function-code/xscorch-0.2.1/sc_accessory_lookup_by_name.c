sc_accessory_info *sc_accessory_lookup_by_name(const sc_accessory_config *ac, const char *name, int flags) {
/* sc_accessory_lookup_by_name
   Tries to find an accessory by roughly the requested name.
   This is much slower than sc_accessory_lookup. */

   sc_registry_iter *iter;
   sc_accessory_test_arg ata;
   sc_accessory_info *info;

   if(ac == NULL || name == NULL) return(NULL);

   /* Set up for iteration. */
   ata.name = name;
   iter = sc_registry_iter_new(ac->registry, ac->registryclass,
                               (flags & SC_ACCESSORY_SCAN_REVERSE) ? SC_REGISTRY_REVERSE : SC_REGISTRY_FORWARD,
                               _sc_accessory_test_lookup_name, (long)(&ata));
   if(iter == NULL) return(NULL);

   /* Iterate using the fast registry iterators. */
   info = (sc_accessory_info *)sc_registry_iterate(iter);

   /* Clean up. */
   sc_registry_iter_free(&iter);

   /* Verify that the rules in place allow viewing the accessory. */
   if(info != NULL && _sc_accessory_viewable(ac, info, flags))
      return(info);
   else
      return(NULL);

}
