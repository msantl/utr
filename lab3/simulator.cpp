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

typedef pair< string, pair< string, string > > zapis;

typedef vector< string >::iterator vit;
typedef set< string >::iterator sit;
typedef map< zapis, pair< string, string> >::iterator mit;

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
	set< string > Q, sigma, F, gama;
	string Q0, K0;
	map< zapis, pair< string, string> > delta;

	vector< string > ulaz;

	void input();	
	void output();
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
	v = split( string(buff), "," );

	for(vit i = v.begin();i != v.end(); i++){
		gama.insert( *i );
	}

	gets( buff );
	v = split( string( buff ), "," );

	for (vit i = v.begin();i != v.end(); i++) {
		F.insert( *i );
	}
	
	gets( buff );
	Q0 = string( buff );

	gets( buff );
	K0 = string( buff );


	while (gets( buff )) {
		v = split( string( buff ), "->" );
		
		vector< string > lhs = split( v[0], "," );
		vector< string > rhs = split( v[1], "," );
		//pair< STANJE, pair< ULAZ, STOG > > => pair< STANJE, STOG > 	
		delta[ make_pair( lhs[0], make_pair( lhs[1], lhs[2] ) ) ] = make_pair( rhs[0], rhs[1] );
		
	}

	return;
}

void automat::output()
{
	for (sit i = Q.begin(); i != Q.end(); i++) {
		printf( "%s,", i->c_str() );
	}
	puts( "" );
		
	for (sit i = sigma.begin(); i != sigma.end(); i++) {
		printf( "%s,", i->c_str() );
	}
	puts( "" );

	for (sit i = gama.begin(); i != gama.end(); i++) {
		printf("%s,", i->c_str());
	}
	puts( "" );


	for (sit i = F.begin(); i != F.end(); i++) {
		printf("%s,", i->c_str());
	}
	puts( "" );
	
	printf("%s\n", Q0.c_str());

	printf("%s\n", K0.c_str());

	for (sit i = Q.begin(); i != Q.end(); i++) {
		for (sit j = sigma.begin(); j != sigma.end(); j++) {
			for (sit k = gama.begin(); k != gama.end(); k++) {
				zapis temp = make_pair( *i, make_pair( *j, *k ) );
				if (delta.find( temp ) == delta.end()) {
					continue;
				}
				printf("%s,%s,%s->%s,%s\n", i->c_str(), j->c_str(), k->c_str(), delta[ temp ].first.c_str(), delta[ temp ].second.c_str() );
			}
			
		}
		
	}
	

	
	return;
}

void automat::simuliraj()
{
	vector< string > znakovi;
	for (vit iter = ulaz.begin(); iter != ulaz.end(); iter++) {
		znakovi = split( *iter, "," );

		string stanje = Q0;
		string sstog;
		bool procitao_sve = true;

		deque< string > stog;
		
		stog.push_front( "#" );
		stog.push_front( K0 );

		printf("%s#%s", Q0.c_str(), K0.c_str());

		for (vit i = znakovi.begin();i != znakovi.end(); i++) {
			zapis temp1 = make_pair( stanje , make_pair( *i, stog.front() ) );	//procitani znak + vrh stoga
			zapis temp2 = make_pair( stanje , make_pair( "$", stog.front() ) );	//bez procitanog znaka + vrh stoga

			zapis temp = temp1;
			
			if (delta.find( temp ) == delta.end()) {
				//ne postoji prijelaz sa procitanim znakom i vrhom stoga
				temp = temp2;
				i--;
			}

			if (delta.find( temp ) == delta.end()) {
				//ne postoji prijelaz bez procitanog znaka s vrhom stoga
				printf( "|fail" );
				procitao_sve = false;
				break;
			}


			stanje = delta[temp].first;
			sstog = delta[temp].second;
			stog.pop_front();
			if (sstog != "$") {
			
				for ( int k = sstog.length() - 1; k >= 0; --k ) {
					stog.push_front( sstog.substr( k, 1 ) );
				}
					
			}
			
			printf("|%s#", stanje.c_str());
			
			for (deque< string >::iterator it = stog.begin(); it < stog.end(); ++it) {
				if( stog.size() == 1 ){
					printf( "$" );
				} else if( stog.size() > 1 && *it != "#" ){
					printf("%s", it->c_str());
				}
			}
		
		}
			
		//ako sam procitao sve i nisam u prihvatljivom stanju, pokusavam epsilon prijelazima doci do prihvatljivog
		while ( procitao_sve == 1 && stog.size() > 1 && F.find( stanje ) == F.end() ) {
			zapis temp = make_pair( stanje , make_pair( "$", stog.front() ) );
			
			if (delta.find( temp ) == delta.end()) {
				//ne ispisujem fail jer sam procitao sve znakove i ako nema prijelaza znaci da se niz ne prihvaca
				break;
			}

			stanje = delta[temp].first;
			sstog = delta[temp].second;
			
			stog.pop_front();
			
			if (sstog != "$") {
			
				for ( int k = sstog.length() - 1; k >= 0; --k ) {
					stog.push_front( sstog.substr( k, 1 ) );
				}
					
			}
			
			printf("|%s#", stanje.c_str());
			for (deque< string >::iterator it = stog.begin(); it < stog.end(); ++it) {
				if( stog.size() == 1 ){
					printf( "$" );
				} else if( stog.size() > 1 && *it != "#" ){
					printf("%s", it->c_str());
				}
			}
			
		}


		if (procitao_sve == 1 && F.find( stanje ) != F.end() ) {
			printf( "|1\n" );
		} else{
			printf( "|0\n" );
		}

	}
	
	return;
}

int main(int argc, const char *argv[])
{
	automat dpa;
	
	dpa.input();

	//dpa.output();

	dpa.simuliraj();

	return 0;
}
