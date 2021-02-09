#include <stdio.h>
#include <stdlib.h>
#include "img.h"
static unsigned char buf[HEIGHT][WIDTH][3];
static int filecnt = 0;
static char fname[100];
void img_clear(void) {
    int i,j;
    for(j=0; j<HEIGHT; j++) {
        for (i=0; i<WIDTH; i++) {
            buf[j][i][0] = buf[j][i][1] = buf[j][i][2] = 255;
        }
    }
}

void img_write(void) {
    sprintf(fname, "img%04d.ppm", filecnt++);
    FILE *f = fopen(fname, "wb");
    if (f == NULL) {
        fprintf(stderr, "can't open %s\n", fname);
        exit(1);
    }

    fprintf(f,"P6\n%d %d\n255\n", WIDTH, HEIGHT);
    fwrite(buf, sizeof(buf), 1, f);
    fclose(f);
}

void img_putpixel(struct color c, int x, int y) {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
        return;
    }

    buf[HEIGHT-y-1][x][0] = c.r;
    buf[HEIGHT-y-1][x][1] = c.g;
    buf[HEIGHT-y-1][x][2] = c.b;
}

void img_fillcircle(struct color c, double x, double y, double r) {
    int imin = (int)(x - r - 1), imax = (int)(x + r + 1);
    int jmin = (int)(y - r - 1), jmax = (int)(y + r + 1);
    int i,j;
    for(j=jmin; j<=jmax; j++) {
        for (i=imin; i<=imax; i++) {
            if((x-i)*(x-i) + (y-j)*(y-j) <= r*r) {
                img_putpixel(c,i,j);
            }
        }
    }
}

void img_fillpartsV(struct color c, double x, double y, double r) {
    img_fillcircle(c, x, y, r);
    img_fillcircle(c, x, y+6*r, r);
    int imin = (int)(x - r), imax = (int)(x + r);
    int jmin = (int)(y), jmax = (int)(y + r * 6);
    int i,j;

    for(j=jmin; j <= jmax; j++) {
        for (i=imin; i <= imax; i++) {
            img_putpixel(c,i,j);
        }
    }
}

void img_fillpartsS(struct color c, double x, double y, double r) {
    img_fillcircle(c, x, y, r);
    img_fillcircle(c, x+6*r, y, r);
    int imin = (int)(x), imax = (int)(x + r*6);
    int jmin = (int)(y - r), jmax = (int)(y + r);
    int i,j;
    for(j=jmin; j <= jmax; j++) {
        for (i=imin; i <= imax; i++) {
            img_putpixel(c,i,j);
        }
    }
}
