static bool _sc_tank_profile_new(sc_tank_profile **plist, reg *r, reg_var *rv) {
/* sc_tank_profile_new
   This function, if successful, allocates and reads a new tank profile, and
   adds it to the END of the linked list specified.  True is returned on
   success; if an error occurred while parsing the tank profile, false is
   returned.  The tank profile data is read from the rowXX assignments in
   the tank_profile_class object, given in rv.  Caution, input pointers are
   not checked for sanity.  */

   sc_tank_profile *profile;  /* Newly allocated profile */
   char linename[0x1000];     /* Variable name for a scanline */
   char linedata[0x1000];     /* Assigned value for a scanline */
   unsigned char *p;          /* Writable pointer into profile data */
   int height;       /* Height of the profile */
   int width;        /* Width of the profile */
   int row;          /* Iterator: current row */
   int col;          /* Iterator: current column */
   int lsz;          /* Length of a scanline, in chars */

   /* Allocate a new profile */   
   profile = (sc_tank_profile *)malloc(sizeof(sc_tank_profile));
   if(profile == NULL) return(false);

   /* Initialise profile data members */
   profile->next        = NULL;
   profile->radius      = 0;
   profile->turretradius= 0;
   profile->shelfsize   = 5;
   profile->efficiency  = 100;
   profile->hardness    = 100;
   profile->mobile      = false;
   strcopyb(profile->name, reg_get_var_name(rv), sizeof(profile->name));

   /* Determine the radius of the profile. Note, if the radius wasn't given, 
      then radius will remain zero, and the trap below will catch it.  So we
      don't need to check return value here, yay. */
   reg_get_integer(r, rv, "radius",      &profile->radius);
   reg_get_integer(r, rv, "turretRadius",&profile->turretradius);
   reg_get_integer(r, rv, "shelfSize",   &profile->shelfsize);
   reg_get_integer(r, rv, "efficiency",  &profile->efficiency);
   reg_get_integer(r, rv, "hardness",    &profile->hardness);
   reg_get_boolean(r, rv, "mobile",      &profile->mobile);

   /* Update name of profile if immobile */
   if(!profile->mobile) {
      strconcatb(profile->name, " (I)", sizeof(profile->name));
   }
   
   /* Calculate and check width, height of the profile. */
   width  = 1 + profile->radius * 2;
   height = 1 + profile->radius;
   if(width <= 0 || height <= 0) {
      fprintf(stderr, "Tank \"%s\" has requires a positive radius, I saw %d\n", profile->name, profile->radius);
      free(profile);
      return(false);
   } /* Sanity check */
   if(profile->turretradius <= profile->radius) {
      fprintf(stderr, "Tank \"%s\" has an absurdly small (<= radius) turretRadius, I saw %d\n",
              profile->name, profile->turretradius);
      free(profile);
      return(false);
   } /* Sanity check */

   /* Allocate the profile data */   
   profile->data = (unsigned char *)malloc(sizeof(char) * width * height);
   if(profile->data == NULL) {
      free(profile);
      return(false);
   }

   /* Read in the profile data from the regvar given. */
   for(p = profile->data, row = 0; row < height; ++row) {
      /* Compute the name of the current scanline */
      sbprintf(linename, sizeof(linename), "row%02d", row);
      /* Attempt to read the scanline. If this call fails, then we assume
         the scanline is entirely blank. Otherwise, compute the length. */
      if(reg_get_string(r, rv, linename, linedata, sizeof(linedata))) {
         lsz = strblenn(linedata, sizeof(linedata));
      } else {
         lsz = 0;
      }
      /* Read data from the scanline. If we run out of characters, we will
         assume the remaining characters encoded for `blank' or clear. */
      for(col = 0; col < width; ++col, ++p) {
         /* Check if we ran off the scanline */
         if(col < lsz) {
            /* read character from scanline */
            if(linedata[col] == ' ' || linedata[col] == '.') {
               *p = SC_TANK_PROFILE_CLEAR;
            } else {
               *p = SC_TANK_PROFILE_SOLID;
            } /* Data read from scanline */
         } else {
            /* Assume clear */
            *p = SC_TANK_PROFILE_CLEAR;
         } /* Scanline check */ 
      } /* Reading the scanline */
   } /* Reading each row... */

   /* Verify that the tank profile is valid. */
   if(!_sc_tank_profile_verify(profile)) {
      free(profile->data);
      free(profile);
      return(false);
   }

   /* Add this profile to the end of the linked list. */
   while(*plist != NULL) plist = &((*plist)->next);
   *plist = profile;
   return(true);

}
