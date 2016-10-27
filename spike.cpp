#define colorR 1
#define colorG 2
#define colorB 3
#define IMPORT_NOT_BMP 0
#define IMPORT_BIT_WRONG 1
#define IMPORT_SUCCESS 2
#include <cstdlib>
#include <cstdio>
#include <cmath>

struct Image{
private:
	char *R, *G, *B;
public:
	int width, height;

	void newImage(int W, int H){
		width = W - 1;
		height = H - 1;
		R = (char*)malloc((width + 1) * (height + 1));
		G = (char*)malloc((width + 1) * (height + 1));
		B = (char*)malloc((width + 1) * (height + 1));
	};

	int openImage(FILE *file){
		unsigned char header[54];
		for(int i = 0; i < 54; i++){
			header[i] = fgetc(file);
		}
		if (header[0] != 0x42 || header[1] != 0x4D){
			return IMPORT_NOT_BMP;
		} else if (header[28] != 24) {
			return IMPORT_BIT_WRONG;
		} else {
			width = header[18] + (header[19]<<8) + (header[20]<<16) + (header[21]<<24);
			height = header[22] + (header[23]<<8) + (header[24]<<16) + (header[25]<<24);
			width--;
			height--;

			R = (char*)malloc((width + 1) * (height + 1));
			G = (char*)malloc((width + 1) * (height + 1));
			B = (char*)malloc((width + 1) * (height + 1));

			char *Rt, *Gt, *Bt;
			Rt = R;
			Gt = G;
			Bt = B;
			
			char dummy;
			
			if((width + 1) % 4){
				for(int y = 0; y < (height + 1); y++){
					for(int x = 0; x < (width + 1); x++){
						*(Bt++) = fgetc(file);
						*(Gt++) = fgetc(file);
						*(Rt++) = fgetc(file);
					}
					for(int i = 0; i < (4 - (width + 1) * 3 % 4); i++){
						dummy = fgetc(file);
					}
				}
			} else {
				for(int x = 0; x < ((width + 1) * (height + 1)); x++){
					*(Bt++) = fgetc(file);
					*(Gt++) = fgetc(file);
					*(Rt++) = fgetc(file);
				}
			}
			return IMPORT_SUCCESS;
		}
	};

	void saveImage(FILE *file){
		fputc(0x42, file);
		fputc(0x4D, file);
		fputc((((width + 1) * (height + 1) * 3) + 54) % ((int)pow(2, 8)), file);
		fputc((((width + 1) * (height + 1) * 3) + 54) % ((int)pow(2, 16)) / ((int)pow(2,8)), file);
		fputc((((width + 1) * (height + 1) * 3) + 54) % ((int)pow(2, 24)) / ((int)pow(2,16)), file);
		fputc((((width + 1) * (height + 1) * 3) + 54) / ((int)pow(2,24)), file);
		fputc(0, file);
		fputc(0, file);
		fputc(0, file);
		fputc(0, file);
		fputc(54, file);
		fputc(0, file);
		fputc(0, file);
		fputc(0, file);
		fputc(40, file);
		fputc(0, file);
		fputc(0, file);
		fputc(0, file);
		fputc((width + 1) % ((int)pow(2, 8)), file);
		fputc((width + 1) % ((int)pow(2, 16)) / ((int)pow(2,8)), file);
		fputc((width + 1) % ((int)pow(2, 24)) / ((int)pow(2,16)), file);
		fputc((width + 1) / ((int)pow(2,24)), file);
		fputc((height + 1) % ((int)pow(2, 8)), file);
		fputc((height + 1) % ((int)pow(2, 16)) / ((int)pow(2,8)), file);
		fputc((height + 1) % ((int)pow(2, 24)) / ((int)pow(2,16)), file);
		fputc((height + 1) / ((int)pow(2,24)), file);
		fputc(1, file);
		fputc(0, file);
		fputc(24, file);
		fputc(0, file);
		fputc(0, file);
		fputc(0, file);
		fputc(0, file);
		fputc(0, file);
		fputc((((width + 1) * (height + 1) * 3)) % ((int)pow(2, 8)), file);
		fputc((((width + 1) * (height + 1) * 3)) % ((int)pow(2, 16)) / ((int)pow(2,8)), file);
		fputc((((width + 1) * (height + 1) * 3)) % ((int)pow(2, 24)) / ((int)pow(2,16)), file);
		fputc((((width + 1) * (height + 1) * 3)) / ((int)pow(2,24)), file);
		fputc(0xC4, file);
		fputc(0x0E, file);
		fputc(0, file);
		fputc(0, file);
		fputc(0xC4, file);
		fputc(0x0E, file);
		fputc(0, file);
		fputc(0, file);
		fputc(0, file);
		fputc(0, file);
		fputc(0, file);
		fputc(0, file);
		fputc(0, file);
		fputc(0, file);
		fputc(0, file);
		fputc(0, file);

		char *Rt, *Gt, *Bt;
		Rt = R;
		Gt = G;
		Bt = B;

		if ((width + 1) % 4){
			for(int y = 0; y < (height + 1); y++){
				for(int x = 0; x < (width + 1); x++){
					fputc(*(Bt++), file);
					fputc(*(Gt++), file);
					fputc(*(Rt++), file);
				}
				for(int i = 0; i < (4 - (width + 1) * 3 % 4); i++){
					fputc(255, file);
				}
			}
		} else {
			for(int x = 0; x < ((width + 1) * (height + 1)); x++){
				fputc(*(Bt++), file);
				fputc(*(Gt++), file);
				fputc(*(Rt++), file);
			}
		}	
	}

	unsigned char getPixelData(int X, int Y, char color){
		switch (color){
			case colorR:
				return *(R + X + (Y * (width + 1)));
				break;
			case colorG:
				return *(G + X + (Y * (width + 1)));
				break;
			case colorB:
				return *(B + X + (Y * (width + 1)));
				break;
			default:
				return 0;
		};
	};

	void setPixelData(int X, int Y, char color, int data){
		if (data > 255) data = 255;
		if (data < 0) data = 0;
		switch (color){
			case colorR:
				*(R + X + (Y * (width + 1))) = data;
				break;
			case colorG:
				*(G + X + (Y * (width + 1))) = data;
				break;
			case colorB:
				*(B + X + (Y * (width + 1))) = data;
				break;
		}
	};

	void unloadImage(){
		free(R);
		free(G);
		free(B);
	}
};

int main(void) {
	char fnbuf[256];
	int thr, siz, ang;
	double val;
	printf("Input file name(only bmp supported): ");
	scanf("%s", fnbuf);
	printf("Input diffraction threshold(0-255): ");
	scanf("%d", &thr);
	if (thr < 0 || thr > 255) {
		printf("Wrong value!");
		return 1;
	}
	printf("Input diffraction strength(0-100): ");
	scanf("%lf", &val);
	if (val < 0 || val > 100) {
		printf("Wrong value!");
		return 1;
	}
	printf("Input diffraction size(0-300): ");
	scanf("%d", &siz);
	if (siz < 0 || siz > 300) {
		printf("Wrong value!");
		return 1;
	}
	printf("Input diffraction angle(0-90): ");
	scanf("%d", &ang);
	if (ang < 0 || ang > 90) {
		printf("Wrong value!");
		return 1;
	}
	/*sprintf(fnbuf, "NP.bmp");
	thr = 230;
	val = 10;
	siz = 100;
	ang = 0;*/

	val /= 10;

	Image image, mask;
	printf("Opening %s...\n", fnbuf);
	FILE *image_f = fopen(fnbuf, "rb");
	printf("Loading %s...\n", fnbuf);
	int imp = image.openImage(image_f);
	if (imp == IMPORT_SUCCESS) {
		printf("Loaded %s (%d x %d)\n", fnbuf, image.width+1, image.height+1);
	} else {
		puts("Failed to load image.");
		return 1;
	}
	printf("Creating buffer...\n");
	mask.newImage(image.width+1, image.height+1);
	printf("Creating mask...\n");

	int percentage = 0;
	int R, G, B;
	printf("0%% complete\n");
	for (int i = 0; i <= image.height; ++i) {
		if (percentage < int(i * 1.0 / image.height * 100)) {
			percentage = int(i * 1.0 / image.height * 100);
			printf("%d%% complete\n", percentage);
		} 
		for (int j = 0; j <= image.width; ++j) {
			R = image.getPixelData(j, i, colorR);
			G = image.getPixelData(j, i, colorG);
			B = image.getPixelData(j, i, colorB);
			if ((R + G + B)/3.0 >= thr) {
				int k, l;
				k = j - siz;
				l = j + siz;
				#define PI 3.14159265
				for (int m = k; m <= l; ++m) {
					double v = val / 100.0 * (siz - abs(m-j)) / siz;
					int x, y;
					x = cos(ang*PI/180)*(m-j) + j;
					y = sin(ang*PI/180)*(m-j) + i;
					if (x >= 0 && x <= image.width && y >= 0 && y <= image.height) {
						mask.setPixelData(x, y, colorR, mask.getPixelData(x, y, colorR) + R*v);
						mask.setPixelData(x, y, colorG, mask.getPixelData(x, y, colorG) + G*v);
						mask.setPixelData(x, y, colorB, mask.getPixelData(x, y, colorB) + B*v);
					}
				}
				k = i - siz;
				l = i + siz;
				for (int m = k; m <= l; ++m) {
					double v = val / 100.0 * (siz - abs(m-i)) / siz;
					int x, y;
					x = -sin(ang*PI/180)*(m-i) + j;
					y = cos(ang*PI/180)*(m-i) + i;
					if (x >= 0 && x <= image.width && y >= 0 && y <= image.height) {
						mask.setPixelData(x, y, colorR, mask.getPixelData(x, y, colorR) + R*v);
						mask.setPixelData(x, y, colorG, mask.getPixelData(x, y, colorG) + G*v);
						mask.setPixelData(x, y, colorB, mask.getPixelData(x, y, colorB) + B*v);
					}
				}
			}
		}
	}

	printf("Finished.\n");
	printf("Combining mask with the original image...\n");
	for (int i = 0; i <= image.width; ++i) {
		for (int j = 0; j <= image.height; ++j) {
			image.setPixelData(i, j, colorR, mask.getPixelData(i, j, colorR) + image.getPixelData(i, j, colorR));
			image.setPixelData(i, j, colorG, mask.getPixelData(i, j, colorG) + image.getPixelData(i, j, colorB));
			image.setPixelData(i, j, colorB, mask.getPixelData(i, j, colorB) + image.getPixelData(i, j, colorB));
		}
	}
	printf("Saving output as output.bmp\n");
	FILE *output = fopen("output.bmp", "wb");
	image.saveImage(output);
}