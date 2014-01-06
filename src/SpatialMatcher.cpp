#include "SpatialMatcher.hpp"

#include <QDebug>

using namespace QPwEntropy;

class SpatialMatcher::KbdCoord
{
public:
  KbdCoord() : mod( Normal ), row( -1 ), col( -1 ) {}
  KbdCoord( Shift mod, int row, int col ) : mod( mod ), row( row ), col( col ) {}

  inline bool isValid() { return !(row == -1 || col == -1); }

  Shift mod;
  int row, col;
};

class SpatialMatcher::KbdVec
{
public:
  KbdVec() : x( 0 ), y( 0 ) {}
  KbdVec( int x, int y ) : x( x ), y( y ) {}

  int x, y;

  bool operator==( const KbdVec &other ) const { return x == other.x && y == other.y; }
  bool operator!=( const KbdVec &other ) const { return !operator==( other ); }
};

SpatialMatcher::SpatialMatcher( QString name, bool backwardsSlant ) : m_name( name ), m_backwardsSlant( backwardsSlant )
{}

void SpatialMatcher::addRow( QString lower, QString shifted, QChar ignore )
{
  lower.replace( ignore, QChar( '\0' ) );
  shifted.replace( ignore, QChar( '\0' ) );

  m_keyboard[ Normal ].append( lower );
  m_keyboard[ Shifted ].append( shifted );

  int width = 0;

  for( int modifier = 0; modifier < ModifierDepth; ++modifier ) {
    for( int row = 0; row < m_keyboard[ Normal ].size(); ++row ) {
      width = qMax( width, m_keyboard[ modifier ][ row ].size() );
    }
  }

  for( int row = 0; row < m_keyboard[ Normal ].size(); ++row ) {
    for( int modifier = 0; modifier < ModifierDepth; ++modifier ) {
      m_keyboard[ modifier ][ row ] = m_keyboard[ modifier ][ row ].leftJustified( width, '\0' );
    }
  }

  m_rows = m_keyboard[ Normal ].size();
  m_cols = width;
}

void SpatialMatcher::addRow( QString lower, QChar ignore )
{
  addRow( lower, "", ignore );
}

inline QChar SpatialMatcher::get( KbdCoord coord ) const
{
  return m_keyboard[ coord.mod ][ coord.row ][ coord.col ];
}

MatchList SpatialMatcher::match( const QString &input ) const
{
  MatchList matchList;

  KbdCoord current;
  KbdVec lastVec;
  bool searching = true;

  int chainLength = 0, numTurns = 0, numShifts = 0;
  int startChr = 0;

  for( int chr = 0; chr <= input.length(); ++chr ) {
    if( !searching && chr < input.length() ) {
      KbdCoord foundKey;
      KbdVec foundVec;

      for( int y = current.row - 1; y <= current.row + 1; ++y ) {
        for( int x = current.col - 1; x <= current.col + 1; ++x ) {
          if( y  < 0 || x  < 0 ) continue;
          if( y == 0 && x == 0 ) continue;

          KbdVec vec( y - current.row, x - current.col );
          KbdCoord key( Normal, y, x );

          // On backwards slanting layouts (eg normal QWERTY), NW and SE directions aren't adjacent
          if( m_backwardsSlant && (vec.x == -1 && vec.y == -1 ) ) continue;
          if( m_backwardsSlant && (vec.x ==  1 && vec.y ==  1 ) ) continue;

          for( int modifier = 0; modifier < ModifierDepth; ++modifier ) {
            key.mod = Shift( modifier );
            if( get( key ) == input[ chr ] ) {
              foundKey = key;
              foundVec = vec;

              break;
            }
          }
        }
      }

      if( foundKey.isValid() ) {
        ++chainLength;

        if( foundVec != lastVec ) ++numTurns;
        if( foundKey.mod != current.mod ) ++numShifts;

        current = foundKey;
        lastVec = foundVec;
      } else {
        searching = true;
      }
    }

    if( (searching || chr == input.length() ) && chainLength > 0 ) {
      // build matcher object
      qDebug() << "Found chain " << input.mid( startChr, chainLength ) << ", turns = " << numTurns << ", shifts = " << numShifts;

      chainLength = 0;
      numTurns = 0;
      numShifts = 0;
    }

    if( searching && chr < input.length() ) {
      for( int modifier = 0; modifier < ModifierDepth; ++modifier ) {
        for( int row = 0; row < m_rows; ++row ) {
          int col = m_keyboard[ modifier ][ row ].indexOf( input[ chr ] );

          if( col >= 0 ) {
            searching = false;
            current = KbdCoord( Shift( modifier ), row, col );
            lastVec = KbdVec();
            startChr = chr;
            chainLength = 1;
            
            break;
          }
        }
      }

    }
  }


  return matchList;
}


QwertyMatcher::QwertyMatcher() : SpatialMatcher( "qwerty", true )
{
  addRow( "`1234567890-= " , "~!@#$%^&*()_+ " , ' ' );
  addRow( " qwertyuiop[]\\", " QWERTYUIOP{}|" , ' ' );
  addRow( " asdfghjkl;'  " , " ASDFGHJKL:\"  ", ' ' );
  addRow( " zxcvbnm,./   " , " ZXCVBNM<>?   " , ' ' );
  addRow( "xx      xxxxxx" , 'x' );
}

KeypadMatcher::KeypadMatcher() : SpatialMatcher( "keypad", false )
{
  addRow( " /*-" );
  addRow( "789+" );
  addRow( "456+" );
  addRow( "123 " );
  addRow( "00. " );
}
