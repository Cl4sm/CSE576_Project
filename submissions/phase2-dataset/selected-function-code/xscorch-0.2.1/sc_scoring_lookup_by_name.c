sc_scoring_info *sc_scoring_lookup_by_name(const sc_economy_config *ec, const char *name) {
/* sc_scoring_lookup_by_name
   Tries to find an economy scoring by roughly the requested name.
   This is much slower than sc_scoring_lookup. */

   sc_registry_iter *iter;
   sc_scoring_info *info;

   /* Don't make these into asserts without fixing
      sgtk/seconomy-gtk.c:_sc_economy_init_names_gtk() first. */
   if(ec == NULL || name == NULL) return(NULL);

   /* Set up for iteration. */
   iter = sc_registry_iter_new(ec->registry, ec->registryclass, SC_REGISTRY_FORWARD,
                               _sc_scoring_test_lookup_name, (long)(name));
   if(iter == NULL) return(NULL);

   /* Iterate using the fast registry iterators. */
   info = (sc_scoring_info *)sc_registry_iterate(iter);

   /* Clean up. */
   sc_registry_iter_free(&iter);

   return(info);

}
