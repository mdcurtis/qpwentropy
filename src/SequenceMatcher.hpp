#ifndef _QPwEntropy_SequenceMatcher_hpp_
#define _QPwEntropy_SequenceMatcher_hpp_

#include "Match.hpp"

namespace QPwEntropy
{
class SequenceMatch : public SubstringMatch
{
public:
  SequenceMatch( QString token, Range range, int startIndex, int seqLength, bool ascending, QString sequenceName );
  virtual ~SequenceMatch();

  virtual double entropy() const;

  virtual QString name() const;

private:
  QString m_sequenceName;

  int m_startIndex, m_seqLength;  
  bool m_ascending;
};

class SequenceMatcher : public SubstringMatcher
{
public:
  SequenceMatcher( QString name, QString sequence );

  virtual MatchList match( const QString &input ) const;
private:
  QString m_name, m_sequence;

  const static int kMinSequenceLength = 3;
};
}

#endif
