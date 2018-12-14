//
//  settingsui.h
//  c36
//
//  Created by Milan Toth on 03/03/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

	#ifndef c36_settingsui_h
	#define c36_settingsui_h

	char* settingsui_structure_desc =
	"settings.base\n"
	"	settings.messages.blur\n"
	"	settings.messages\n"
	"		settings.messages.list\n"
	"	settings.copyright.blur\n"
	"		settings.copyright\n"
	"	overlay\n";

	char* settingsui_desc =
	"view settings.base x 0 y 0 width 900 height -800 focuslevel 0\n"
	"modifier layout options 001111\n"
	"modifier solidcolor color 0xFFFFFF22\n"
	"modifier animator\n"

	"view settings.messages.blur x 2 y -50 width 898 height -698 focuslevel 2\n"
	"modifier layout options 001111\n"
	"modifier blurcolor\n"

	"view settings.messages x 2 y -50 width 898 height -698 focuslevel 2\n"
	"modifier layout options 001111\n"
	"modifier solidcolor color 0xFFFFFF66\n"
	"modifier scroller enablex 0 enablescrollbary 1\n"

	"view settings.messages.list x 0 y 0 width 898 height -698 focuslevel 1\n"
	"modifier layout options 001111\n"
	"modifier solidcolor color 0xFFFFFF00\n"
	"modifier itemlist\n"

	"view settings.copyright.blur x 2 y -750 width 898 height -50 focuslevel 1 alpha -0.9\n"
	"modifier layout options 001101\n"
	"modifier blurcolor\n"

	"view settings.copyright x 0 y 0 width 898 height -50 focuslevel 1\n"
	"modifier layout options 001100\n"
	"modifier solidcolor color 0xFFFFFF66\n"
	"modifier textarea linesize 25.0 aligntype center linetype single inputtype normal marginsize 10.0 textcolor 0x000000FF text Powered_by_Kinetic_UI_and_Shag.UK_\n"

	"view overlay x 0 y 0 width 900 height -800 focuslevel 0 invisible 1\n"
	"modifier layout options 001111\n"
	"modifier button event tapped action zoomtosettings\n";

	#endif
