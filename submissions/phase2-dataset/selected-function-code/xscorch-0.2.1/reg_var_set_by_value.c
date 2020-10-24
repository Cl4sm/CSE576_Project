bool reg_var_set_by_value(const reg *r, reg_var *b, const char *name, char *value) {
/* reg_var_set_by_value
   Adds a new variable, based on the contents of the string value.  First,
   this function attempts to set an integer variable; failing that, it will
   try seting a double variable, and as a last resort it uses a string
   variable.  */

   char *p;                /* Pointer to store result of strto? */
   int valint;             /* Attempted conversion to integer */
   double valdbl;          /* Attempted conversion to double */

   /* Make sure value isn't NULL */
   if(value == NULL || *value == '\0') return(false);

   /* Check for True/False and other boolean keywords */
   if(strequal(value, "True")  || strequal(value, "true"))  return(reg_var_set_boolean(r, b, name, true, REG_FORMAT_BOOL_TF));
   if(strequal(value, "Yes")   || strequal(value, "yes"))   return(reg_var_set_boolean(r, b, name, true, REG_FORMAT_BOOL_YN));
   if(strequal(value, "On")    || strequal(value, "on"))    return(reg_var_set_boolean(r, b, name, true, REG_FORMAT_BOOL_OO));
   if(strequal(value, "T")     || strequal(value, "t"))     return(reg_var_set_boolean(r, b, name, true, REG_FORMAT_BOOL_TNIL));
   if(strequal(value, "False") || strequal(value, "false")) return(reg_var_set_boolean(r, b, name, false, REG_FORMAT_BOOL_TF));
   if(strequal(value, "No")    || strequal(value, "no"))    return(reg_var_set_boolean(r, b, name, false, REG_FORMAT_BOOL_YN));
   if(strequal(value, "Off")   || strequal(value, "off"))   return(reg_var_set_boolean(r, b, name, false, REG_FORMAT_BOOL_OO));
   if(strequal(value, "NIL")   || strequal(value, "nil"))   return(reg_var_set_boolean(r, b, name, false, REG_FORMAT_BOOL_TNIL));

   /* Attempt to convert to an integer first (most restrictive) */
   valint = strtol(value, &p, 0);
   if(*p == '\0') return(reg_var_set_integer(r, b, name, valint));

   /* Attempt to convert to a double */
   valdbl = strtod(value, &p);
   if(*p == '\0') return(reg_var_set_doublev(r, b, name, valdbl));

   /* Assume the value is a string if it has a quote. */
   if(*value == '\"') {
      unescape_quoted(value);
      return(reg_var_set_string(r, b, name, value));
   }

   /* Failure mode */
   reg_error2(r, "Malformed value for \"%s\": %s\n", name, value);
   return(false);

}
