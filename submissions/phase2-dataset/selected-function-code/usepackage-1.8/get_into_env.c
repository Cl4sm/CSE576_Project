list_node* get_into_env(variable_t* var)
{
   variable_t* env_var;

   env_var = new(variable_t);
   env_var->name = var->name;
   env_var->type = var->type;

   switch(var->type)
   {
      case VAR_LIT_SET:
         env_var->literal = NULL;
         break;

      case VAR_PATH_SET:
         env_var->pathlist = NULL;
         break;

      case VAR_PATH_ADD:
         env_var->pathlist = make_pathlist(getenv(var->name));
         break;

      default:
         break;
   }

   return(add_to_tail(the_environment, env_var));
}
