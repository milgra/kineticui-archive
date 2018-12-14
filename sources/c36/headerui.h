//
//  headerui.c
//  c36
//
//  Created by Milan Toth on 03/03/15.
//  Copyright (c) 2015 Milan Toth. All rights reserved.
//

	#ifndef c36_headerui_h
	#define c36_headerui_h

	char* headerui_structure_desc =
	"header.base\n"
	"	header.base.white\n"
	"	header.label\n"
	"		header.text\n"
	"	header.closebutton\n"
	"		header.closebutton.image\n"
	"	header.maxbutton\n"
	"		header.maxbutton.image\n"
	"	header.exposebutton\n"
	"		header.exposebutton.image\n";

	char* headerui_desc =
	"view header.base x 0 y 0 width 900 height -50 focuslevel 2\n"
	"modifier layout options 001110\n"								// layout modifier always first, order counts!!!
	"modifier dragger press dragwindow release dropwindow\n"
	"modifier blurcolor\n"

	"view header.base.white x 0 y 0 width 900 height -50\n"
	"modifier layout options 001110\n"
	"modifier solidcolor color 0xFFFFFFAA\n"

	"view header.label x 100 y 0 width 700 height -50 focuslevel 1 invisible 1\n"
	"modifier layout options 001100\n"
	"modifier scroller enablex 1 enabley 0 enablescrollbarx 0\n"

	"view header.text x 0 y 0 width 700 height -50 focuslevel 1 invisible 1\n"
	"modifier cropper\n"
	"modifier layout options 001100\n"
	"modifier textarea linesize 24.0 aligntype center linetype single inputtype normal marginsize 13.0 textcolor 0x000000FF text Login_\n"

	"view header.closebutton x 0 y 0 width 50 height -50 focuslevel 2 invisible 1\n"
	"modifier button event tapped action closewindow\n"
	"modifier animator\n"

	"view header.closebutton.image x 15 y -15 width 20 height -20 focuslevel 0\n"
	"modifier imagearea image exit.png\n"

	"view header.maxbutton x 50 y 0 width 50 height -50 focuslevel 2 invisible 1\n"
	"modifier button event tapped action maximizewindow\n"
	"modifier animator\n"

	"view header.maxbutton.image x 15 y -15 width 20 height -20 focuslevel 0\n"
	"modifier imagearea image maximize.png\n"

	"view header.exposebutton x 850 y 0 width 50 height -50 focuslevel 2 invisible 1\n"
	"modifier layout options 000100\n"
	"modifier button action expose\n"
	"modifier animator\n"

	"view header.exposebutton.image x 15 y -15 width 20 height -20 focuslevel 0\n"
	"modifier imagearea image status.png\n";

	#endif
