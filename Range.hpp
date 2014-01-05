#ifndef _QPwEntropy_Range_hpp_
#define _QPwEntropy_Range_hpp_

namespace QPwEntropy
{
class Range
{
public:
  Range( int start, int end );

  bool contains( int value ) const;
  bool overlap( const Range & ) const;

  inline int start() const { return m_start; }
  inline int end() const { return m_end; }

  inline int length() const { return m_end - m_start; }

  void setStart( int start );
  void setEnd( int end );

  bool operator==( const Range & ) const;
  bool operator!=( const Range & ) const;

  Range &operator++();
  Range operator++( int );

private:
  int m_start;
  int m_end;
};
}

#endif
