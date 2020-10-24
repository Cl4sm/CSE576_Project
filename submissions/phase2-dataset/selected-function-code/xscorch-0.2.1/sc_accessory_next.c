/* sc_accessory_next
   Advance to the next/prev accessory in the list (with wrapping).  */

   sc_accessory_test_arg ata;
   sc_accessory_info *nxtnfo;

   if(ac == NULL || info == NULL) return(NULL);

   /* Set up the flags and args for the mission. */
   ata.ac    = ac;
   ata.flags = flags;

   /* Try and find the first/last accessory that meets our criteria. */
   nxtnfo = sc_registry_find_next(ac->registry, ac->registryclass, info->ident,
                                  (flags & SC_ACCESSORY_SCAN_REVERSE) ? SC_REGISTRY_REVERSE : SC_REGISTRY_FORWARD,
                                  _sc_accessory_test_viewable, (long)(&ata));

   /* In case we iterated off the end of the list, wrap to the beginning. */
   if(nxtnfo == NULL)
      nxtnfo = sc_accessory_first(ac, flags);

   return(nxtnfo);

}
