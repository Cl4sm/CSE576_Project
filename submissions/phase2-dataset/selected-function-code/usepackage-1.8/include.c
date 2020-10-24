{
   static linked_list* include_path = NULL;
   list_node* node;
   char* dir;
   char the_file_name[256];
   FILE* the_file = NULL;
   char* path;

   if (!include_path)
   {
      path = getenv(PACKAGE_PATH_VAR);
      if (!path)
         path = DEFAULT_PACKAGE_PATH;

      include_path = make_pathlist(path);
   }

   strcpy(the_file_name, expand(filename));
   if (the_file_name[0] == '/')
      the_file = fopen(the_file_name, "r");

   for (node = head(include_path) ; !the_file && node ; node = next(node))
   {
      dir = (char*) get_value(node);
      sprintf(the_file_name, "%s/%s", expand(dir), filename);
      the_file = fopen(the_file_name, "r");
   }

   if (!the_file)
   {
      DEBUG(stderr, "cannot open file `%s'\n", filename);
      return(1);
   }

   DEBUG(stderr, "reading from `%s'...\n", the_file_name);
   stack_pointer++;
   strcpy(file_name[stack_pointer], the_file_name);
   line_number[stack_pointer] = 1;
   yyin = file[stack_pointer] = the_file;

   return(0);
}
