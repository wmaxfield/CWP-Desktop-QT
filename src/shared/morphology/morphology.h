#ifndef MORPHOLOGY_H
#define MORPHOLOGY_H

#include <QString>
#include "cwpdefines.h"


 QString startMorphology(BOOL *Fin, QString  MorphTags);

 QString getCase(char Letter);
 QString handleMorphology( QString MorphTag);

 QString getVoice(char Letter);
 QString getTense( QString Letter);
 QString getMood(char Letter);
 QString getGender(char Letter);
 QString getExtra( QString Letter);
 QString getNumber(char Letter);
  BOOL     getPronoun( QString MorphTag,  QString wordNumber,  QString wordCase);


#endif // MORPHOLOGY_H
