                         const reg_var_data *data, bool overwrite) {
/* reg_set_var
   Sets the variable to the value based on the description in data.
   If overwrite is not true, then this function will not attempt to
   replace any existing value binding.  This function returns true
   if the assignment is successful, OR if overwrite = false and the
   value was already defined.  It returns false in other cases.

   The data contains a reg_value which is used to initialize the
   variable.  As such, it is able to handle multiple types of
   data.  This function can only define integers, floats, booleans
   and strings at this time.  It cannot bulk-define variables that
   represent subclasses yet, or variant variables.  */

   char thispath[REG_SYMBOL_SIZE];

   /* Construct the final name of the variable to define. */
   if(path == NULL) {
      strcopyb(thispath, data->name, sizeof(thispath));
   } else {
      sbprintf(thispath, sizeof(thispath), "%s/%s", path, data->name);
   }

   /* Figure out if we would be overwriting an existing var */
   if(!overwrite && reg_get_var(r, v, thispath) != NULL) {
      /* This is not considered an error case */
      return(true);
   }

   /* Assign the variable, depending on the type of data */
   switch(data->type) {
   case REG_INTEGER:
      return(reg_set_integer(r, v, thispath, data->value.integer));
   case REG_DOUBLEV:
      return(reg_set_doublev(r, v, thispath, data->value.doublev));
   case REG_BOOLEAN:
      return(reg_set_boolean_f(r, v, thispath, data->value.boolean.value,
                               data->value.boolean.format));
   case REG_STRING:
      return(reg_set_string(r, v, thispath, data->value.string));
   case REG_BLOCK:
   case REG_ANY:
      break;
   }

   /* If we're here, then something went wrong */
   return(false);

}
