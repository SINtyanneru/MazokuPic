#include <kos.h>
#include <png/png.h>

const static int SCREEN_WIDTH = 640;
const static int SCREEN_HEIGHT = 480;

int image_pos_x = 0;
int image_pos_y = 132;
float image_zoom = 2.0f;

#define MENU_ICON_LENGTH 2
pvr_ptr_t menu_icon_list[MENU_ICON_LENGTH];
pvr_ptr_t icon_cursor;
unsigned int menu_select = 0;

pvr_ptr_t image_list[1024];

#include "Draw.c"
#include "KeyEvent.c"

void Main() {
	pvr_init_defaults();

	image_pos_x = (SCREEN_WIDTH - 512) / 2;

	icon_cursor = pvr_mem_malloc(16 * 256 * 2);
	png_to_texture("/rd/ICON/CURSOR.png", icon_cursor, PNG_NO_ALPHA);

	menu_icon_list[0] = pvr_mem_malloc(16 * 256 * 2);
	png_to_texture("/rd/ICON/LEFT.png", menu_icon_list[0], PNG_NO_ALPHA);

	menu_icon_list[1] = pvr_mem_malloc(16 * 256 * 2);
	png_to_texture("/rd/ICON/RIGHT.png", menu_icon_list[1], PNG_NO_ALPHA);

	//画像をロード
	image_list[0] = pvr_mem_malloc(512 * 256 * 2);
	png_to_texture("/rd/IMAGE/1.png", image_list[0], PNG_NO_ALPHA);

	//最初の画面描画
	draw();

	int key_down = 0;

	while(true) {
		MAPLE_FOREACH_BEGIN(MAPLE_FUNC_CONTROLLER, cont_state_t, state);

		int move_x = image_pos_x - (state->joyx / 50);
		int move_y = image_pos_y - (state->joyy / 50);
		float move_zoom = image_zoom;

		//押した瞬間のみ実行する部
		if (state->dpad_left && !key_down) {
			//←
			key_down = 1;
			on_key_dleft();
		} else if (state->dpad_right && !key_down) {
			//→
			key_down = 1;
			on_key_dright();
		}

		//推してるフラグを折る
		if (!state->dpad_up && !state->dpad_down && !state->dpad_left && !state->dpad_right) {
			key_down = 0;
		}

		//押してる間ずっと反応する
		if (state->dpad_up && !key_down && image_zoom > 0.0f) {
			//↑
			move_zoom += 0.01f;
			draw();
		} else if (state->dpad_down && !key_down) {
			//↓
			move_zoom -= 0.01f;
			draw();
		}

		//移動が有るなら移動し、描画する
		if (image_pos_x != move_x || image_pos_y != move_y || move_zoom != image_zoom) {
			//アナログスティック
			image_pos_x = move_x;
			image_pos_y = move_y;
			image_zoom = move_zoom;

			//描画
			draw();
		}

		MAPLE_FOREACH_END();
	}
}