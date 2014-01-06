#include "SubstitutionCipher.hpp"

#include <QStringList>
#include <QVector>

using QPwEntropy::SubstitutionCipher;
using QPwEntropy::L33tCipher;

SubstitutionCipher::SubstitutionCipher()
{}

SubstitutionCipher::~SubstitutionCipher()
{}

void SubstitutionCipher::addSubstitution( QString original, QChar substituted )
{
  m_substitutions.insertMulti( substituted, original );
}

QStringList SubstitutionCipher::decipher( QString input, QVector< int > *substitutedChars ) const
{
  QStringList result;
  result.append( "" );
  result[ 0 ].reserve( input.length() );

  int numSubstitutions = 0;

  for( int chr = 0; chr < input.length(); ++chr ) {
    if( m_substitutions.contains( input.at( chr ) ) ) {
      ++numSubstitutions;
      if( substitutedChars ) {
        substitutedChars->append( chr );
      }

      QStringList possibilities = m_substitutions.values( input.at( chr ) );

      QStringList substituted;
      Q_FOREACH( QString candidateOriginal, possibilities ) {
        Q_FOREACH( QString string, result ) {
          string.append( candidateOriginal );
          substituted.append( string );
        }
      }

      substituted.swap( result );
    } else {
      for( QStringList::iterator string = result.begin(); string != result.end(); ++string ) {
        string->append( input[ chr ] );
      }
    }
  }

  if( numSubstitutions == 0 ) {
    return QStringList();
  } else {
    return result;
  }
}


L33tCipher::L33tCipher() : SubstitutionCipher()
{
  addSubstitution( "a", '@' );

  addSubstitution( "o", '0' );
  addSubstitution( "i", '1' );
  addSubstitution( "l", '1' );
  addSubstitution( "z", '2' );
  addSubstitution( "e", '3' );
  addSubstitution( "a", '4' );
  addSubstitution( "s", '5' );
  addSubstitution( "g", '6' );
  addSubstitution( "t", '7' );
  addSubstitution( "l", '7' );
  addSubstitution( "b", '8' );
  addSubstitution( "g", '9' );

  addSubstitution( "i", '!' );
  addSubstitution( "i", '|' );
  addSubstitution( "l", '!' );
  addSubstitution( "l", '|' );

  addSubstitution( "s", '$' );
  addSubstitution( "x", '%' );

  addSubstitution( "c", '(' );
  addSubstitution( "c", '[' );
  addSubstitution( "c", '<' );
  addSubstitution( "c", '{' );

  addSubstitution( "t", '+' );
}
