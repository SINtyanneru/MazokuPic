#include <kos.h>
#include <png/png.h>

/* textures */
pvr_ptr_t back_tex;

/* init background */
void back_init(void) {
	back_tex = pvr_mem_malloc(512 * 256 * 2);
	png_to_texture("/rd/1.png", back_tex, PNG_NO_ALPHA);
}

/* draw background */
void draw_back(void) {
	pvr_poly_cxt_t cxt;
	pvr_poly_hdr_t hdr;
	pvr_vertex_t vert;

	pvr_poly_cxt_txr(&cxt, PVR_LIST_OP_POLY, PVR_TXRFMT_RGB565, 512, 256, back_tex, PVR_FILTER_BILINEAR);
	pvr_poly_compile(&hdr, &cxt);
	pvr_prim(&hdr, sizeof(hdr));

	vert.argb = PVR_PACK_COLOR(1.0f, 1.0f, 1.0f, 1.0f);
	vert.oargb = 0;
	vert.flags = PVR_CMD_VERTEX;

	vert.x = 0.0f;
	vert.y = 0.0f;
	vert.z = 1.0f;
	vert.u = 0.0f;
	vert.v = 0.0f;
	pvr_prim(&vert, sizeof(vert));

	vert.x = 640.0f;
	vert.y = 0.0f;
	vert.z = 1.0f;
	vert.u = 1.0f;
	vert.v = 0.0f;
	pvr_prim(&vert, sizeof(vert));

	vert.x = 1.0f;
	vert.y = 320.0f;
	vert.z = 1.0f;
	vert.u = 0.0f;
	vert.v = 1.0f;
	pvr_prim(&vert, sizeof(vert));

	vert.x = 640.0f;
	vert.y = 320.0f;
	vert.z = 1.0f;
	vert.u = 1.0f;
	vert.v = 1.0f;
	vert.flags = PVR_CMD_VERTEX_EOL;
	pvr_prim(&vert, sizeof(vert));
}

/* draw one frame */
void draw_frame(void) {
	pvr_wait_ready();
	pvr_scene_begin();

	pvr_list_begin(PVR_LIST_OP_POLY);
	draw_back();
	pvr_list_finish();

	pvr_scene_finish();
}

int main(int argc, char **argv) {
	/* init kos  */
	pvr_init_defaults();

	/* init background */
	back_init();

	/* keep drawing frames until start is pressed */
	while(true) {
		MAPLE_FOREACH_BEGIN(MAPLE_FUNC_CONTROLLER, cont_state_t, state)

		if(state->buttons & CONT_DPAD_LEFT){
			printf("LEFT\n");
		}

		if(state->buttons & CONT_DPAD_RIGHT){
			printf("RIGHT\n");
		}

		MAPLE_FOREACH_END()

		draw_frame();
	}

	return 0;
}

