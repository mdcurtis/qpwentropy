#include "Match.hpp"

using QPwEntropy::SubstringMatch;
using QPwEntropy::SubstringMatcher;

SubstringMatch::SubstringMatch( QString token, Range matchRange ) : m_token( token ), m_range( matchRange )
{}

double SubstringMatch::entropy() const
{
  return 0; // TODO
}

//  double bruteforceCardinality() const;


SubstringMatcher::SubstringMatcher()
{
  
}
