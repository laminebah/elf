#include <stdio.h>
int addition(int a, int b){
	
	return a + b ;
}
int main(int argc, char** argv){
	int a = 3;
	int b = 4;
	int c = addition(a,b);
	printf("%d + %d = %d \n", a, b, c);
	
	return 0;
}
