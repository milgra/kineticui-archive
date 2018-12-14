//
//  loginui.h
//  c36
//
//  Created by Milan Toth on 03/03/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

	#ifndef c36_loginui_h
	#define c36_loginui_h

	char* loginui_structure_desc =
	"login.base\n"
	"	login.box\n"
	"		login.box.white\n"
	"		login.text\n"
	"		login.inputlabel\n"
	"			login.inputtext\n"
	"		login.oktext\n"
	"	overlay\n";

	char* loginui_desc =
	"view login.base x 0 y -50 width 900 height -800 focuslevel 1 invisible 1\n"
	"modifier layout options 001111\n"
	"modifier animator\n"

	"view login.box x 250 y -225 width 400 height -250 focuslevel 1\n"
	"modifier layout options 110000\n"
	"modifier blurcolor\n"

	"view login.box.white x 0 y 0 width 400 height -250 focuslevel 0\n"
	"modifier layout options 001111\n"
	"modifier solidcolor color 0xFFFFFFAA\n"

	"view login.text x 50 y -25 width 300 height -72 focuslevel 0 invisible 1\n"
	"modifier layout options 110000\n"
	"modifier textarea linesize 24.0 aligntype center linetype single inputtype normal marginsize 6.0 textcolor 0x000000FF text Please_enter_your_nickname_:\n"

	"view login.inputlabel x 50 y -100 width 300 height -50 focuslevel 2\n"
	"modifier solidcolor color 0xFFFFFFAA\n"
	"modifier scroller enabley 0 enablescrollbarx 1\n"

	"view login.inputtext x 0 y 0 width 300 height -50 focuslevel 1 invisible 1\n"
	"modifier textarea linesize 24.0 aligntype left linetype single inputtype input marginsize 13.0 textcolor 0x000000FF enteraction loginenterpressed\n"
	"modifier cropper\n"

	"view login.oktext x 100 y -170 width 200 height -50 focuslevel 1 invisible 1\n"
	"modifier layout options 110000\n"
	"modifier textarea linesize 24.0 aligntype center linetype single inputtype normal marginsize 13.0 textcolor 0x000000FF text Login touchaction loginbuttonpressed\n"

	"view overlay x 0 y 0 width 900 height -800 focuslevel 0 invisible 1\n"
	"modifier layout options 001111\n"
	"modifier button event tapped action zoomtologin\n";

	#endif
