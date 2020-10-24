{
    char *name = NULL;

    if ( PopupDialog( input_dialog, 
                      "Add extension name:", 
                      "", &name,
                      XtGrabExclusive ) == Okay ) {

        if ( name != NULL && strcmp( name, "" ) != 0 ) {

            /* add to menu, if necessary */
            if ( ! XtNameToWidget(extensionMenu_widget, name) )
		AddToExtensionMenu (name);

            PWAddExtension( pixmap_widget, name );  /* add, if necessary */

            DoEditExtension( name );
        }
    }
}
