void on_key_dleft() {
	if (menu_select != 0) {
		menu_select -= 1;

		draw();
	}
}

void on_key_dright() {
	if (menu_select + 1 < MENU_ICON_LENGTH) {
		menu_select += 1;

		draw();
	}
}