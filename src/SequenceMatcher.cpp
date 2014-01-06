
#include "SequenceMatcher.hpp"

#include <math.h>

using namespace QPwEntropy;

SequenceMatch::SequenceMatch( QString token, Range range, int startIndex, int seqLength, bool ascending, QString sequenceName )
  : SubstringMatch( token, range ),
  m_sequenceName( sequenceName ),
  m_startIndex( startIndex ),
  m_seqLength( seqLength ),
  m_ascending( ascending )
{}

SequenceMatch::~SequenceMatch()
{}

double SequenceMatch::entropy() const
{
  double result = 0;

  if( m_startIndex == 0 ) {
    result = 1;    
  } else {
    result = log2( m_seqLength );
  }

  if( !m_ascending ) {
    result += 1;
  }

  result += log2( m_token.length() );

  result += uppercaseEntropy();

  return result;
}

QString SequenceMatch::name() const
{
  return "Sequence";
}

SequenceMatcher::SequenceMatcher( QString name, QString sequence ) : 
  m_name( name ),
  m_sequence( sequence )
{
}

MatchList SequenceMatcher::match( const QString &input ) const
{
  MatchList matchList;

  QString lower = input.toLower();

  enum State
  {
    Searching,
    HaveStart,
    HaveSequence
  };

  enum Direction
  {
    Forwards,
    Reverse,
    None
  };

  State state = Searching;
  int sequenceLength = 0;
  int previousIndex = -1, startingChr = -1;
  int startIndex = 0;
  Direction direction = None;

  for( int chr = 0; chr <= lower.length(); ++chr ) {
    int index = -1;
    if( chr < lower.length() ) {
      index = m_sequence.indexOf( lower[ chr ] );
    }

    Direction matchDirection = None;
    if( state == HaveStart || state == HaveSequence ) {
      if( index == (previousIndex + 1) ) {
        matchDirection = Forwards;
      } else if( index == (previousIndex - 1 ) ) {
        matchDirection = Reverse;
      }
    }

    if( state == HaveStart ) {
      if( matchDirection == None ) {
        state = Searching;
      } else {
        direction = matchDirection;
        state = HaveSequence;
        ++sequenceLength;
      }
    }

    if( state == HaveSequence ) {
      if( matchDirection == direction ) {
        ++sequenceLength;
      } else if ( sequenceLength >= 3 ) {
        QString token = input.mid( startingChr, sequenceLength );
        Range range( startingChr, sequenceLength + startingChr );

        QSharedPointer< SubstringMatch > match( 
          new SequenceMatch( token, range, startIndex, m_sequence.length(), direction == Forwards, m_name ) );
        matchList.append( match );

        state = Searching;
      }
    }

    if( index < 0 ) state = Searching;

    if( state == Searching && index >= 0 ) {
      state = HaveStart;
      startIndex = index;
      direction = None;
      sequenceLength = 0;
      startingChr = chr;
    }
    
    previousIndex = index;
  }

  return matchList;
}
