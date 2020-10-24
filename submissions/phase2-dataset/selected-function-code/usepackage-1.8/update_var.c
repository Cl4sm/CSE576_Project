{
   linked_list* testlist;

   switch (vvar->type)
   {
      case VAR_LIT_SET:
         evar->literal = vvar->literal;
         evar->type = VAR_LIT_SET;
         break;

      case VAR_PATH_SET:
         evar->pathlist = vvar->pathlist;
         evar->type = VAR_PATH_SET;
         break;

      case VAR_PATH_ADD:
         switch (evar->type)
         {
            case VAR_LIT_SET:
               evar->pathlist = merge_paths(make_pathlist(evar->literal),
                                            vvar->pathlist);
               break;

            case VAR_PATH_SET:
            case VAR_PATH_ADD:
               evar->pathlist = merge_paths(evar->pathlist, vvar->pathlist);
               break;

            default:
               break;
         }
         evar->type = VAR_PATH_ADD;
         break;
         
      case VAR_PATH_TESTSET:
         testlist = test_paths(vvar->pathlist);
         if (head(testlist))
         {
            evar->pathlist = testlist;
         }
         else
         {
            evar->pathlist = NULL;
         }
         evar->type = VAR_PATH_SET;
         break;

      case VAR_PATH_TESTADD:
         switch (evar->type)
         {
            case VAR_LIT_SET:
               evar->pathlist = merge_paths(make_pathlist(evar->literal),
                                            test_paths(vvar->pathlist));
               break;

            case VAR_PATH_SET:
            case VAR_PATH_ADD:
               evar->pathlist = merge_paths(evar->pathlist, test_paths(vvar->pathlist));
               break;
               
            default:
               break;
         }
         evar->type = VAR_PATH_ADD;
         break;

      default:
         break;
   }

   return(evar);
}
