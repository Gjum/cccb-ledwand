#include "ledwand.h"
#include <stdio.h>
#include <unistd.h>
#include <math.h>

#define numFuns 23
int vals[numFuns][LEDWAND_PIXEL_X];

void frameCalcs(int frame) {
	for (int x = 0; x < LEDWAND_PIXEL_X; x++) {
		float framem = -2.0 * M_PI * frame / LEDWAND_PIXEL_X;
		float xm = (sin(framem)*2+4) * M_PI * x / LEDWAND_PIXEL_X - framem;
		for (int i = 0; i < numFuns; i++) {
			float im = (1.0*i / numFuns)*2.0;
			im = ( (im < 1.0) ? ( powf( 1-im, 2 ) ) : ( -powf( im-1, 2 ) ) ) * M_PI;
			vals[i][x] = ( sin(xm + framem + im) * sin(framem + im) / -2.0 + 0.5 ) * (LEDWAND_PIXEL_Y-4);
		}
	}
}

char step(int x, int y, int frame) {
	for (int i = 0; i < numFuns; i++) {
		if (y == vals[i][x])
			return 1;
	}
	return 0;
}

int main(int argc, char **argv) {
	Ledwand ledwand;
	if (ledwand_init(&ledwand)) {
		perror("FEHLER beim init\n");
		return -1;
	}

	uint32_t i = 0, j = 0, k = 0, x = 0, y = 0;
	const uint32_t buf_len = LEDWAND_PIXEL_X * LEDWAND_PIXEL_Y;
	uint8_t buffer[buf_len];
	k = 0;
	while (++k) {
		frameCalcs(k);
		for (i = 0; i < buf_len; i++) {
			x = (i*8) % LEDWAND_PIXEL_X;
			y = (i*8) / LEDWAND_PIXEL_X;
			buffer[i] = 0;
			for (j = 0; j < 8; j++) {
				buffer[i] <<= 1;
				buffer[i] += step(x, y, k);
				x++;
			}
		}
		ledwand_draw_image(&ledwand, buffer, buf_len);
		//usleep(100000);
	}

	//ledwand_clear(&ledwand);
	close(ledwand.s_sock);
	return 0;
}

