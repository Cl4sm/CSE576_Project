/* reg_new
   Create a new registry structure, with the associated filename.
   The filename is used by reg_load() and reg_save() functions,
   and may be modified later.  */

   reg *r;        /* Newly constructed reg variable */
   reg_class *tc; /* Class registry for top class. */

   /* Create the new object */
   r = (reg *)malloc(sizeof(reg));
   if(r == NULL) return(NULL);

   /* Initialise class registry */
   r->classes = NULL;
   reg_class_add(r, REG_TOP);
   tc = reg_class_lookup(r->classes, REG_TOP);

   /* Initialise toplevel block */
   r->top = reg_var_new_block(r, REG_TOP, tc);
   if(r->top == NULL) {
      /* Failed to allocate toplevel */
      free(r);
      return(false);
   } /* Do we have toplevel block? */

   /* Initialise associated filename */
   r->filename = NULL;
   reg_set_name(r, filename);

   /* Return the register structure */
   return(r);

}
