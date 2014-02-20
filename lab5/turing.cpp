#include <cstdio>
#include <algorithm>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <string>
#include <set>
#include <map>
#include <queue>
using namespace std;

typedef vector< string >  vstr; 
typedef set< string >     skup;

typedef vector< string >::iterator  vit;
typedef set< string >::iterator     sit;

#define MIN 0
#define MAX 69


char buff[ 1 << 16 ];                                                                        
string readline( void ) { memset( buff, 0, sizeof buff ); gets( buff );  return string( buff ); }

vstr split( string a, string del )
{
  vstr ret;
  string temp = "";

  int len = del.length(), n = a.length();

  for (int i = 0; i < n; i++) {
    if (i + len <= n && a.substr( i, len ) == del) {
      if ( temp.length() > 0 )ret.push_back( temp );
      i += len - 1;
      temp = "";
    } else {
      temp = temp + a.at( i );
    }
  }

  if (temp.length() > 0) {
    ret.push_back( temp );
  }

  return ret;
}

struct turing_t {
  skup Q, sigma, theta, F;
  string prazanZnak;
  vstr traka;
  string Q0;
  int head;
  
  struct tuple{ string first, second; bool operator <( const tuple &b ) const { return first < b.first || ( first == b.first && second < b.second ); } };
  struct triple{ string first, second, third; };

  map< tuple, triple >delta;

  void input( void );
  void output( void );
  void simuliraj( void );

};

void turing_t::input()
{
  vstr v;
  string line;

  v = split( readline(), "," );
  for ( vit it = v.begin(); it != v.end(); ++it ) {
    Q.insert( *it );
  }

  v = split( readline(), "," );
  for ( vit it = v.begin(); it != v.end(); ++it ) {
    sigma.insert( *it );
  }

  v = split( readline(), "," );
  for ( vit it = v.begin(); it != v.end(); ++it ) {
    theta.insert( *it );
  }

  prazanZnak = readline();

  line = readline();
  for (int i = 0; i < line.length(); i++) {
    traka.push_back( line.substr( i, 1 ) );
  }
  
  v = split( readline(), "," );
  for ( vit it = v.begin(); it != v.end(); ++it ) {
    F.insert( *it );
  }

  Q0 = readline();

  line = readline();  sscanf( line.c_str(), "%d", &head );

  while (1) {
    line = readline();
    if (line.length() <= 0) { break; }

    v = split( line, "->" );
    string RHS = v[1], LHS = v[0];

    tuple a;  triple b;

    v = split( LHS, "," );
    a.first = v[0]; a.second = v[1];

    v = split( RHS, "," );
    b.first = v[0]; b.second = v[1]; b.third = v[2];

    delta[a] = b;

  }

  return;
}

void turing_t::output()
{
  for ( sit it = Q.begin(); it != Q.end(); ++it ) {
    printf("%s ", it->c_str() );
  } puts( "" );
  
  for ( sit it = sigma.begin(); it != sigma.end(); ++it ) {
    printf("%s ", it->c_str() );
  } puts( "" );
  
  for ( sit it = theta.begin(); it != theta.end(); ++it ) {
    printf("%s ", it->c_str() );
  } puts( "" );

  printf("%s\n", prazanZnak.c_str());

  for ( vit it = traka.begin(); it != traka.end(); ++it ) {
    printf("%s", it->c_str());
  } puts( "" );

  for ( sit it = F.begin(); it != F.end(); ++it ) {
    printf("%s ", it->c_str() );
  } puts( "" );

  printf("%s\n", Q0.c_str());
  printf("%d\n", head);

  for ( map< tuple, triple >::iterator it = delta.begin(); it != delta.end(); ++it ) {
    printf("%s,%s -> %s,%s,%s\n", 
           it->first.first.c_str(), it->first.second.c_str(), 
           it->second.first.c_str(), it->second.second.c_str(), it->second.third.c_str());
  }
  
  return;
}

void turing_t::simuliraj()
{
  string curr = Q0;
  triple prijelaz;
  tuple QT;

  while (1) {
    QT.first = curr;  QT.second = traka.at( head );

    if (delta.find( QT ) == delta.end()) {
      break;
    } else {
      prijelaz = delta[ QT ];

      if ( (head == MIN && prijelaz.third == "L") || ( head == MAX && prijelaz.third == "R" ) ) {
        break;
      }

      curr = prijelaz.first;
      traka[ head ] = prijelaz.second;
      head += prijelaz.third == "R" ? +1 : -1;

    }
  }

  printf("%s|%d|", curr.c_str(), head);

  for ( vit it = traka.begin(); it != traka.end(); ++it ) {
    printf("%s", it->c_str());
  }

  printf("|%d\n", (int)F.count( curr ));

  return;
}


int main(int argc, const char *argv[])
{
  turing_t TS;

  TS.input();

  TS.simuliraj();

  //TS.output();

  return 0;
}
