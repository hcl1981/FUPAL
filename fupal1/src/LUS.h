/*
	Author: Jan Schulz 2019-2021
	License: 
	Creative Commons Attribution-NonCommercial-ShareAlike 4.0
	https://creativecommons.org/licenses/by-nc-sa/4.0/
	
	For further details check out: 
		https://513kid.de
*/

	void selectionLUS(VGA3BitIscale vga){
		const char *k_menu[5];
		k_menu[0] = "1. (coming soon)";
		k_menu[1] = "2. (Additive Farbmischung)";
		k_menu[2] = "3. (Lichtgeschwindigkeit mit oszi..)";
		k_menu[3] = "4. ";
		k_menu[4] = "5. ";
		vga.setTextColor(vga.RGB(0, 0, 0));
		UI ui(vga);
		switch (ui.drawSelection(k_menu, 1, true)) {
			case 0:
			//v17();
			break;
			case 1:
			//v18();
			break;
			case 2:
			//v2();
			break;
			case 3:
			//v3();
			break;
			case 4:
			//v4();
			break;
			default:
			//
			break;
		}
	}
//};
