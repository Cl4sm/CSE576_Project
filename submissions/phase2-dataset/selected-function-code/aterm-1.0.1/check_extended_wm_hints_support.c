void 
check_extended_wm_hints_support()
{
	memset( &ExtWM, 0x00, sizeof(ExtWM));

	if( read_32bit_property (Xroot, _XA_NET_SUPPORTING_WM_CHECK, &ExtWM.supporting_wm_check) )
		if( ExtWM.supporting_wm_check != None ) 
		{	
			Window w;
			if( !read_32bit_property (ExtWM.supporting_wm_check, _XA_NET_SUPPORTING_WM_CHECK, &w) )
				ExtWM.supporting_wm_check = None ;
			else if( w != ExtWM.supporting_wm_check ) 
				ExtWM.supporting_wm_check = None ;
			else
			{
				CARD32 *supported_props = NULL;
				long nitems	= 0 ;
				Bool curr_desk = False ; 
				Bool app_desk = False ; 
				if( read_32bit_proplist (Xroot, _XA_NET_SUPPORTED, 20, &supported_props, &nitems) )
				{
					int i ; 
					for( i = 0 ; i < nitems; ++i ) 
					{	
						if( supported_props[i] == _XA_NET_CURRENT_DESKTOP ) 
							curr_desk = True ; 
						else if( supported_props[i] == _XA_NET_WM_DESKTOP ) 
							app_desk = True ; 
					}
				}
				if( curr_desk && app_desk )
				{			
					set_flags( ExtWM.flags, WM_ClaimSupportsDesktops );
					if( read_32bit_property (Xroot, _XA_NET_CURRENT_DESKTOP, &ExtWM.current_desktop) )
						set_flags( ExtWM.flags, WM_SupportsDesktops	);
				}
			}
		}

}
