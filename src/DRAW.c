#include <kos.h>
#include <png/png.h>

extern int IMG_SELECT;
extern const int IMG_LENGTH;
extern pvr_ptr_t BACK_TEX;
extern pvr_ptr_t IMAGE_TEX[1024];
extern pvr_ptr_t FONT_TEX[10];

//画像を描画する
void DRAW_IMAGE(pvr_ptr_t TEX, int X, int Y, int W, int H, int ORIGINAL_W, int ORIGINAL_H){
	pvr_poly_cxt_t cxt;
	pvr_poly_hdr_t hdr;
	pvr_vertex_t vert;

	pvr_poly_cxt_txr(&cxt, PVR_LIST_OP_POLY, PVR_TXRFMT_RGB565, ORIGINAL_W, ORIGINAL_H, TEX, PVR_FILTER_BILINEAR);
	pvr_poly_compile(&hdr, &cxt);
	pvr_prim(&hdr, sizeof(hdr));

	vert.argb = PVR_PACK_COLOR(1.0f, 1.0f, 1.0f, 1.0f);
	vert.oargb = 0;
	vert.flags = PVR_CMD_VERTEX;

	vert.x = X;
	vert.y = Y;
	vert.z = 1.0f;
	vert.u = 0.0f;
	vert.v = 0.0f;
	pvr_prim(&vert, sizeof(vert));

	vert.x = W + X;
	vert.y = Y;
	vert.z = 1.0f;
	vert.u = 1.0f;
	vert.v = 0.0f;
	pvr_prim(&vert, sizeof(vert));

	vert.x = 1.0f + X;
	vert.y = H + Y;
	vert.z = 1.0f;
	vert.u = 0.0f;
	vert.v = 1.0f;
	pvr_prim(&vert, sizeof(vert));

	vert.x = W + X;
	vert.y = H + Y;
	vert.z = 1.0f;
	vert.u = 1.0f;
	vert.v = 1.0f;
	vert.flags = PVR_CMD_VERTEX_EOL;
	pvr_prim(&vert, sizeof(vert));
}

void DRAW_NUM(int NUM, int X, int Y){
	char STR[12];
	snprintf(STR, sizeof(STR), "%d", NUM);

	int LENGTH = strlen(STR);
	int OFFSET = 0;
	for(int I = 0; I < LENGTH; I++){
		//Charからintに型変換
		int NUMI = STR[I] - '0';

		//数字を描画
		DRAW_IMAGE(FONT_TEX[NUMI], X + OFFSET, Y, 32, 64, 32, 64);

		//横軸をずらす
		OFFSET = OFFSET + 32;
	}
}

//描画
void DRAW() {
	//初期化
	pvr_wait_ready();
	pvr_scene_begin();
	pvr_list_begin(PVR_LIST_OP_POLY);

	//手前に描画するものを前に書かないと、おかしなことになるぞ。。。

	//デバッグ用画像
	DRAW_NUM(IMG_SELECT, 0, SCREEN_HEIGHT - (64 * 2));
	DRAW_NUM(IMG_SELECT + 1, 0, SCREEN_HEIGHT - 64);

	//背景
	DRAW_IMAGE(BACK_TEX, -1, SCREEN_HEIGHT - 128, 1024, 128, 1024, 128);

	//画像本体
	float CENTER_X = IMAGE_POS_X + 512 / 2;
	float CENTER_Y = IMAGE_POS_Y + 256 / 2;
	float IW = 512 * IMAGE_ZOOM;
	float IH = 256 * IMAGE_ZOOM;
	float IX = CENTER_X - IW / 2;
	float IY = CENTER_Y - IH / 2;

	DRAW_IMAGE(IMAGE_TEX[IMG_SELECT], IX, IY, IW, IH, 512, 256);

	//描画完了
	pvr_list_finish();
	pvr_scene_finish();
}