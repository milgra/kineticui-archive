//
//  conversationitemui.h
//  c36
//
//  Created by Milan Toth on 03/03/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

	#ifndef c36_conversationitemui_h
	#define c36_conversationitemui_h

	char* conversationitemui_structure_desc =
	"convitem.base\n"
	"	convitem.overlay\n"
	"	convitem.strip\n"
	"	convitem.label\n"
	"		convitem.text\n";

	char* conversationitemui_desc =
	"view convitem.base x 0 y 0 width 240 height -50 focuslevel 1\n"
	"modifier cropper\n"
	"modifier layout options 001100\n"
	"modifier solidcolor color 0x00000000\n"

	"view convitem.overlay x 0 y 0 width 240 height -50 focuslevel 0\n"
	"modifier cropper\n"
	"modifier layout options 001100\n"
	"modifier solidcolor color 0xFFFFFFBB\n"

	"view convitem.strip x 120 y -50 width 10 height -1 focuslevel 0\n"
	"modifier cropper\n"
	"modifier layout options 100000\n"
	"modifier solidcolor color 0x000000FF\n"

	"view convitem.label x 0.0 y 0.0 width 240 height -50 focuslevel 1\n"
	"modifier cropper\n"
	"modifier layout options 001100\n"
	"modifier solidcolor color 0x00000000\n"
	"modifier scroller enablex 1 enabley 0\n"

	"view convitem.text x 0.0 y 0.0 width 240.0 height -50.0 focuslevel 0\n"
	"modifier cropper\n"
	"modifier layout options 001100\n"
	"modifier solidcolor color 0x00000000\n"
	"modifier textarea linesize 24.0 aligntype left linetype single inputtype normal marginsize 13.0 textcolor 0x000000FF text Koródi_Tamás\n";

	#endif
