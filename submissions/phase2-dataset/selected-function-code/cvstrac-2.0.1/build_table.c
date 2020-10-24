static void build_table(void){
  int i;

  qsort(aEntry, nFixed, sizeof(aEntry[0]), e_compare);
  for(i=0; i<nFixed; i++){
    printf("extern void %s(void);\n", aEntry[i].zFunc);
  }
  printf(
    "static struct {\n"
    "  const char *zPath;\n"
    "  void (*xFunc)(void);\n"
    "} aSearch[] = {\n"
  );
  for(i=0; i<nFixed; i++){
    printf("  { \"%s\",%*s %s },\n",
      aEntry[i].zPath, (int)(25-strlen(aEntry[i].zPath)), "",
      aEntry[i].zFunc
    );
  }
  printf("};\n");
}