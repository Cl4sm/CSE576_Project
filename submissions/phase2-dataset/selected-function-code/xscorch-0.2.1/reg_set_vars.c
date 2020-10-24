bool reg_set_vars(reg *r, reg_var *v, const char *path, const reg_var_data *data) {
/* reg_set_vars
   This function takes a list of reg_var_data's in data, which is
   terminated by a record whose name field is NULL.  It assigns
   each of the names given, relative to the reg_var v.  If path
   is not NULL, then it is prepended to each name in turn to
   construct the final variable name (which itself is relative
   to v).  If v is NULL, then the topmost variable in the registry
   is used.

   For each entry in the data list, this sets the variable to the
   value based on the description in data.  This function will
   overwrite existing bindings in the registry.  This function
   returns true if ALL assignments are successful (an assignment is
   successful if the variable is actually reassigned).  If any
   single assignemnt fails, then this function will attempt to
   assign the remainder of the variables in the data list, but
   will ultimately return false.

   Each entry in the data list contains a reg_value which is used to
   initialize the variable.  As such, it is able to handle multiple
   types of data.  This function can only define integers, floats,
   booleans and strings at this time.  It cannot bulk-define
   variables that represent subclasses yet, or variant variables.  */

   return(_reg_set_vars(r, v, path, data, true));

}
