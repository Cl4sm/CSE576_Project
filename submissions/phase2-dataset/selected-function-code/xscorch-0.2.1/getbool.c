/* Get-Boolean
   Returns "true" if the value in <s> is a boolean identifier string, in
   which case, <result> is set to true or false based on <s>. If <result>
   is NULL, then this function just returns whether or not <s> is a valid
   boolean identifier. Valid identifiers are...
      true: "true" "True" "TRUE" "t" "T" "yes" "Yes" "YES" "y" "Y" "+"
      false: "false" "False" "FALSE" "f" "F" "no" "No" "NO" "n" "N" "-" */

   if( streql(s, "true") || streql(s, "True") || streql(s, "TRUE")
    || streql(s, "t") || streql(s, "T")
    || streql(s, "yes") || streql(s, "Yes") || streql(s, "YES")
    || streql(s, "y") || streql(s, "Y")
    || streql(s, "+")) {
      if(result) *result = true;
   } else if(streql(s, "false") || streql(s, "False") || streql(s, "FALSE")
    || streql(s, "f") || streql(s, "F")
    || streql(s, "no") || streql(s, "No") || streql(s, "NO")
    || streql(s, "n") || streql(s, "N")
    || streql(s, "-")) {
      if(result) *result = false;
   } else return(false);
   return(true);

}
