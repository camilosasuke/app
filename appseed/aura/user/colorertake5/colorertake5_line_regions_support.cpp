#include "framework.h" // previously aura/user/user.h


namespace colorertake5
{


   LineRegionsSupport::LineRegionsSupport()
   {
      lineCount = 0;
      firstLineNo = 0;
      regionMapper = NULL;
      special = NULL;
   }

   LineRegionsSupport::~LineRegionsSupport()
   {
      clear();
      while(schemeStack.get_size() > 1)
      {

         delete schemeStack.last();

         schemeStack.remove_last();

      }

   }

   void LineRegionsSupport::resize(index lineCount)
   {
      ::count iOldLineCount = lineRegions.get_size();
      lineRegions.allocate(lineCount);
      for(index i = iOldLineCount; i < lineCount; i++)
      {
         lineRegions[i] = NULL;
      }
      this->lineCount = lineCount;
   }

   ::count LineRegionsSupport::get_size()
   {
      return lineCount;
   }

   void LineRegionsSupport::clear()
   {
      
      for(index idx = 0; idx < lineRegions.get_size(); idx++)
      {
         
         LineRegion *ln = lineRegions.element_at(idx);
         
         lineRegions.set_at(idx, NULL);
         
         while(ln != NULL)
         {
            LineRegion *lnn = ln->next;
            delete ln;
            ln = lnn;
         }

      }

   }

   index LineRegionsSupport::getLineIndex(index lno) const
   {
      
      return ((firstLineNo % lineCount) + lno - firstLineNo) % lineCount;

   }

   LineRegion *LineRegionsSupport::getLineRegions(index lno) const
   {

      if (!checkLine(lno))
         return NULL;

      return lineRegions.element_at(getLineIndex(lno));

   }

   void LineRegionsSupport::setFirstLine(index first)
   {
      firstLineNo = first;
   }

   index LineRegionsSupport::getFirstLine()
   {
      return firstLineNo;
   }

   void LineRegionsSupport::setBackground(RegionDefine *back)
   {

      background.rdef = (RegionDefine*)back;

   }

   void LineRegionsSupport::setSpecialRegion(class region *special)
   {

      this->special = special;

   }

   void LineRegionsSupport::setRegionMapper(RegionMapper*rs)
   {

      regionMapper = rs;

   }

   bool LineRegionsSupport::checkLine(index lno) const
   {

      if (lno < firstLineNo || lno >= firstLineNo+lineCount)
      {
         CLR_WARN("LineRegionsSupport", "checkLine: line %d out of range", lno);
         return false;
      
      }
      return true;

   }

   void LineRegionsSupport::startParsing(index lno)
   {

      UNREFERENCED_PARAMETER(lno);

      while(schemeStack.get_size() > 1)
      {

         delete schemeStack.last();

         schemeStack.remove_last();

      }

      schemeStack.remove_all();

      schemeStack.add(&background);

   }

   void LineRegionsSupport::clearLine(index lno, const char * pszLine)
   {
      
      UNREFERENCED_PARAMETER(pszLine);
      
      if(!checkLine(lno))
         return;

      LineRegion *ln = getLineRegions(lno);
      
      while(ln != NULL)
      {
         LineRegion *lnn = ln->next;
         delete ln;
         ln = lnn;
      }

      LineRegion *lfirst = new LineRegion(*schemeStack.last());
      lfirst->start = 0;
      lfirst->end = -1;
      lfirst->next = NULL;
      lfirst->prev = lfirst;
      lineRegions.set_at(getLineIndex(lno), lfirst);
      flowBackground = lfirst;
   }

   void LineRegionsSupport::addRegion(index lno, const char *line, strsize sx, strsize ex, class region* region)
   {
      
      UNREFERENCED_PARAMETER(line);

      // ignoring out of cached interval lines

      if (!checkLine(lno)) 
         return;

      LineRegion *lnew = new LineRegion();
      lnew->start = sx;
      lnew->end = ex;
      lnew->region = region;
      lnew->scheme = schemeStack.last()->scheme;
      if (region->hasParent(special))
         lnew->special = true;
      if (regionMapper != NULL)
      {
         RegionDefine *rd = regionMapper->getRegionDefine(region);
         if (rd == NULL) rd = schemeStack.last()->rdef;
         if (rd != NULL)
         {
            lnew->rdef = rd->clone();
            lnew->rdef->assignParent(schemeStack.last()->rdef);
         }
      }
      addLineRegion(lno, lnew);
   }

   void LineRegionsSupport::enterScheme(index lno, const char *line, strsize sx, strsize ex, class region* region, class scheme *scheme)
   {
      UNREFERENCED_PARAMETER(line);
      UNREFERENCED_PARAMETER(ex);
      LineRegion * lr = new LineRegion();
      lr->region = region;
      lr->scheme = scheme;
      lr->start = sx;
      lr->end = -1;
      if (regionMapper != NULL)
      {
         
         const RegionDefine *rd = regionMapper->getRegionDefine(region);
         
         if (rd == NULL) 
            rd = schemeStack.last()->rdef;

         if (rd != NULL)
         {
            lr->rdef = rd->clone();
            lr->rdef->assignParent(schemeStack.last()->rdef);
         }

      }

      schemeStack.add(lr);
      // ignoring out of cached interval lines
      if (!checkLine(lno)) return;
      // we must skip transparent regions
      if (lr->region != NULL)
      {
         LineRegion *lr_add = new LineRegion(*lr);
         flowBackground->end = lr_add->start;
         flowBackground = lr_add;
         addLineRegion(lno, lr_add);
      };
   }

   void LineRegionsSupport::leaveScheme(index lno, const char *line, strsize sx, strsize ex, class region* region, class scheme * scheme)
   {
      UNREFERENCED_PARAMETER(line);
      UNREFERENCED_PARAMETER(sx);
      UNREFERENCED_PARAMETER(region);
      UNREFERENCED_PARAMETER(scheme);
      class region* scheme_region = schemeStack.last()->region;
      delete schemeStack.last();
      schemeStack.allocate(schemeStack.get_size()-1);
      // ignoring out of cached interval lines
      if (!checkLine(lno)) return;
      // we have to skip transparent regions
      if (scheme_region != NULL){
         LineRegion *lr = new LineRegion(*schemeStack.last());
         lr->start = ex;
         lr->end = -1;
         flowBackground->end = lr->start;
         flowBackground = lr;
         addLineRegion(lno, lr);
      };
   }

   void LineRegionsSupport::addLineRegion(index lno, LineRegion *lr)
   {
      
      LineRegion *lstart = getLineRegions(lno);
      
      lr->next = NULL;
      lr->prev = lr;

      if(lstart == NULL)
      {

         lineRegions.set_at(getLineIndex(lno), lr);

      }
      else
      {

         lr->prev = lstart->prev;

         lr->prev->next = lr;

         lstart->prev = lr;

      }

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
