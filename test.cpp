#include "SubstitutionCipher.hpp"

#include "RankedDictionary.hpp"
#include "DictionaryMatcher.hpp"
#include "SubstitutionMatcher.hpp"
#include "SequenceMatcher.hpp"
#include "SpatialMatcher.hpp"

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
  dictionaries << "passwords" << "surnames" << "male_names" << "female_names" << "english" << "german";

  DictionaryMatcher matcher;

  L33tCipher cipher;
  SubstitutionMatcher subMatcher( &matcher );
  subMatcher.addSubstitution( &cipher );

  QTime loadTimer;
  loadTimer.start();

  SubstitutionCipher germanUmlauts;
  germanUmlauts.addSubstitution( "ae", L'ä' );
  germanUmlauts.addSubstitution( "oe", L'ö' );
  germanUmlauts.addSubstitution( "ue", L'ü' );
  germanUmlauts.addSubstitution( "a", L'ä' ); // lazy typist versions
  germanUmlauts.addSubstitution( "o", L'ö' );
  germanUmlauts.addSubstitution( "u", L'ü' );
  germanUmlauts.addSubstitution( "ss", L'ß' );

  Q_FOREACH( QString dictionaryFilename, dictionaries ) {
    QFile file( dictionaryFilename + ".txt" );
    if( file.open( QIODevice::ReadWrite ) ) {
      std::cout << "Parsing dictionary " << dictionaryFilename.toStdString() << std::endl;

      RankedDictionary *dict = new RankedDictionary( dictionaryFilename );
      if( dictionaryFilename == "german" ) {
        dict->addPreprocessor( &germanUmlauts );
      }
      
      dict->loadDictionary( &file );

      dict->buildIndex();
      
      matcher.addDictionary( dict );
    } else {
      std::cout << "Could not open dictionary " << dictionaryFilename.toStdString() << std::endl;
    }
  }


  SequenceMatcher alphaSequence( "alphabet", "abcdefghijklmnopqrstuvwxyz" );
  SequenceMatcher numSequence( "numerals", "0123456789" );

  QwertyMatcher qwertyMatcher;
  KeypadMatcher keypadMatcher;

  std::cout << "Dictionary build time: " << loadTimer.elapsed() << " ms" << std::endl;

  QTime timer;
  timer.start();

  Q_FOREACH( QString test, tests ) {
    MatchList results = matcher.match( test );
    results.append( subMatcher.match( test ) );
    results.append( numSequence.match( test ) );
    results.append( alphaSequence.match( test ) );
    results.append( qwertyMatcher.match( test ) );
    results.append( keypadMatcher.match( test ) );

    std::cout << test.toUtf8().data() << std::endl;

    Q_FOREACH( QSharedPointer< SubstringMatch > match, results ) {
      std::cout << "  " << match->token().toUtf8().data() << " (" << match->range().start() << ", " << match->range().end() << "); entropy = " << match->entropy();

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