{
  int tile_x, tile_y, left_w, bott_h;

	if( src_w > 0 && src_h > 0 )
	{
    	tile_x = x%src_w ;
	    tile_y = y%src_h ;
	    left_w = min(src_w-tile_x,w);
  		bott_h = min(src_h-tile_y,h);
/*fprintf( stderr, "\nShadeTiledPixmap(): tile_x = %d, tile_y = %d, left_w = %d, bott_h = %d, SRC = %dx%d TRG=%dx%d", tile_x, tile_y, left_w, bott_h, src_w, src_h, w, h);*/
	    CopyAndShadeArea( src, trg, tile_x, tile_y, left_w, bott_h, 0, 0, gc, shading );
  		if( bott_h < h )
	    {  /* right-top parts */
			CopyAndShadeArea( src, trg, tile_x, 0, left_w, h-bott_h, 0, bott_h, gc, shading );
	    }
  		if( left_w < w )
	    {  /* left-bott parts */
			CopyAndShadeArea( src, trg, 0, tile_y, w-left_w, bott_h, left_w, 0, gc, shading );
	        if( bott_h < h  )      /* left-top parts */
			    CopyAndShadeArea( src, trg, 0, 0, w-left_w, h-bott_h, left_w, bott_h, gc, shading );
	    }
	}
}
