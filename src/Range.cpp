#include "Range.hpp"

using QPwEntropy::Range;

Range::Range( int start, int end ) : m_start( start ), m_end( end )
{}

bool Range::contains( int value ) const
{
  if( value >= m_start && value < m_end ) return true;

  return false;
}

bool Range::overlap( const Range & other ) const
{
  if( contains( other.end() ) || contains( other.start() ) ) return true;

  return false;
}

void Range::setStart( int start ) 
{
  m_start = start;

  if( m_end < m_start ) m_end = m_start;
}

void Range::setEnd( int end )
{
  m_end = end;

  if( m_start > m_end ) m_start = m_end;
}

bool Range::operator==( const Range &other ) const
{
  return other.end() == end() && other.start() == start();
}

bool Range::operator!=( const Range &other ) const
{
  return !operator==( other );
}

Range &Range::operator++()
{
  ++m_start;
  ++m_end;

  return *this;
}

Range Range::operator++( int )
{
  Range previous = *this;

  operator++();

  return previous;
}