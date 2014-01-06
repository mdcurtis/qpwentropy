#ifndef _QPwEntropy_RankedDictionary_hpp_
#define _QPwEntropy_RankedDictionary_hpp_

#include <QHash>
#include <QList>

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

  typedef QList< Word > WordList;

private:
  struct SearchNode;

public:
  RankedDictionary( QString name );
  ~RankedDictionary();

  void addPreprocessor( SubstitutionCipher *substitution );

  void buildIndex();
  void loadDictionary( QIODevice *dictionary );

  QString name() const { return m_name; }

  WordList match( const QString &word ) const;

private:
  void buildIndex( SearchNode *node, int chrIndex );

  Range indexMatch( const QString &word, const SearchNode *node, int chrIndex, WordList *matches ) const;

  void insert( const QString & word, int rank );

private:
  QString m_name;

  QList< SubstitutionCipher * > m_preprocessors;
  WordList m_dictionary;
  SearchNode *m_searchRoot;

  static const int kIndexDepth = 3;

  int m_maxLength;
};
}

#endif
