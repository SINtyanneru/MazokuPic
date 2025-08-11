#include <kos.h>
#include <png/png.h>

void draw() {
	pvr_wait_ready();
	pvr_scene_begin();
	pvr_list_begin(PVR_LIST_OP_POLY);

	//手前に描画するものを前に書かないと、おかしなことになるぞ。。。

	int icon_original_size = 16;
	int icon_size = 32;

	//カーソル
	draw_image(icon_cursor, icon_size + (icon_size*2) * menu_select, SCREEN_HEIGHT - (icon_size / 2), icon_size, icon_size, icon_original_size, icon_original_size);

	//アイコン
	for (int i = 0; i < MENU_ICON_LENGTH; i++) {
		draw_image(menu_icon_list[i], icon_size * i, SCREEN_HEIGHT - icon_size, icon_size, icon_size, icon_original_size, icon_original_size);
	}

	float scale = image_zoom;
	if (scale < 0) {
		scale = -scale;
	}

	float scale_w = 512.0f * scale;
	float scale_h = 256.0f * scale;

	float center_x = image_pos_x + 512 / 2;
	float center_y = image_pos_y + 256 / 2;
	float x = center_x - scale_w / 2;
	float y = center_y - scale_h / 2;
	draw_image(image_list[0], (int)x, (int)y, (int)scale_w , (int)scale_h, 512, 256);

	pvr_list_finish();
	pvr_scene_finish();
}

void draw_image(pvr_ptr_t texture, int x, int y, int w, int h, int original_w, int original_h) {
	pvr_poly_cxt_t cxt;
	pvr_poly_hdr_t hdr;
	pvr_vertex_t vert;

	pvr_poly_cxt_txr(&cxt, PVR_LIST_OP_POLY, PVR_TXRFMT_RGB565, original_w, original_h, texture, PVR_FILTER_BILINEAR);
	pvr_poly_compile(&hdr, &cxt);
	pvr_prim(&hdr, sizeof(hdr));

	vert.argb = PVR_PACK_COLOR(1.0f, 1.0f, 1.0f, 1.0f);
	vert.oargb = 0;
	vert.flags = PVR_CMD_VERTEX;

	vert.x = x;
	vert.y = y;
	vert.z = 1.0f;
	vert.u = 0.0f;
	vert.v = 0.0f;
	pvr_prim(&vert, sizeof(vert));

	vert.x = w + x;
	vert.y = y;
	vert.z = 1.0f;
	vert.u = 1.0f;
	vert.v = 0.0f;
	pvr_prim(&vert, sizeof(vert));

	vert.x = 1.0f + x;
	vert.y = h + y;
	vert.z = 1.0f;
	vert.u = 0.0f;
	vert.v = 1.0f;
	pvr_prim(&vert, sizeof(vert));

	vert.x = w + x;
	vert.y = h + y;
	vert.z = 1.0f;
	vert.u = 1.0f;
	vert.v = 1.0f;
	vert.flags = PVR_CMD_VERTEX_EOL;
	pvr_prim(&vert, sizeof(vert));
}