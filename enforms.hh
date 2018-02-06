
#ifndef __ENFORMS_HH_INCLUDED__
#define __ENFORMS_HH_INCLUDED__

//#include "dictionary.hh"
#include <vector>
#include "btreeidx.hh"
#include "wstring_qt.hh"

namespace EnForms {

using std::vector;
//using std::string;
using gd::wstring;

using BtreeIndexing::WordArticleLink;
using BtreeIndexing::BtreeDictionary;

// class BtreeIndex
//   vector< WordArticleLink > findArticles( wstring const & );

// class BtreeDictionary: public Dictionary::Class, public BtreeIndex

// class StardictDictionary: public BtreeIndexing::BtreeDictionary


vector< WordArticleLink > searchAlternatives( wstring const &, BtreeIndexing::BtreeDictionary &);

}

#endif
