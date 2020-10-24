int LC_Crypt_Token_ResultToError(LC_CLIENT_RESULT res) {
  int rv;

  switch(res) {
  case LC_Client_ResultOk:
    rv=0;
    break;
  case LC_Client_ResultWait:
    rv=GWEN_ERROR_TIMEOUT;
    break;
  case LC_Client_ResultIpcError:
  case LC_Client_ResultCmdError:
  case LC_Client_ResultDataError:
    rv=GWEN_ERROR_IO;
    break;

  case LC_Client_ResultAborted:
    rv=GWEN_ERROR_USER_ABORTED;
    break;

  case LC_Client_ResultInvalid:
    rv=GWEN_ERROR_INVALID;
    break;

  case LC_Client_ResultNoData:
    rv=GWEN_ERROR_NO_DATA;
    break;

  case LC_Client_ResultCardRemoved:
    rv=GWEN_ERROR_REMOVED;
    break;

  case LC_Client_ResultNotSupported:
    rv=GWEN_ERROR_NOT_SUPPORTED;
    break;

  case LC_Client_ResultInternal:
  case LC_Client_ResultGeneric:
  default:
    rv=GWEN_ERROR_GENERIC;
    break;
  }

  return rv;
}