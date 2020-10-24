{
   linked_list* list;
   int start,i,end;
   char* path;

   list = new_list();

   if (!path_string)
      return(list);

   start = 0;
   i = 0;
   end = strlen(path_string);

   /* if (!end)
      return(list); */

   while (i <= end)
   {
      if ((path_string[i] == ':') || (path_string[i] == '\0'))
      {
         path = (char*) malloc((i - start) + 1);
         strncpy(path, path_string + start, i - start);
         path[i - start] = '\0';
         add_to_tail(list, path);
         start = i + 1;
      }

      i++;
   }

   return(list);
}
