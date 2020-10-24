char *extstr(ExtDescriptor *ext)
{
  static char str[20];

  switch (ext->xdrNumABlks)
    {
    case 0:
      return "[]";

    case 1:
      sprintf(str, "1[%u]", ext->xdrStABN);
      break;

    default:
      sprintf(str, "%u[%u..%u]", ext->xdrNumABlks,
	      ext->xdrStABN, ext->xdrStABN + ext->xdrNumABlks - 1);
    }

  return str;
}
