#include <png/png.h>

extern int IMG_SELECT;
extern const int IMG_LENGTH;
extern pvr_ptr_t BACK_TEX;
extern pvr_ptr_t IMAGE_TEX[1024];
extern pvr_ptr_t FONT_TEX[10];
extern pvr_ptr_t ICON_TEX[3];


/**
 * 画像ファイルを読み込みます
 */
void LOAD_IMAGE_FILE(pvr_ptr_t TARGET, char *FILE_NAME, int W, int H){
	TARGET = pvr_mem_malloc(W * H * 2);
	png_to_texture(FILE_NAME, TARGET, PNG_NO_ALPHA);
}

/**
 * データを読み込み初期化します
 */
void INIT_LOAD_DATA(){
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

	//アイコンをロード
	LOAD_IMAGE_FILE(ICON_TEX[0], "/rd/ICON/CURSOR.png", 16, 16);
	LOAD_IMAGE_FILE(ICON_TEX[1], "/rd/ICON/LEFT.png", 16, 16);
	LOAD_IMAGE_FILE(ICON_TEX[2], "/rd/ICON/RIGHT.png", 16, 16);
}

/**
 * 画像を読み込みます
 */
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