#ifndef _SUBSTITUTIONCIPHER_HPP_
#define _SUBSTITUTIONCIPHER_HPP_

#include <QString>
#include <QHash>
#include <QVector>

namespace QPwEntropy
{
/** Simple class that stores character substitutions and computes all possible original strings from an input string
 *  Substitution ciphers are a form of simple encryption used famously in WWII (the Enigma machine),
 *  these simply replace one character with another symbol - which might be a letter (eg ROT13) or a number or symbol
 *  (eg, Hax0r or l33t-sp34k).  In the simple case, these ciphers are easy to decode (if the encoding is known)
 *   - a simple find&replace operation is essentially all that is needed.  
 *
 *  However, with l33t-sp34k, and possibly other internet 'ciphers', there are a number of symbols which map back to
 *  more than one character.  For example, '!' can stand in for 'l' or 'i'.  This class handles the storage of
**  these substitutions and builds a list of possible original (deocded) strings */
class SubstitutionCipher
{
public:
  SubstitutionCipher();
  virtual ~SubstitutionCipher();

  /** add a substitution to the internal dictionary.
    *  @param original    Character as it would appear in the original text before substitution
    *  @param substituted Character that is used to replace original in the substituted text
  ***/
  void addSubstitution( QString original, QChar substituted );

  /** Return a list of candidate strings that could have been the orignal text before being ciphered. */
  QStringList decipher( QString input, QVector< int > *substitutedChars = 0 ) const;

private:
  QHash< QChar, QString > m_substitutions;
};


/** Specialisation of L33tCipher with appropriate subsitutions added to the table */
class L33tCipher : public SubstitutionCipher
{
public:
  L33tCipher();

protected:
  using SubstitutionCipher::addSubstitution;
};
}

#endif
