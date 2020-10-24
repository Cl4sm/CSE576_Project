                            reg_type type, const char *klass) {
/* reg_class_register_var
   Register a variable with name <name>, and specified <type> (and <klass>
   if a block), to the class entry <classname> in <r>.  NULL pointers are
   not tolerated by this function, except for <klass>.  If usedef is true,
   then the variable is registered with the indicated default value; other-
   wise, defvalue is ignored.  */

   reg_class *bc;

   if(r == NULL || classname == NULL || name == NULL) {
      return(false);
   }
   bc = reg_class_lookup(r->classes, classname);
   if(bc == NULL) {
      return(false);
   }
   if(reg_var_info_new(&bc->vars, name, type, reg_class_lookup(r->classes, klass)) == NULL) {
      return(false);
   }
   return(true);

}
