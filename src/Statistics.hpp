#ifndef _QPwEntropy_Statistics_hpp_
#define _QPwEntropy_Statistics_hpp_

namespace QPwEntropy
{
class Statistics
{
public:
  inline static double nPr( int n, int r )
  {
    if( r  > n ) return 0;
    if( n == 0 ) return 1;

    double result = n;

    for ( int d = 2; d <= r; ++d ) {
      result *= double( n ) / double( d );

      n--;
    }

    return result;
  }

  inline static double nPrange( int n, int range ) {
    double result = 0;

    for( int ways = 0; ways <= range; ++ways ) {
      result += Statistics::nPr( n, ways );
    }

    return result;
  }

private:
  Statistics();
};
}

#endif
