{
   linked_list* elist = new_list();
   list_node* vnode;
   char* vpath;
   FILE* test;
   
   for (vnode = list_tail(vlist) ; vnode ; vnode = previous(vnode))
   {
      vpath = (char*) get_value(vnode);
      
      test = fopen(vpath, "r");
      if ( test )
      {
         add_to_head(elist, vpath);
         fclose(test);
      }
   }
   
   return(elist);
}
