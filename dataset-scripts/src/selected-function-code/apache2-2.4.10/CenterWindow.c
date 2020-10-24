BOOL CenterWindow(HWND hwndChild)
{
   RECT rChild, rWorkArea;
   int wChild, hChild;
   int xNew, yNew;
   BOOL bResult;

   /* Get the Height and Width of the child window */
   GetWindowRect(hwndChild, &rChild);
   wChild = rChild.right - rChild.left;
   hChild = rChild.bottom - rChild.top;

   /* Get the limits of the 'workarea' */
   bResult = SystemParametersInfo(SPI_GETWORKAREA, sizeof(RECT),
                                  &rWorkArea, 0);
   if (!bResult) {
      rWorkArea.left = rWorkArea.top = 0;
      rWorkArea.right = GetSystemMetrics(SM_CXSCREEN);
      rWorkArea.bottom = GetSystemMetrics(SM_CYSCREEN);
   }

   /* Calculate new X and Y position*/
   xNew = (rWorkArea.right - wChild) / 2;
   yNew = (rWorkArea.bottom - hChild) / 2;
   return SetWindowPos(hwndChild, HWND_TOP, xNew, yNew, 0, 0,
                       SWP_NOSIZE | SWP_SHOWWINDOW);
}