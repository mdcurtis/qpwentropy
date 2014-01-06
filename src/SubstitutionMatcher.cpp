#include "SubstitutionMatcher.hpp"

#include <QStringList>

#include "SubstitutionCipher.hpp"

using namespace QPwEntropy;

SubstitutionMatch::SubstitutionMatch( DictionaryMatch &dictionary, QString originalToken ) :
  DictionaryMatch( dictionary ),
  m_original( originalToken )
{}

double SubstitutionMatch::entropy() const
{
  // Calculation in zxcvbn is more complex than this - TODO
  return 1 + DictionaryMatch::entropy();
}

QString SubstitutionMatch::name() const
{
  return "Substitution";
}

SubstitutionMatcher::SubstitutionMatcher( DictionaryMatcher *dictionary ) :
  m_dictionary( dictionary )
{
}

void SubstitutionMatcher::addSubstitution( SubstitutionCipher *cipher )
{
  m_substitutions.append( cipher );
}

MatchList SubstitutionMatcher::match( const QString &input ) const
{
  
  MatchList results;

  Q_FOREACH( SubstitutionCipher *cipher, m_substitutions ) {
    QStringList alternates;
    QVector< int > includedChars;

    alternates.append( cipher->decipher( input, &includedChars ) );
        
    Q_FOREACH( QString possibleMatch, alternates ) {
      MatchList possibleResults = m_dictionary->match( possibleMatch, includedChars );

      Q_FOREACH( QSharedPointer< SubstringMatch > result, possibleResults ) {
        DictionaryMatch *dictionaryMatch = dynamic_cast< DictionaryMatch * >( result.data() );
        QString originalSubstring = input.mid( result->range().start(), result->range().length() );

        if( dictionaryMatch && originalSubstring != result->token() ) {
          QSharedPointer< SubstringMatch > subMatch( new SubstitutionMatch( *dictionaryMatch, originalSubstring ) );

          results.append( subMatch );
        }
      }
    }
  }

  return results;
}
