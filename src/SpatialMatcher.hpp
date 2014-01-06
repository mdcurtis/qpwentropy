#ifndef _QPwEntropy_SpatialMatcher_hpp_
#define _QPwEntropy_SpatialMatcher_hpp_

#include "Match.hpp"

#include <QVector>

namespace QPwEntropy
{
class SpatialMatch : public SubstringMatch
{
private:
  int m_turns;
  QString m_graph;
};

class SpatialMatcher : public SubstringMatcher
{
public:
  SpatialMatcher( QString name, bool backwardsSlant );

  void addRow( QString lower, QString shifted, QChar ignore = ' ' );
  void addRow( QString lower, QChar ignore = ' ' );

  virtual MatchList match( const QString &input ) const;

private:
  enum Shift
  {
    Normal = 0,
    Shifted = 1,
    ModifierDepth
  };

  class KbdCoord;
  class KbdVec;
private:
  inline QChar get( KbdCoord ) const;

private:

  QVector< QString > m_keyboard[ ModifierDepth ];

  QString m_name;
  bool m_backwardsSlant;

  int m_rows, m_cols;

  static const int kMinChainLength = 3;
};

class QwertyMatcher : protected SpatialMatcher
{
public:
  QwertyMatcher();

  using SpatialMatcher::match;
};

class KeypadMatcher : protected SpatialMatcher
{
public:
  KeypadMatcher();

  using SpatialMatcher::match;
};

}

#endif
