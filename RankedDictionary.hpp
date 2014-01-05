#ifndef _QPwEntropy_RankedDictionary_hpp_
#define _QPwEntropy_RankedDictionary_hpp_

#include <QHash>
#include <QMap>

class QIODevice;

namespace QPwEntropy
{
class SubstitutionCipher;
class Range;

class RankedDictionary
{
public:
  const static int kNotFound = -1;

  class Word
  {
  public:
    Word();
    Word( const Word & );
    
    inline const QString & string() const { return m_string; }
    inline int rank() const { return m_rank; }

    bool operator<( const Word &other ) const;
    Word & operator=( const Word &other );

  private:
    friend class RankedDictionary;

    QString m_string;
    int m_rank;
  };

private:
  struct SearchNode;

public:
  RankedDictionary();
  ~RankedDictionary();

  void addPreprocessor( SubstitutionCipher *substitution );

  void loadDictionary( QIODevice *dictionary );

  int match( const QString &word ) const;

  QList< Word > match2( const QString &word ) const;

private:
  void buildIndex();
  void buildIndex( SearchNode *node, int chrIndex );

  Range indexMatch( const QString &word, const SearchNode *node, int chrIndex, QList< Word > *matches ) const;


  void insert( const QString & word, int rank );

private:
  QList< SubstitutionCipher * > m_preprocessors;
  QList< Word >  m_dictionary;
  SearchNode *m_searchRoot;

  static const int kIndexDepth = 3;

  QHash< QString, int > m_oldDictionary;

  int m_maxLength;
};
}

#endif
