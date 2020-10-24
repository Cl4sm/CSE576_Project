void print_env(void)
{
   list_node* node;
   variable_t* var;

   DEBUG(stderr, "dumping environment...\n");

   for (node = head(the_environment) ; node ; node = next(node))
   {
      var = (variable_t*) get_value(node);
      switch (var->type)
      {
         case VAR_LIT_SET:
            if (var->literal)
            {
	       if (csh_user)
		  printf("setenv %s \"%s\";\n", var->name, var->literal);
	       else
		  printf("%s=\"%s\";\nexport %s;\n", var->name, var->literal, var->name);
            }
            break;

         case VAR_PATH_ADD:
         case VAR_PATH_SET:
            if (var->pathlist)
            {
	       if (csh_user)
	       {
		  printf("setenv %s ", var->name);
		  print_path(var->name, var->pathlist);
		  printf(";\n");
	       }
	       else
	       {
		  printf("%s=", var->name);
		  print_path(var->name, var->pathlist);
		  printf(";\nexport %s;\n", var->name);
	       }
            }
            break;

         default:
            break;
      }
   }
}
