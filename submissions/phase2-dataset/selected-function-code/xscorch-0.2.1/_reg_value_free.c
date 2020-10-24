static void _reg_value_free(reg_type type, reg_value *value) {
/* reg_value_free */

   switch(type) {
      case REG_STRING:
         free(value->string);
         value->string = NULL;
         break;

      case REG_BLOCK:
         reg_block_free(&value->block);
         break;

      case REG_INTEGER:
      case REG_BOOLEAN:
      case REG_DOUBLEV:
      case REG_ANY:
         break;
   }
}
