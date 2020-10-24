/* sc_accessory_first
   Return the first/last viable accessory in the accessory config space. */

   sc_accessory_test_arg ata;
   sc_accessory_info *info;

   if(ac == NULL) return(NULL);

   ata.ac    = ac;
   ata.flags = flags;

   /* Try and find the first/last accessory that meets our criteria. */
   info = sc_registry_find_first(ac->registry, ac->registryclass,
                                 (flags & SC_ACCESSORY_SCAN_REVERSE) ? SC_REGISTRY_REVERSE : SC_REGISTRY_FORWARD,
                                 _sc_accessory_test_viewable, (long)(&ata));

   return(info);

}
