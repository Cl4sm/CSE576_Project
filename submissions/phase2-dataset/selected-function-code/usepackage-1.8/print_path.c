{
   list_node* node;
   char* dirname;
   DIR* dir;

   /* special case for the empty path: */
   node = head(pathlist);
   dirname = get_value(node);
   if ((strlen(dirname) == 0) && !next(node))
   {
      printf("\"\"");
      return;
   }

   for (node = head(pathlist) ; node ; node = next(node))
   {
      dirname = get_value(node);
      printf(next(node) ? "%s:" : "%s", dirname);
   }
}
