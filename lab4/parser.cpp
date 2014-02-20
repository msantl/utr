#include <cstdio>
#include <algorithm>
#include <vector>
#include <cstring>
#include <cstdlib>
using namespace std;
const int MAX = 1<<16;

char buff[ MAX ];
int N;
char ulaz;

void A();
void B();
void C();
void S();

char next_char( void )
{
	static int i = 0;
	return i < N ? buff[ i++ ] : -1;
}

void fail()
{
	puts( "\nNE" );
	exit( 0 );
	return;
}

void A()
{
	putchar('A');
	switch( ulaz ) {
		case 'a':
			ulaz = next_char();
			break;
		case 'b':
			ulaz = next_char();
			C();
			break;
		default:
			fail();
			break;
	}
	return;
}

void B()
{
	putchar('B');
	switch( ulaz ) {
		case 'c':
			ulaz = next_char();
			if( ulaz != 'c' )fail();
			
			ulaz = next_char();
			S();
			if( ulaz != 'b' )fail();
			
			ulaz = next_char();
			if( ulaz != 'c' )fail();
			
			ulaz = next_char();
			break;
		default:
			break;
	}
	return;
}

void C()
{
	putchar('C');
	A();
	A();
	return;
}

void S()
{
	putchar('S');

	switch( ulaz ) {
		case 'a':
			ulaz = next_char();
			A();
			B();
			break;
		case 'b':
			ulaz = next_char();
			B();
			A();
			break;
		default:
			fail();
			break;
	}

	return;
}

int main(int argc, const char *argv[])
{
	gets( buff );
	N = strlen( buff );

	ulaz = next_char();

	S();
	
	puts( ulaz == -1 ? "\nDA" : "\nNE" );

	return 0;
}
