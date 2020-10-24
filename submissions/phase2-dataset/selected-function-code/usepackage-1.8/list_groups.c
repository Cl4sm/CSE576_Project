void list_groups()
{
   list_node* gnode;
   list_node* pnode;
   group_t* group;

   for (gnode = head(the_groups) ; gnode ; gnode = next(gnode))
   {
      group = (group_t*) get_value(gnode);

      fprintf(stderr, "  %s - ", group->name);

      for (pnode = head(group->packages) ; pnode ; pnode = next(pnode))
      {
         fprintf(stderr, "%s", (char*) get_value(pnode));
         if (next(pnode))
            fprintf(stderr, ", ");
      }

      fprintf(stderr, "\n");
   }
} 
