#include <stdlib.h>
#include <stdio.h>

#define clear() printf("\033[H\033[J")

struct my_complex{
	int real;
	int image;
} z1, z2, res;

void input(){
	printf ("Input real of z1: ");
	scanf("%d", &z1.real);
	printf ("Input image of z1: ");
	scanf ("%d", &z1.image);
	printf ("Input real of z2: ");
	scanf ("%d", &z2.real);
	printf ("Input image of z2: ");
	scanf ("%d", &z2.image);
}
void print(int real, int image){
	if (image > 0)
		printf ("Result: %d + %di", real, image);
	else 
		printf ("Result: %d - %di", real, image*-1);
}
void add(){
	res.real = z1.real+z2.real;
	res.image = z1.image+z2.image;
	print (res.real, res.image);
}
void sub(){
	res.real = z1.real-z2.real;
	res.image = z1.image-z2.image;
	print (res.real, res.image);
}
void mult(){
	res.real = z1.real*z2.real - z1.image*z2.image;
	res.image = z1.real*z2.image + z2.real*z1.image;
	print (res.real, res.image);
}
void divis(){
	float real = (float)(z1.real*z2.real+z1.image*z2.image)/(z2.real*z2.real+z2.image*z2.image);
	float image = (float)(z2.real*z1.image-z1.real*z2.image)/(z2.real*z2.real+z2.image*z2.image);
	if (image > 0)
		printf ("Result: %f + %fi", real, image);
	else 
		printf ("Result: %f - %fi", real, image*-1);
}
