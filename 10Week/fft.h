#include<iostream>
#include<fstream>
#include <math.h>
using namespace std;
#pragma once

#define uchar unsigned char

typedef struct tagCOMPLEX {
    double real, imag;
} COMPLEX;

void Compute_Spectrum(double **Real, double **Imag, int W, int H, double **Spectrum)
{
    int u, v;
    for (u = 0; u < W; u++)
    {
        for (v = 0; v < H; v++)
        {
            Spectrum[v][u] = log(1. + sqrt(Real[v][u] * Real[v][u] + Imag[v][u] * Imag[v][u]));
        }
    }
}

void fft(COMPLEX  *x, int m)
{
    static COMPLEX  *w;           /* used to store the w COMPFLEX array */
    static int mstore = 0;       /* stores m for future reference */
    static int n = 1;            /* length of fft stored for future */

    COMPLEX u,temp,tm;
    COMPLEX  *xi,*xip,*xj,*wptr;

    long i,j,k,l,le,windex;

    double arg,w_real,w_imag,wrecur_real,wrecur_imag,wtemp_real;

    if(m != mstore) {

/* free previously allocated storage and set new m */

	if(mstore != 0) free(w);
	mstore = m;
        if(m == 0) return;       /* if m=0 then done */

/* n = 2**m = fft length */

        n = 1 << m;
        le = n/2;

/* allocate the storage for w */

	w = (COMPLEX  *)malloc((long)(le-1) * (long)sizeof(COMPLEX));

/* calculate the w values recursively */

        arg = 4.0*atan(1.0)/le;         /* PI/le calculation */
        wrecur_real = w_real = cos(arg);
        wrecur_imag = w_imag = -sin(arg);
	for (j = 1 ; j < le ; j++) {
	    (w + j - 1)->real = (float)wrecur_real;
	    (w + j - 1)->imag = (float)wrecur_imag;
	    wtemp_real = wrecur_real*w_real - wrecur_imag*w_imag;
            wrecur_imag = wrecur_real*w_imag + wrecur_imag*w_real;
            wrecur_real = wtemp_real;
        }
    }

/* start fft */

    le = n;
    windex = 1;
    for (l = 0 ; l < m ; l++) {
        le = le/2;

/* first iteration with no multiplies */

        for(i = 0 ; i < n ; i = i + 2*le) {
            xi = x + i;
            xip = xi + le;
	    temp.real = xi->real + xip->real;
	    temp.imag = xi->imag + xip->imag;
	    xip->real = xi->real - xip->real;
	    xip->imag = xi->imag - xip->imag;
            *xi = temp;
        }

/* remaining iterations use stored w */

        wptr = w + windex - 1;
        for (j = 1 ; j < le ; j++) {
            u = *wptr;
            for (i = j ; i < n ; i = i + 2*le) {
                xi = x + i;
                xip = xi + le;
		temp.real = xi->real + xip->real;
		temp.imag = xi->imag + xip->imag;
		tm.real = xi->real - xip->real;
		tm.imag = xi->imag - xip->imag;
		xip->real = tm.real*u.real - tm.imag*u.imag;
		xip->imag = tm.real*u.imag + tm.imag*u.real;
                *xi = temp;
            }
            wptr = wptr + windex;
        }
        windex = 2*windex;
    }            

/* rearrange data by bit reversing */

    j = 0;
    for (i = 1 ; i < (n-1) ; i++) {
        k = n/2;
        while(k <= j) {
            j = j - k;
            k = k/2;
        }
        j = j + k;
        if (i < j) {
            xi = x + i;
            xj = x + j;
            temp = *xj;
            *xj = *xi;
            *xi = temp;
        }
    }
}

void FFT_2D(uchar **Real, double **Real2, double **Imag2, int width, int height)
{
	int x,y,LN;
	COMPLEX *tmp;

	tmp = new COMPLEX[width];
	LN = (int) (log((double)width) / log(2.) + 0.1);
	//ROW -- FFT Transform 

	for(y = 0; y < height; y++)
	{
		for(x = 0; x< width ; x++)
		{
			tmp[x].real = Real[y][x];
			tmp[x].imag = 0;
		}
		fft(tmp, LN);

		for(x=0;x<width;x++)
		{
			Real2[y][x] = tmp[x].real;
			Imag2[y][x] = tmp[x].imag;
		}

	}

	// Column-FFT Transform
	for(x = 0; x< width; x++)
	{
		for(y=0; y<height; y++)
		{
			tmp[y].real = Real2[y][x];
			tmp[y].imag = Imag2[y][x];
		}
		fft(tmp, LN);

		for(y=0 ;y <height ;y++)
		{
			Real2[y][x] = tmp[y].real;
			Imag2[y][x] = tmp[y].imag;
		}

	}
	delete tmp;

}


void ifft(COMPLEX  *x, int m)
{
    static COMPLEX  *w;           /* used to store the w COMPLEX array */
    static int mstore = 0;       /* stores m for future reference */
    static int n = 1;            /* length of fft stored for future */

    COMPLEX u,temp,tm;
    COMPLEX  *xi,*xip,*xj,*wptr;

    long i,j,k,l,le,windex;

    double arg,w_real,w_imag,wrecur_real,wrecur_imag,wtemp_real;
    float scale;

    if(m != mstore) {

/* free previously allocated storage and set new m */

	if(mstore != 0) free(w);
	mstore = m;
        if(m == 0) return;       /* if m=0 then done */

/* n = 2**m = inverse fft length */

        n = 1 << m;
        le = n/2;

/* allocate the storage for w */

	w = (COMPLEX  *)malloc( (long)(le-1) * (long)sizeof(COMPLEX));

/* calculate the w values recursively */

        arg = 4.0*atan(1.0)/le;         /* PI/le calculation */
        wrecur_real = w_real = cos(arg);
	wrecur_imag = w_imag = sin(arg); 

	for (j = 1 ; j < le ; j++) {
	    (w + j - 1)->real = (float)wrecur_real;
	    (w + j - 1)->imag = (float)wrecur_imag;
	    wtemp_real = wrecur_real*w_real - wrecur_imag*w_imag;
            wrecur_imag = wrecur_real*w_imag + wrecur_imag*w_real;
            wrecur_real = wtemp_real;
        }
    }

/* start inverse fft */

    le = n;
    windex = 1;
    for (l = 0 ; l < m ; l++) {
        le = le/2;

/* first iteration with no multiplies */

        for(i = 0 ; i < n ; i = i + 2*le) {
            xi = x + i;
            xip = xi + le;
	    temp.real = xi->real + xip->real;
	    temp.imag = xi->imag + xip->imag;
	    xip->real = xi->real - xip->real;
	    xip->imag = xi->imag - xip->imag;
            *xi = temp;
        }

/* remaining iterations use stored w */

        wptr = w + windex - 1;
        for (j = 1 ; j < le ; j++) {
            u = *wptr;
            for (i = j ; i < n ; i = i + 2*le) {
                xi = x + i;
                xip = xi + le;
		temp.real = xi->real + xip->real;
		temp.imag = xi->imag + xip->imag;
		tm.real = xi->real - xip->real;
		tm.imag = xi->imag - xip->imag;
		xip->real = tm.real*u.real - tm.imag*u.imag;
		xip->imag = tm.real*u.imag + tm.imag*u.real;
                *xi = temp;
            }
            wptr = wptr + windex;
        }
        windex = 2*windex;
    }            

/* rearrange data by bit reversing */

    j = 0;
    for (i = 1 ; i < (n-1) ; i++) {
        k = n/2;
        while(k <= j) {
            j = j - k;
            k = k/2;
        }
        j = j + k;
        if (i < j) {
            xi = x + i;
            xj = x + j;
            temp = *xj;
            *xj = *xi;
            *xi = temp;
        }
    }

/* scale all results by 1/n */
    scale = (float)(1.0/n);
    for(i = 0 ; i < n ; i++) {
        x->real = scale*x->real;
	x->imag = scale*x->imag;
        x++;
    }
}

void IFFT_2D(double **Real, double **Imag, double **Real2, double **Imag2, int width, int height)
{
	int x,y,LN;
	COMPLEX *tmp;

	tmp = new COMPLEX[width];

	LN = (int) (log((double)width) / log(2.) + 0.1);
	//ROW -- FFT Transform 

	for(y = 0; y < height; y++)
	{
		for(x = 0; x< width ; x++)
		{
			tmp[x].real = Real[y][x];
			tmp[x].imag = Imag[y][x];
		}
		
		ifft(tmp, LN);

		for(x=0;x<width;x++)
		{
			Real2[y][x] = tmp[x].real;
			Imag2[y][x] = tmp[x].imag;
		}

	}

	// Column-FFT Transform
	for(x = 0; x< width; x++)
	{
		for(y=0; y<height; y++)
		{
			tmp[y].real = Real2[y][x];
			tmp[y].imag = Imag2[y][x];
		}
		ifft(tmp, LN);

		for(y=0 ;y <height ;y++)
		{
			Real2[y][x] = tmp[y].real;
			Imag2[y][x] = tmp[y].imag;
		}

	}
    delete tmp;
}


void DNormalize2D(double **p1, uchar **p2, int W, int H)
{
	int x,y;
	double min = 9999;
	double max = -9999;
	double val;

	for(y=0;y<H;y++)
	{
		for(x=0;x<W; x++)
		{
			val =p1[y][x];
			if(val > max) max = val;
			if(val < min) min = val;
		}
	}

	if(max == min)
	{
		for(y=0; y<H;y++)
		{
			for(x=0; x<W; x++)
			{
				p2[y][x] = 0;
			}
		}
		return;
	}
	
	double dfactor = 255/(max-min);
	for(y=0; y<H; y++)
	{
		for(x=0; x<W; x++)
		{
			p2[y][x] = (uchar) ( (p1[y][x] - min) * dfactor);
		}
	}
}