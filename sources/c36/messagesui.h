//
//  messagesui.h
//  c36
//
//  Created by Milan Toth on 03/03/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

	#ifndef c36_messagesui_h
	#define c36_messagesui_h

	char* messagesui_structure_desc =
	"messages.base\n"
	"	messages.list\n";

	char* messagesui_desc =
	"view messages.base x 0 y 0 width 646 height -700 focuslevel 1 invisible 1\n"
	"modifier layout options 001111\n"
	"modifier scroller enablex 0 enablescrollbary 1\n"
	"modifier animator\n"

	"view messages.list x 0 y 0 width 646 height -700 focuslevel 0 invisible 1\n"
	"modifier cropper\n"
	"modifier layout options 001100\n"
	"modifier itemlist\n";

	#endif
