#include "SubstitutionCipher.hpp"

#include "DictionaryMatcher.hpp"
#include "SubstitutionMatcher.hpp"
#include "RankedDictionary.hpp"

#include <iostream>

#include <QFile>
#include <QTime>
#include <QStringList>

QStringList testVectors();

using namespace QPwEntropy;

int main( int argc, char **argv )
{
  QStringList tests;

  if( argc > 1 ) {
    for( int arg = 1; arg < argc; ++arg ) {
      tests << QString::fromUtf8( argv[ arg ] );
    }
  } else {
    tests= testVectors();
  } 

  QStringList dictionaries;
  dictionaries << "passwords" << "surnames" << "male_names" << "female_names" << "english";

  DictionaryMatcher matcher;

  L33tCipher cipher;
  SubstitutionMatcher subMatcher( &matcher );
  subMatcher.addSubstitution( &cipher );


  Q_FOREACH( QString dictionaryFilename, dictionaries ) {
    QFile file( dictionaryFilename + ".txt" );
    if( file.open( QIODevice::ReadWrite ) ) {
      std::cout << "Parsing dictionary " << dictionaryFilename.toStdString() << std::endl;

      RankedDictionary *dict = new RankedDictionary();
      dict->loadDictionary( &file );

      std::cout << "Dictionary loaded, trying some stuff" << std::endl;
      QList< RankedDictionary::Word > words = dict->match2( "everybody" );
      Q_FOREACH( RankedDictionary::Word word, words ) {
        std::cout << "Found: " << word.string().toStdString() << ", rank: " << word.rank() << std::endl;
      }

      matcher.addDictionary( dict );

      
    } else {
      std::cout << "Could not open dictionary " << dictionaryFilename.toStdString() << std::endl;
    }
  }

  QTime timer;
  timer.start();

  Q_FOREACH( QString test, tests ) {
    MatchList results = matcher.match( test );
    results.append( subMatcher.match( test ) );

    std::cout << test.toStdString() << std::endl;

    Q_FOREACH( QSharedPointer< SubstringMatch > match, results ) {
      std::cout << "  " << match->token().toStdString() << " (" << match->range().start() << ", " << match->range().end() << "); entropy = " << match->entropy();

      DictionaryMatch *dMatch = dynamic_cast< DictionaryMatch * >( match.data() );
      if( dMatch ) {
        std::cout << "  u/C " << dMatch->uppercaseEntropy();
      }

      std::cout << std::endl;
    }
  }

  std::cout << "Time to perform tests: " << timer.elapsed() << " ms" << std::endl;

  return 0;
}

/*
  L33tCipher deLeet;

  Q_FOREACH( QString test, tests ) {
    QStringList outcomes = deLeet.decipher( test );

    if( !outcomes.empty() ) {
      std::cout << test.toStdString() << std::endl;
      Q_FOREACH( QString outcome, outcomes ) {
        std::cout << "  " << outcome.toStdString() << std::endl;
      }
      std::cout << std::endl;
    }
  }

  SubstitutionCipher germanUmlauts;
  germanUmlauts.addSubstitution( "ae", L'ä' );
  germanUmlauts.addSubstitution( "oe", L'ö' );
  germanUmlauts.addSubstitution( "ue", L'ü' );
  germanUmlauts.addSubstitution( "a", L'ä' ); // lazy typist versions
  germanUmlauts.addSubstitution( "o", L'ö' );
  germanUmlauts.addSubstitution( "u", L'ü' );
  germanUmlauts.addSubstitution( "ss", L'ß' );

  Q_FOREACH( QString outcome, germanUmlauts.decipher( QString::fromWCharArray( L"Ich habe zwei Füße" ) ) ) {
    std::cout << "  " << outcome.toStdString() << std::endl;
  }

  return 0;
}
*/

QStringList testVectors()
{
  const char *test_passwords[] = {
    "zxcvbn",
    "5Pr!ng-8",
    "qwER43@!",
    "Tr0ub4dour&3",
    "correcthorsebatterystaple",
    "coRrecth0rseba++ery9.23.2007staple$",
    "D0g..................",
    "abcdefghijk987654321",
    "neverforget13/3/1997",
    "1qaz2wsx3edc",
    "temppass22",
    "briansmith",
    "briansmith4mayor",
    "password1",
    "viking",
    "thx1138",
    "ScoRpi0ns",
    "do you know",
    "ryanhunter2000",
    "rianhunter2000",
    "asdfghju7654rewq",
    "AOEUIDHG&*()LS_",
    "12345678",
    "defghi6789",
    "rosebud",
    "Rosebud",
    "ROSEBUD",
    "rosebuD",
    "ros3bud99",
    "r0s3bud99",
    "R0$38uD99",
    "verlineVANDERMARK",
    "eheuczkqyq",
    "rWibMFACxAUGZmxhVncy",
    "Ba9ZyWABu99[BK#6MBgbH88Tofv)vs$w",
    0
  };

  QStringList vector;

  for( const char ** password = test_passwords; *password; ++password ) {
    vector.append( *password );
  }

  return vector;
}