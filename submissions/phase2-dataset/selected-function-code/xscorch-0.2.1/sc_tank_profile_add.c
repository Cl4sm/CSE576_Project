bool sc_tank_profile_add(sc_tank_profile **plist, const char *datafile) {
/* sc_tank_profile_add
   Adds a new set of tank profiles to the linked list given, by reading data
   from the datafile given.  On success, this function returns true (even if
   the datafile contained no new tank profiles).  */

   reg_var *rv;      /* Regvar for each block */
   reg *r;           /* Registry structure */

   /* Sanity checks */
   if(plist == NULL || datafile == NULL) return(false);
   
   /* Compute filename and initialise registry */
   r = reg_new(datafile);
   if(r == NULL) {
      fprintf(stderr, "%s: core dump imminent. Critical alloc error, aborting.\n", datafile);
      return(false);
   }
   reg_class_add(r, "tank_profile_class");
   reg_class_register_vars(r, "tank_profile_class", _reg_tank_profile_class);

   /* Attempt to load the registry file containing tank profiles. */
   if(!reg_load(r)) {
      fprintf(stderr, "%s: core dump imminent. Cannot locate datafile \"%s\", aborting.\n", datafile, datafile);
      return(false);
   }

   /* Get the first variable in the file. */
   rv = reg_get_block_head(reg_get_top(r));
   while(rv != NULL) {
      /* Process this variable. */
      if(strequal(reg_get_var_class(rv), "tank_profile_class")) {
         if(!_sc_tank_profile_new(plist, r, rv)) {
            fprintf(stderr, "%s: core dump imminent. tank profile %s failed, aborting.\n", datafile, reg_get_var_name(rv));
            return(false);
         } /* Failed to create profile? */
      } else {
         fprintf(stderr, "%s: warning: \"%s\" isn't a valid class in this context, for variable \"%s\".\n", datafile, reg_get_var_class(rv), reg_get_var_name(rv));
      } /* Is regvar a valid tank profile? */
      rv = reg_get_next_var(rv);
   }

   /* Release the registry and exit. */
   reg_free(&r);
   return(true);

}
