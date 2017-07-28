#include "framework.h"

const int32_t xfplayer_view_line::AlignLeft = 1;
const int32_t xfplayer_view_line::AlignRight = 2;

xfplayer_view_line::xfplayer_view_line(::aura::application * papp) :
   object(papp),
   m_dibMain(allocer()),
   m_dcextension(papp),
   m_font(allocer())
{
   m_bColonPrefix = false;
   m_pContainer = NULL;
   m_bEnhancedEmboss = true;
   m_bCacheEmboss = false;
   m_cr = ARGB(255, 255, 255, 255);
   m_crOutline = ARGB(255, 0, 0, 0);
   m_nFont = 0;
   m_lpBitmapData = NULL;
   m_bAutoSizeX = false;
   m_bAutoSizeY = false;
   m_iAlign = AlignLeft;
   m_rect.left = 0;
   m_rect.top = 0;
   m_rect.right = 0;
   m_rect.bottom = 0;
   m_bVisible = false;
   m_iAnimateType = AnimateNoAnimate;
   m_iTextEffect = EffectSimple;
   m_dAnimateProgress = 0;
   m_iIndent = 0;
   m_dAnimateProgressIncrement = 1.0;
   m_dXfplayerViewLineBlend = 1.0;
   m_iIndex = 0;
}

xfplayer_view_line::xfplayer_view_line(xfplayer_view_linea * pContainer) :
   object(pContainer->get_app()),
   m_dibMain(pContainer->allocer()),
   m_dcextension(pContainer->get_app()),
   m_font(allocer())
{
   m_pContainer = pContainer;
   m_bEnhancedEmboss = true;
   m_bCacheEmboss = false;
   m_nFont = 0;
   m_lpBitmapData = NULL;
   m_bAutoSizeX = false;
   m_bAutoSizeY = false;
   m_iAlign = AlignLeft;
   m_rect.left = 0;
   m_rect.top = 0;
   m_rect.right = 0;
   m_rect.bottom = 0;
   m_bVisible = false;
   m_iAnimateType = AnimateNoAnimate;
   m_iTextEffect = EffectSimple;
   m_dAnimateProgress = 0;
   m_dAnimateProgress = 0;
   m_dAnimateProgressIncrement = 1.0;
   m_iIndex = 0;
}

xfplayer_view_line::xfplayer_view_line(const xfplayer_view_line & line) :
   object(line.get_app()),
   m_dibMain(allocer()),
   m_dcextension(line.get_app()),
   m_font(allocer())
{
   operator = (line);
}

xfplayer_view_line::~xfplayer_view_line()
{
}


bool xfplayer_view_line::PrepareLine(::draw2d::graphics * pgraphics, const string & str, int32_t flags, const RECT & rect)
{

   single_lock sl(m_pContainer->m_pmutex);

   UNREFERENCED_PARAMETER(flags);
   m_straLink.remove_all();
   m_iaLinkStart.remove_all();
   m_iaLinkEnd.remove_all();
   strsize               iChars;
   strsize               iStr;
   strsize               iStrLen;
   ASSERT(pgraphics != NULL);
   iStrLen = str.get_length();
   iChars = -1;
   m_str.Empty();
   m_iIndent = 0;
   for (iStr = 0; iStr < iStrLen; iStr++)
   {
      AddChar(str[iStr], iChars);
   }
   //   CalcCharsPositions(pgraphics, GetFonts(), pRect);
   CalcCharsPositions(pgraphics, rect);
   return true;
}


void xfplayer_view_line::AddChar(WCHAR wch, strsize & index)
{

   single_lock sl(m_pContainer->m_pmutex);

   m_str += wch;

   index = m_str.get_length() - 1;

   if (m_iaPosition.get_size() < index + 2)
   {

      m_iaPosition.allocate(m_iaPosition.get_size() + 10);

   }

}


void xfplayer_view_line::AddChar(WCHAR wch, strsize & index, visual::font * pFont)
{

   single_lock sl(m_pContainer->m_pmutex);

   UNREFERENCED_PARAMETER(pFont);
   index++;
   if (m_iaPosition.get_size() < index + 2)
   {
      m_iaPosition.allocate(m_iaPosition.get_size() + 10);
   }
   m_str += wch;
   ASSERT(m_str.get_length() - 1 == index);

}


void xfplayer_view_line::GetPlacement(LPRECT lprect)
{

   single_lock sl(m_pContainer->m_pmutex);

   *lprect = m_rect;
}

bool xfplayer_view_line::to(::draw2d::graphics * pgraphics, bool bDraw, const RECT & rect, rect_array & rectaModified, bool bRecalcLayout)
{

   single_lock sl(m_pContainer->m_pmutex);

   

   string strFinal(m_str);

   double dBlend;

   if (m_dXfplayerViewLineBlend <= 0.0)
      return true;
   if (m_dXfplayerViewLineBlend >= 1.0)
      dBlend = 1.0;
   else
      dBlend = m_dXfplayerViewLineBlend;

   pgraphics->SelectObject(m_font);

   //   pgraphics->SetBkMode(TRANSPARENT);

   point iMargin;
   iMargin.x = 3;
   iMargin.y = 3;

   if (!IsVisible())
   {
      return true;
   }
   if (bRecalcLayout || m_rectClient != rect)
   {
      m_bCacheEmboss = false;
      CalcCharsPositions(
         pgraphics,
         rect);
   }

   ::rect rectTextOut;
   GetPlacement(rectTextOut);

   switch (m_iAnimateType)
   {
   case AnimateNoAnimate:
      {
         if (bDraw)
         {
            strsize iLink = 0;
            strsize iChar = 0;
            while (true)
            {
               pgraphics->SelectObject(m_font);
               if (iChar >= strFinal.get_length())
                  break;
               if (iLink >= m_iaLinkStart.get_size())
               {
                  size size = pgraphics->GetTextExtent(strFinal.Left(iChar));
                  EmbossedTextOut(pgraphics, strFinal.Mid(iChar), rectTextOut.left + size.cx, rectTextOut.top, 0, m_cr, m_crOutline, strFinal.get_length() - iChar, dBlend);
                  break;
               }
               else if (m_iaLinkStart[iLink] > iChar)
               {
                  size size = pgraphics->GetTextExtent(strFinal.Left(iChar));
                  EmbossedTextOut(pgraphics, strFinal.Mid(iChar), rectTextOut.left + size.cx, rectTextOut.top, 0, m_cr, m_crOutline, m_iaLinkStart[iLink], dBlend);
               }
               pgraphics->SelectObject(m_fontLink);
               size size = pgraphics->GetTextExtent(strFinal.Left(m_iaLinkStart[iLink]));

               EmbossedTextOut(pgraphics, strFinal.Mid(m_iaLinkStart[iLink]), rectTextOut.left + size.cx, rectTextOut.top, 0, m_cr, m_crOutline, m_iaLinkEnd[iLink] - m_iaLinkStart[iLink] + 1, dBlend);
               iChar = m_iaLinkEnd[iLink] + 1;
               iLink++;
            }
            if (GetSelection().m_iCharStartSource >= 0)
            {
               index iStart;
               index iLineStart;
               index iLineEnd;
               strsize iCharStart;
               strsize iCharEnd;
               GetSelection().GetNormalSelection(iLineStart, iCharStart, iLineEnd, iCharEnd);
               if (iLineStart < m_iIndex)
               {
                  iStart = 0;
               }
               else if (iLineStart > m_iIndex)
               {
                  iStart = m_iaPosition.get_count();
               }
               else
               {
                  iStart = iCharStart;
               }
               index iEnd;
               if (iLineEnd < m_iIndex)
               {
                  iEnd = -1;
               }
               else if (iLineEnd > m_iIndex)
               {
                  iEnd = m_iaPosition.get_upper_bound();
               }
               else
               {
                  iEnd = iCharEnd;
               }
               if (iStart < iEnd)
               {
                  class rect rectPlacement;
                  GetPlacement(rectPlacement);
                  class rect rect = rectPlacement;
                  size size1 = pgraphics->GetTextExtent(strFinal.Left(iStart));
                  size size2 = pgraphics->GetTextExtent(strFinal.Left(iEnd + 1));
                  rect.left = rectPlacement.left + size1.cx;
                  rect.right = rectPlacement.left + size2.cx;
                  ::draw2d::dib_sp dib(allocer());
                  if (rect.area() > 0)
                  {
                     dib->create(rect.size());
                     dib->Fill(255, 255, 255, 255);
                     dib->get_graphics()->set_alpha_mode(::draw2d::alpha_mode_blend);
                     pgraphics->flush();

                     point pt = pgraphics->GetViewportOrg();
                     dib->from(null_point(), pgraphics, pt + rect.top_left(), rect.size());
                     //dib->get_graphics()->FillSolidRect(0, 0, 16, 16, ARGB(255, 255, 0, 255));
                     dib->Invert();
                     //dib->fill_channel(0, ::visual::rgba::channel_blue);
                     dib->fill_channel(255, ::visual::rgba::channel_alpha);
                     dib->to(pgraphics, rect.top_left(), rect.size(), null_point());
                  }
               }
            }

         }
      }
      break;
   case AnimateRHL:
      {
         int32_t iLeft = m_iaPosition.element_at(0);
         int32_t iLeftDiff = 0;
         int32_t iLastLeftDiff = 0;
         int32_t i;
         for (i = 0; i < m_iaPosition.get_size(); i++)
         {
            iLastLeftDiff = iLeftDiff;
            iLeftDiff = m_iaPosition.element_at(i) - iLeft;
            if ((int32_t)m_dAnimateProgress <= iLeftDiff)
            {
               //i--;
               break;
            }
         }
         ::draw2d::region rgn;
         string strFinal(m_str);
         string strLeft = strFinal.Right(strFinal.get_length() - i);
         int32_t iLeftOffset;
         iLeftOffset = iLastLeftDiff - (int32_t)m_dAnimateProgress;
         ::rect rectTextOut;
         GetPlacement(rectTextOut);
         rectTextOut.left += iLeftOffset;
         if (bDraw)
         {
            EmbossedTextOut(pgraphics, strLeft, rectTextOut.left, rectTextOut.top,
                            0,
                            m_cr,
                            m_crOutline,
                            strFinal.get_length(),
                            dBlend);
         }
         int32_t iMaxCounter = MAX((int32_t)m_iaPosition.element_at(m_str.get_length()) - m_iaPosition.element_at(0) + 100, m_rect.right - m_rect.left);
         int32_t iRight = iMaxCounter - (int32_t)m_dAnimateProgress;
         if (iRight < m_rect.right)
         {
            int32_t iRightEnd;
            int32_t i;
            for (i = 0; i < m_iaPosition.get_size(); i++)
            {
               iRightEnd = iRight + m_iaPosition.element_at(i) - iLeft;
               if (iRightEnd >= m_rect.right)
               {
                  break;
               }
            }
            string strRight = strFinal.Left(i);
            rectTextOut.left = iRight;
            if (bDraw)
            {
               EmbossedTextOut(pgraphics, strRight, rectTextOut.left, rectTextOut.top, 0, m_cr, m_crOutline, strFinal.get_length(), dBlend);
            }
         }

         if (&rectaModified != NULL)
         {
            ::rect baserect;
            rgn.get_bounding_box(baserect);
            rectaModified.add(baserect);

         }
      }
      break;
   default:
      ASSERT(FALSE);
   }




   return true;

}

bool xfplayer_view_line::to(::draw2d::graphics * pgraphics, bool bDraw, const RECT & rect, rect_array & rectaModified, ::count * count, bool bRecalcLayout, COLORREF crColor, ::draw2d::pen_sp sppen)
{

   single_lock sl(m_pContainer->m_pmutex);

   UNREFERENCED_PARAMETER(count);

   

   ::rect rectPlacement;

   GetPlacement(rectPlacement);

   pgraphics->set_font(m_font);

   //   pgraphics->SetBkMode(TRANSPARENT);

   point iMargin;
   {
      iMargin.x = (LONG)(sppen->m_dWidth / 2.0);
      iMargin.y = (LONG)(sppen->m_dWidth / 2.0);
   }

   if (!IsVisible())
   {
      class rect rect(m_rectInvalidate);
      if (!is_null(rectaModified))
      {
         class rect baserect(rect);
         rectaModified.add(baserect);
      }

      Validate(rect);

      return true;
   }
   if (bRecalcLayout)
   {
      CalcCharsPositions(pgraphics, rect);
      pgraphics->SelectObject(m_font);
   }


   switch (m_iAnimateType)
   {
   case AnimateNoAnimate:
      {
         string strFinal;
         strFinal = m_str;
         pgraphics->SelectObject(sppen);

         ::draw2d::brush_sp brushText(allocer(), crColor);

         pgraphics->SelectObject(brushText);

         //pgraphics->set_text_color(crColor);

         ::rect rectTextOut;
         GetPlacement(rectTextOut);
         if (bDraw)
         {
            pgraphics->_DrawText(
               strFinal, strFinal.get_length(),
               rectTextOut,
               DT_LEFT | DT_BOTTOM);
         }
      }
      break;
   case AnimateRHL:
      {
         if (m_iaPosition.get_size() <= 0)
            break;
         //         int32_t iLeft = m_iaPosition.element_at(0);
         int32_t iLeftDiff = 0;
         //int32_t iLastLeftDiff = 0;
         int32_t i = 0;
         /*            for(int32_t i = 0; i < m_iaPosition.get_size(); i++)
                     {
                         iLastLeftDiff = iLeftDiff;
                         iLeftDiff = m_iaPosition.element_at(i) - iLeft;
                         if(iLeftDiff >= (int32_t) m_dAnimateProgress)
                         {
                             break;
                         }
                     }*/

         string strFinal(m_str);
         string strLeft = strFinal.Right(strFinal.get_length() - i);
         int32_t iLeftOffset;
         iLeftOffset = iLeftDiff - (int32_t)m_dAnimateProgress;

         pgraphics->SelectObject(sppen);
         ::draw2d::brush_sp brushText(allocer(), crColor);

         pgraphics->SelectObject(brushText);

         pgraphics->SelectObject(m_font);
         ::rect rectTextOut;
         GetPlacement(rectTextOut);
         rectTextOut.left += iLeftOffset;
         if (bDraw)
         {
            pgraphics->_DrawText(
               strLeft, strLeft.get_length(),
               rectTextOut,
               DT_LEFT | DT_BOTTOM);
         }
         /*           pFont->TextOutEx(
                         pdcForeground,
                       rectTextOut,
                     1.0,
                     rectTextOut.height(),
                       strLeft,
                       m_iaPosition.get_data(),
                         m_iaPosition.get_size(),
                       0,
                         MapToFontEffect(m_iTextEffect));   */
         int32_t iSpacing = 100;
         int32_t iMaxCounter = MAX(
                                  (int32_t)m_iaPosition.element_at(m_str.get_length())
                                  - m_iaPosition.element_at(0) + iSpacing, m_rect.right - m_rect.left);
         int32_t iRight = iMaxCounter - (int32_t)m_dAnimateProgress;
         if (iRight < m_rect.right)
         {
            /*    int32_t iRightEnd;
                for(int32_t i = 0; i < m_iaPosition.get_size(); i++)
                {
                    iRightEnd = iRight + m_iaPosition.element_at(i) - iLeft;
                    if(iRightEnd >= m_rect.right)
                    {
                        break;
                    }
                }*/
            //string strRight = strFinal.Left(i);
            string strRight = strFinal;
            rectTextOut.left = iRight;
            if (bDraw)
            {

               pgraphics->_DrawText(
                  strRight, strRight.get_length(),
                  rectTextOut,
                  DT_LEFT | DT_BOTTOM);
            }
            /*               pFont->TextOutEx(
                                pdcForeground,
                              rectTextOut,
                           1.0,
                           rectTextOut.height(),
                              strRight,
                              m_iaPosition.get_data(),
                                m_iaPosition.get_size(),
                              0,
                                MapToFontEffect(m_iTextEffect));   */
         }

      }
      break;
   default:
      ASSERT(FALSE);
   }

   return true;

}



/*void xfplayer_view_line::CalcCharsPositions(
::draw2d::graphics * pdcForeground,
   ref_array <  visual::font > * pFonts,
   const RECT & rect)
{
   m_bCacheEmboss = false;
   //visual::font * pFont;
//    ::draw2d::graphics * pdcForeground = m_ptwi->TwiGetDC();
   if(m_str.get_length() <= 0)
      return;
   if(pFonts->get_size() > 0)
   {
      int32_t i, nFont, iFontFound, iMaxExtent, iLeft;
      size size;
      rect clientRect;
      nFont = 0;
      iFontFound = -1;
      for(nFont = 0; nFont < pFonts->get_size(); nFont++)
      {
         pdcForeground->SelectObject(pFonts->get_at(nFont)->GetFont());
         GetTextExtentPoint32W(
            pdcForeground->get_os_data(),
            m_str,
            m_str.get_length(),
            &size);
         if((m_iAlign & AlignLeft) > 0)
         {
            if(size.cx + m_rect.left <= lpcrect->right)
            {
               iFontFound = nFont;
               break;
            }
         }
         else if((m_iAlign & AlignRight) > 0)
         {
            if(m_rect.right - size.cx >= 0)
            {
               iFontFound = nFont;
               break;
            }
         }
      }
      if(iFontFound < 0)
      {
         m_nFont = pFonts->get_size() - 1;
      }
      else
      {
         m_nFont = iFontFound;
      }
      pdcForeground->SelectObject(pFonts->get_at(m_nFont)->GetFont());
      if(m_iAlign == AlignLeft)
      {
         m_iaPosition[0] = m_rect.left + m_iIndent;
         for(i = 1; i <= m_str.get_length(); i++)
         {
            GetTextExtentPoint32W(
               pdcForeground->get_os_data(),
               m_str,
               i,
               &size);
            m_iaPosition[i] = size.cx + m_rect.left  + m_iIndent;
         }
         if(m_bAutoSizeX)
         {
            m_rect.right = m_rect.left  + m_iIndent + size.cx;
            }
         if(m_bAutoSizeY)
         {
            m_rect.bottom = m_rect.top + size.cy;
         }
      }
      else if(m_iAlign == AlignRight)
      {
         GetTextExtentPoint32W(
            pdcForeground->get_os_data(),
            m_str,
            m_str.get_length(),
            &size);
         iMaxExtent = size.cx;
         iLeft = m_rect.right - iMaxExtent;
         m_iaPosition[0] = iLeft;
         for(i = 1; i <= m_str.get_length(); i++)
         {
            GetTextExtentPoint32W(
               pdcForeground->get_os_data(),
               m_str,
               i,
               &size);
            m_iaPosition[i] = size.cx + iLeft;
         }
         if(m_bAutoSizeX)
         {
            m_rect.left = m_rect.right - size.cx;
            }
            if(m_bAutoSizeY)
            {
            m_rect.bottom = m_rect.top + size.cy;
         }
      }

   }

   else
   {
      int32_t i; //, width;
      size size;
         m_iaPosition[0] = m_rect.left;
         for(i = 1; i <= m_str.get_length(); i++)
         {
            GetTextExtentPoint32W(
               pdcForeground->get_os_data(),
               m_str,
               i ,
               &size);
            m_iaPosition[i] = m_rect.left + size.cx;
         }
      if(m_bAutoSizeX)
      {
         m_rect.right = m_rect.left + size.cx;
        }
      if(m_bAutoSizeY)
      {
         m_rect.bottom = m_rect.top + size.cy;
      }
   }
//    m_ptwi->TwiReleaseDC(pdcForeground);

}*/

void xfplayer_view_line::CalcCharsPositions(::draw2d::graphics *             pgraphics, const RECT & rect)
{

   single_lock sl(m_pContainer->m_pmutex);

   m_bCacheEmboss = false;
   if (m_str.get_length() <= 0)
      return;

   //   ::draw2d::font * pfontOld = pgraphics->get_current_font();

   int32_t i;
   size size;
   ::rect rectClient(rect);
   m_rectClient = rectClient;
   ::rect rectPlacement;
   GetPlacement(rectPlacement);
   string strMain = m_str;
   pgraphics->SelectObject(m_font);
   size = pgraphics->GetTextExtent(strMain);
   if (size.cx > rectClient.width())
   {
      m_floatRateX =
         (float)
         rectClient.width() /
         size.cx;
   }
   else
   {
      m_floatRateX = 1.0;
   }

   //   pgraphics->SelectObject(fontOriginal);
   ::draw2d::text_metric tm;
   pgraphics->get_text_metrics(&tm);
   // lf.lfWidth = (long) (tm.tmAveCharWidth * m_floatRateX - 1);

   // if(m_font->get_os_data() != NULL)
//      m_font->delete_object();

   ///m_font->CreateFontIndirect(&lf);

   m_font.m_p->m_dFontWidth = m_floatRateX;
   m_font.m_p->m_bUpdated = false;


   if (m_straLink.get_size() > 0)
   {
      *m_fontLink.m_p = *m_font.m_p;
      m_fontLink.m_p->set_underline();
   }


   if (m_bColonPrefix)
   {

      m_fontPrefix.alloc(allocer());

      *m_fontPrefix = *m_font;

      m_fontPrefix->m_dFontSize *= 0.5;


   }

   m_str.Truncate(MIN(84, m_str.length()));

   if (m_bColonPrefix)
   {

      m_strPrefix = m_str.Left(MAX(0, m_str.find(":")));

      m_strRoot = m_str.Mid(MAX(0, m_str.find(":") + 1));

      m_strRoot.trim_left();

      pgraphics->SelectObject(m_fontPrefix);

      m_iaPosition[0] = 0;
      for (i = 1; i <= m_strPrefix.get_length(); i++)
      {
         m_dcextension.GetTextExtent(
            pgraphics,
            m_strPrefix,
            i,
            size);
         m_iaPosition.add(size.cx);
      }
      int iSize = size.cx;
      m_dcextension.GetTextExtent(
         pgraphics,
         " ",
         1,
         size);
      m_iaPosition.add(iSize + size.cx);
      pgraphics->SelectObject(m_font);
      for (i = 1; i <= m_strRoot.get_length(); i++)
      {
         m_dcextension.GetTextExtent(
            pgraphics,
            m_strRoot,
            i,
            size);
         m_iaPosition.add(iSize + size.cx);
      }
   }
   else
   {

      pgraphics->SelectObject(m_font);

      m_iaPosition[0] = 0;
      for (i = 1; i <= m_str.get_length(); i++)
      {
         m_dcextension.GetTextExtent(
            pgraphics,
            m_str,
            i,
            size);
         m_iaPosition[i] = size.cx;
      }

   }

   /*if(m_iAlign == AlignLeft)
   {
      m_iaPosition[0] = m_rect.left + m_iIndent;
      iRight = 0;
      for(i = 1; i <= m_str.get_length(); i++)
      {
         GetTextExtentPoint32W(
            (HDC)pgraphics->get_os_data(),
            m_str,
            i,
            &size);
         m_iaPosition[i] = iRight = size.cx + m_rect.left + m_iIndent;
      }
      if(m_bAutoSizeX)
      {
         m_rect.right = iRight;
        }
      if(m_bAutoSizeY)
      {
         m_rect.bottom = m_rect.top + size.cy;
      }
   }
   else if(m_iAlign == AlignRight)
   {
      GetTextExtentPoint32W(
         (HDC)pgraphics->get_os_data(),
         m_str,
         m_str.get_length(),
         &size);
      iMaxExtent = size.cx;
      iLeft = m_rect.right - iMaxExtent;
      m_iaPosition[0] = iLeft;
      for(i = 1; i <= m_str.get_length(); i++)
      {
         GetTextExtentPoint32W(
            (HDC)pgraphics->get_os_data(),
            m_str,
            i,
            &size);
         m_iaPosition[i] = size.cx + iLeft;
      }
      if(m_bAutoSizeX)
      {
         m_rect.left = iLeft;
        }
      if(m_bAutoSizeY)
      {
         m_rect.bottom = m_rect.top + size.cy;
      }
   }
   pgraphics->SelectObject(pfontOld);*/
}

/*void xfplayer_view_line::CalcCharsPositions(
   ::draw2d::graphics *             pdcForeground,
   visual::font *     pFont,
   LPCRECT           lpcrect)
{
   m_bCacheEmboss = false;
   if(m_str.get_length() <= 0)
      return;
    ::draw2d::font * pfontOriginal = pdcForeground->get_current_font();
   pdcForeground->SelectObject(pFont->GetFont());
   int32_t i, iLeft, iRight, iMaxExtent;
   size size;
   rect rectClient(lpcrect);
   m_rectClient = rectClient;
   visual::font * pfont = pFont;
   ::draw2d::graphics * pgraphics = pdcForeground;
   ASSERT(pfont != NULL);
   rect rectPlacement;
   GetPlacement(rectPlacement);
   string strMain = m_str;
   GetTextExtentPoint32W(
      (HDC)pgraphics->get_os_data(),
      strMain,
      strMain.get_length(),
      &size);
   pgraphics->SelectObject(pfont->GetFont());
   if(size.cx > rectClient.width())
   {
      m_floatRateX =
         (float)
         rectClient.width()/
         size.cx;
   }
   else
   {
      m_floatRateX = 1.0;
   }

   m_font->delete_object();
   LOGFONTW lf;
   pfont->GetFont()->GetLogFont(&lf);
   pgraphics->SelectObject(pfont->GetFont());
   TEXTMETRICW tm;
   pgraphics->GetTextMetrics(&tm);
   lf.lfWidth = (long) (tm.tmAveCharWidth * m_floatRateX - 1);
   m_font->CreateFontIndirect(&lf);





   pgraphics->SelectObject(m_font);
   if(m_iAlign == AlignLeft)
   {
      m_iaPosition[0] = m_rect.left + m_iIndent;
      iRight = 0;
      for(i = 1; i <= m_str.get_length(); i++)
      {
         GetTextExtentPoint32W(
            pdcForeground->get_os_data(),
            m_str,
            i,
            &size);
         m_iaPosition[i] = iRight = size.cx + m_rect.left + m_iIndent;
      }
      if(m_bAutoSizeX)
      {
         m_rect.right = iRight;
        }
      if(m_bAutoSizeY)
      {
         m_rect.bottom = m_rect.top + size.cy;
      }
   }
   else if(m_iAlign == AlignRight)
   {
      GetTextExtentPoint32W(
         pdcForeground->get_os_data(),
         m_str,
         m_str.get_length(),
         &size);
      iMaxExtent = size.cx;
      iLeft = m_rect.right - iMaxExtent;
      m_iaPosition[0] = iLeft;
      for(i = 1; i <= m_str.get_length(); i++)
      {
         GetTextExtentPoint32W(
            pdcForeground->get_os_data(),
            m_str,
            i,
            &size);
         m_iaPosition[i] = size.cx + iLeft;
      }
      if(m_bAutoSizeX)
      {
         m_rect.left = iLeft;
        }
      if(m_bAutoSizeY)
      {
         m_rect.bottom = m_rect.top + size.cy;
      }
   }
   pdcForeground->SelectObject(pfontOriginal);
}*/

void xfplayer_view_line::SetAutoSize(bool bAutoSize)
{

   single_lock sl(m_pContainer->m_pmutex);

   m_bAutoSizeX = bAutoSize;
   m_bAutoSizeY = bAutoSize;
}

void xfplayer_view_line::SetAlign(int32_t iAlign)
{

   single_lock sl(m_pContainer->m_pmutex);

   m_iAlign = iAlign;
}

xfplayer_view_line & xfplayer_view_line::operator = (const xfplayer_view_line & src)
{
   m_pContainer = src.m_pContainer;
   m_bAutoSizeX = src.m_bAutoSizeX;
   m_bAutoSizeY = src.m_bAutoSizeY;
   m_str = src.m_str;
   m_iaPosition = src.m_iaPosition;
   m_iAlign = src.m_iAlign;
   m_rect.bottom = src.m_rect.bottom;
   m_rect.left = src.m_rect.left;
   m_rect.right = src.m_rect.right;
   m_rect.top = src.m_rect.top;
   m_lpBitmapData = src.m_lpBitmapData;
   m_nFont = src.m_nFont;
   m_iAnimateType = src.m_iAnimateType;
   m_iTextEffect = src.m_iTextEffect;
   m_dAnimateProgress = src.m_dAnimateProgress;
   m_dAnimateProgressIncrement = src.m_dAnimateProgressIncrement;
   m_logfont = src.m_logfont;
   m_font = src.m_font;
   m_bEnhancedEmboss = src.m_bEnhancedEmboss;
   m_bCacheEmboss = false;
   m_dXfplayerViewLineBlend = src.m_dXfplayerViewLineBlend;
   m_iIndex = src.m_iIndex;
   return *this;
}

void xfplayer_view_line::Show(bool bShow)
{

   single_lock sl(m_pContainer->m_pmutex);

   if (bShow && !m_bVisible)
   {
      m_bVisible = true;
      Invalidate();
      if (m_pContainer != NULL)
      {
         m_pContainer->OnChildSetVisible(this, m_bVisible);
      }

   }
   else if (!bShow && m_bVisible)
   {
      m_bVisible = false;
      Invalidate();
      if (m_pContainer != NULL)
      {
         m_pContainer->OnChildSetVisible(this, m_bVisible);
      }
   }
}


void xfplayer_view_line::OnTimerAnimate(::draw2d::graphics * pdcForeground, rect_array &  rectaModified)
{

   single_lock sl(m_pContainer->m_pmutex);

   UNREFERENCED_PARAMETER(pdcForeground);
   if (IsVisible())
   {
      switch (m_iAnimateType)
      {
      case AnimateNoAnimate:
         break;
      case AnimateRHL:
         {
            m_dAnimateProgress += m_dAnimateProgressIncrement;
            if (m_iaPosition.get_size() > 0)
            {
               if ((int32_t)m_dAnimateProgress > MAX(m_iaPosition.element_at(m_str.get_length()) - m_iaPosition.element_at(0) + 100, m_rect.right - m_rect.left))
                  m_dAnimateProgress = 0.0;
               rect rect;
               GetPlacement(rect);
               Invalidate();
               rectaModified.add(rect);
               /*to(
                   pdcForeground,
                   false,
                   rect, rectaModified,
                   false);*/
            }
         }
         break;
      default:
         ASSERT(FALSE);
      }
   }
}

void xfplayer_view_line::SetAnimateType(int32_t iAnimateType)
{

   single_lock sl(m_pContainer->m_pmutex);

   m_iAnimateType = iAnimateType;
   m_dAnimateProgress = 0.0;

}

void xfplayer_view_line::SetTextEffect(int32_t iTextEffect)
{

   single_lock sl(m_pContainer->m_pmutex);

   m_iTextEffect = iTextEffect;
}

void xfplayer_view_line::SetEmbossPen(::draw2d::pen *lpPen)
{

   single_lock sl(m_pContainer->m_pmutex);

   m_lpPenEmboss = lpPen;

}

void xfplayer_view_line::SetForegroundColor(COLORREF cr)
{

   single_lock sl(m_pContainer->m_pmutex);

   m_crForeground = cr;
}

/*ref_array <  visual::font > * xfplayer_view_line::GetFonts()
{
    return &m_fonts;
}
*/
int32_t xfplayer_view_line::MapToFontEffect(int32_t iLineEffect)
{

   single_lock sl(m_pContainer->m_pmutex);

   switch (iLineEffect)
   {
   case EffectSimple:
      return visual::font::EffectSimple;
   case EffectEmbossed:
      return visual::font::EffectEmbossed;
   }
   ASSERT(FALSE);
   return -1;
}

void xfplayer_view_line::SetAnimateIncrement(double dIncrement)
{

   single_lock sl(m_pContainer->m_pmutex);

   m_dAnimateProgressIncrement = dIncrement;
}

//void xfplayer_view_line::SetRedrawMode(int32_t iMode)
//{
//  m_iRedrawMode = iMode;
//}

//void xfplayer_view_line::SetRenderWindow(::window_sp pwindow)
//{
//  m_pRenderWnd = pwindow;
//}

void xfplayer_view_line::SetRenderCriticalSection(critical_section * pcs)
{
   UNREFERENCED_PARAMETER(pcs);
   //    m_pcsRender =   pcs;
}

int32_t xfplayer_view_line::SetLyricPens(::draw2d::pen * ppenLeft, ::draw2d::pen * ppenRight)
{

   single_lock sl(m_pContainer->m_pmutex);

   m_ppenLyricLeft = ppenLeft;
   m_ppenLyricRight = ppenRight;
   return true;
}

int32_t xfplayer_view_line::SetLyricColors(COLORREF crLeft, COLORREF crRight)
{

   single_lock sl(m_pContainer->m_pmutex);

   m_crLyricLeft = crLeft;
   m_crLyricRight = crRight;
   return true;

}


void xfplayer_view_line::SetPlacement(const RECT & rect)
{

   single_lock sl(m_pContainer->m_pmutex);

   m_rect = rect;

   m_bPendingLayoutUpdate = true;

}


//int32_t xfplayer_view_line::GetVmsFontCount()
/*{
   return m_fonts.get_size();
}

void xfplayer_view_line::AddVmsFont(visual::font * pfont)
{
   m_fonts.add(pfont);
}*/

void xfplayer_view_line::Invalidate(const RECT & rectParam)
{

   single_lock sl(m_pContainer->m_pmutex);

   rect rectPlacement;

   GetPlacement(rectPlacement);

   rect rectInvalidate;

   if (IsRectEmpty(&rectParam))
   {

      rectInvalidate = rectPlacement;

   }
   else
   {

      rectInvalidate = rectParam;

   }

   m_rectInvalidate.unite(m_rectInvalidate, rectInvalidate);

   m_rectInvalidate.intersect(m_rectInvalidate, rectPlacement);

}


void xfplayer_view_line::Validate(const RECT & rectParam)
{

   single_lock sl(m_pContainer->m_pmutex);

   rect rectPlacement;

   GetPlacement(rectPlacement);

   rect rectValidate;

   if (IsRectEmpty(&rectParam))
   {

      rectValidate = rectPlacement;

   }
   else
   {

      rectValidate = rectParam;

   }

   m_rectInvalidate -= rectValidate;

   m_rectInvalidate.intersect(m_rectInvalidate, rectPlacement);

}


bool xfplayer_view_line::IsVisible()
{

   single_lock sl(m_pContainer->m_pmutex);

   return m_bVisible;

}

void xfplayer_view_line::EmbossedTextOut(::draw2d::graphics * pgraphics, const char * lpcsz, int32_t iLeft, int32_t iTop, int32_t iWidth, COLORREF cr, COLORREF crOutline, strsize iLen, double dBlend)
{

   single_lock sl(m_pContainer->m_pmutex);

   EmbossedTextOut(
      pgraphics,
      m_dibMain,
      lpcsz,
      iLeft,
      iTop,
      iWidth,
      cr,
      crOutline,
      iLen,
      dBlend);

   /*   rect rect;
      size size;
      size sizeInt;
      size sizeExt;
      pgraphics->SelectObject(m_font);
      int32_t cx = 0;
      int32_t width;
      int32_t widthInt;
      int32_t widthExt;
      int32_t cxInt;
      int32_t cxExt;
      int32_t cyInt;
      int32_t cyExt;
      ::GetTextExtentPoint32W(
         (HDC)pgraphics->get_os_data(),
         lpcsz,
         iLen,
         &size);
      pgraphics->SelectObject(m_fontInt);
      ::GetTextExtentPoint32W(
         (HDC)pgraphics->get_os_data(),
         lpcsz,
         iLen,
         &sizeInt);
      //cyInt = (size.cy - sizeInt.cy) / 2;
      cyInt = (size.cy * 1000 / 1200) / 2;

      pgraphics->SelectObject(m_fontExt);
      ::GetTextExtentPoint32W(
         (HDC)pgraphics->get_os_data(),
         lpcsz,
         iLen,
         &sizeExt);
      //cyExt = (size.cy - sizeExt.cy) / 2;
      cyExt = -(size.cy * 1200 / 1000) / 2;

      //XFORM xform;

      //pgraphics->GetWorldTransform(&xform);

      ::SetTextColor((HDC)pgraphics->get_os_data(), crOutline);
      for(int32_t i = 0; i < iLen; i++)
      {
         WCHAR wch = lpcsz[i];


         pgraphics->SelectObject(m_font);
         ::GetTextExtentPoint32W(
            (HDC)pgraphics->get_os_data(),
            lpcsz,
            i + 1,
            &size);

         pgraphics->SetMapMode( MM_ANISOTROPIC);
         cxInt = (size.cx * 1000 / 1200) / 2;
         ::ScaleViewportExtEx(
            (HDC)pgraphics->get_os_data(),
            1000,
            1200,
            1000,
            1200,
            NULL);
         ::OffsetViewportOrgEx(
            (HDC)pgraphics->get_os_data(),
            iLeft + cx,
            iTop * cyInt,
            NULL);
         ::TextOutW((HDC)pgraphics->get_os_data(), 0, 0, &wch, 1);

         cxInt = -(size.cx * 1000 / 1200) / 2;
         ::ScaleViewportExtEx(
            (HDC)pgraphics->get_os_data(),
            1000,
            1200,
            1000,
            1200,
            NULL);
         ::OffsetViewportOrgEx(
            (HDC)pgraphics->get_os_data(),
            iLeft + cx,
            iTop  + cyExt,
            NULL);
         ::TextOutW((HDC)pgraphics->get_os_data(), 0, 0, &wch, 1);

         //pgraphics->SelectObject(m_fontInt);
         ::GetTextExtentPoint32W(
            (HDC)pgraphics->get_os_data(),
            &wch,
            1,
            &sizeInt);
         ::TextOutW((HDC)pgraphics->get_os_data(), 0, 0, &wch, 1);

         //pgraphics->SelectObject(m_fontExt);
         ::GetTextExtentPoint32W(
            (HDC)pgraphics->get_os_data(),
            &wch,
            1,
            &sizeExt);
         cxExt = (size.cx * 1200 / 1000);
         cyExt = (size.cy - sizeExt.cy) / 2;
         ::TextOutW((HDC)pgraphics->get_os_data(), iLeft + cx + cxExt, iTop + cyExt, &wch, 1);



         cx = size.cx;
      }
      pgraphics->SetMapMode(MM_TEXT);
      ::SetTextColor((HDC)pgraphics->get_os_data(), cr);
      pgraphics->SelectObject(m_font);
      ::TextOutW((HDC)pgraphics->get_os_data(), iLeft, iTop, lpcsz, iLen);*/


   /*   ::SetTextColor((HDC)pgraphics->get_os_data(), crOutline);
      ::TextOutW((HDC)pgraphics->get_os_data(), iLeft - iWidth, iTop - iWidth, lpcsz, iLen);
      ::TextOutW((HDC)pgraphics->get_os_data(), iLeft - iWidth, iTop, lpcsz, iLen);
      ::TextOutW((HDC)pgraphics->get_os_data(), iLeft - iWidth, iTop + iWidth, lpcsz, iLen);
      ::TextOutW((HDC)pgraphics->get_os_data(), iLeft, iTop - iWidth, lpcsz, iLen);
      ::TextOutW((HDC)pgraphics->get_os_data(), iLeft + iWidth, iTop, lpcsz, iLen);
      ::TextOutW((HDC)pgraphics->get_os_data(), iLeft + iWidth, iTop - iWidth, lpcsz, iLen);
      ::TextOutW((HDC)pgraphics->get_os_data(), iLeft, iTop + iWidth, lpcsz, iLen);
      ::TextOutW((HDC)pgraphics->get_os_data(), iLeft + iWidth, iTop + iWidth, lpcsz, iLen);

      ::SetTextColor((HDC)pgraphics->get_os_data(), cr);
      ::TextOutW((HDC)pgraphics->get_os_data(), iLeft, iTop, lpcsz, iLen);

      return;

      size size;
      ::GetTextExtentPoint32W(
         (HDC)pgraphics->get_os_data(),
         lpcsz,
         iLen,
         &size);

      if(m_dc1.get_os_data() == NULL)
      {
         m_dc1.CreateCompatibleDC(pgraphics);

      }
      m_dc1.SelectObject(pgraphics->get_current_font());

      if(m_bmp1.get_os_data() == NULL)
      {
         m_bmp1.CreateCompatibleBitmap(pgraphics, size.cx, size.cy);
         m_dc1.SelectObject(m_bmp1);
      }
      else
      {
         BITMAP bm;
         m_bmp1.GetBitmap(&bm);
         if(bm.bmHeight < size.cx ||
            bm.bmWidth < size.cy)
         {
            m_bmp1.delete_object();
            m_bmp1.CreateCompatibleBitmap(pgraphics, size.cx, size.cy);
         }
         m_dc1.SelectObject(m_bmp1);
      }


      m_dc1.FillSolidRect(0, 0, size.cx,size.cy, RGB(0, 0, 0));
      m_dc1.SetTextColor(RGB(255, 255, 255));
      ::TextOutW(m_dc1.get_os_data(), 0, 0, lpcsz, iLen);

      System.visual().imaging().channel_gray_blur(&m_dc1,0, 0, size.cx, size.cy,
         &m_dc1, 0, 0, 0, 2);

      System.visual().imaging().clip_color_blend(pgraphics, iLeft, iTop, size.cx, size.cy,
         crOutline, &m_dc1, 0, 0);


      pgraphics->BeginPath();
      ::TextOutW((HDC)pgraphics->get_os_data(), iLeft, iTop, lpcsz, iLen);
      pgraphics->EndPath();

      LOGBRUSH lb;
      lb.lbStyle = BS_SOLID;
      lb.lbColor = crOutline;
      //::draw2d::pen_sp pen(get_app(), PS_SOLID, iWidth * 2 + 2, crOutline);
      ::draw2d::pen_sp pen(get_app(),
         PS_SOLID
         | PS_GEOMETRIC
         | PS_ENDCAP_ROUND
         | PS_JOIN_ROUND,
         iWidth * 2,
         &lb);

      ::draw2d::pen * ppenOld = pgraphics->SelectObject(pen);
      pgraphics->StrokePath();
      pgraphics->SelectObject(ppenOld);*/

   //   ::SetTextColor((HDC)pgraphics->get_os_data(), cr);
   //m_dc1.SetBkMode(TRANSPARENT);
   // ::TextOutW((HDC)pgraphics->get_os_data(), iLeft, iTop, lpcsz, iLen);


}

void xfplayer_view_line::EmbossedTextOut(::draw2d::graphics * pgraphics, ::draw2d::dib * pdibCache, const char * lpcsz, int32_t iLeft, int32_t iTop, int32_t iWidth, COLORREF cr, COLORREF crOutline, strsize iLen, double dBlend)
{

   single_lock sl(m_pContainer->m_pmutex);

   UNREFERENCED_PARAMETER(pdibCache);

   bool bSaveProcessing = !m_bEnhancedEmboss;


   if (bSaveProcessing)
   {
      pgraphics->BeginPath();
      pgraphics->text_out(iLeft, iTop, string(lpcsz, iLen));
      pgraphics->EndPath();

      //      LOGBRUSH lb;
      //    lb.lbStyle = BS_SOLID;
      //  lb.lbColor = crOutline;
      ::draw2d::pen_sp pen;
      /*      pen->construct(
               PS_SOLID
               | PS_GEOMETRIC
               | PS_ENDCAP_ROUND
               | PS_JOIN_ROUND,
               iWidth * 2,
               &lb);*/
      pen->create_solid(iWidth * 2, crOutline);


      ::draw2d::pen * ppenOld = pgraphics->SelectObject(pen);
      pgraphics->StrokePath();
      pgraphics->SelectObject(ppenOld);


      ::draw2d::brush_sp brushText(allocer(), cr);

      pgraphics->SelectObject(brushText);

      pgraphics->text_out(iLeft, iTop, string(lpcsz, iLen));
   }
   else
   {

      if (m_bEnhancedEmboss)
      {

         if (!m_bCacheEmboss || m_strCache != lpcsz)
         {

            CacheEmboss(pgraphics, m_str, m_str.get_length(), m_dibMain);

         }

      }

      pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);

      point pt;

      pt.x = (LONG) (iLeft - 1 - (m_font->m_dFontSize > 8 ? 32 / m_font->m_dFontSize : 0));

      pt.y = (LONG) (iTop - 1 - (m_font->m_dFontSize > 8 ? 32 / m_font->m_dFontSize : 0));

      System.visual().imaging().color_blend(pgraphics, pt, m_dibMain->m_size, m_dibMain->get_graphics(), point(iLeft, 0), dBlend);

      if (m_bColonPrefix)
      {

         pgraphics->SelectFont(m_fontPrefix);

         ::size size;

         size = pgraphics->GetTextExtent(m_strPrefix);

         System.visual().imaging().AlphaTextOut(pgraphics, iLeft, iTop + m_rect.height() - size.cy, m_strPrefix, (int32_t)m_strPrefix.get_length(), cr, dBlend);

         pgraphics->SelectFont(m_font);

         int iOffset;

         if (m_strPrefix.get_length() <= 0)
         {

            iOffset = 0;

         }
         else
         {

            iOffset = (int) (size.cx + size.cx / m_strPrefix.get_length());

         }

         System.visual().imaging().AlphaTextOut(pgraphics, iLeft + iOffset, iTop, m_strRoot, (int32_t)m_strRoot.get_length(), cr, dBlend);



      }
      else
      {

         pgraphics->SelectFont(m_font);

         System.visual().imaging().AlphaTextOut(pgraphics, iLeft, iTop, lpcsz, (int32_t)iLen, cr, dBlend);

      }


   }


}



void xfplayer_view_line::SetColors(COLORREF cr, COLORREF crOutline)
{

   single_lock sl(m_pContainer->m_pmutex);

   m_cr = cr;
   m_crOutline = crOutline;
}




void xfplayer_view_line::GetLogFont(LOGFONTW &lf)
{

   single_lock sl(m_pContainer->m_pmutex);

   lf = m_logfont;
}


void xfplayer_view_line::CacheEmboss(::draw2d::graphics * pgraphics, const char * lpcsz, strsize iLen, ::draw2d::dib * pdibCache)
{

   single_lock sl(m_pContainer->m_pmutex);

   if (!m_bEnhancedEmboss)
      return;

   if (m_bCacheEmboss && m_strCache == lpcsz)
      return;

   m_strCache = lpcsz;
   m_bCacheEmboss = true;

   //TRACE("CLyricViewLine::CacheEmboss: %s\n", lpcsz);
   size size;
   pgraphics->select_font(m_font);
   m_dcextension.GetTextExtent(pgraphics, lpcsz, iLen, size);

   size.cx += (LONG)(2 * (MAX(2.0, m_floatRateX * 8.0)));
   size.cy += (LONG)(2 * (MAX(2.0, m_floatRateX * 8.0)));


   if (!pdibCache->create(size))
      return;

   pdibCache->Fill(0, 0, 0, 0);
   ::draw2d::graphics * pdcCache = pdibCache->get_graphics();
   pdcCache->SelectObject(m_font);

   pdcCache->set_alpha_mode(::draw2d::alpha_mode_set);
   pdcCache->set_alpha_mode(::draw2d::alpha_mode_blend);
   draw2d::brush_sp brushText(allocer());
   brushText->create_solid(ARGB(84, 84, 84, 84));
   pdcCache->SelectObject(brushText);
   //pdcCache->SetTextColor();
   ::size s;
   if (m_bColonPrefix)
   {

      pdcCache->select_font(m_fontPrefix);
      ::size size = pdcCache->GetTextExtent(m_strPrefix);
      m_dcextension.text_out(pdcCache, (int32_t)(int32_t)((MAX(2.0, m_floatRateX * 4.0)) / 2), (int32_t)1 * (int32_t)((MAX(2.0, m_floatRateX * 4.0)) / 2) + m_rect.height() - size.cy, m_strPrefix, m_strPrefix.get_length(), s);
      pdcCache->select_font(m_font);

      int x = (int32_t) (s.cx + (s.cx / m_strPrefix.get_length()) + (int32_t)(int32_t)((MAX(2.0, m_floatRateX * 8.0)) / 2));

      int y = (int32_t) (1 * (int32_t)((MAX(2.0, m_floatRateX * 8.0)) / 2));
      
      m_dcextension.text_out(pdcCache, x, y, m_strRoot, m_strRoot.get_length(), s);

   }
   else
   {

      m_dcextension.text_out(pdcCache, (int32_t)(int32_t)((MAX(2.0, m_floatRateX * 8.0)) / 2), (int32_t)1 * (int32_t)((MAX(2.0, m_floatRateX * 8.0)) / 2), lpcsz, iLen, s);

   }


   System.visual().imaging().channel_spread_set_color(pdcCache, null_point(), size, pdcCache, null_point(), 0, int32_t(MAX(1.0, m_floatRateX * 2.0 + 2)), ARGB(23, 23, 23, 23));

   pdcCache->set_alpha_mode(::draw2d::alpha_mode_blend);
   System.visual().imaging().channel_alpha_gray_blur(pdcCache, null_point(), size, pdcCache, null_point(), 0, int32_t(MAX(1.0, m_floatRateX * 2.5)));
   System.visual().imaging().channel_alpha_gray_blur(pdcCache, null_point(), size, pdcCache, null_point(), 0, int32_t(MAX(1.0, m_floatRateX * 2.5)));

   /*pdibCache->fill_channel(92, ::visual::rgba::channel_blue);
   pdibCache->fill_channel(92, ::visual::rgba::channel_green);
   pdibCache->fill_channel(92, ::visual::rgba::channel_red);*/

   //System.visual().imaging().pre_color_blend(pdcCache, pdcCache, ARGB(92, 92, 92, 92));

   pdibCache->set(0, 0, 0);

}


void xfplayer_view_line::SetFont(::draw2d::font * pfont)
{

   single_lock sl(m_pContainer->m_pmutex);

   m_font = pfont;

}

void xfplayer_view_line::SetFont(visual::font * pfont)
{

   single_lock sl(m_pContainer->m_pmutex);

   SetFont(pfont->GetFont());

}

void xfplayer_view_line::PrepareURLLinks()
{

   single_lock sl(m_pContainer->m_pmutex);

   strsize iStart = 0;

   strsize iEnd = 0;

   string str;

   while (cregexp_util::find(m_str, "/^|\\s|([;\"()]+))(((((http|https)://))|(www\\.))[0-9a-zA-Z./\\-_?=]+)(([;\"()]+)|\\s|$/", 3, &iStart, &iEnd))
   {
      m_straLink.add(m_str.Mid(iStart, iEnd - iStart));
      m_iaLinkStart.add(iStart);
      m_iaLinkEnd.add(iEnd - 1);
   }


}

bool xfplayer_view_line::CharHasLink(strsize iChar)
{

   single_lock sl(m_pContainer->m_pmutex);

   return GetCharLink(iChar) > -1;
}

bool xfplayer_view_line::GetCharLink(string & str, strsize iChar)
{

   single_lock sl(m_pContainer->m_pmutex);

   index iLink = GetCharLink(iChar);
   if (iLink < 0)
      return false;
   str = m_straLink[iLink];
   return true;
}

::user::e_line_hit xfplayer_view_line::GetLink(string & strUrl, point ptCursor)
{

   single_lock sl(m_pContainer->m_pmutex);

   strsize iChar;
   ::user::e_line_hit etest = hit_test(ptCursor, iChar);
   if (etest == ::user::line_hit_link)
   {
      GetCharLink(strUrl, iChar);
   }
   return etest;
}

index xfplayer_view_line::GetCharLink(strsize iChar)
{

   single_lock sl(m_pContainer->m_pmutex);

   for (index i = 0; i < m_iaLinkStart.get_size(); i++)
   {
      if (iChar >= m_iaLinkStart[i] &&
            iChar <= m_iaLinkEnd[i])
      {
         return i;
      }
   }
   return -1;
}

::user::e_line_hit xfplayer_view_line::hit_test(const POINT &ptCursorParam, strsize &iChar)
{

   single_lock sl(m_pContainer->m_pmutex);

   bool bInside;
   point ptCursor = ptCursorParam;
   rect rectPlacement;
   GetPlacement(rectPlacement);
   bInside = rectPlacement.contains(ptCursor) != 0;
   if (!bInside)
   {
      iChar = -1;
      return ::user::line_hit_none;
   }
   else
   {
      if (CalcChar(ptCursor, iChar))
      {
         if (CharHasLink(iChar))
         {
            return ::user::line_hit_link;
         }
         else
         {
            return ::user::line_hit_normal;
         }
      }
      else
      {
         iChar = -1;
         return ::user::line_hit_none;
      }
   }

}

bool xfplayer_view_line::CalcChar(point pt, strsize &iChar)
{

   single_lock sl(m_pContainer->m_pmutex);

   rect rectPlacement;
   GetPlacement(rectPlacement);
   bool bInside = rectPlacement.contains(pt) != 0;
   if (!bInside)
      return false;
   for (int32_t i = 0; i < m_iaPosition.get_size() - 1; i++)
   {
      if (pt.x >= m_iaPosition[i] &&
            pt.x < m_iaPosition[i + 1])
      {
         iChar = i;
         return true;
      }
   }
   return false;
}


void xfplayer_view_line::OnMouseMove(signal_details * pobj)
{

   single_lock sl(m_pContainer->m_pmutex);

   SCAST_PTR(::message::mouse, pmouse, pobj);
   strsize iChar;
   if (CalcChar(pmouse->m_pt, iChar))
   {
      if (CharHasLink(iChar))
      {
         pmouse->m_ecursor = ::visual::cursor_hand;
      }
   }
   /*
   lyric_view_line_selection & selection = GetSelection();

   if(selection.GetState() == lyric_view_line_selection::StateTracking)
   {

      bool bInside;
      int32_t iToken;
      int32_t iChar;
      rect rectPlacement;

      GetPlacement(rectPlacement);
      bInside = rectPlacement.contains(pt) != 0;

      if(!bInside)
      {
         if(pt.y < rectPlacement.top
            || (pt.y <= rectPlacement.bottom &&
            pt.x < rectPlacement.left))
         {
            selection.SetSelBefore(*this);
            GetWndRender()->Redraw(rectPlacement);
         }
         else
         {
            selection.SetSelAfter(*this);
            GetWndRender()->Redraw(rectPlacement);
         }
         return false;
      }
      else // bInside == true
      {
         uint32_t fwKeys = user; // key flags
         if(m_tokenaMain.get_size() <= 0)
            return false;
         if(CalcChar(pt, iToken, iChar))
         {
            selection.m_item.SetTokenEnd(iToken);
            selection.m_item.SetCharEnd(iChar);
            GetWndRender()->Redraw(rectPlacement);
         }
         else
         {
            if(pt.x < m_tokenaMain.element_at(0).get_position())
            {
               selection.SetSelBefore(*this);
            }
            else
            {
               selection.SetSelAfter(*this);
            }
            GetWndRender()->Redraw(rectPlacement);
         }
         return true;
      }
   }
   else
   {
      if(IsInHover())
      {
         UpdateHover(pt);
      }
      else
      {
         bool bInside;
         rect rectPlacement;

         GetPlacement(rectPlacement);
         bInside = rectPlacement.contains(pt);
         if(bInside)
         {
            UpdateHover(pt);
         }
      }

   }*/

   if (GetSelection().OnMouseMove(*this, (UINT)pmouse->m_nFlags, pmouse->m_pt))
   {

      pmouse->m_bRet = true;

      pmouse->set_lresult(1);

   }

}


void xfplayer_view_line::OnSetCursor(signal_details * pobj)
{

   UNREFERENCED_PARAMETER(pobj);

   //if(IsInHover())
   //{
   //
   //    pmouse->m_ecursor = ::visual::cursor_hand;
   //
   //    return TRUE;
   //
   //}

}


void xfplayer_view_line::OnLButtonDown(signal_details * pobj)
{

   single_lock sl(m_pContainer->m_pmutex);

   SCAST_PTR(::message::mouse, pmouse, pobj);

   if (GetSelection().OnLButtonDown(*this, (UINT)pmouse->m_nFlags, pmouse->m_pt))
   {

      pmouse->m_bRet = true;

      pmouse->set_lresult(1);

   }

}


void xfplayer_view_line::OnLButtonUp(signal_details * pobj)
{

   single_lock sl(m_pContainer->m_pmutex);

   SCAST_PTR(::message::mouse, pmouse, pobj);

   strsize iChar;

   if (CalcChar(pmouse->m_pt, iChar))
   {

      if (CharHasLink(iChar))
      {

         string str;

         str = m_straLink[GetCharLink(iChar)];

         ASSERT(m_oswindow->IsWindow());

         System.open_link(str, "", "");

      }

   }

   if (GetSelection().OnLButtonUp(*this, (UINT)pmouse->m_nFlags, pmouse->m_pt))
   {

      pmouse->m_bRet = true;

      pmouse->set_lresult(1);

   }

}


void xfplayer_view_line::_001OnTimer(::timer * ptimer)
{

   UNREFERENCED_PARAMETER(ptimer);

   //if(GetSelection().OnTimer(*this, user))
   //{
   //
   //    return true;
   //
   //}

}


::draw2d::font * xfplayer_view_line::GetFont()
{

   single_lock sl(m_pContainer->m_pmutex);

   return m_font;

}


void xfplayer_view_line::SetBlend(double d)
{

   single_lock sl(m_pContainer->m_pmutex);

   ASSERT(d >= 0.0);

   ASSERT(d <= 1.0);

   if (d < 0.0 || d > 1.0)
   {

      return;

   }

   m_dXfplayerViewLineBlend = d;

}


void xfplayer_view_line::UpdateHover(point &ptCursor)
{

   single_lock sl(m_pContainer->m_pmutex);

   ::index iLine = m_iIndex;

   strsize iChar;

   ::user::e_line_hit etest = hit_test(ptCursor, iChar);

   if (etest == ::user::line_hit_link)
   {

      if (m_iLinkHoverIndex != GetLinkIndex(iLine, iChar))
      {

         m_iLinkHoverIndex = GetLinkIndex(iLine, iChar);

         rect rect;

         GetPlacement(rect);

         get_interaction()->RedrawWindow();

      }

   }
   else
   {

      if (m_iLinkHoverIndex != -1)
      {

         m_iLinkHoverIndex = -1;

         rect rect;

         GetPlacement(rect);

         get_interaction()->RedrawWindow();

      }

   }

}


bool xfplayer_view_line::IsInHover()
{

   if (m_iLinkHoverIndex >= 0)
   {

      return true;

   }
   else
   {

      return false;

   }

}


index xfplayer_view_line::GetLinkIndex(index iLine, strsize iChar)
{

   single_lock sl(m_pContainer->m_pmutex);

   if (!HasLink())
   {

      return -1;

   }

   if (iLine != m_iIndex)
   {

      return -1;

   }

   return GetCharLink(iChar);

}


bool xfplayer_view_line::HasLink()
{

   single_lock sl(m_pContainer->m_pmutex);

   return m_iaLinkStart.get_count() > 0;

}


::user::interaction * xfplayer_view_line::get_interaction()
{

   return m_pContainer->m_pinteraction;

}


inline XfplayerViewLineSelection & xfplayer_view_line::GetSelection()
{

   single_lock sl(m_pContainer->m_pmutex);

   if (m_pContainer == NULL)
      return m_selection;
   else
      return m_pContainer->GetSelection();
}
