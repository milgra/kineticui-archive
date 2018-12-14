//
//  menuitemui.h
//  c36
//
//  Created by Milan Toth on 03/03/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

	#ifndef c36_menuitemui_h
	#define c36_menuitemui_h

	char* menuitemui_structure_desc =
	"menuitem.base\n"
	"	menuitem.label\n"
	"		menuitem.text\n";

	char* menuitemui_desc =
	"view menuitem.base x 0 y 0 width 890 height -50 focuslevel 1\n"
	"modifier layout options 001100\n"
	"modifier solidcolor color 0x00000000\n"

	"view menuitem.label x 0 y 0 width 890 height -50 focuslevel 0\n"
	"modifier layout options 001100\n"
	"modifier solidcolor color 0x00000000\n"
	"modifier scroller enabley 0\n"

	"view menuitem.text x 0 y 0 width 890 height -50 focuslevel 0\n"
	"modifier layout options 001100\n"
	"modifier solidcolor color 0x00000000\n"
	"modifier textarea linesize 25.0 aligntype center linetype single inputtype normal marginsize 10.0 textcolor 0x000000FF text default\n";

	#endif
