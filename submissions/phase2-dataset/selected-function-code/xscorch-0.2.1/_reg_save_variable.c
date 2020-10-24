
   char buf[REG_BUFFER];

   switch(var->type) {
      case REG_INTEGER:
         _reg_indent(r->handle, indent);
         fprintf(r->handle, "%-16s = %d;\n", var->name, var->value.integer);
         break;

      case REG_DOUBLEV:
         _reg_indent(r->handle, indent);
         fprintf(r->handle, "%-16s = %e;\n", var->name, var->value.doublev);
         break;

      case REG_BOOLEAN:
         _reg_indent(r->handle, indent);
         switch(var->value.boolean.format) {
            case REG_FORMAT_BOOL_TF:
               fprintf(r->handle, "%-16s = %s;\n", var->name, (var->value.boolean.value ? "True" : "False"));
               break;
            case REG_FORMAT_BOOL_YN:
               fprintf(r->handle, "%-16s = %s;\n", var->name, (var->value.boolean.value ? "Yes" : "No"));
               break;
            case REG_FORMAT_BOOL_OO:
               fprintf(r->handle, "%-16s = %s;\n", var->name, (var->value.boolean.value ? "On" : "Off"));
               break;
            case REG_FORMAT_BOOL_TNIL:
               fprintf(r->handle, "%-16s = %s;\n", var->name, (var->value.boolean.value ? "T" : "NIL"));
               break;
         }
         break;

      case REG_STRING:
         strcopyb(buf, var->value.string, REG_BUFFER);
         escapeb(buf, REG_BUFFER);

         _reg_indent(r->handle, indent);
         fprintf(r->handle, "%-16s = \"%s\";\n", var->name, buf);
         break;

      case REG_BLOCK:
         _reg_indent(r->handle, indent);
         fprintf(r->handle, "%s = %s {\n", var->name, var->value.block->klass == NULL ? "null" : var->value.block->klass->name);

         _reg_save_block(r, var, indent + 3);
         _reg_indent(r->handle, indent);
         fprintf(r->handle, "}; End of block %s\n", var->name);
         if(indent == 0) fprintf(r->handle, "\n\n");
         break;

      case REG_ANY:
         break;
   }

}
