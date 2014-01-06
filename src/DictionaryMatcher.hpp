#ifndef _QPwEntropy_DictionaryMatcher_hpp_
#define _QPwEntropy_DictionaryMatcher_hpp_

#include "Match.hpp"

#include <QHash>
#include <QVector>

namespace QPwEntropy
{
class RankedDictionary;

class DictionaryMatch : public SubstringMatch
{
public:
  DictionaryMatch( QString token, Range range, int rank );

  virtual double entropy() const;

  virtual QString name() const;

  inline int rank() const { return m_rank; }


protected:
  int m_rank;

};

class DictionaryMatcher : public SubstringMatcher
{
public:
  DictionaryMatcher();
  ~DictionaryMatcher();

  void addDictionary( RankedDictionary *, int scaling = 1 );

  virtual MatchList match( const QString &input ) const;
  virtual MatchList match( const QString &input, QVector< int > includedRanges ) const;

private:
  class DictionaryInfo;

  QList< DictionaryInfo > m_dictionaries;
};

}

#endif
