reg_class *reg_class_new(reg_class **classes, const char *name) {
/* reg_class_new
   Creates a new block class object.  Each block class holds the
   list of variables and their data types, for the given class.
   The new class is prepended to the <classes> linked list.  */

   reg_class *bc;

   if(classes == NULL || name == NULL) return(NULL);

   bc = (reg_class *)malloc(sizeof(reg_class));
   if(bc == NULL) return(NULL);

   strcopyb(bc->name, name, REG_SYMBOL_SIZE);
   bc->vars = NULL;
   bc->next = *classes;
   *classes = bc;
   return(bc);

}
