#include <kos.h>
#include <png/png.h>
#include <oggvorbis/sndoggvorbis.h>
#include <dc/biosfont.h>

#include "Main.h"

int IMAGE_POS_X = 0;
int IMAGE_POS_Y = 0;
float IMAGE_ZOOM = 1.5f;

int IMG_SELECT = 0;
const int IMG_LENGTH = 12;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//テクスチャ
pvr_ptr_t BACK_TEX;
pvr_ptr_t IMAGE_TEX[1024];
pvr_ptr_t FONT_TEX[10];

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

void IMAGE_LOAD(){
	//次の画像は無いなら、0番目を読み込む
	if(IMG_SELECT + 1 > (IMG_LENGTH - 1)){
		IMAGE_TEX[0] = pvr_mem_malloc(512 * 256 * 2);
		png_to_texture("/rd/IMAGE/1.png", IMAGE_TEX[0], PNG_NO_ALPHA);
	} else {
		char FILE_NAME[256];
		sprintf(FILE_NAME, "/rd/IMAGE/%d.png", IMG_SELECT + 2);      //※画像は1スタートの連番やぞ！！だからIMG_SELECTに+2しないと参照地とファイル名が一致しない！

		//次の画像を読み込む
		IMAGE_TEX[IMG_SELECT + 1] = pvr_mem_malloc(512 * 256 * 2);
		png_to_texture(FILE_NAME, IMAGE_TEX[IMG_SELECT + 1], PNG_NO_ALPHA);

		//前の前の画像をメモリから消す
		pvr_mem_free(IMAGE_TEX[IMG_SELECT - 1]);
	}
}

void Main() {
	//init kos
	pvr_init_defaults();

	//背景画像を読み込む
	BACK_TEX = pvr_mem_malloc(1024 * 128 * 2);
	png_to_texture("/rd/backgroud.png", BACK_TEX, PNG_NO_ALPHA);

	//0番目と1番目の画像を読み込む
	IMAGE_TEX[0] = pvr_mem_malloc(512 * 256 * 2);
	png_to_texture("/rd/IMAGE/1.png", IMAGE_TEX[0], PNG_NO_ALPHA);
	IMAGE_TEX[1] = pvr_mem_malloc(512 * 256 * 2);
	png_to_texture("/rd/IMAGE/2.png", IMAGE_TEX[1], PNG_NO_ALPHA);

	//フォントを読み込む
	for(int I = 0; I < 10; I++){
		char FILE_NAME[256];
		sprintf(FILE_NAME, "/rd/FONT/%d.png", I);

		FONT_TEX[I] = pvr_mem_malloc(32 * 64 * 2);
		png_to_texture(FILE_NAME, FONT_TEX[I], PNG_NO_ALPHA);
	}

	/*OGGを再生する実験の残骸
	snd_stream_init();
	sndoggvorbis_init();
	sndoggvorbis_start("/rd/BGM/Soaring_RES.ogg", 0);
	*/

	//画像を真ん中に表示するための座標を計算して代入する
	IMAGE_POS_X = (SCREEN_WIDTH - 512) / 2;

	int PLES = 0;
	while(true) {
		MAPLE_FOREACH_BEGIN(MAPLE_FUNC_CONTROLLER, cont_state_t, state)

		//押されているフラグが経っていないなら処理
		if(PLES == 0){
			//何かが押されているなら
			if(state->buttons != 0){
				//押されているフラグを建てる
				PLES = 1;
			}

			//画像切り替え(Aボタン)
			if(state->buttons & CONT_A){
				//次の画像を指定
				IMG_SELECT++;

				//画像の最大枚数よりも多かったら
				if(IMG_SELECT > (IMG_LENGTH - 1)){
					//0に戻す
					IMG_SELECT = 0;
				}

				//画像を読み込む
				IMAGE_LOAD();
			}

			//移動系
			if(state->buttons & CONT_DPAD_UP){
				//上
				printf("UP\n");
				//IMAGE_POS_Y = IMAGE_POS_Y - 5;
				IMAGE_ZOOM = IMAGE_ZOOM + 0.05f;
			}else if(state->buttons & CONT_DPAD_DOWN){
				//下
				printf("UP\n");
				//IMAGE_POS_Y = IMAGE_POS_Y + 5;

				if(IMAGE_ZOOM > 1){
					IMAGE_ZOOM = IMAGE_ZOOM - 0.05f;
				}
			}else if(state->buttons & CONT_DPAD_LEFT){
				//左
				printf("LEFT\n");
				IMAGE_POS_X = IMAGE_POS_X - 5;
			}else if(state->buttons & CONT_DPAD_RIGHT){
				//右
				printf("RIGHT\n");
				IMAGE_POS_X = IMAGE_POS_X + 5;
			}
		} else {
			if(state->buttons == 0){
				//押されているフラグを折る
				PLES = 0;
			}
		}

		//入力チェック終了
		MAPLE_FOREACH_END();

		//画面病が
		DRAW();
	}

	return;
}