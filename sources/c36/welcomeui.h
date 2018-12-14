//
//  welcomeui.h
//  c36
//
//  Created by Milan Toth on 03/03/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

	#ifndef c36_welcomeui_h
	#define c36_welcomeui_h

	char* welcomeui_structure_desc =
	"welcome.base\n"
	"	welcome.body\n"
	"		welcome.body.white\n"
	"		welcome.text\n"
	"		welcome.oktext\n";

	char* welcomeui_desc =
	"view welcome.base x 0 y -50 width 900 height -800 focuslevel 1 invisible 1\n"
	"modifier layout options 001111\n"
	"modifier animator\n"

	"view welcome.body x 250 y -230 width 400 height -250 focuslevel 1\n"
	"modifier layout options 110000\n"
	"modifier blurcolor\n"

	"view welcome.body.white x 0 y 0 width 400 height -250 focuslevel 0\n"
	"modifier solidcolor color 0xFFFFFFCC\n"

	"view welcome.text x 30 y -30 width 340 height -120 focuslevel 0\n"
	"modifier solidcolor color 0x00000000\n"
	"modifier textarea linesize 24.0 aligntype center linetype multi inputtype normal marginsize 3.0 textcolor 0x000000FF\n"

	"view welcome.oktext x 100 y -170 width 200 height -50 focuslevel 1\n"
	"modifier solidcolor color 0xFFFFFF22\n"
	"modifier textarea linesize 24.0 aligntype center linetype single inputtype normal marginsize 13.0 textcolor 0x000000FF text OK touchaction removewelcome\n";

	#endif
