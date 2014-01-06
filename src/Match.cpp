#include "Match.hpp"

#include <math.h>

#include "Statistics.hpp"

using QPwEntropy::SubstringMatch;
using QPwEntropy::SubstringMatcher;

SubstringMatch::SubstringMatch( QString token, Range matchRange ) : m_token( token ), m_range( matchRange )
{}

double SubstringMatch::entropy() const
{
  return 0; // TODO
}

QString SubstringMatch::name() const
{
  return "Substring";
}

//  double bruteforceCardinality() const;

double SubstringMatch::uppercaseEntropy() const
{
  bool startUpper = m_token[ 0 ].isUpper();
  bool endUpper   = m_token[ m_token.length() - 1 ].isUpper();

  bool allUpper = true, allLower = true;
  int numUpper = 0;
  int numLower = 0;

  for( int chr = 0; chr < m_token.length(); ++chr ) {
    if( m_token[ chr ].isUpper() ) {
      allLower = false;
      ++numUpper;
    } else if( m_token[ chr ].isLower() ) {
      ++numLower;
      allUpper = false;
    }
  }

  if( allLower ) {
    return 0;
  }

  if( allUpper || startUpper || endUpper ) {
    // a capitalized word is the most common capitalization scheme,
    // so it only doubles the search space (uncapitalized + capitalized): 1 extra bit of entropy.
    // allcaps and end-capitalized are common enough too, underestimate as 1 extra bit to be safe.
    return 1;
  }

  // otherwise calculate the number of ways to capitalize U+L uppercase+lowercase letters with U uppercase letters or less.
  // or, if there's more uppercase than lower (for e.g. PASSwORD), the number of ways to lowercase U+L letters with L lowercase letters or less.
  double possibilities = Statistics::nPrange( numUpper + numLower, qMin( numUpper, numLower ) );
  
  return log2( possibilities );
}


SubstringMatcher::SubstringMatcher()
{
  
}
