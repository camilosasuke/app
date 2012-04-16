#include "StdAfx.h"

namespace colorertake5
{

#ifndef __TIMESTAMP__
#define __TIMESTAMP__ "28 May 2006"
#endif


void ParserFactory::init()
{
   hrcParser = NULL;
   fileErrorHandler = NULL;
   xml::document document(get_app());
   try
   {
      document.load_location(catalogPath);
   }
   catch(exception &e)
   {
      throw ParserFactoryException(*e.getMessage());
   }
   catch(ex1::exception * pe)
   {
      string str;
      pe->GetErrorMessage(str);
      throw ParserFactoryException(str);
   }

   xml::node * catalog = document.get_root();
   if(catalog == NULL || catalog->get_name() != "catalog")
   {
      throw ParserFactoryException(string("bad catalog structure"));
   }

   xml::node * elem = catalog->first_child();
   while(elem != NULL)
   {
      // hrc locations
      if (elem->get_type() == xml::node_element && elem->get_name() == "hrc-sets")
      {
         string logLocation = (elem)->attr("log-location");

         if (logLocation.has_char())
         {
            string str = Application.file().as_string(logLocation);
            colorer_logger_set_target(str);
         }
         if (fileErrorHandler == NULL)
         {
            fileErrorHandler = new DefaultErrorHandler();
         }
         xml::node *loc = elem->first_child();
         while(loc != NULL)
         {
            if(loc->get_type() == xml::node_element && loc->get_name() == "location")
            {
               hrcLocations.add((const char *) (loc)->attr("link"));
            }
            loc = loc->get_next_sibling();
         }
      }
      // hrd locations
      else if (elem->get_type() == xml::node_element && elem->get_name() == "hrd-sets")
      {
         xml::node *hrd = elem->first_child();
         while(hrd != NULL)
         {
            if(hrd->get_type() == xml::node_element && hrd->get_name() == "hrd")
            {
               string hrd_class = (hrd)->attr("class");
               string hrd_name = (hrd)->attr("name");
               if(hrd_class.is_empty() || hrd_name.is_empty())
               {
                  hrd = hrd->get_next_sibling();
                  continue;
               }
               string hrd_descr = (hrd)->attr("description");
               if(hrd_descr.is_empty())
               {
                  hrd_descr = hrd_name;
               }
               hrdDescriptions.set_at(hrd_class + "-" + hrd_name, hrd_descr);
               ::collection::string_map<stringa> & hrdClass = hrdLocations[hrd_class];
               stringa & hrdLocV =  hrdClass[hrd_name];
               xml::node *loc = hrd->first_child();
               while(loc != NULL)
               {
                  if(loc->get_type() == xml::node_element && loc->get_name() == "location")
                  {
                     hrdLocV.add((const char *) loc->attr("link"));
                  }
                  loc = loc->get_next_sibling();
               }
            }
            hrd = hrd->get_next_sibling();
         }
      }
      elem = elem->get_next_sibling();
   }
}

string ParserFactory::searchPath()
{
   return Application.dir().matter("colorer.zip:catalog.xml");
};

ParserFactory::ParserFactory(::ca::application * papp) :
   ca(papp)
{

  fileErrorHandler = NULL;

  keeper < bool > keepZipAsDir(&papp->m_bZipIsDir, true, papp->m_bZipIsDir, true);

  catalogPath = searchPath();

  init();

}

ParserFactory::ParserFactory(::ca::application * papp, string catalogPath) :
   ca(papp)
{

   fileErrorHandler = NULL;

   if(catalogPath.is_empty())
      this->catalogPath = searchPath();
   else
      this->catalogPath = (catalogPath);

   init();

}

ParserFactory::~ParserFactory()
{
}

const char *ParserFactory::getVersion()
{

   return "Colorer-take5 Library be5 "__TIMESTAMP__;

}



//string ParserFactory::enumerateHRDClasses(int idx){
  //return hrdLocations.key(idx);
//};
//string ParserFactory::enumerateHRDInstances(const string &classID, int idx){
  //::collection::string_map<pointer_object<stringa>> *hash = hrdLocations.pget(classID);
  //if (hash == NULL) return "";
  //return hash->key(idx);
//};
string ParserFactory::getHRDescription(const string &classID, const string &nameID){
  return hrdDescriptions[classID+"-"+nameID];
};

HRCParser* ParserFactory::getHRCParser()
{
   if(hrcParser != NULL)
      return hrcParser;
   hrcParser = new HRCParserImpl(get_app());
   hrcParser->setErrorHandler(fileErrorHandler);
   for(int idx = 0; idx < hrcLocations.get_size(); idx++)
   {
         string relPath = hrcLocations.element_at(idx);
         string path;
         if(System.file().path().is_relative(relPath))
         {
            path = System.dir().path(System.dir().name(catalogPath), relPath);
            string path2del = path;
            gen::str::begins_eat(path, "file://");
            gen::str::begins_eat(path, "file:/");
            gen::str::begins_eat(path, "file:");
         }
         else
         {
            path = relPath;
         }
         if(Application.dir().is(path))
         {
            //if(path == "v:\\votagus\\basis\\app\\appmatter\\main\\_std\\_std\\colorer\\hrc\\auto")
            {
              // AfxDebugBreak();
            }
            stringa straPath;
            System.dir().rls(get_app(), path, &straPath);
            ex1::filesp spfile(get_app());
            for(int i = 0; i < straPath.get_count(); i++)
            {
               if(!Application.dir().is(straPath[i]))
               {
                  string str = Application.file().as_string(straPath[i]);
                  try
                  {
                     hrcParser->loadSource(straPath[i], str);
                  }
                  catch(base_exception &e)
                  {
                     string str;
                     e.GetErrorMessage(str);
                     TRACE("exception %s", str.c_str());
                  }
               }
            }
         }
         else
         {
            string strSource = Application.file().as_string(path);
            if(strSource.has_char())
            {
               try
               {
                  hrcParser->loadSource(path, strSource);
               }
               catch(base_exception & e)
               {
                  string str;
                  e.GetErrorMessage(str);
                  TRACE("exception %s", str.c_str());
               }
            }
         }
      }
       return hrcParser;
     }

text_parser *ParserFactory::createTextParser(){
  return new text_parser_impl();
};

StyledHRDMapper *ParserFactory::createStyledMapper(string classID, string nameID)
{
  ::collection::string_map<stringa> * hrdClass = NULL;
  if (classID.is_empty())
    hrdClass = hrdLocations.pget(("rgb"));
  else
    hrdClass = hrdLocations.pget(classID);

  if (hrdClass == NULL)
    throw ParserFactoryException(string("can't find hrdClass '")+classID+"'");

  stringa *hrdLocV = NULL;
  if (nameID.is_empty())
  {
    char *hrd = getenv("COLORER5HRD");
    hrdLocV = (hrd) ? hrdClass->pget((hrd)) : hrdClass->pget(("default"));
    if(hrdLocV == NULL)
    {
      hrdLocV = hrdClass->pget(("default"));
    }
  }
  else
    hrdLocV = hrdClass->pget(nameID);
  if (hrdLocV == NULL)
    throw ParserFactoryException(string("can't find hrdName '")+nameID+"'");

  StyledHRDMapper *mapper = new StyledHRDMapper(get_app());
  for(int idx = 0; idx < hrdLocV->get_size(); idx++)
  {
    if (hrdLocV->element_at(idx).has_char())
    {

       try
       {
          ex1::byte_stream spfile(Application.get_byte_stream(hrdLocV->element_at(idx), ::ex1::file::mode_read | ::ex1::file::type_binary));
          if(spfile.is_reader_set())
          {
            mapper->loadRegionMappings(spfile);
          }
       }
       catch(base_exception & )
       {
       }
      };
    };
  return mapper;
};

TextHRDMapper *ParserFactory::createTextMapper(string nameID){
  // fixed class 'text'
  ::collection::string_map<stringa> *hrdClass = hrdLocations.pget(("text"));
  if (hrdClass == NULL) throw ParserFactoryException(string("can't find hrdClass 'text'"));

  stringa *hrdLocV = NULL;
  if (nameID.is_empty())
    hrdLocV = hrdClass->pget(("default"));
  else
    hrdLocV = hrdClass->pget(nameID);
  if (hrdLocV == NULL)
    throw ParserFactoryException(string("can't find hrdName '")+nameID+"'");

  TextHRDMapper *mapper = new TextHRDMapper(get_app());
  for(int idx = 0; idx < hrdLocV->get_size(); idx++)
  {
    if (hrdLocV->element_at(idx).has_char())
    {
       try
       {
          ex1::byte_stream stream(Application.get_byte_stream(hrdLocV->element_at(idx), ::ex1::file::mode_read |::ex1::file::type_binary));
          if(stream.is_reader_set())
          {
               mapper->loadRegionMappings(stream);
          }
       }
       catch(base_exception &)
       {
       }
     }
  }
  return mapper;
}

} // namespace colorertake5

/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is the Colorer Library.
 *
 * The Initial Developer of the Original Code is
 * Cail Lomecb <cail@nm.ru>.
 * Portions created by the Initial Developer are Copyright (C) 1999-2005
 * the Initial Developer.
 *
 * Contributor(s):
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */
