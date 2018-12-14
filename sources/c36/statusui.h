//
//  statusui.h
//  c36
//
//  Created by Milan Toth on 03/03/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

	#ifndef c36_statusui_h
	#define c36_statusui_h

	char* statusui_structure_desc =
	"status.base\n"
	"	status.messages.blur\n"
	"	status.messages\n"
	"		status.messages.list\n"
	"	overlay\n";

	char* statusui_desc =
	"view status.base x 0 y 0 width 900 height -800 focuslevel 0\n"
	"modifier layout options 001111\n"
	"modifier solidcolor color 0xFFFFFF22\n"
	"modifier animator\n"

	"view status.messages.blur x 5 y -50 width 890 height -750 focuslevel 0\n"
	"modifier layout options 001111\n"
	"modifier blurcolor\n"

	"view status.messages x 5 y -50 width 890 height -750 focuslevel 2\n"
	"modifier layout options 001111\n"
	"modifier solidcolor color 0xFFFFFF66\n"
	"modifier scroller enablex 0 enablescrollbary 1\n"

	"view status.messages.list x 0 y 0 width 890 height -750 focuslevel 1\n"
	"modifier layout options 001111\n"
	"modifier solidcolor color 0xFFFFFF00\n"
	"modifier itemlist\n"

	"view overlay x 0 y 0 width 900 height -800 focuslevel 0 invisible 1\n"
	"modifier layout options 001111\n"
	"modifier button event tapped action zoomtostatus\n";

	#endif
