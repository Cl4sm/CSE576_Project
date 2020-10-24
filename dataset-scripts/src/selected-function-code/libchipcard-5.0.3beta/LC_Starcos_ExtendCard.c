int LC_Starcos_ExtendCard(LC_CARD *card){
  LC_STARCOS *scos;
  int rv;

  rv=LC_ProcessorCard_ExtendCard(card);
  if (rv) {
    DBG_ERROR(LC_LOGDOMAIN, "Could not extend card as processor card");
    return rv;
  }

  GWEN_NEW_OBJECT(LC_STARCOS, scos);
  GWEN_INHERIT_SETDATA(LC_CARD, LC_STARCOS, card, scos,
                       LC_Starcos_freeData);

  LC_Card_SetLastResult(card, 0, 0, 0, 0);
  scos->openFn=LC_Card_GetOpenFn(card);
  scos->closeFn=LC_Card_GetCloseFn(card);
  scos->keyDescriptors=LC_Starcos_KeyDescr_List_new();
  LC_Card_SetOpenFn(card, LC_Starcos_Open);
  LC_Card_SetCloseFn(card, LC_Starcos_Close);

  LC_Card_SetGetInitialPinFn(card, LC_Starcos_GetInitialPin);
  LC_Card_SetGetPinStatusFn(card, LC_Starcos_GetPinStatus);

  LC_Card_SetIsoSignFn(card, LC_Starcos__Sign);
  LC_Card_SetIsoVerifyFn(card, LC_Starcos__Verify);

  return 0;
}