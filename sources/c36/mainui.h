//
//  mainui.h
//  c36
//
//  Created by Milan Toth on 03/03/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

	#ifndef c36_mainui_h
	#define c36_mainui_h

	char* mainui_structure_desc =
	"main.base\n"
	"	main.scrollbox\n"
	"		main.scrollbase\n"
	"			main.messages.blur\n"
	"			main.messages\n"
	"			main.conversations.blur\n"
	"			main.conversations.base\n"
	"				main.conversations.list\n"
	"	main.composer\n"
	"		main.composer.white\n"
	"		main.composer.label\n"
	"			main.composer.text\n"
	"	main.sendbutton\n"
	"		main.sendbutton.image\n"
	"	overlay\n";

	char* mainui_desc =
	"view main.base x 0 y 0 width 900 height -800 focuslevel 2\n"
	"modifier layout options 001111\n"
	"modifier solidcolor color 0xFFFFFF22\n"
	"modifier animator\n"

	"view main.scrollbox x 0 y 0 width 900 height -800 focuslevel 1 invisible 1\n"
	"modifier layout options 001111\n"
	"modifier scroller enabley 0 rubber 0\n"

	"view main.scrollbase x 0 y 0 width 900 height -800 focuslevel 0 invisible 1\n"
	"modifier cropper\n"
	"modifier layout options 001111 minwidth 600.0\n"

	"view main.messages.blur x 2 y -50 width 646 height -700 focuslevel 0\n"
	"modifier cropper\n"
	"modifier layout options 001111 minwidth 600.0\n"
	"modifier blurcolor\n"

	"view main.messages x 2 y -50 width 646 height -700 focuslevel 0\n"
	"modifier cropper\n"
	"modifier layout options 001111\n"
	"modifier solidcolor color 0xFFFFFF88\n"

	"view main.conversations.blur x 650 y -50 width 248 height -700 focuslevel 0\n"
	"modifier cropper\n"
	"modifier blurcolor\n"
	"modifier layout options 000111\n"

	"view main.conversations.base x 650 y -50 width 248 height -700 focuslevel 2\n"
	"modifier cropper\n"
	"modifier layout options 000111\n"
	"modifier solidcolor color 0xFFFFFF88\n"
	"modifier scroller enablex 0 enablescrollbary 1\n"

	"view main.conversations.list x 0 y 0 width 248 height -700 focuslevel 0 invisible 1\n"
	"modifier cropper\n"
	"modifier layout options 001100\n"
	"modifier itemlist\n"

	"view main.composer x 0 y -750 width 900 height -740 focuslevel 0\n"
	"modifier layout options 001100\n"
	"modifier blurcolor\n"
	"modifier cropper\n"

	"view main.composer.white x 0 y 0 width 900 height -740 focuslevel 0\n"
	"modifier layout options 001111\n"
	"modifier solidcolor color 0xFFFFFFAA\n"
	"modifier cropper\n"

	"view main.composer.label x 0 y 0 width 900 height -740 focuslevel 0\n"
	"modifier layout options 001111\n"
	"modifier solidcolor color 0xFFFFFF00\n"
	"modifier animator\n"
	"modifier scroller\n"

	"view main.composer.text x 0 y 0 width 850 height -50 focuslevel 1\n"
	"modifier layout options 001100\n"
	"modifier solidcolor color 0xFFFFFF00\n"
	"modifier textarea linesize 24.0 aligntype justify linetype multi inputtype input marginsize 13.0 textcolor 0x000000FF enteraction sendmessage resizeaction resizecomposer\n"

	"view main.sendbutton x 850 y -750 width 50 height -50 focuslevel 2 invisible 1\n"
	"modifier layout options 000101\n"
	"modifier animator\n"
	"modifier button action sendmessage\n"

	"view main.sendbutton.image x 15 y -15 width 20 height -20 focuslevel 0\n"
	"modifier imagearea image send.png\n"

	"view overlay x 0 y 0 width 900 height -800 focuslevel 0 invisible 1\n"
	"modifier layout options 001111\n"
	"modifier button event tapped action zoomtomain\n";

	#endif
