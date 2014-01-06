#include "RankedDictionary.hpp"

#include <QIODevice>
#include <QString>
#include <QIODevice>
#include <QStringList>

#include "Range.hpp"
#include "SubstitutionCipher.hpp"

using QPwEntropy::RankedDictionary;
using QPwEntropy::Range;

struct RankedDictionary::SearchNode
{
  SearchNode() : symbol( '\0' ), parent( 0 ), firstIndex( 0 ), lastIndex( 0 )
  {
  }

  QChar symbol;

  SearchNode *parent;
  QVector< SearchNode > children;

  int firstIndex, lastIndex;
};

RankedDictionary::Word::Word() : m_rank( 0 )
{
}

RankedDictionary::Word::Word( const Word &other )
{
  operator=( other );
}

bool RankedDictionary::Word::operator<( const Word &other ) const
{
  if( m_string == other.m_string ) {
    return m_rank < other.m_rank;
  } else {
    return m_string < other.m_string;
  }
}

RankedDictionary::Word & RankedDictionary::Word::operator=( const Word &other )
{
  m_string = other.m_string;
  m_rank = other.m_rank;

  return *this;
}


RankedDictionary::RankedDictionary( QString name ) : m_maxLength( 0 ), m_searchRoot( 0 ), m_name( name )
{}

RankedDictionary::~RankedDictionary()
{
  delete m_searchRoot;
}

void RankedDictionary::addPreprocessor( SubstitutionCipher *substitution )
{
  Q_ASSERT( substitution != 0 );

  m_preprocessors.append( substitution );
}

void RankedDictionary::buildIndex()
{
  delete m_searchRoot;

  qSort( m_dictionary.begin(), m_dictionary.end() );

  m_searchRoot = new SearchNode();
  m_searchRoot->lastIndex = m_dictionary.size() - 1;
  
  buildIndex( m_searchRoot, 0 );
}

void RankedDictionary::buildIndex( SearchNode *node, int chrIndex )
{
  for( int index = node->firstIndex; index <= node->lastIndex; ++index ) {
    const QString &testString = m_dictionary[ index ].m_string;

    if( testString.length() <= chrIndex ) {
      continue;
    }

    QChar testChr = testString[ chrIndex ];

    if( node->children.empty() || node->children.last().symbol != testChr ) {
      SearchNode child;
      child.firstIndex = index;
      child.lastIndex = index;

      child.symbol = testChr;
      child.parent = node;

      node->children.append( child );
    } else {
      node->children.last().lastIndex = index;
    }
  }


  ++chrIndex;
  if( chrIndex < kIndexDepth ) {
    for( QVector< SearchNode >::iterator child = node->children.begin(); child != node->children.end(); ++child ) {
      buildIndex( &(*child), chrIndex );
      if( !child->children.empty() ) {
        // Truncate the search space of the node to only those strings which are shorter than the next search depth
        node->lastIndex = child->children.first().firstIndex - 1;
      }
    }
  }
}

void RankedDictionary::loadDictionary( QIODevice *dictionary )
{
  int rank = 1;

  while( !dictionary->atEnd() ) {
    QString word = QString::fromUtf8( dictionary->readLine() ).trimmed();

    insert( word, rank );
    Q_FOREACH( SubstitutionCipher *substitution, m_preprocessors ) {
      QStringList results = substitution->decipher( word );
      Q_FOREACH( QString substitutedWord, results ) {
        insert( substitutedWord, rank );
      }
    }

    ++rank;
  }


  //buildIndex();
}

inline void RankedDictionary::insert( const QString &wordString, int rank )
{
  Word word;
  word.m_string = wordString;
  word.m_rank = rank;

  m_dictionary.append( word );
}

Range RankedDictionary::indexMatch( const QString &word, const SearchNode *node, int chrIndex, WordList *matches ) const
{
  if( chrIndex >= word.length() ) {
    return Range( 0, -1 ); // no point continuing
  }

  if( chrIndex >= kIndexDepth || node->children.empty() ) {
    return Range( node->firstIndex, node->lastIndex );
  }

  const SearchNode *foundNode = 0;
  QChar testChr = word[ chrIndex ];

  Q_FOREACH( const SearchNode & child, node->children ) {
    if( child.symbol == testChr ) {
      foundNode = &child;
      break;
    }
  }

  if( !foundNode ) {
    return Range( 0, -1 );
  } else {
    for( int index = foundNode->firstIndex; index <= foundNode->lastIndex; ++index ) {
      if( m_dictionary[ index ].m_string.length() > (chrIndex + 1) ) break;

      matches->append( m_dictionary[ index ] );
    }

    return indexMatch( word, foundNode, chrIndex + 1, matches );
  }
}


RankedDictionary::WordList RankedDictionary::match( const QString &word ) const
{
  WordList matches;

  Range search = indexMatch( word, m_searchRoot, 0, &matches );

  for( int index = search.start(); index <= search.end(); ++index ) {
    if( m_dictionary[ index ].m_string == word.mid( 0, m_dictionary[ index ].m_string.length() ) ) {
      matches.append( m_dictionary[ index ] );
    }
  }

  return matches;

}

