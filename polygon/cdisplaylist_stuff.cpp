/* stuff for class CDisplayList */

#include "PolyLine.h"

dl_element * CDisplayList::Add( id id, void * ptr, int glayer, int gtype, int visible,
						int w, int holew, int x, int y, int xf, int yf, int xo, int yo, 
						int radius, int orig_layer )
{
	return NULL;
}

dl_element * CDisplayList::AddSelector( id id, void * ptr, int glayer, int gtype, int visible,
						int w, int holew, int x, int y, int xf, int yf, int xo, int yo, int radius )
{
	return NULL;
}


void CDisplayList::Set_visible( dl_element * el, int visible )
{
}


int CDisplayList::StopDragging()
{
	return 0;
}

int CDisplayList::CancelHighLight()
{
	return 0;
}

void CDisplayList::Set_id( dl_element * el, id * id )
{
}

id CDisplayList::Remove( dl_element * element )
{
	return 0;
}

int CDisplayList::Get_w( dl_element * el )
{
	return 0;
}

int CDisplayList::Get_x( dl_element * el )
{
	return 0;
}
int CDisplayList::Get_y( dl_element * el )
{
	return 0;
}

int CDisplayList::Get_xf( dl_element * el )
{
	return 0;
}
int CDisplayList::Get_yf( dl_element * el )
{
	return 0;
}

int CDisplayList::HighLight( int gtype, int x, int y, int xf, int yf, int w, int orig_layer )
{
	return 0;
}

int CDisplayList::StartDraggingLineVertex( CDC * pDC, int x, int y, int xi, int yi,
								int xf, int yf,
								int layer1, int layer2, int w1, int w2,
								int style1, int style2,
								int layer_no_via, int via_w, int via_holew, int dir,
								int crosshair )
{
	return 0;
}

int CDisplayList::StartDraggingArc( CDC * pDC, int style, int x, int y, int xi, int yi, 
								int layer, int w, int crosshair )
{
	return 0;
}
