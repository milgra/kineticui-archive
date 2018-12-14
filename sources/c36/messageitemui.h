//
//  messageitemui.h
//  c36
//
//  Created by Milan Toth on 03/03/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

	#ifndef c36_messageitemui_h
	#define c36_messageitemui_h

	char* messageitemui_structure_desc =
	"messageitem.base\n"
	"	messageitem.text\n";

	char* messageitemui_desc =
	"view messageitem.base x 0 y 0 width 550 height -50 focuslevel 0\n"
	"modifier cropper\n"
	"modifier layout options 001100\n"
	"modifier solidcolor color 0x00FF0000\n"

	"view messageitem.text x 0.0 y 0.0 width 550.0 height -50.0 focuslevel 0\n"
	"modifier cropper\n"
	"modifier layout options 001100\n"
	"modifier solidcolor color 0xFFFFFF00\n"
	"modifier textarea linesize 24.0 aligntype justify linetype multi inputtype normal marginsize 10.0 textcolor 0x000000FF text Koródi_Tamás\n";

	#endif
