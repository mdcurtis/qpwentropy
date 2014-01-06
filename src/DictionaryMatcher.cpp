#include "DictionaryMatcher.hpp"

#include <math.h>

#include "RankedDictionary.hpp"

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

QString DictionaryMatch::name() const
{
  return "Dictionary";
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
    QList< RankedDictionary::Word > wordList;

    QString token = lower.mid( start );

    Q_FOREACH( DictionaryInfo info, m_dictionaries ) {
      wordList.append( info.dictionary->match( token ) );
    }

    qSort( wordList.begin(), wordList.end() );

    QList< RankedDictionary::Word > uniqueWords;
    // Remove lower-ranked duplicates
    for( QList< RankedDictionary::Word >::iterator word = wordList.begin(); word != wordList.end(); ) {
      QList< RankedDictionary::Word >::iterator nextWord = word;
      ++nextWord;

      uniqueWords.append( *word );

      while( nextWord != wordList.end() && nextWord->string() == word->string() ) {
        ++nextWord;
      }

      word = nextWord;
    }

    Q_FOREACH( RankedDictionary::Word word, uniqueWords ) {
      bool include = true;

      Range range( start, start + word.string().length() );
      QString originalToken = input.mid( start, range.length() );

      if( !includedRanges.empty() ) {
        include = false;

        Q_FOREACH( int position, includedRanges ) {
          if( range.contains( position ) ) {
            include = true;
            break;
          }
        }
      }

      if( include ) {
        QSharedPointer< DictionaryMatch > match( new DictionaryMatch( originalToken, range, word.rank() ) );

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

