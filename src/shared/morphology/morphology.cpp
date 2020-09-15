/* ========================================
 *
 * Copyright Wade Maxfield, 2010-2020
 * Commercial license available
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.

 * ========================================
*/#include <QStringList>
#include "morphology.h"

//
//  morphology.m
//  CWP-Student
//
//  Created by WADE MAXFIELD on 12/30/10.
//  Copyright 2010 Wade Maxfield, All rights reserved.
//

#include "misclibrary.h"

#if 1
 QString getCase(char Letter)
{

    switch(Letter) { // Select Case Letter
        case 'N':
            //-N = Nominative
            return  "Nominative";
            break;
        case 'V':
            //-V = Vocative
            return "Vocative";
            break;
        case 'G':
            //-G = Genitive
            return "Genitive";
            break;
        case 'D':
            //-D = Dative
            return  "Dative";
            break;
        case 'A':
            //-A = Accusative
            return "Accusative";
    } // End Select


    return "";
}


 QString getExtra( QString Letter)
{

    //Extra:
    //
     switch(Letter.at(0).toLatin1()) { // Select Case Letter
        case 'M':
            //-M = Middle significance
            return "Middle significance";
            break;
        case 'C':
            //-C = Contracted form
            return "Contracted form";
            break;
        case 'T':
            //-T = Transitive
            return "Transitive";
            break;

        case 'A':
            if (StringsAreEqual(Letter, "ATT")) {
                return "Attic";
            }
            if (StringsAreEqual(Letter, "AP")) {
                return "Apocopated form";
            }

            //-A = Aeolic
            return "Aeolic";// default
            break;


        case 'I':
            if (StringsAreEqual(Letter, "IRR")) {
                //-IRR = Irregular or Impure form
                return "Irregular or Impure form";
            }
    } // End Select

    return "";
}


 QString getGender(char Letter)
{


    //Gender:
    //select Case m(3)
    switch(Letter) { // Select Case Letter
        case 'M':
            //M = Masculine
            return "Masculine";
            break;
        case 'F':
            return "Feminine";
            //F = Feminine
            break;
        case 'N':
            //N = Neuter
            return "Neuter";
            break;

    } // End Select

    return "";
}


 QString getMood(char Letter)
{


    //Mood:
    //
    switch(Letter) { // Select Case Letter
        case 'I':
            //I = Indicative
            return "Indicative";
            break;
        case 'S':
            //S = Subjunctive
            return "Subjunctive";
            break;
        case 'O':
            //O = Optative
            return "Optative";
            break;
        case 'M':
            //M = iMperative
            return "imperative";
            break;
        case 'N':
            //N = iNfinitive
            return "infinitive";
            break;
        case 'P':
            //P = Participle
            return "Participle";
            break;
        case 'R':
            //R = Imperative-sense participle
            return "Imperative-sense participle";
            break;
    } // End Select

    return "";
}

 QString getNumber(char Letter)
{


    //Number:
    switch(Letter) { // Select Case Letter
            break;
        case 'S':
            //S = Singular
            return  "Singular";
            break;
        case 'P':
            //P = Plural
            return "Plural";

        case '1':
            return "First Person";

        case '2':
            return "Second Person";

        case '3':
            return    "Third Person";

    } // End Select

    return "";
}


BOOL getPronoun( QString MorphTag,  QString wordNumber,  QString wordCase)
{
    QStringList m;
    int index;// As integer


    for (index = 1 ; index <  5 ; index++) {
        m.append( "");
    } // next

    for (index = 1 ; index <  MorphTag.length() ; index++) {
        m[index] =  Mid(MorphTag,index,1);
    } // next


    if (Instr("123",m[1])>0 ) { // Then
        wordNumber = m[1];//m(1)

        if (StringsAreEqual(wordNumber, "1") ) { // Then
            wordNumber = wordNumber + QString(" person ");
        } else {
            wordNumber = wordNumber + QString(" persons ");
        } // end if

        wordNumber = wordNumber + getNumber(m[3].at(0).toLatin1());
        wordCase = getCase(m[2].at(0).toLatin1());
        return YES;
    } // end if

    return NO;
}




 QString getSuffix( QString Letter)
{

    //Suffixes:
    //
    if (StringsAreEqual(Letter, "S")) {
        return "Superlative";
    }

    if (StringsAreEqual(Letter, "C")) {
        //-C = Comparative (used only with adjectives and some adverbs)
        return "Comparative";
    }
    if (StringsAreEqual(Letter, "ABB")) {
        //-ABB = ABBreviated form (used only with various numerals)
        return "Abbreviated form";
    }
    if (StringsAreEqual(Letter, "I")) {
        //-I = Interrogative
        return "Interogative";
    }
    if (StringsAreEqual(Letter, "N")) {
        //-N = Negative (used only with particles as PRT-N)
        return "Negative";
    }
    if (StringsAreEqual(Letter, "C")) {
        //-C = Contracted form, or two words merged by crasis
        return "Contracted form";
    }
    if (StringsAreEqual(Letter, "ATT")) {
        //-ATT = ATTic Greek form
        return "Attic Greek form";
    }
    if (StringsAreEqual(Letter, "P")) {
        //-P = Particle attached (with relative pronoun)
        return "Particle attached (with relative pronoun)";
    } // End Select
    //

    return "";
}



 QString getTense( QString Letter)
{


    //Tense:
    //
     switch(Letter.at(0).toLatin1()) { // Select Case Letter
            break;
        case 'P':
            //P = Present
            return "Present";
            break;
        case 'I':
            //I = Imperfect
            break;
        case 'F':
            //F = Future
            return "Future";
            break;
        case 'A':
            //A = Aorist
            return "Aorist";
            break;

        case '2':
            if (StringsAreEqual(Letter, "2F")) {
                //Second Future = 2F
                return "Second Future";
            }
            if (StringsAreEqual(Letter, "2R")) {
                //Second peRfect = 2R
                return "Second Perfect";
            }


            if (StringsAreEqual(Letter, "2A")) {
                //Second Aorist = 2A
                return "Second Aorist";
            }

            if (StringsAreEqual(Letter, "2L")) {
                //Second pLuperfect = 2L
                return "Second Pluperfect";
            }
            break;

        case 'R':
            //R = peRfect
            return "Perfect";
            break;
        case 'L':
            //L = pLuperfect
            return "Pluperfect";
            break;
        case 'X':
            //X = no tense stated (adverbial imperative)
            return "No tense (adverbial imperative)";
    } // End Select
    //
    return "";
}

 QString getVoice(char Letter)
{

    //Voice:
    //
    switch(Letter) { // Select Case Letter
            break;
        case 'A':
            //A = Active
            return "Active";
            break;
        case 'M':
            //M = Middle
            return "Middle";
            break;
        case 'P':
            //P = Passive
            return "Passive";
            break;
        case 'E':
            //E = Either middle or passive
            return "Either middle or passive";
            break;
        case 'D':
            //D = middle Deponent
            return "middle Deponent";
            break;
        case 'O':
            //O = passive depOnent
            return "Passive deponent";
            break;
        case 'N':
            //N = middle or passive depoNent
            return "Middle or passive deponent";
            break;
        case 'Q':
            //Q = impersonal active
            return "impersonal active";
            break;
        case 'X':
            //X = no voice stated
            return "";
            //return "(No voice stated)";
    } // End Select

    return "";
}

 QString handleMorphology( QString MorphTag)
{
    QStringList a;
     QString s, Desc, wordCase,wordNumber,wordGender,wordSuffix;
    BOOL Fin;
    int index;
    BOOL Found=YES;// As Boolean = True
    QStringList m;


    wordCase="";
    wordNumber = "";
    wordGender = "";
    wordSuffix = "";
    //Desc="";


    s = MorphTag;

    Desc = startMorphology(&Fin,s);

    if (Fin ) { // Then
        return Desc;
    } // end if

    //
    //DECLINED FORMS:
    //
    //All follow the order: prefix-case-number-gender-(suffix)
    //
    //Prefixes:
    //
    //
    if (Instr(s,"-") == 0 ) return Desc;

    // we have a compound, work on it
    a = mySplit(s,"-");

    if (a.count() < 2 ) return Desc; // we don't have a decode

    //----------------------------------------------------------
    //----------------------------------------------------------


   // m = [NSMutableArray arrayWithCapacity:7];

    for (index = 0 ; index <  6; index++) {
        m.append("");
    } // next

    //Cases (5-case system only):
    //
    if (a.count() >= 2 ) { // Then
        for (index = 0 ; index <  a[1].length(); index++) {
            m[index] = Mid(a[1], index, 1) ;//  m(index)=mid(a(1),index,1)
        } // next

    } // end if


    // Handle the PREFIX, all forms

    switch ( a[0].at(0).toLatin1()){
            //N- = Noun
        case 'N':
            Desc = "Noun";
            //A- = Adjective
            break;
        case 'A':
            Desc = "Adjective";

            //R- = Relative pronoun
            break;
        case 'R':
            Desc ="Relative pronoun";
            //C- = reCiprocal pronoun
            break;
        case 'C':
            Desc ="Reciprocal pronoun";
            break;
        case 'D':
            //D- = Demonstrative pronoun
            Desc ="Demonstrative pronoun";
            break;
        case 'T':
            //T- = definite article
            Desc ="Definite article";
            break;
        case 'K':
            //K- = correlative pronoun
            Desc ="Correlative pronoun";
            break;
        case 'I':
            //I- = Interrogative pronoun
            Desc ="Interrogative pronoun";
            break;
        case 'X':
            //X- = indefinite pronoun
            Desc ="indefinite pronoun";
            break;
        case 'Q':
            //Q- = correlative or interrogative pronoun
            Desc ="Correlative or Interrogative Pronoun";
            break;
        case 'F':
            //F- = reFlexive pronoun (person 1,2,3 added)
            Desc ="Reflexive Pronoun";
            goto  HandlePronoun;
            break;
        case 'S':
            //S- = possessive pronoun (person 1,2,3 added)
            Desc ="Possessive Pronoun ";
        HandlePronoun:
            if (getPronoun(a[1],wordNumber,wordCase) ) { // Then
                return Desc+QString(" - ")+ wordCase +QString(" ")+ wordNumber +QString(" ")+ wordGender+ QString(" ")+wordSuffix + QString(" %");
                //return [NSString stringWithFormat:"%@ - %@ %@ %@ %", Desc, wordCase ,wordNumber , wordGender, wordSuffix];
            } // end if
            break;
        case 'P':
            //P- = Personal pronoun (person 1,2,3 added) > (Note: 1st and 2nd personal pronouns have no gender)
            Desc ="Personal Pronoun";
            goto  HandlePronoun;

            // Verb, changes description
        case 'V':
            goto HandleVerb;
            break;

        default:
            Found =NO;
    } // End Select


    if (Found ) { // Then
    //    if (a.count() >= 3 ) { // Then
   //         wordSuffix = getSuffix(a[2] );
   //     } // end if

        if (wordCase.length()==0 )  {
            wordCase = getCase(m[0].at(0).toLatin1());//m(1))
        }
        if (wordNumber.length()==0 )  {
            wordNumber = getNumber(m[1].at(0).toLatin1());//m(2))
        }

        if (m[2].length()>0) {
            wordGender = getGender(m[2].at(0).toLatin1());//m(3))
        } else {
            wordGender ="";
        }

        if (m[3].length() > 0) {
            wordSuffix = getSuffix(m[3]);//m(4))
        }else {
            wordSuffix ="";
        }

        return Desc+" - "+ wordCase+ QString(" ")+wordNumber +QString(" ")+ wordGender +QString(" ")+ wordSuffix;
       // return [NSString stringWithFormat:"%@ - %@ %@ %@ %", Desc, wordCase ,wordNumber , wordGender, wordSuffix];

    }



    //ADDITIONAL NOTES:
    //
    //Declined proper nouns are not specifically identified as such; only indeclinable forms are so identified.
    //
    //It is ASSUMED that all Strong//s numbers are correct; these have not all been verified, except in a few cases where errors
    //were corrected during the process.
    //
    //Reflexive pronouns have the Person (1,2,3) attached, e.g. F-3DSF See "auth" in Perschbacher for (further examples.
    //
    //"rmd" is the abbreviated numeral for (144,000. This term has no  Strong's number. It therefore has been assigned three Strong//s numbers, reflecting the individual Greek words which make up its > component parts.
    //
    //; index < p of Page
    //
    //===========================
    //
    //THE ONLINE GREEK NEW TESTAMENT PARSING CODES for (VERB-RELATED FORMS
    //
    //Derived, compared and corrected from
    //Davidson//s "Analytical Greek Lexicon,':
    //with comparison made against
    //Perschbacher//s "New Analytical Greek Lexicon':
    //
    //Abbreviated in a form similar ; index <  that found in
    //Friberg//s "Analytical Greek New Testament':
    //
    //Maurice A. Robinson 2 May 2000
    //
    //The codes which follow reflect an original abridgment and correction of the parsing data presented in Samuel Davidson, "The Analytical Greek Lexicon" (London: Samuel Bagster and Sons, 1859), which volume is now in the public domain.
    //
    //Comparison also has been made against the parsing information cited in Wesley J. Perschbacher, "The New Analytical Greek Lexicon" (Peabody, MA: Hendrickson, 1990). The Perschbacher volume is itself an updated and corrected revision of the original Davidson "Analytical Greek Lexicon".
    //
    //Note that more than 500 parsing or other errors which occurred in Davidson and remained uncorrected in the Perschbacher revision have been corrected within the present Online Bible parsing data.
    //
    //The abbreviation system herein utilized, although developed independently, retains features similar but not identical ; index <  those found in Timothy and Barbara Friberg "The Analytical Greek New Testament" (Grand Rapids: Baker, 1981), and can be used with ease by anyone familiar with either the Davidson, Perschbacher, or Friberg volumes.
    //
    //Many non-verbal New Testament Greek forms (e.g., nouns, adjectives, conjunctions, prepositions, and particles) can be declined in more than one manner. Following the example of Davidson and Perschbacher, the declensions given reflect only those forms which actually occur in the Greek New Testament.
    //
    //The present verbal codes exhaustively cover every possible verb parsing situation cited by Perschbacher, and reflect the ; index < tality of forms used in the Online Bible Greek module for (verb parsings.
    //
    //The present parsing list makes no claim ; index <  be error-free; should the user discover any errors in either the parsing/declension codes or Strong//s numbers while using the Online Bible, please inform the present edi; index < r c/o SEBTS, P. O. Box 1889, Wake Forest, NC 27588.
    //

    // see if is verb
    if (a[0].at(0).toUpper().toLatin1() !='V' ){
        return ""; // Then
    }

HandleVerb:
    {
     QString  wordTense="";// As String
     QString  wordVoice="";// As String
     QString  wordMood="";// As String
     QString  wordExtra="";// As String
     QString  wordExtraNumber="";
     QString  wordExtraGender="";
     QString  wordExtraCase="";
    //All Greek verbs are listed in one of three various forms:
    //
    //1) V-tense-voice-mood
    //2) V-tense-voice-mood-person-number
    //3) V-tense-voice-mood-case-number-gender
    //
    index = 1;

    if (m[0].at(0).toLatin1()=='2'){//m(1) = "2" ) { // Then
        m[0] = m[0]+m[1];
        //m(1) = m(1)+m(2)
        index = 2;
    } // end if

    wordTense = getTense(m[0]);//m(1))
    wordVoice = getVoice(m[index ].at(0).toLatin1());//m(index))
    index++;
    wordMood = getMood(m[index ].at(0).toLatin1());//m(index))

    if (m[3].length()) {
        if (Instr("123",m[3] )) {
            wordExtraNumber = getNumber(m[3].at(0).toLatin1());
        } else {
            wordExtraCase = getCase(m[3].at(0).toLatin1());
        }

    if (m[4].length()) {
        wordExtraNumber = getNumber(m[4].at(0).toLatin1());
    }
        if (m[5].length()) {
            wordExtraGender = getGender(m[5].at(0).toLatin1());
        }
    }


    if (a.count() >= 3 ) { // Then

        for ( index = 0 ; index < a[2].length() /*a(2).len*/ ;index++){
            m[index] = Mid(a[2],index,1);//m(index)=mid(a(2),index,1)
        } // next

        if (Instr("123",m[0])>0 ) { // Then
            wordNumber = m[0];

            if (m[0].at(0) == '1' ) { // Then
                wordNumber = wordNumber + QString( " person ");
            }   else {
                wordNumber =  wordNumber + QString( " persons ");
            } // end if

            wordNumber =  wordNumber + getNumber(m[1].at(0).toLatin1());//wordNumber + Morphology.GetNumber(m(2))

            wordCase = getCase(m[2].at(0).toLatin1());//m(2))
            wordGender ="";
        }    else {
            wordCase = getCase(m[0].at(0).toLatin1());//m(1))
            wordNumber = getNumber(m[1].at(0).toLatin1());//m(2))
            wordGender = getGender(m[2].at(0).toLatin1());//m(3))
        } // end if


    } // end if

    if (a.count() >= 4 ) { // Then
        wordExtra = getExtra(a[3] );//a(3))
    } // end if


    if (m[3].length()) {
        return QString("Verb - ")+wordTense+QString(" ")+ wordVoice +QString(" ")+wordMood +QString(" ")+ wordExtraCase +QString(" ")+  wordExtraNumber+QString(" ")+ wordExtraGender+QString("- ")+ wordCase+QString(" ")+ wordNumber+QString(" ")+wordGender+QString(" ")+ wordExtra;
       // return [NSString stringWithFormat:"Verb - %@ %@ %@ %@ %@ %@- %@ %@ %@ %", wordTense, wordVoice ,wordMood , wordExtraCase, wordExtraNumber, wordExtraGender, wordCase, wordNumber,wordGender, wordExtra ];
    } else {
        return QString("Verb - ")+wordTense+QString(" ")+ wordVoice +QString(" ")+wordMood +QString(" - ")+  wordCase+QString(" ")+ wordNumber+QString(" ")+wordGender+QString(" ")+ wordExtra;
        //return [NSString stringWithFormat:"Verb - %@ %@ %@ - %@ %@ %@ %", wordTense, wordVoice ,wordMood , wordCase, wordNumber,wordGender, wordExtra ];

    }


    }
}


 QString startMorphology(BOOL *Fin, QString  MorphTags)
{


     QString  Desc;// As String
    QStringList a;// (-1) As String

    //UNDECLINED FORMS:
    //
    //ADV = ADVerb or adverb and particle combined
    //CONJ = CONJunction or conjunctive particle
    //COND = CONDitional particle or conjunction
    //PRT = PaRTicle, disjunctive particle
    //PREP = PREPosition
    //INJ = INterJection
    //ARAM = ARAMaic transliterated word (indeclinable)
    //HEB = HEBrew transliterated word (indeclinable)
    //N-PRI = Indeclinable PRoper Noun
    //A-NUI = Indeclinable NUmeral (Adjective)
    //N-LI = Indeclinable Letter (Noun)
    //N-OI = Indeclinable Noun of Other type



#define  ADV  "ADV"
#define  CONJ  "CONJ"
#define  COND  "COND"
#define  PRT  "PRT"
#define  PREP  "PREP"
#define  INJ  "INJ"
#define  ARAM  "ARAM"
#define  HEB  "HEB"
#define  N_PRI  "N-PRI"
#define  A_NUI  "A-NUI"
#define  N_LI  "N-LI"
#define  N_OI  "N-OI"

    *Fin = NO;
    Desc = "";

    if (StringsAreEqual( MorphTags, ADV)){
        Desc = "Adverb";
        *Fin = YES;
    }

    if (StringsAreEqual( MorphTags, CONJ)){
        Desc = "Conjunction";
        *Fin = YES;
    }

    if (StringsAreEqual( MorphTags, COND)){
        Desc = "Conditional particle or conjunction";
        *Fin = YES;
    }

    if (StringsAreEqual( MorphTags, PRT)){// , "PRT-N" ,  "PRT-P";
        Desc = "Particle";
        if (Instr(MorphTags,"-")>0 ) { // Then
            a = mySplit(MorphTags,"-");

            if (a.count() >= 2 ) { // Then
                if (a[1].at(0).toLatin1()=='N'/*a(1) = "N"*/ ) { // Then
                    Desc = Desc +QString(" Negative" );
                } // end if

                if (a[1].at(0).toLatin1()=='P'/*a(1) = "P"*/ ) { // Then
                    Desc = Desc+QString(" Positive");
                } // end if
            } // end if
        } // end if
        *Fin = YES;
    }

    if (StringsAreEqual( MorphTags, PREP)){
        Desc = "Preposition";
        *Fin = YES;
    }

    if (StringsAreEqual( MorphTags, INJ)){
        Desc = "Interjection";
        *Fin = YES;
    }

    if (StringsAreEqual( MorphTags, ARAM)){
        Desc ="Aramaic transliterated word";
        *Fin = YES;
    }

    if (StringsAreEqual( MorphTags, HEB)){
        Desc ="Hebrew transliterated word";
        *Fin = YES;
    }

    if (StringsAreEqual( MorphTags, N_PRI)){
        Desc = "Indeclinable proper noun";
        *Fin = YES;
    }


    if (StringsAreEqual( MorphTags, A_NUI)){
        Desc = "Indeclinable numeral";
        *Fin = YES;
    }


    if (StringsAreEqual( MorphTags, N_LI)){
        Desc = "Indeclinable letter noun ";
        *Fin = YES;
    }

    if (StringsAreEqual( MorphTags, N_OI)){
        Desc = "Indeclinable noun";
        *Fin = YES;
    }



    return Desc;
}
#endif
