
#include "stardict.hh"
#include "btreeidx.hh"
#include "folding.hh"
#include "wstring_qt.hh"
//for debug
//#include <iostream>

#include <vector>
#include <map>


namespace EnForms {

using std::map;
//using std::multimap;
using std::pair;
using std::vector;
using std::string;
using gd::wstring;

//( begin: search alternatives 
using gd::toQString;
using gd::toWString;
//) end: search alternatives 

using BtreeIndexing::WordArticleLink;
using BtreeIndexing::BtreeDictionary;

map<wstring, wstring> getIrregularVerbsMap();

map<wstring, wstring> wordMap = getIrregularVerbsMap();

 
map<wstring, wstring> getIrregularVerbsMap()
{
  map<wstring, wstring> mapw;

  mapw[ L"am" ] = L"be";
  mapw[ L"is" ] = L"be";
  mapw[ L"are" ] = L"be";
  mapw[ L"was" ] = L"be";
  mapw[ L"were" ] = L"be";
  mapw[ L"been" ] = L"be";
  mapw[ L"began" ] = L"begin";
  mapw[ L"begun" ] = L"begin";
  mapw[ L"broke" ] = L"break";
  mapw[ L"broken" ] = L"break";
  mapw[ L"brought" ] = L"bring";
  mapw[ L"built" ] = L"build";
  mapw[ L"bought" ] = L"buy";

  mapw[ L"chose" ] = L"choose";
  mapw[ L"chosen" ] = L"choose";
  mapw[ L"came" ] = L"come";

  mapw[ L"did" ] = L"do";
  mapw[ L"done" ] = L"do";
  mapw[ L"drawn" ] = L"draw";
  mapw[ L"drew" ] = L"draw";
  mapw[ L"driven" ] = L"drive";
  mapw[ L"drove" ] = L"drive";

  mapw[ L"ate" ] = L"eat";
  mapw[ L"eaten" ] = L"eat";

  mapw[ L"felt" ] = L"feel";
  mapw[ L"found" ] = L"find";

  mapw[ L"got" ] = L"get";
  mapw[ L"gotten" ] = L"get";
  mapw[ L"gave" ] = L"give";
  mapw[ L"given" ] = L"give";
  mapw[ L"gone" ] = L"go";
  mapw[ L"went" ] = L"go";

  mapw[ L"had" ] = L"have";
  mapw[ L"has" ] = L"have";
  mapw[ L"heard" ] = L"hear";
  mapw[ L"held" ] = L"hold";

  mapw[ L"kept" ] = L"keep";
  mapw[ L"knew" ] = L"know";
  mapw[ L"known" ] = L"know";

  mapw[ L"led" ] = L"lead";
  mapw[ L"left" ] = L"leave";
  mapw[ L"lain" ] = L"lie";
  mapw[ L"lay" ] = L"lie";
  mapw[ L"lost" ] = L"lose";

  mapw[ L"made" ] = L"make";
  mapw[ L"meant" ] = L"mean";
  mapw[ L"met" ] = L"meet";

  mapw[ L"paid" ] = L"pay";
  mapw[ L"ran" ] = L"run";

  mapw[ L"said" ] = L"say";
  mapw[ L"saw" ] = L"see";
  mapw[ L"seen" ] = L"see";
  mapw[ L"sold" ] = L"sell";
  mapw[ L"sent" ] = L"send";
  mapw[ L"sat" ] = L"sit";
  mapw[ L"spoke" ] = L"speak";
  mapw[ L"spoken" ] = L"speak";
  mapw[ L"spent" ] = L"spend";
  mapw[ L"stood" ] = L"stand";

  mapw[ L"taken" ] = L"take";
  mapw[ L"took" ] = L"take";
  mapw[ L"taught" ] = L"teach";
  mapw[ L"told" ] = L"tell";
  mapw[ L"thought" ] = L"think";
  mapw[ L"threw" ] = L"throw";
  mapw[ L"thrown" ] = L"throw";

  mapw[ L"understood" ] = L"understand";

  mapw[ L"wore" ] = L"wear";
  mapw[ L"worn" ] = L"wear";
  mapw[ L"won" ] = L"win";
  mapw[ L"written" ] = L"write";
  mapw[ L"wrote" ] = L"write";

  return mapw;
}


//( begin: search alternatives 
bool isEndWithNN( wstring & wordNN )
{
  bool isResult = false;

  int lenWord = wordNN.size();

  if ( 2 < lenWord )
  {
    wstring strNN = wordNN.substr( lenWord - 2, 2 );

    if ( (strNN == L"gg") || (strNN == L"ll") || (strNN == L"mm") 
      || (strNN == L"nn") || (strNN == L"pp") || (strNN == L"rr") 
      || (strNN == L"tt") )
    {
      //stemmed --> stem
      //occurred
      //omitted
      isResult = true;
    }
  };

  return isResult;
}

pair< wstring, vector< WordArticleLink > > getVerbFormOne(wstring const & folded,
  BtreeDictionary & dict )
{
  int lenWord = folded.size();

  vector<wstring> options;
  vector<WordArticleLink> chainTest;
  wstring resultWord77 = L"";

  // if (!isOneWord)
  // {
  //   //holding out
  //   options.push_back( folded );
  // }
  options.push_back( folded );

  if ( 1 < lenWord && folded[lenWord - 1] == L's' )
  {    
    // classes --> class
    if ( 2 < lenWord && folded.substr( lenWord - 2, 2 ) == L"es" )
    {
      options.push_back( folded.substr( 0, lenWord - 2 ) );
    }

    if ( 3 < lenWord )
    {
      if ( folded.substr( lenWord - 3, 3 ) == L"ves" )
      {
        // wolves --> wolf
        options.push_back( folded.substr( 0, lenWord - 3 ) + L"f" );

        // knives --> knife
        options.push_back( folded.substr( 0, lenWord - 3 ) + L"fe" );
      }
      // parties --> party
      if (folded.substr( lenWord - 3, 3 ) == L"ies" )
      {
        options.push_back( folded.substr( 0, lenWord - 3 ) + L"y" );
      }
    }

    wstring folded_noS = folded.substr( 0, lenWord - 1 );
    int lenWordNoS = lenWord - 1;

    if ( (2 < lenWordNoS) && (folded_noS.substr( lenWordNoS - 2, 2 ) == L"er" ) )
    {
      options.push_back( folded_noS.substr( 0, lenWordNoS - 2 ) );
      //infringers --> infringe
      options.push_back( folded_noS.substr( 0, lenWordNoS - 1 ) );
    }

    // books --> book
    options.push_back( folded_noS );    
  } 
  else if ( 2 < lenWord && folded.substr( lenWord - 2, 2 ) == L"ed" )
  {

    wstring folded_noED = folded.substr( 0, lenWord - 2 );
    int lenWordNoED = lenWord - 2;
    //stemmed --> stem
    //occurred
    //omitted
    if ( isEndWithNN( folded_noED ) )
      options.push_back( folded_noED.substr( 0, lenWordNoED - 1 ) );

    if ( (1 < lenWordNoED) && (folded_noED.substr( lenWordNoED - 1, 1 ) == L"i" ) )
    {
      //carried --> carry
      options.push_back( folded_noED.substr( 0, lenWordNoED - 1 ) + L"y" );
    }

    // worked --> work
    options.push_back( folded_noED );

    //leveraged --> leverage
    //staged --> stage, -stag
    options.push_back( folded.substr( 0, lenWord - 1 ) );
  }
  else if ( 3 < lenWord && folded.substr( lenWord - 3, 3 ) == L"ing" )
  {
    wstring folded_noING = folded.substr( 0, lenWord - 3 );
    int lenWordNoING = lenWord - 3;

    // stunning --> stun
    if ( isEndWithNN( folded_noING ) )
      options.push_back( folded_noING.substr( 0, lenWordNoING - 1 ) );

    //clanging --> clang
    options.push_back( folded_noING );

    //inlining --> inline, in second dict, but not found
    options.push_back( folded_noING + L"e" );
  }
  else if ( 2 < lenWord && folded.substr( lenWord - 2, 2 ) == L"al" )
  {
    wstring folded_noAL = folded.substr( 0, lenWord - 2 );
//    int lenWordNoAL = lenWord - 2;

    //
    options.push_back( folded_noAL );

    //traversal --> traverse
    options.push_back( folded_noAL + L"e" );
  }
  else if ( 2 < lenWord && folded.substr( lenWord - 2, 2 ) == L"ly" )
  {
    wstring folded_noLY = folded.substr( 0, lenWord - 2 );
    int lenWordNoLY = lenWord - 2;
    if ( 1 < lenWordNoLY && folded_noLY.substr( lenWordNoLY - 1, 1 ) == L"i")
    {
      //unnecessarily --> unnecessary
      options.push_back( folded_noLY.substr( 0, lenWordNoLY - 1 ) + L"y" );
    }

    // painstakingly --> painstaking
    options.push_back( folded_noLY );
  }
  else if ( 2 < lenWord && folded.substr( lenWord - 2, 2 ) == L"er" )
  {
    wstring folded_noER = folded.substr( 0, lenWord - 2 );
    int lenWordNoER = lenWord - 2;

    //bigger --> big
    if ( isEndWithNN( folded_noER ) )
      options.push_back( folded_noER.substr( 0, lenWordNoER - 1 ) );

    // taller --> 
    options.push_back( folded_noER );
  }
  else if ( 3 < lenWord && folded.substr( lenWord - 3, 3 ) == L"est" )
  {
    wstring folded_noEST = folded.substr( 0, lenWord - 3 );
    int lenWordNoEST = lenWord - 3;

    //biggest --> big
    if ( isEndWithNN( folded_noEST ) )
      options.push_back( folded_noEST.substr( 0, lenWordNoEST - 1 ) );

    // tallest --> 
    options.push_back( folded_noEST );
  };



//need <iostream>
//  std::wcout << "before map find*" << folded << "*\n";
  map<wstring, wstring>::iterator it = wordMap.find(folded);
  if (it != wordMap.end())
  {
    options.push_back( it->second );
//    options.insert( options.begin, it->second );
//    std::wcout << "in map *" << it->second << "*\n";
  };

  // if (isOneWord)
  // {
  //   //holding out
  //   options.push_back( folded );
  // };


//  for (vector<wstring>::iterator rit = options.rbegin(); rit!= options.rend(); ++rit)
  for (int j=options.size()-1; 0 <= j; j--)
  {
//    std::wcout << "options test1: *" << options[j] << "*\n";
    chainTest = dict.findArticles( options[j] ); 
    if ( chainTest.size() != 0 )
    {
//      std::wcout << "options test1 found: *" << options[j] << "*\n";
      resultWord77 = options[j];
      break;
    };
  };

  // chainTest = findArticles( folded ); 
  // if ( chain.size() != 0 )
  // {
  //   return folded;
  // };

  return make_pair( resultWord77, chainTest );
}

vector< WordArticleLink > searchAlternativesSingleWord( wstring const & word0130,
  BtreeDictionary & dict )
{
//  vector<WordArticleLink> chain;

  wstring folded = Folding::apply( word0130 );
//  wstring verbOne = getVerbFormOne( folded );
//  if ( ! verbOne.empty() ) 
//    chain = dict.findArticles( verbOne );      
//  };

//  return chain;  

  pair<wstring, vector<WordArticleLink> > result = getVerbFormOne( folded, dict );

  return result.second;  
}

  //pulled him a face --> pull a face
  //todo ...
  //  pulled a face --> [pulled, him, a, face]
  //  v1) [pull, pulled] * [ him a face, a face, face]
  //      test: pull+him a face
  //      test: pull+a face
  //      test: pull+face
  //      -test: pulled+him a face
  //      test: pulled+a face
  //      test: pulled+face
  //  v2) [pulled] * [ him a face, a face, face]
  //      -test: pulled+him a face
  //      test: pulled+a face
  //      test: pulled+face
vector< WordArticleLink > searchAlternativesPhrasalVerb( vector< wstring > words,
  BtreeDictionary & dict )
{
  vector< WordArticleLink > chain;

//  wstring wordPV = ;
  wstring word0 = Folding::apply( words[0] );
//  wstring verbOne = getVerbFormOne( word0 );
  pair<wstring, vector<WordArticleLink> > verbOne = getVerbFormOne( word0, dict );

//  if ( ! verbOne.empty() ) 
  if ( ! verbOne.first.empty() ) 
  {
    word0 = verbOne.first;
  };


  vector<wstring> options;
//  for (int i= words.size()-1; iBegWordAdd <= i; i-- )
  for (unsigned int i= words.size()-1; 1 <= i; i-- )
  {
    if ( i==words.size()-1 )
    {
      options.push_back( words[i] );
    }
    else
    {
      options.push_back( words[i] + L" " + options[options.size()-1] );
    }
  };


  for (int j=options.size()-1; 0 <= j; j--)
  {    
    chain = dict.findArticles( word0 + L" " + options[j] );      
    if ( chain.size() != 0 )
    {
//      return chain;
      break;
    };
  }


  if ( chain.size() == 0 && (! verbOne.first.empty() ) )
  {
    chain = verbOne.second;    
  }

  return chain;
}

//You'ra --> You are
//dont, don't --> do not
//'em --> them'
vector< wstring > splitIntoWords( wstring const & input )
{
  vector< wstring > result;

  QString inputQ = toQString(input);
//  inputQ.replace( "'ra", " are" );
//  inputQ.replace( "'em", " them" );
//  inputQ.replace( "don't", "do not" );

  QChar const * ptr = inputQ.data();
//  QString signAp = toQString(L"'");

  for( ; ; )
  {
//    QString spacing;
    QString puncting = toQString(L"");
//    for( ; ptr->unicode() && ( Folding::isPunct( ptr->unicode() ) || Folding::isWhitespace( ptr->unicode() ) ); ++ptr )
    for( ; ptr->unicode() && ( (Folding::isPunct( ptr->unicode() ) && ptr->unicode() != 0x0027) || Folding::isWhitespace( ptr->unicode() ) ); ++ptr )
    {
//      spacing.append( *ptr );      
      if (Folding::isPunct( ptr->unicode() )) 
      {
        puncting.append( *ptr );
      }
    }
/*
    for( ; ptr->unicode() && ( Folding::isPunct( ptr->unicode() ) ); ++ptr )
      puncting.append( *ptr );
*/

//    if ( (0 < result.size()) && (! puncting.isEmpty()) && (puncting!=signAp) )
    if ( (0 < result.size()) && (! puncting.isEmpty()) )
      break;

    QString word77 = toQString(L"");
    for( ; ptr->unicode() && !( ( Folding::isPunct( ptr->unicode() ) && ptr->unicode() != 0x0027) || Folding::isWhitespace( ptr->unicode() ) ); ++ptr )
      word77.append( *ptr );

    if ( word77.isEmpty() )
      break;

//    result.append( toWString(word) );
    wstring wsWord = toWString(word77);
    wstring wsword = Folding::apply( wsWord );
    if (wsword == L"'em" || wsword == L"em") 
    {
      result.push_back( L"them" );
    }
    else
    {
      result.push_back( wsWord );
    };

  }

  return result;
}

// qmake-qt4 "CONFIG+=old_hunspell" && make
// only for Language_From==English and Format_Of_Dictionary=StarDict
// useful when search with scan popup: 
// search alternatives 
//    for plural or 
//    for Past tense of regular verbs, 
//    for -ing,
//    for Phrasal Verbs
//
//
//    (not in dictionary): books --> book (in dictionary),
//    (not in dictionary): classes --> class (in dictionary),
//    (not in dictionary): parties --> party (in dictionary),
//        ?shelves --> shelve or (shelf)
        // knives --> knife
        // wolves --> wolf
//
//    (not in dictionary): worked --> work (in dictionary)
//    (not in dictionary): leveraged --> leverage (in dictionary)
//      stemmed  --> stem
//      occurred --> ocur
     // carried --> carry
//
//    (not in dictionary): clanging --> clang (in dictionary)
//    (not in dictionary): icing --> ice (in dictionary)
//      stunning --> stun
//      snapping --> snap
//
//    (not in dictionary): reaching out --> reach out (in dictionary)
//    (not in dictionary): pushes out --> push out (in dictionary)
//    (not in dictionary): pushed out --> push out (in dictionary)
//    
//    (not in dictionary): turn them on --> turn on
//    (not in dictionary): turn 'em on --> turn on
//    (not in dictionary): pulled him a face --> pull a face
//    (not in dictionary): got at --> get at, thrown off
// error?:
//  "roasting" --+ dict1: "roast"; dict2: "roasting", "roast"
//      dict1 (stardict) not have "roasting", have "roast"
//      dict2 (stardict) have "roasting" 
//    without dict1: "roasting" --+ dict2: "roasting"
//  need algorithm to ? dict1 and dict2
//
//  painstakingly
//  taller
//  tallest --> 
//  ?deterrent, deter
//  holding out
//  unnecessarily --> unnecessary
//  ?taught --> taught (in dict) before smart_search, i want taught --> teach
vector< WordArticleLink > searchAlternatives( wstring const & inWord96,
  BtreeDictionary & dict )
{  
  vector< wstring > words = splitIntoWords( inWord96 );

//  std::wcout << "words in *" << inWord << "*\n";
  if ( 1 < words.size() ) 
  {
//    std::wcout << "words *" << words.size() << "*" << words[0] << "*" << words[1] << "*\n";
    return searchAlternativesPhrasalVerb( words, dict );
  }
  else
  {
//    std::wcout << "1 word *" << words.size() << "*" << words[0] << "*\n";
    return searchAlternativesSingleWord( inWord96, dict );
  }
}
//) end: search alternatives 


}

//opt-out --> opt out
//roundtrips --> round trip (found with stemmed search, one result)

// article "payroll":  сущ. ; = pay-roll
//  :pre.dictionary, where payroll --> at once article for "pay-roll"
//    generate predictionare: articles, where has "=', and one string
//    place predictionary before common dictionaries
//  option 2)
//    transfer with link in article for "payroll"

// colour <--> color

//1)stemmed search found options (result)
//2)show
//    article for best result from step 1 (main window, up)
//    show all options from stemmed search to choose (bottum)

//show like words (by letters or by pronouncation)

//search with wildcast: small list for, co[lmnr]*
//  only one word articles

//statistics fot history: where 2 < query_word_times

//composability --> compose ability