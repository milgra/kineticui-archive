//
//  newconversationui.h
//  c36
//
//  Created by Milan Toth on 03/03/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

	#ifndef c36_newconversationui_h
	#define c36_newconversationui_h

	char* newconversationui_structure_desc =
	"newconversation.base\n"
	"	newconversation.body\n"
	"		newconversation.body.white\n"
	"		newconversation.text\n"
	"		newconversation.inputlabel\n"
	"			newconversation.inputtext\n"
	"		newconversation.oktext\n"
	"		newconversation.closebutton\n"
	"			newconversation.closebutton.image\n";

	char* newconversationui_desc =
	"view newconversation.base x 0 y 0 width 900 height -800 focuslevel 0 invisible 1\n"
	"modifier layout options 001111\n"
	"modifier animator\n"

	"view newconversation.body x 250 y -300 width 400 height -250 focuslevel 0\n"
	"modifier layout options 110000\n"
	"modifier blurcolor\n"
	"modifier animator\n"

	"view newconversation.body.white x 0 y 0 width 400 height -250 focuslevel 0\n"
	"modifier layout options 110000\n"
	"modifier solidcolor color 0xFFFFFFCC\n"

	"view newconversation.text x 0 y -50 width 400 height -60 focuslevel 0\n"
	"modifier layout options 110000\n"
	"modifier solidcolor color 0x00000000\n"
	"modifier textarea linesize 24.0 aligntype center linetype single inputtype normal marginsize 13.0 textcolor 0x000000FF text Please_enter_peer_names\n"

	"view newconversation.inputlabel x 50 y -100 width 300 height -50 focuslevel 2\n"
	"modifier solidcolor color 0xFFFFFF66\n"
	"modifier scroller enabley 0\n"

	"view newconversation.inputtext x 0 y 0 width 300 height -50 focuslevel 1\n"
	"modifier solidcolor color 0x00000000\n"
	"modifier textarea linesize 24.0 aligntype left linetype single inputtype input marginsize 13.0 textcolor 0x000000FF enteraction createconversation\n"

	"view newconversation.oktext x 100 y -170 width 200 height -50 focuslevel 1\n"
	"modifier layout options 110000\n"
	"modifier solidcolor color 0xFFFFFF22\n"
	"modifier textarea linesize 24.0 aligntype center linetype single inputtype normal marginsize 13.0 textcolor 0x000000FF text Create touchaction createconversation\n"

	"view newconversation.closebutton x 0 y 0 width 50 height -50 focuslevel 2 invisible 1\n"
	"modifier button event tapped action removenewconv\n"
	"modifier animator\n"

	"view newconversation.closebutton.image x 15 y -15 width 20 height -20 focuslevel 0\n"
	"modifier imagearea image exit.png\n";

	#endif
