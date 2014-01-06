#ifndef _QPwEntropy_Match_hpp_
#define _QPwEntropy_Match_hpp_

#include "Range.hpp"

#include <QString>
#include <QSharedPointer>
#include <QList>

namespace QPwEntropy
{
class SubstringMatch
{
public:
  SubstringMatch( QString token, Range matchRange );

  virtual double entropy() const;

  virtual QString name() const;

  inline const Range & range() const { return m_range; }
  inline const QString & token() const { return m_token; }

  virtual double uppercaseEntropy() const;

protected:
  double bruteforceCardinality() const;

protected:
  QString m_token;
  Range m_range;
};

typedef QList< QSharedPointer< SubstringMatch > > MatchList;

class SubstringMatcher
{
public:
  SubstringMatcher();

  virtual MatchList match( const QString &input ) const = 0;
};
/*
class SubstitutionMatch : public DictionaryMatch
{
public:
  SubstitutionMatch( QString token, Range range, int rank );

  QHash< QChar, QChar > substitutions;

};

class SpatialMatch : public SubstringMatch
{
  QString graph;
  int turns;
  int shiftedCount;
};

class RepeatMatch : public SubstringMatch
{
  QChar repeatedChar
};

class SequenceMatch : public SubstringMatch
{
  QString name;
  int space;
  bool ascending;
};


class DateMatch : public SubstringMatch
{
  int day, month, year;
  QDate date;  // may be convenient, although needs to handle invalid dates (so they can be tested and then rejected)
  QChar separator;
};*/
}

#endif
