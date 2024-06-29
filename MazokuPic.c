#include <kos.h>
#include <png/png.h>
#include <dc/minifont.h>

//textures
pvr_ptr_t back_tex;

int OFFSET_X = 0;
int OFFSET_Y = 0;

int IMG_SELECT = 0;
const int IMG_LENGTH = 3;

//draw background
void draw_back() {
	pvr_poly_cxt_t cxt;
	pvr_poly_hdr_t hdr;
	pvr_vertex_t vert;

	pvr_poly_cxt_txr(&cxt, PVR_LIST_OP_POLY, PVR_TXRFMT_RGB565, 512, 256, back_tex, PVR_FILTER_BILINEAR);
	pvr_poly_compile(&hdr, &cxt);
	pvr_prim(&hdr, sizeof(hdr));

	vert.argb = PVR_PACK_COLOR(1.0f, 1.0f, 1.0f, 1.0f);
	vert.oargb = 0;
	vert.flags = PVR_CMD_VERTEX;

	vert.x = 0.0f + OFFSET_X;
	vert.y = 0.0f + OFFSET_Y;
	vert.z = 1.0f;
	vert.u = 0.0f;
	vert.v = 0.0f;
	pvr_prim(&vert, sizeof(vert));

	vert.x = 640.0f + OFFSET_X;
	vert.y = 0.0f + OFFSET_Y;
	vert.z = 1.0f;
	vert.u = 1.0f;
	vert.v = 0.0f;
	pvr_prim(&vert, sizeof(vert));

	vert.x = 1.0f + OFFSET_X;
	vert.y = 320.0f + OFFSET_Y;
	vert.z = 1.0f;
	vert.u = 0.0f;
	vert.v = 1.0f;
	pvr_prim(&vert, sizeof(vert));

	vert.x = 640.0f + OFFSET_X;
	vert.y = 320.0f + OFFSET_Y;
	vert.z = 1.0f;
	vert.u = 1.0f;
	vert.v = 1.0f;
	vert.flags = PVR_CMD_VERTEX_EOL;
	pvr_prim(&vert, sizeof(vert));
}

//draw one frame
void draw_frame() {
	pvr_wait_ready();
	pvr_scene_begin();

	pvr_list_begin(PVR_LIST_OP_POLY);
	draw_back();
	pvr_list_finish();

	pvr_scene_finish();
}

void SELECT_IMAGE(int I){
	char FILE_NAME[256];

	sprintf(FILE_NAME, "/rd/IMAGE/%d.png", I);

	//読み込む
	back_tex = pvr_mem_malloc(512 * 256 * 2);
	png_to_texture(FILE_NAME, back_tex, PNG_NO_ALPHA);
}

int main() {
	//init kos
	pvr_init_defaults();

	//画像を読み込む
	SELECT_IMAGE(IMG_SELECT);

	//keep drawing frames
	while(true) {
		MAPLE_FOREACH_BEGIN(MAPLE_FUNC_CONTROLLER, cont_state_t, state)
		//画像切り替え
		if(state->buttons & CONT_A){
			printf("A\n");
			
			//次の画像を指定
			IMG_SELECT++;

			//画像の最大枚数よりも多かったら
			if(IMG_SELECT > IMG_LENGTH){
				//0に戻す
				IMG_SELECT = 0;
			}

			//画像を読み込む
			SELECT_IMAGE(IMG_SELECT);
		}

		//移動系
		if(state->buttons & CONT_DPAD_UP){
			//上
			printf("UP\n");
			OFFSET_Y--;
		}else if(state->buttons & CONT_DPAD_DOWN){
			//下
			printf("UP\n");
			OFFSET_Y++;
		}else if(state->buttons & CONT_DPAD_LEFT){
			//左
			printf("LEFT\n");
			OFFSET_X--;
		}else if(state->buttons & CONT_DPAD_RIGHT){
			//右
			printf("RIGHT\n");
			OFFSET_X++;
		}

		MAPLE_FOREACH_END()

		draw_frame();
	}

	return 0;
}

