#include <cstdio>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <cstring>
#include <string>
#include <set>
#include <map>
#include <queue>
using namespace std;

char buff[ 1 << 16 ];
typedef vector< string >::iterator vit;
typedef set< string >::iterator sit;
typedef map< pair< string, string> , set< string > >::iterator mit;

vector< string > split( string ulaz, string delimiter)
{
	vector< string > ret;
	string match = "", temp = "";

	for (int i = 0; i < ulaz.length(); i++) {
		match = "";
		for (int j = 0; i + j < ulaz.length() && j < delimiter.length(); j++) {
			match += ulaz[ i + j ];
		}

		if (match == delimiter) {
			ret.push_back( temp );
			temp = "";
			i += delimiter.length() - 1;
		} else {
			temp += ulaz[i];
		}
		
	}

	if (temp != "") {
		ret.push_back( temp );
	}
	
	return ret;
}

void merge( set< string >&a, set< string > b)
{
	for (sit i = b.begin(); i != b.end(); i++) {
		a.insert( *i );
	}
	return;
}

struct automat {
	set< string > Q, sigma, F;
	set< string > Q0;
	map< pair< string, string >, set< string > > delta;

	vector< string > ulaz;

	void input();
	void dopuni();
	set< string > epsilon_okolina( string Q );
	void dopuni_epsilon_okolina( string Q );
	void simuliraj();
	
};

void automat::input()
{
	vector< string > v;
	
	gets( buff );
	ulaz = split( string( buff ), "|" );

	gets( buff );
	v = split( string( buff ), "," );

	for (vit i = v.begin();i != v.end(); ++i) {
		Q.insert( *i );
	}

	gets( buff );
	v = split( string( buff ), "," );

	for (vit i = v.begin();i != v.end(); i++) {
		sigma.insert( *i );
	}
	
	gets( buff );
	v = split( string( buff ), "," );

	for (vit i = v.begin();i != v.end(); i++) {
		F.insert( *i );
	}
	
	gets( buff );
	Q0.insert( string( buff ) );

	while (gets( buff )) {
		v = split( string( buff ), "->" );
		
		string q1 = split( v[0], "," )[0];
		string aa = split( v[0], "," )[1];
	
		v = split( v[1], "," );
		for (vit i = v.begin();i != v.end(); i++) {
			if( *i != "#" )delta[ make_pair( q1, aa ) ].insert( *i );
		}
		
	}

	return;
}

set<string> automat::epsilon_okolina( string Q )
{
	set< string > okolina;
	queue< string > red;
	
	red.push( Q );
	okolina.insert( Q );

	while ( !red.empty() ) {
		string now = red.front();
		red.pop();

		set< string > temp_delta = delta[ make_pair( now, "$" ) ];
		
		for (sit i = temp_delta.begin(); i != temp_delta.end(); i++) {
			if (okolina.count( *i ) == 0) {
				red.push( *i );
				okolina.insert( *i );
			}
		}
	}
	
	return okolina;
}

void automat::dopuni_epsilon_okolina( string Q )
{
	set< string > okolina = epsilon_okolina( Q );

	for (mit i = delta.begin();i != delta.end(); i++) {
		if (i->second.count( Q )) {
			for (sit j = okolina.begin(); j != okolina.end(); j++) {
				i->second.insert( *j );
			}
			
		}
	}
	
	return;
}

void automat::dopuni()
{
	for (sit i = Q.begin(); i != Q.end(); i++) {
		dopuni_epsilon_okolina( *i );
	}

	Q0 = epsilon_okolina( *Q0.begin() );
	
	return;
}


void automat::simuliraj()
{
	vector< string > znakovi;
	for (vit iter = ulaz.begin(); iter != ulaz.end(); iter++) {
		znakovi = split( *iter, "," );
		znakovi.push_back( "" );

		set< string > stanja = Q0;

		for (int i = 0; i < znakovi.size(); ) {
			if (stanja.size() == 0) {
				printf("#");
			} else {
				set< string > next;

				for (sit k = stanja.begin(); k != stanja.end(); ) {
					merge( next, delta[ make_pair( *k, znakovi[i] ) ] );

					printf("%s", k->c_str() );
					++k;
					if (k != stanja.end()) {
						printf(",");
					}
				}
				
				stanja = next;

			}
			
			++i;
			printf( "%c", i == znakovi.size() ? '\n' : '|' );
		}
		

	}
	
	return;
}

int main(int argc, const char *argv[])
{
	automat enka;
	
	enka.input();	
	enka.dopuni();

	enka.simuliraj();

	return 0;
}
