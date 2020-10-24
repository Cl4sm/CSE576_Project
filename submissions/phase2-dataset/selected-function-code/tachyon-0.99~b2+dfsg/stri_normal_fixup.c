  stri *t = (stri *) otri;

  switch (mode) {
/*    case RT_NORMAL_FIXUP_GUESS: */
    case 2:
      t->methods = &stri_methods_guess;
      break;

/*    case RT_NORMAL_FIXUP_FLIP: */
    case 1:
      t->methods = &stri_methods_reverse;
      break;

/*    case RT_NORMAL_FIXUP_OFF: */
    case 0:
    default:
      t->methods = &stri_methods;
      break;
  }
}
