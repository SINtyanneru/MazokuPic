#include <kos.h>
#include <png/png.h>
#include <oggvorbis/sndoggvorbis.h>

#include "Main.h"

int OFFSET_X = 0;
int OFFSET_Y = 0;

int IMG_SELECT = 0;
const int IMG_LENGTH = 4;

//テクスチャ
pvr_ptr_t BACK_TEX;
pvr_ptr_t IMAGE_TEX[5];

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

//描画
void DRAW() {
	//初期化
	pvr_wait_ready();
	pvr_scene_begin();
	pvr_list_begin(PVR_LIST_OP_POLY);

	//手前に描画するものを前に書かないと、おかしなことになるぞ。。。

	/*
	char FILE_NAME[256];
	sprintf(FILE_NAME, "/rd/FONT/%d.png", IMG_SELECT);
	pvr_ptr_t FONT_TEX = pvr_mem_malloc(32 * 64 * 2);
	png_to_texture(FILE_NAME, FONT_TEX, PNG_NO_ALPHA);
	DRAW_IMAGE(FONT_TEX, 0, 0, 32, 64, 32, 64);
	*/

	DRAW_IMAGE(IMAGE_TEX[IMG_SELECT], OFFSET_X, OFFSET_Y, 640.0f, 320.0f, 512, 256);
	DRAW_IMAGE(BACK_TEX, 0, 0, 640.0f, 480.0f, 512, 512);

	//描画完了
	pvr_list_finish();
	pvr_scene_finish();
}

void Main() {
	//init kos
	pvr_init_defaults();

	//背景画像を読み込む
	BACK_TEX = pvr_mem_malloc(512 * 512 * 2);
	png_to_texture("/rd/backgroud.png", BACK_TEX, PNG_NO_ALPHA);

	//画像を読み込む
	for(int I = 0; I < IMG_LENGTH + 1; I++){
		char FILE_NAME[256];
		sprintf(FILE_NAME, "/rd/IMAGE/%d.png", I);

		IMAGE_TEX[I] = pvr_mem_malloc(512 * 512 * 2);
		png_to_texture(FILE_NAME, IMAGE_TEX[I], PNG_NO_ALPHA);
	}

	/*OGGを再生する実験の残骸
	snd_stream_init();
	sndoggvorbis_init();
	sndoggvorbis_start("/rd/BGM/Soaring_RES.ogg", 0);
	*/

	int PLES = 0;
	while(true) {
		MAPLE_FOREACH_BEGIN(MAPLE_FUNC_CONTROLLER, cont_state_t, state)

		//押されているフラグが経っていないなら処理
		if(PLES == 0){
			//画像切り替え(Aボタン)
			if(state->buttons & CONT_A){
				//次の画像を指定
				IMG_SELECT++;

				//画像の最大枚数よりも多かったら
				if(IMG_SELECT > IMG_LENGTH){
					//0に戻す
					IMG_SELECT = 0;
				}

				//押されているフラグを建てる
				PLES = 1;
			}
		} else {
			if(state->buttons == 0){
				//押されているフラグを折る
				PLES = 0;
			}
		}

		//移動系
		if(state->buttons & CONT_DPAD_UP){
			//上
			printf("UP\n");
			OFFSET_Y = OFFSET_Y - 5;
		}else if(state->buttons & CONT_DPAD_DOWN){
			//下
			printf("UP\n");
			OFFSET_Y = OFFSET_Y + 5;
		}else if(state->buttons & CONT_DPAD_LEFT){
			//左
			printf("LEFT\n");
			OFFSET_X = OFFSET_X - 5;
		}else if(state->buttons & CONT_DPAD_RIGHT){
			//右
			printf("RIGHT\n");
			OFFSET_X = OFFSET_X + 5;
		}

		MAPLE_FOREACH_END();
		DRAW();

		thd_sleep(10);
	}

	return;
}