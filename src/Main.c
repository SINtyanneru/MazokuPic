#include <kos.h>
#include <png/png.h>

#include "Main.h"
#include "LOADER.c"
#include "DRAW.c"

int IMAGE_POS_X = 0;
int IMAGE_POS_Y = 0;
float IMAGE_ZOOM = 1.0f;

int IMG_SELECT = 0;
extern const int IMG_LENGTH;

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

//テクスチャ
pvr_ptr_t BACK_TEX;
pvr_ptr_t IMAGE_TEX[1024];
pvr_ptr_t FONT_TEX[10];

void Main() {
	//init kos
	pvr_init_defaults();

	INIT_LOAD_DATA();

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
			}else if(state->buttons & CONT_DPAD_RIGHT){
				//右
				printf("RIGHT\n");
			}
		} else {
			if(state->buttons == 0){
				//押されているフラグを折る
				PLES = 0;
			}
		}

		//アナログスティック
		IMAGE_POS_X = IMAGE_POS_X - (state->joyx / 50);
		IMAGE_POS_Y = IMAGE_POS_Y - (state->joyy / 50);

		//入力チェック終了
		MAPLE_FOREACH_END();

		//画面病が
		DRAW();
	}

	return;
}