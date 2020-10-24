GWEN_INHERIT(LC_CARD, LC_MEMORYCARD)



int LC_MemoryCard_ExtendCard(LC_CARD *card){
  LC_MEMORYCARD *mc;

  GWEN_NEW_OBJECT(LC_MEMORYCARD, mc);

  mc->openFn=LC_Card_GetOpenFn(card);
  mc->closeFn=LC_Card_GetCloseFn(card);
  LC_Card_SetOpenFn(card, LC_MemoryCard_Open);
  LC_Card_SetCloseFn(card, LC_MemoryCard_Close);

  GWEN_INHERIT_SETDATA(LC_CARD, LC_MEMORYCARD, card, mc,
                       LC_MemoryCard_freeData);

  LC_MemoryCard__CalculateCapacity(card);

  return 0;
}