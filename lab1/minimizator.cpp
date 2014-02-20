#include <cstdio>
#include <algorithm>
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <cstring>
#include <string>
#include <cstdlib>
#include <unistd.h>
using namespace std;

char buff[ 1 << 16 ];

struct DKA {
	set< string > Q, F;
	set< string > sigma;
	map< pair< string, string>, string > delta;
	string Q0;

	void print( void )
	{
		for( set< string >::iterator it = Q.begin(); it != Q.end(); ) {
			printf("%s", it->c_str());
			++it;
			if( it != Q.end() ) putchar( ',' );
		}
		putchar( '\n' );
		
		for( set< string >::iterator it = sigma.begin(); it != sigma.end(); ) {
			printf("%s", it->c_str());
			++it;
			if( it != sigma.end() ) putchar( ',' );
		}
		putchar( '\n' );
		
		for( set< string >::iterator it = F.begin(); it != F.end(); ) {
			printf("%s", it->c_str());
			++it;
			if( it != F.end() ) putchar( ',' );
		}
		putchar( '\n' );
		
		printf("%s\n", Q0.c_str());
		for( map< pair< string, string >, string >::iterator it = delta.begin(); it != delta.end(); ++it ) {
			printf("%s,%s->%s\n", it->first.first.c_str(), it->first.second.c_str(), it->second.c_str());
		}
	}
};

map< string, int > IDS;

string strip( string s )
{
	string ret = "";
	for (int i = 0; i < s.length(); i++) {
		if (s[i] != '\n') {
			ret = ret + s[i];
		}
	}
	
	return ret;	
}

vector< string > split( string s, string c )
{
	vector< string > ret;
	string temp = "", cmp = "";
	//printf("\t%s %s ", s.c_str(), c.c_str());
	//s = strip(s); c = strip( c );
	
	for (int i = 0; i < s.length(); i++) {

		cmp = "";
		for (int j = 0; j < c.length() && i + j < s.length(); j++) {
			cmp = cmp + s[ i + j ];
		}
		
		if (cmp == c) {
			if (temp != "") ret.push_back( temp );
			i += c.length() - 1;
			temp = "";
		} else {
			temp = temp + s[i];
		}

	}
	
	if (temp != "") ret.push_back( temp );

	//for (int i = 0; i < ret.size(); i++) { printf("%s ", ret[i].c_str()); } printf("\n");

	return ret;
}


void izbaci_nedohvatljiva( DKA &automat )
{
	set< string > noviQ;
	queue< string > q;
	q.push( automat.Q0 );

	while (!q.empty()) {
		string stanje = q.front();
		noviQ.insert( stanje );
		q.pop();


		for (set< string >::iterator it = automat.sigma.begin(); it != automat.sigma.end(); ++it) {
			string novo = automat.delta[ make_pair( stanje, *it ) ];
			
			if (noviQ.count( novo ) == 0) {
				q.push( novo );
				noviQ.insert( novo );
			}
		}

	}
	
	
	
	if( noviQ.count( automat.Q0 ) == 0 )
		automat.Q0 = "";

	automat.Q = noviQ;

	set< string > noviF;
	for( set< string >::iterator it = automat.F.begin(); it != automat.F.end(); ++it ) {
		if (noviQ.count( *it )) {
			noviF.insert( *it );
		}
	}
	automat.F = noviF;

	map< pair< string, string>, string > noviDelta;
	for( set< string >::iterator i1 = noviQ.begin(); i1 != noviQ.end(); ++i1 ) {
		for( set< string >::iterator i2 = automat.sigma.begin(); i2 != automat.sigma.end(); ++i2 ) {
			noviDelta[ make_pair( *i1, *i2 ) ] = automat.delta[ make_pair( *i1, *i2 ) ];
		}
	}
	automat.delta = noviDelta;

	return;
}


void minimiziraj( DKA &automat )
{
	#define MAX 110
	int old = 0, now = 1;
	
	int pripadnost[2][ MAX ];

	set< string > grupe[2][MAX];

	for( set< string >::iterator it = automat.Q.begin(); it != automat.Q.end(); ++it ) {
		grupe[old][ automat.F.count( *it ) ].insert( *it );
		pripadnost[old][ IDS[ *it ] ] = automat.F.count( *it );
	}

	int last = -1, curr = 2;

	while ( last != curr ) {
		for (int i = 0; i < MAX; i++) { grupe[now][i].clear(); pripadnost[now][i] = -1; }

		int counter = 0;
		
		for (int i = 0; i < MAX; i++) {
			if( grupe[old][i].size() <= 0 ) continue;
			//set< string >[old][i]
			for( set< string >::iterator i1 = grupe[old][i].begin(); i1 != grupe[old][i].end(); ++ i1 ) {
				for( set< string>::iterator i2 = i1; i2 != grupe[old][i].end(); ++ i2 ) {
					if ( i1 == i2) {
						continue;
					}
					int id1 = IDS[ *i1 ];
					int id2 = IDS[ *i2 ];
					
					bool ok = true;

					for( set< string >::iterator i3 = automat.sigma.begin(); i3 != automat.sigma.end(); ++i3 ) {
						if (pripadnost[old][ IDS[ automat.delta[ make_pair( *i1, *i3 ) ] ] ] != pripadnost[old][ IDS[ automat.delta[ make_pair( *i2, *i3 ) ] ] ]) {
							ok = false;
							break;
						}
					}

					if (ok) {
						if (pripadnost[now][id1] == -1 && pripadnost[now][id2] == -1) {
							pripadnost[now][id1] = pripadnost[now][id2] = counter++;
						} else if( pripadnost[now][id1] != -1 && pripadnost[now][id2] == -1 ) {
							pripadnost[now][id2] = pripadnost[now][id1];
						} else if( pripadnost[now][id1] == -1 && pripadnost[now][id2] != -1 ) {
							pripadnost[now][id1] = pripadnost[now][id2];
						} else if( pripadnost[now][id1] != pripadnost[now][id2] ) {
							int size1 = 0, size2 = 0;
							int g1 = pripadnost[now][id1];
							int g2 = pripadnost[now][id2];
							for( set< string >::iterator it = automat.Q.begin(); it != automat.Q.end(); ++it ) {
								if ( pripadnost[now][ IDS[*it] ] == g1 ) ++size1;
								if ( pripadnost[now][ IDS[*it] ] == g2 ) ++size2;
							}
							
							int manji = (size1 < size2) ? g1 : g2;
							int veci = (size1 >= size2) ? g2 : g1;
							for( set< string >::iterator it = automat.Q.begin(); it != automat.Q.end(); ++it ) {
								if ( pripadnost[now][ IDS[*it] ] == manji ){
									pripadnost[ now ][ IDS[*it] ]  = veci;
								}
							}
						
						}
					}

				}
			}
		}
	
	for ( set< string >::iterator it = automat.Q.begin(); it != automat.Q.end(); ++it ) {
		if (pripadnost[now][IDS[ *it ]] == -1) {
			pripadnost[now][IDS[ *it ]] = counter++;
		}
		grupe[now][ pripadnost[now][ IDS[ *it ] ] ].insert( *it );
	}
	
	swap( old, now );
		
	last = curr;
	curr = counter;
	}
	
	if( automat.Q0 != "" )automat.Q0 = *grupe[ old ][ pripadnost[old][ IDS[ automat.Q0 ] ] ].begin();
	
	set< string > noviF;
	for( set< string >::iterator it = automat.F.begin(); it != automat.F.end(); ++it ) {
		 noviF.insert( *grupe[ old ][ pripadnost[old][ IDS[ *it ] ] ].begin() );
	}
	automat.F = noviF;

	set< string > noviQ;
	for( set< string >::iterator it = automat.Q.begin(); it != automat.Q.end(); ++it ) {
		 noviQ.insert( *grupe[ old ][ pripadnost[old][ IDS[ *it ] ] ].begin() );
	}
	automat.Q = noviQ;

	map< pair< string, string>, string > noviDelta;
	for( set< string >::iterator i1 = noviQ.begin(); i1 != noviQ.end(); ++i1 ) {
		for( set< string >::iterator i2 = automat.sigma.begin(); i2 != automat.sigma.end(); ++i2 ) {
			noviDelta[ make_pair( *i1, *i2 ) ] = *grupe[old][ pripadnost[old][ IDS[ automat.delta[ make_pair( *i1, *i2 ) ] ] ] ].begin();
		}
	}
	automat.delta = noviDelta;

	return;
}

int main(int argc, const char *argv[])
{
	DKA automat;
	vector< string > temp;
	
	//fgets( buff, 2048, stdin );
	gets( buff );
	
	temp = split( string(buff), "," );

	for (int i = 0; i < temp.size(); i++) {
		automat.Q.insert( temp[i] );
	}

	//fgets( buff, 2048, stdin );
	gets( buff );
	
	temp = split( string(buff), "," );
	
	for (int i = 0; i < temp.size(); i++) {
		automat.sigma.insert( temp[i] );
	}
	
	//fgets( buff, 2048, stdin );
	gets( buff );
	

	temp = split( string(buff), "," );

	for (int i = 0; i < temp.size(); i++) {
		automat.F.insert( temp[i] );
	}

	//fgets( buff, 2048, stdin );
	gets( buff );

	automat.Q0 = string( buff );
	
	int counter = 1;

	for( set< string >::iterator it = automat.Q.begin(); it != automat.Q.end(); ++it ) {
		IDS[ *it ] = counter ++;
	}
	
	
	while (gets( buff )) {
		temp = split( string( buff ), "->" );
		
		string q1 = split( temp[0], "," )[0];
		string dd = split( temp[0], "," )[1];
		
		string q2 = temp[1];
		
		automat.delta[ make_pair( q1, dd ) ] = q2;
	}	
	
	
	izbaci_nedohvatljiva( automat );
	
	minimiziraj( automat );
	
	automat.print();

	return 0;
}
