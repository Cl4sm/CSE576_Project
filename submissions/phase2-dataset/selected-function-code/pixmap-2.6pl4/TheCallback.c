     Widget w;
     XtPointer client_data, call_data;
{
    int   *id = (int *)client_data;
    
    switch (*id) {
      
    case Load:
      DoLoad();
      break;
      
    case Insert:
      DoInsert();
      break;
      
    case Save:
      DoSave();
      break;
      
    case SaveAs:
      DoSaveAs();
      break;
      
    case Filename:
      DoFilename();
      break;
      
    case Hints:
      DoHintsCmt();
      break;
    case Colors:
      DoColorsCmt();
      break;
    case Pixels:
      DoPixelsCmt();
      break;
      
    case Quit:
      DoQuit();
      break;
      
    case ColorPixel:
      DoAddColor();
      break;
    case ColorSname:
      DoSymbolicName();
      break;
    case ColorMname:
      DoMonochromeName();
      break;
    case ColorG4name:
      DoGrey4Name();
      break;
    case ColorGname:
      DoGreyName();
      break;
    case ColorCname:
      DoColorName();
      break;
      
    case Image:
      SwitchImage();
      break;
      
    case Grid:
      SwitchGrid();
      break;
      
    case Axes:
      SwitchAxes();
      break;	
      
    case Proportional:
      SwitchProportional();
      break;
      
    case Zoom:
      SwitchZoom();
      break;

    case ZoomIn:
      DoZoomIn();
      break;
      
    case ZoomOut:
      DoZoomOut();
      break;
      
    case ZoomFactor:
      DoZoomFactor();
      break;

    case Resize:
      DoResize();
      break;
      
    case Rescale:
      DoRescale();
      break;

    case Copy:
      DoCopy();
      break;
      
    case Cut:
      DoCut();
      break;
      
    case Paste:
      DoPaste();
      break;
      
    case Crop:
      DoCrop();
      break;
      
    case Clear:
      PWStoreToBuffer(pixmap_widget);
      PWClear(pixmap_widget);
      PWChangeNotify(pixmap_widget, NULL, NULL);
      PWSetChanged(pixmap_widget);
      break;
      
    case Set:
      PWStoreToBuffer(pixmap_widget);
      PWSet(pixmap_widget);
      PWChangeNotify(pixmap_widget, NULL, NULL);
      PWSetChanged(pixmap_widget);
      break;
      
    case Redraw:
      PWRedraw(pixmap_widget);
      FixImage(pixmap_widget, NULL, NULL);
      break;
      
    case CopyImm:
      PWRemoveAllRequests(pixmap_widget);
      if (PWQueryMarked(pixmap_widget)) {
	PWAddRequest(pixmap_widget, MarkRequest, False, Plain);
	PWEngageRequest(pixmap_widget, CopyRequest, True, Plain);
      }
      else {
	PWEngageRequest(pixmap_widget, MarkRequest, False, Plain);
	PWAddRequest(pixmap_widget, CopyRequest, True, Plain);
      }
      break;
      
    case MoveImm:
      PWRemoveAllRequests(pixmap_widget);
      if (PWQueryMarked(pixmap_widget)) {
	PWAddRequest(pixmap_widget, MarkRequest, False, Plain);
	PWEngageRequest(pixmap_widget, MoveRequest, True, Plain);
      }
      else {
	PWEngageRequest(pixmap_widget, MarkRequest, False, Plain);
	PWAddRequest(pixmap_widget, MoveRequest, True, Plain);
      }
      break;
      
    case MarkImm:
      PWRemoveAllRequests(pixmap_widget);
      PWEngageRequest(pixmap_widget, MarkRequest, True, Plain);
      break;
      
    case UnmarkImm:
      PWUnmark((Widget)pixmap_widget);
      break;
      
    case Up:
      PWStoreToBuffer(pixmap_widget);
      PWUp(pixmap_widget);
      PWChangeNotify(pixmap_widget, NULL, NULL);
      PWSetChanged(pixmap_widget);
      break;
      
    case Down:
      PWStoreToBuffer(pixmap_widget);
      PWDown(pixmap_widget);
      PWChangeNotify(pixmap_widget, NULL, NULL);
      PWSetChanged(pixmap_widget);
      break;
      
    case Left:
      PWStoreToBuffer(pixmap_widget);
      PWLeft(pixmap_widget);
      PWChangeNotify(pixmap_widget, NULL, NULL);
      PWSetChanged(pixmap_widget);
      break;
      
    case Right:
      PWStoreToBuffer(pixmap_widget);
      PWRight(pixmap_widget);
      PWChangeNotify(pixmap_widget, NULL, NULL);
      PWSetChanged(pixmap_widget);
      break;
      
    case Fold:
      PWStoreToBuffer(pixmap_widget);
      PWFold(pixmap_widget);
      PWChangeNotify(pixmap_widget, NULL, NULL);
      PWSetChanged(pixmap_widget);
      break;
      
    case FlipHoriz:
      PWStoreToBuffer(pixmap_widget);
      PWFlipHoriz(pixmap_widget);
      PWChangeNotify(pixmap_widget, NULL, NULL);
      PWSetChanged(pixmap_widget);
      break;
      
    case FlipVert:
      PWStoreToBuffer(pixmap_widget);
      PWFlipVert(pixmap_widget);
      PWChangeNotify(pixmap_widget, NULL, NULL);
      PWSetChanged(pixmap_widget);
      break;
      
    case RotateRight:
      PWStoreToBuffer(pixmap_widget);
      PWRotateRight(pixmap_widget);
      PWChangeNotify(pixmap_widget, NULL, NULL);
      PWSetChanged(pixmap_widget);
      break;
      
    case RotateLeft:
      PWStoreToBuffer(pixmap_widget);
      PWRotateLeft(pixmap_widget);
      PWChangeNotify(pixmap_widget, NULL, NULL);
      PWSetChanged(pixmap_widget);
      break;
      
    case Point:
      PWRemoveAllRequests(pixmap_widget);
      PWEngageRequest(pixmap_widget, PointRequest, True, Plain);
      break;
      
    case Curve:
      PWRemoveAllRequests(pixmap_widget);
      PWEngageRequest(pixmap_widget, CurveRequest, True, Plain);
      break;
      
    case Line:
      PWRemoveAllRequests(pixmap_widget);
      PWEngageRequest(pixmap_widget, LineRequest, True, Plain);
      break;
      
    case Rectangle:
      PWRemoveAllRequests(pixmap_widget);
      PWEngageRequest(pixmap_widget, RectangleRequest, True, Plain);
      break;
      
    case FilledRectangle:
      PWRemoveAllRequests(pixmap_widget);
      PWEngageRequest(pixmap_widget, FilledRectangleRequest, True, Plain);
      break;
      
    case Circle:
      PWRemoveAllRequests(pixmap_widget);
      PWEngageRequest(pixmap_widget, CircleRequest, True, Plain);
      break;
      
    case FilledCircle:
      PWRemoveAllRequests(pixmap_widget);
      PWEngageRequest(pixmap_widget, FilledCircleRequest, True, Plain);
      break;
      
    case FloodFill:
      PWRemoveAllRequests(pixmap_widget);
      PWEngageRequest(pixmap_widget, FloodFillRequest, True, Plain);
      break;
      
    case Text:
      PopupTextOptions( w, textOptions );
      PWRemoveAllRequests(pixmap_widget);
      PWEngageRequest(pixmap_widget, TextRequest, True, Plain);
      break;
      
    case SetHotSpot:
	PWRemoveAllRequests(pixmap_widget);
	PWEngageRequest(pixmap_widget, HotSpotRequest, True, Plain);
	break;
	
    case ClearHotSpot:
	PWStoreToBuffer(pixmap_widget);
	PWClearHotSpot(pixmap_widget);
	PWChangeNotify(pixmap_widget, NULL, NULL);
	PWSetChanged(pixmap_widget);
	break;

    case Undo:
      PWUndo(pixmap_widget);
#ifdef USE_ATHENA
      PWRedraw(pixmap_widget);
#endif /* USE_ATHENA */
      PWChangeNotify(pixmap_widget, NULL, NULL);
      PWSetChanged(pixmap_widget);
      FixStatus();
      break;	
    case Color: /* Start of colors management */
    default: /* Don't change this default section: color management */
      PWSetForeground(pixmap_widget,(*id - Color_offset));
      FixColor(pixmap_widget, *id - Color_offset);
      break; 
    }
}
