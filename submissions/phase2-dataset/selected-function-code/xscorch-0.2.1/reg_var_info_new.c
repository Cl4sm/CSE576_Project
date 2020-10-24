                               reg_type type, const reg_class *klass) {
/* reg_var_info_new */

   reg_var_info *new;

   if(reg == NULL) return(NULL);

   new = (reg_var_info *)malloc(sizeof(reg_var_info));
   if(new == NULL) return(NULL);

   strcopyb(new->name,  name, REG_SYMBOL_SIZE);
   new->klass = klass;
   new->type = type;
   new->next = *reg;

   /* Prepend the new variable definition */
   *reg = new;
   return(new);

}
