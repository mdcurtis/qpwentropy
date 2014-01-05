#include "DictionaryMatcher.hpp"

#include <math.h>

#include "RankedDictionary.hpp"
#include "Statistics.hpp"

using namespace QPwEntropy;

class DictionaryMatcher::DictionaryInfo
{
public:
  RankedDictionary *dictionary;
  int scaling;
};

DictionaryMatch::DictionaryMatch( QString token, Range range, int rank ) : 
  SubstringMatch( token, range ),
  m_rank( rank )
{}

double DictionaryMatch::entropy() const
{
  return log2( m_rank ) + uppercaseEntropy();
}

double DictionaryMatch::uppercaseEntropy() const
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
      // because this is a word, assume that all non-uppercase characters are lowercase
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
  double possibilities = 0;
  
  for( int ways = 0; ways <= qMin( numUpper, numLower ); ++ways ) {
    possibilities += Statistics::nPr( numUpper + numLower, ways );
  }
  
  return log2( possibilities );
}

DictionaryMatcher::DictionaryMatcher() : SubstringMatcher()
{
}

DictionaryMatcher::~DictionaryMatcher()
{
}

void DictionaryMatcher::addDictionary( RankedDictionary *dictionary, int scaling )
{
  if( scaling < 1 ) scaling = 1;

  DictionaryInfo info;
  info.dictionary = dictionary;
  info.scaling = scaling;

  m_dictionaries.append( info );
}

MatchList DictionaryMatcher::match( const QString &input, QVector< int > includedRanges ) const
{
  MatchList result;

  QString lower = input.toLower();

  for( int start = 0 ; start < lower.length(); ++start ) {
    // No point wasting cycles on one character words...
    for( Range range( start, start + 2 ); range.end() <= lower.length(); range.setEnd( range.end() + 1 ) ) {
      if( !includedRanges.empty() ) {
        bool skip = true;

        Q_FOREACH( int position, includedRanges ) {
          if( range.contains( position ) ) {
            skip = false;
            break;
          }
        }

        if( skip ) continue;
      }

      QString token = lower.mid( range.start(), range.length() );

      int bestRank = RankedDictionary::kNotFound;

      Q_FOREACH( DictionaryInfo info, m_dictionaries ) {
        int rank = info.dictionary->match( token );

        if( rank != RankedDictionary::kNotFound && (rank < bestRank || bestRank == RankedDictionary::kNotFound) ) {
          bestRank = rank;
        }
      }

      if( bestRank != RankedDictionary::kNotFound ) {
        QString originalToken = input.mid( range.start(), range.length() );
        QSharedPointer< DictionaryMatch > match( new DictionaryMatch( originalToken, range, bestRank ) );

        result.append( match );
      }
    }
  }

  return result;  
}

MatchList DictionaryMatcher::match( const QString &input ) const
{
  return match( input, QVector< int >() );
}

