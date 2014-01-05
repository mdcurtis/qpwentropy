#ifndef _QPwEntropy_SubstitutionMatcher_hpp_
#define _QPwEntropy_SubstitutionMatcher_hpp_

#include "DictionaryMatcher.hpp"

namespace QPwEntropy
{
class SubstitutionCipher;

class SubstitutionMatch : public DictionaryMatch
{
public:
  SubstitutionMatch( DictionaryMatch &dictionary, QString originalToken );

  virtual double entropy() const;

  const QString &original() const { return m_original; }

protected:
  QString m_original;

};

class SubstitutionMatcher : public SubstringMatcher
{
public:
  SubstitutionMatcher( DictionaryMatcher *dictionary ); 

  void addSubstitution( SubstitutionCipher *cipher );

  virtual MatchList match( const QString &input ) const;

private:
  QList< SubstitutionCipher * > m_substitutions;

  DictionaryMatcher *m_dictionary;
};
}

#endif

