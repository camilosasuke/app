﻿#include "StdAfx.h"
#include "KaraokeBouncingBall.h"

namespace kar
{

   KaraokeView::KaraokeView(::ca::application * papp) :
ca(papp),
   m_lyrictemplatelines(papp),
   m_penLeft(papp),
   m_penRight(papp),
   m_penLeftSmall(papp),
   m_penRightSmall(papp),
   m_penLinkLeft(papp),
   m_penLinkRight(papp),
   m_penHoverLeft(papp),
   m_penHoverRight(papp),
   m_fontLyrics(papp)
{
   _iDebugNote = -1;
   _iDebugToken = -1;
   m_plinesTitle = new XfplayerViewLines(papp);
   m_plinesTitle->set_user_interaction(this);

   m_lpbouncingball = new KaraokeBouncingBall(get_app());
   m_lpbouncingball->m_pviewlines = &m_lyrictemplatelines.GetLines();

   m_bSuperScriptEmphasis = true;

   m_lyrictemplatelines.SetKaraokeView(this);


   m_bBouncingBall = false;
   m_bGradualFilling = false;
   m_bEmbossed = true;
   // m_lpbouncingball    = NULL;
   /*   m_crLeft          = RGB(255, 180, 0);
   m_crRight         = RGB(255, 255, 240);
   //m_crLeftOutline   = RGB(255, 255, 255);
   m_crLeftOutline   = RGB(70, 30, 180);
   m_crRightOutline  = RGB(70, 30, 180);*/

   m_crLeft          = RGB(70, 200, 100);
   m_crLeftOutline   = RGB(70, 80, 200);
   m_crRight         = RGB(255, 255, 240);
   m_crRightOutline  = RGB(70, 80, 200);


   m_bHardWriting = false;

}

KaraokeView::~KaraokeView()
{

}



void KaraokeView::OnLyricEvent(LyricEventV1 *pevent, bool bRepaint)
{
   UNREFERENCED_PARAMETER(bRepaint);

   if(!is_ok_for_rendering())
   {
      // it is ok to return here, this function is stateful
      // by stateful I want to mean that it does not depends on previous calls of the function,
      // only in the current state of variables
      return;
   }



   int iLineIndex;

   ::ca::graphics * pdc = pevent->m_pdc;

   ikar::data & data = GetKaraokeData();
   if(&data == NULL)
      return;
   ikar::static_data & staticdata = data.GetStaticData();
   ikar::dynamic_data & dynamicdata = data.GetDynamicData();
   ikar::karaoke * pinterface = data.GetInterface();

   single_lock slKaraokeData(&data.m_mutex, FALSE);

   if(!slKaraokeData.lock(duration::zero()))
      return;

   int iPlayingIndex = -1;


   visual::font * pfont = GetLyricFont();
   ASSERT(pfont != NULL);


   LyricEventsV2 * pevents = NULL;
   LyricEventsV2 * peventsBB = NULL;
   if(staticdata.m_eventstracksV002.get_size() > 0)
   {
      pevents = (LyricEventsV2 *) staticdata.m_eventstracksV002.operator [](0);
      ASSERT(pevents != NULL);
      ASSERT(pevents->GetClassOrder() == 2);
   }
   if(staticdata.m_eventsTracksForBouncingBall.get_size() > 0)
   {
      peventsBB = (LyricEventsV2 *) staticdata.m_eventsTracksForBouncingBall.operator [](0);
      ASSERT(peventsBB != NULL);
      ASSERT(peventsBB->GetClassOrder() == 2);
   }
   if(pevents == NULL)
      return;

   imedia::position position;
   KaraokeGetPosition(position);
   imedia::time time = pinterface->PositionToTime(position);

   dynamicdata.m_position  = position;
   dynamicdata.m_time      = time;



   int iTokenIndex = pevents->GetPositionToken(position);
   //int iTokenFirstNoteIndex = pevents->GetTokenFirstNote(iTokenIndex);
   int iNoteIndex = pevents->GetPositionNote(position);
   //iNoteIndex = max(iNoteIndex, iTokenFirstNoteIndex);
   //      int iNextTokenIndex = iTokenIndex;




   if(_iDebugNote != iNoteIndex)
   {
      _iDebugNote = iNoteIndex;
      _dDebugOldFillRate = 0.0;
   }
   _iDebugToken = iTokenIndex;

   imedia::position   positionToken;
   imedia::time       timeToken;
   imedia::position   positionNote;
   imedia::time       timeNote;

   if(iTokenIndex >= 0
      && iTokenIndex < pevents->m_tkaTokensPosition.get_size())
   {
      positionToken = pevents->m_tkaTokensPosition[iTokenIndex];
      timeToken = pevents->m_msaTokensPosition[iTokenIndex];
   }
   else
   {
      positionToken = 0;
      timeToken = 0;
   }

   if(iNoteIndex >= 0
      && iNoteIndex < pevents->m_tkaNotesPosition.get_size())
   {
      positionNote = pevents->m_tkaNotesPosition[iNoteIndex];
      timeNote = pevents->m_msaNotesPosition[iNoteIndex];
   }
   else
   {
      positionNote = 0;
      timeNote = 0;
   }


   imedia::time timeSpan;
   if(time > timeToken)
   {
      timeSpan = time - timeToken;
   }
   else
   {
      timeSpan = 0;
   }
   imedia::time msElapsed = timeSpan;
   imedia::time msTotalLength = 0;
   imedia::time msUntilLength = 0;
   int iUntil = 0;
   int iCount = pevents->m_riiTokenNote.b(iTokenIndex).get_count();
   for(int i = 0; i < iCount; i++)
   {
      int iIndex = pevents->m_riiTokenNote.b(iTokenIndex)[i];
      msTotalLength += pevents->m_msaNotesDuration[iIndex];
      if(iIndex <= iNoteIndex)
      {
         iUntil = i;
         msUntilLength += pevents->m_msaNotesDuration[iIndex];
      }
   }

   imedia::time timeAdvance = time + imedia::time(1000);
   imedia::position positionAdvance = pinterface->TimeToPosition(timeAdvance);

   //      int iNoteIndexAdvance = pevents->GetPositionNote(positionAdvance);
   int iTokenIndexAdvance = pevents->GetPositionToken(positionAdvance);

   imedia::position   positionTokenAdvance;
   imedia::time       timeTokenAdvance;

   if(iTokenIndexAdvance >= 0)
   {
      positionTokenAdvance = pevents->m_tkaTokensPosition[iTokenIndexAdvance];
      timeTokenAdvance = pevents->m_msaTokensPosition[iTokenIndexAdvance];
   }
   else
   {
      positionTokenAdvance = 0;
      timeTokenAdvance = 0;
   }


   dynamicdata.m_iBufferTokenIndex = iTokenIndexAdvance;
   dynamicdata.m_iPlayingTokenIndex = iTokenIndex;
   dynamicdata.m_iPlayingNoteIndex = iNoteIndex;


   LyricViewLine * pviewline;
   LyricViewLines & lyriclines = GetLyricLines();

#ifdef _DEBUG
   /*   {
   int i = lyriclines.GetTokenLine(iTokenIndex, &pviewline);
   TRACE("Token = %d ",
   dynamicdata.m_iPlayingTokenIndex);

   if(dynamicdata.m_iPlayingTokenIndex >= 0 &&
   dynamicdata.m_iPlayingTokenIndex < staticdata.m_str2aRawTokens[0].get_size())
   {
   TRACE("(%s)",
   staticdata.m_str2aRawTokens[0][dynamicdata.m_iPlayingTokenIndex]);
   }

   TRACE(", Note = %d \n", dynamicdata.m_iPlayingNoteIndex);
   }*/
#endif



   if(m_lyrictemplatelines.get_size() <= 0)
   {
      return;

   }



   lyriclines.RenderEnable(false);

   // Prepara todos System aparatos para se desenhar
   rect clientRect;
   GetClientRect(&clientRect);

   if(m_bBouncingBall)
   {
      ASSERT(m_lpbouncingball != NULL);
      imedia::position tkPosition;
      pinterface->get_position(tkPosition);

      int iTokenIndexA = dynamicdata.m_iPlayingTokenIndex;

      if(iTokenIndexA < 0)
      {
         iTokenIndexA = 0;
      }

      //          int iNoteIndexA = pevents->GetTokenFirstNote(iTokenIndexA);
      if(0 <= (iLineIndex = lyriclines.GetTokenLine(iTokenIndexA, &pviewline)))
      {
         m_lpbouncingball->to(
            pdc,
            false,
            pviewline,
            peventsBB,
            true);
      }


   }

   if(m_bBouncingBall)
   {
      m_dwLastShiftUpdate = GetTickCount();
      m_lpbouncingball->ClearPendingBBArrange();
      lyriclines.BBArrange(
         pdc,
         NULL,
         false,
         m_lpbouncingball->GetBBArrangeLine(),
         data,
         pfont,
         m_lpbouncingball->GetBBArrangeRate());
   }

   LyricViewLineTemplate * lpTemplateLine;

   //      int iPlayingLineIndex = lyriclines.GetTokenLine(dynamicdata.m_iPlayingTokenIndex, &pviewline);

   // Desenha as linhas
   if(!m_bBouncingBall)
   {

      // Limpeza de todas as linhas j・tocadas...
      // ou seja, as linhas j・tocadas v縊 ser
      // deixadas em branco
      lyriclines.UpdateFalseVisibility(
         pdc,
         NULL,
         false,
         data,
         pfont);

      // se encontrar a linha do correspondente
      // ao token em advance atual (iTokenIndexAdvance),
      // torna a linha vis咩el.
      if(0 <= (iLineIndex = lyriclines.GetTokenLine(iTokenIndexAdvance, &pviewline)))
      {
         if(!pviewline->IsVisible())
         {
            lpTemplateLine = &m_lyrictemplatelines.element_at(pviewline->m_iRelativeLineIndex);
            pviewline->Show();
            pviewline->SetNoteMsLength(0);
            pviewline->SetNoteFillRate(0.0);
            pviewline->SetNewTime();
            pviewline->to(
               pdc,
               data,
               true);
         }
      }

   }

   if(m_bBouncingBall)
   {
      ASSERT(m_lpbouncingball != NULL);
      imedia::position tkPosition;
      pinterface->get_position(tkPosition);

      int iTokenIndexA = dynamicdata.m_iPlayingTokenIndex;

      if(iTokenIndexA < 0)
      {
         iTokenIndexA = 0;
      }

      //          int iNoteIndexA = pevents->GetTokenFirstNote(iTokenIndexA);
      if(0 <= (iLineIndex = lyriclines.GetTokenLine(iTokenIndexA, &pviewline)))
      {
         m_lpbouncingball->to(
            pdc,
            false,
            pviewline,
            peventsBB,
            false);
      }


   }

   {
      // Encontra a linha do token sendo indicado para
      // grifo.
      if((iLineIndex =
         lyriclines.GetTokenLine(iTokenIndex, &pviewline)) >= 0)
      {
         iPlayingIndex = iLineIndex;
         if(m_bGradualFilling)
         {

            if(iNoteIndex >= pevents->m_msaNotesDuration.get_size())
            {
               pviewline->SetNoteMsLength(0);
               pviewline->SetNoteFillRate(1.0);
            }
            else if(iNoteIndex < 0)
            {
               pviewline->SetNoteMsLength(0);
               pviewline->SetNoteFillRate(0.0);
            }
            else
            {
               double dFillRate;
               if(msElapsed <= 0)
               {
                  dFillRate = 0.0;
               }
               else if(msTotalLength == 0.0)
               {
                  dFillRate = 1.0;
               }
               else if(iCount <= 1)
               {
                  dFillRate = 0.33 + ((0.67 * (double) (msElapsed)) / ((double) msTotalLength));
               }
               else
               {
                  dFillRate = ((iUntil + 1) * 0.33 / iCount) + (0.33 * msUntilLength / msTotalLength) + ((0.33 * (double) (msElapsed)) / ((double) msTotalLength));
               }
               if(dFillRate < 0.0)
                  dFillRate = 0.0;
               else if(dFillRate > 1.0)
                  dFillRate = 1.0;
               pviewline->SetNoteMsLength(msTotalLength);
               pviewline->SetNoteFillRate(dFillRate);

               /*    ->UpdateFillRate(
               timeSpan,
               pevents->m_msaNotesDuration[iNoteIndex],
               pevents->m_riiTokenNote.b(iToken).find_first(iNoteIndex),
               pevents->m_riiTokenNote.get_b_item_count()); */
            }
            //            ASSERT(pviewline->m_dFillRate >= _dDebugOldFillRate);
            _dDebugOldFillRate = pviewline->GetFillRate();
         }
         else
         {
            pviewline->SetNoteMsLength(0);
            pviewline->SetNoteFillRate(1.0);
         }

         pviewline->Invalidate(
            dynamicdata.m_iPlayingTokenIndex,
            dynamicdata.m_iPlayingNoteIndex,
            pevents);

         /*pviewline->to(
         pdc,
         data,
         true);*/
      }
   }


   slKaraokeData.unlock();

}


visual::font * KaraokeView::GetLyricFont()
{
   return &m_fontLyrics;

}

LyricViewLines & KaraokeView::GetLyricLines()
{
   return m_lyrictemplatelines.GetLines();
}


void KaraokeView::PrepareLyricTemplateLines()
{

   keeper < bool > keepHardWriting(&m_bHardWriting, true, m_bHardWriting, true);


   visual::font * pfont = GetLyricFont();
   ASSERT(pfont != NULL);

   if(m_lyrictemplatelines.get_size() <= 0)
      return;

   ::ca::dib_sp spdib(get_app());
   spdib->create(184, 184);
   ::ca::graphics * pdc = spdib->get_graphics();

   rect clientRect;
   GetClientRect(&clientRect);
   m_lyrictemplatelines.m_bEmbossed = m_bEmbossed;
   m_lyrictemplatelines.Prepare();


   //   ::ca::graphics_sp   * pgraphics = GetDC();
   TEXTMETRIC textMetric;
   pdc->SelectObject(pfont->GetFont());
   pdc->GetTextMetrics(&textMetric);

   if(m_lyrictemplatelines.get_size() > 0)
   {
      LyricViewLineTemplate * pviewline = NULL;
      rect rect;
      int i;
      for(i = 0; i < m_lyrictemplatelines.get_size(); i++)
      {
         pviewline = &m_lyrictemplatelines.element_at(i);
         pviewline->SetFirstLineFlag(i == 0);
         pviewline->SetColors(m_crLeft, m_crRight, m_crLeftOutline, m_crRightOutline);
         pviewline->SetPens(
            m_penLeft,
            m_penRight,
            m_penLeftSmall,
            m_penRightSmall,
            m_penLinkLeft,
            m_penLinkRight,
            m_penHoverLeft,
            m_penHoverRight
            );

      }
      LayoutLyricTemplateLines();
      LayoutKaraokeBouncingBall();



   }
}











void KaraokeView::PrepareLyricLines()
{

   keeper < bool > keepHardWriting(&m_bHardWriting, true, m_bHardWriting, true);

   string strLine;
   string wstrLine;

   visual::font * pfont = GetLyricFont();
   ASSERT(pfont != NULL);

   ikar::data & data = GetKaraokeData();
   ikar::static_data & staticdata = data.GetStaticData();
   ikar::dynamic_data & dynamicdata = data.GetDynamicData();

   EventsTracksV1 * peventsarray = NULL;
   peventsarray = &staticdata.m_eventstracksV002;

   LyricEventsV2 * pevents;

   if(!peventsarray ||
      staticdata.m_str2aRawTokens.get_size() <= 0)
      pevents = NULL;
   else if(peventsarray->get_size() < staticdata.m_str2aRawTokens.get_size())
      pevents = NULL;
   else
   {
      //pevents = (LyricEventsV2 *) peventsarray->operator [](spfile->m_str2aRawTokens.get_size());
      //pevents = (LyricEventsV2 *) peventsarray->operator [](peventsarray->get_size() / 2);
      pevents = (LyricEventsV2 *) peventsarray->operator [](0);
      ASSERT(pevents != NULL);
      ASSERT(pevents->GetClassOrder() == 2);
   }


   rect clientRect;

   GetClientRect(&clientRect);
   TRACE("CXfplayerView::PrepareLyricLines 1 slGdi.lock\n");

   ::ca::graphics_sp spgraphics(get_app());
   spgraphics->CreateCompatibleDC(NULL);
   ::ca::graphics * pdc = spgraphics;


   LyricViewLine::ERenderResult elvlr;
   int iTokenIndex = 0;
   int iNextTokenIndex;

   int iTemplate = 0;
   int iPreviousNonEmptyTemplate = -1;

   //   if(pDoc->m_xflha.get_size() > 0) TODO
   // {
   //  m_lyrictemplatelines.m_wstrLanguage = pDoc->m_xflha.element_at(0).m_wstrLanguage;
   //}

   LyricViewLines & lyriclines = GetLyricLines();
   lyriclines.RemoveAllLines();


   string strLyrics;
   string wstrLyrics;

   staticdata.GetLyrics(wstrLyrics);
   strLyrics = wstrLyrics;

   DWORD dwEncodingCodePage = KaraokeGetCodePage(strLyrics);


   TRACE("KaraokeView::PrepareLyricLines %s\n", strLyrics);
   if(staticdata.m_str2aRawTokens.get_size() > 0)
   {
      //pfont->SetDC(pdcScreen);
      //pfont->ClearDC();

      int iEmptyCount = 0;
      bool bPageBreak = false;

      while(true)
      {
         LyricViewLine & rviewline = lyriclines.add_new_line();
         rviewline.m_bAutoSizeX = true;
         rviewline.m_bAutoSizeY = true;

         //         viewLine.m_bFirstLine = m_lyrictemplatelines[iRelativeLineIndex].IsFirstLine();
         rviewline.m_iRelativeLineIndex = iTemplate;
         //         viewLine.SetTwi(this);
         //         viewLine.SetTextEffect(XfplayerViewLine::EffectEmbossed);
         rviewline.SetTemplate(&m_lyrictemplatelines.element_at(iTemplate));
         elvlr = rviewline.Prepare(
            dwEncodingCodePage,
            pdc,
            staticdata.m_str2aRawTokens.ptr_at(0),
            iTokenIndex,
            &iNextTokenIndex,
            bPageBreak ? 0 : LyricViewLine::XFPLYR_LINE_PAGE_BREAK,
            &clientRect,
            pevents);
         rviewline.m_tokenaRaw.GetText(strLine);
         wstrLine = gen::international::utf8_to_unicode(strLine);
         strLine.trim();
         if(strLine.is_empty())
         {
            iEmptyCount++;
         }
         else
         {
            iEmptyCount = 0;
            iPreviousNonEmptyTemplate = iTemplate;
         }
         bPageBreak = elvlr == LyricViewLine::XFPLYR_LINE_PAGE_BREAK;
         if(m_bBouncingBall)
         {
            if(bPageBreak)
            {
               int iInsertEmpty = m_lyrictemplatelines.get_size() - iEmptyCount - 2;
               while(iInsertEmpty > 0)
               {
                  iInsertEmpty--;
                  iTemplate++;
                  if(iTemplate >= m_lyrictemplatelines.get_size())
                     iTemplate = 0;
                  LyricViewLine & rviewlineEmpty = lyriclines.add_new_line();
                  rviewlineEmpty.m_bAutoSizeX = true;
                  rviewlineEmpty.m_bAutoSizeY = true;
                  rviewlineEmpty.m_iRelativeLineIndex = iTemplate;
                  rviewlineEmpty.SetTemplate(&m_lyrictemplatelines.element_at(iTemplate));
                  rviewlineEmpty.m_iFirstToken = rviewline.m_iFirstToken + rviewline.m_iTokenCount;
                  rviewlineEmpty.m_iTokenCount = 0;
               }
               continue;
            }
            else if(elvlr == LyricViewLine::XFPLYR_LINE_SUCCESS_NO_LINE)
            {
               break;
            }
         }
         else
         {
            if(bPageBreak)
            {
               iTemplate = 0;
               continue;
            }
            else if(elvlr == LyricViewLine::XFPLYR_LINE_SUCCESS_NO_LINE)
            {
               break;
            }
         }
         if(iEmptyCount < (m_lyrictemplatelines.get_size() - 1))
         {
            iTemplate++;
            if(iTemplate >= m_lyrictemplatelines.get_size())
               iTemplate = 0;
         }

            iTokenIndex = iNextTokenIndex;

            TRACE("%s\n", strLine);
         }
      }

      lyriclines.Prepare();
      lyriclines.PrepareURLLinks();

      //      KaraokeBouncingBall * lpbb = m_lpbouncingball;

      TRACE("CXfplayerView::PrepareLyricLines 1 slGdi.UnLock\n");

      LayoutLyricLines();

      if(staticdata.m_str2aTitle.get_size() > 0)
      {
         m_plinesTitle->remove_all();
         XfplayerViewLine line(get_app());
         line.SetBlend(0.0);
         m_plinesTitle->add(line);

         string  str;


         _TranslateAndFormat(
            str,
            staticdata.m_straTitleFormat[0],
            staticdata.m_str2aTitle[0]);

         XfplayerViewLine & rline = m_plinesTitle->element_at(m_plinesTitle->get_upper_bound());
         rline.SetFont(System.font_central().GetTitleFont()->GetFont());
         rline.PrepareLine(
            pdc,
            str,
            0,
            clientRect);
         rline.Show(true);
         rline.SetColors(RGB(255, 255, 255), RGB(127, 127, 120));
         m_bTitle = true;

         //string strFormat;
         //strFormat.Format("%S", staticdata.m_wstraTitle[0]);
         //System.simple_message_box(strFormat);

         int i = 1;
         /*      if(staticdata.m_wstraPerformer.get_size() > 0)
         {
         staticdata.m_wstraPerformer.get_format_string(str, L";");
         gen::international::UnicodeToACP(str, str);
         dwCodePage = KaraokeGetCodePage(str);
         gen::international::CPToUnicode(
         dwCodePage,
         str, str);
         str = L"Performer: " + str;
         m_plinesTitle->add(line);
         XfplayerViewLine & rline = m_plinesTitle->element_at(i);
         rline.SetFont(System.font_central().GetSubTitleFont()->GetFont());
         rline.PrepareLine(
         pdcScreen,
         str,
         0,
         clientRect);
         rline.PrepareURLLinks();

         rline.m_hwnd = pwnd->GetSafeHwnd();

         rline.Show(true);
         rline.SetColors(RGB(235, 235, 230), RGB(127, 127, 120));
         i++;
         }*/

         for(; i < staticdata.m_str2aTitle.get_size(); i++)
         {
            m_plinesTitle->add(line);
            XfplayerViewLine & rline = m_plinesTitle->element_at(i);
            rline.SetFont(System.font_central().GetSubTitleFont()->GetFont());

            _TranslateAndFormat(
               str,
               staticdata.m_straTitleFormat[i],
               staticdata.m_str2aTitle[i]);

            rline.PrepareLine(
               pdc,
               str,
               0,
               clientRect);

            rline.Show(true);
            rline.SetColors(RGB(235, 235, 230), RGB(127, 127, 120));
         }

         visual::font * pfont = System.font_central().GetTitleFont();
         ASSERT(pfont != NULL);

         rect rectClient;
         GetClientRect(&rectClient);

         ::ca::graphics_sp spgraphics(get_app());
         spgraphics->CreateCompatibleDC(NULL);

         int pos = 100;
         for(i = 0; i < m_plinesTitle->get_size(); i++)
         {
            XfplayerViewLine & rline = m_plinesTitle->element_at(i);
            spgraphics->SelectObject(m_plinesTitle->element_at(i).GetFont());
            TEXTMETRIC textmetric;
            spgraphics->GetTextMetrics(&textmetric);
            int iFontMegaHeight =
               (textmetric.tmHeight +
               textmetric.tmInternalLeading +
               textmetric.tmExternalLeading);
            rect rect(
               rectClient.left,
               rectClient.top + pos,
               rectClient.right,
               rectClient.top + pos + iFontMegaHeight);
            rline.SetPlacement(rect);
            pos += iFontMegaHeight;
         }
      }
      if(staticdata.m_str2aRawTokens.get_size() > 0)
      {
         dynamicdata.initialize_standup(staticdata.m_str2aRawTokens[0].get_size());
      }
   }

   bool KaraokeView::ShowBouncingBall(bool bShow)
   {
      if(bShow)
      {
         m_bBouncingBall = true;
         keeper < bool > keepHardWriting(&m_bHardWriting, true, false, true);
         PrepareLyricLines();
         PrepareLyricTemplateLines();
      }
      else
      {
         m_bBouncingBall = false;
         keeper < bool > keepHardWriting(&m_bHardWriting, true, false, true);
         PrepareLyricLines();
         PrepareLyricTemplateLines();
      }
      OnShowBouncingBall(bShow);
      return true;
   }

   void KaraokeView::LayoutLyricTemplateLines()
   {


      visual::font * pfont = GetLyricFont();
      ASSERT(pfont != NULL);

      rect rectClient;
      GetClientRect(&rectClient);

      TEXTMETRIC & textmetric = pfont->m_tm;
      int iFontMegaHeight =
         (textmetric.tmHeight +
         textmetric.tmInternalLeading +
         textmetric.tmExternalLeading);

      int iMainDeltaY = (int) (iFontMegaHeight * 0.3);

      iFontMegaHeight += iMainDeltaY;


      if(m_lyrictemplatelines.get_size() > 0)
      {
         LyricViewLineTemplate * lpTemplateLine = NULL;
         int i, pos;
         for(
            pos = iFontMegaHeight,
            i = m_lyrictemplatelines.get_size() - 1; i >= 0 ; i--)
         {
            lpTemplateLine = &m_lyrictemplatelines.element_at(i);
            rect rect(
               rectClient.left,
               //              m_lineSongLabel.m_iTop - pos, TODO
               rectClient.bottom  - pos,
               rectClient.right,
               //m_lineSongLabel.m_iTop - pos + iFontMegaHeight); TODO
               rectClient.bottom  - pos + iFontMegaHeight);
            lpTemplateLine->SetPlacement_(rect, iMainDeltaY);
            pos += iFontMegaHeight;
         }
      }
      m_lyrictemplatelines.m_bEmbossed = m_bEmbossed;

      m_lyrictemplatelines.m_bEmbossed = m_bEmbossed;

   }

   void KaraokeView::LayoutKaraokeBouncingBall()
   {

      if(m_lyrictemplatelines.get_size() > 0)
      {
         rect rectClient;
         GetClientRect(rectClient);
         KaraokeBouncingBall * lpbb = m_lpbouncingball;
         visual::font * pfont = GetLyricFont();
         int iMegaHeight = pfont->GetMegaHeight() * 3 / 2;
         LyricViewLineTemplate & templateline = m_lyrictemplatelines.element_at(0);
         rect rect(rectClient);
         class rect rectTemplate;
         templateline.GetPlacement_(rectTemplate);
         rect.top = rectTemplate.top - iMegaHeight;
         rect.bottom = rectTemplate.top;
         lpbb->SetPlacement(rect);
         lpbb->SetBallSize(size(rectTemplate.height() * 1 / 3, rectTemplate.height() * 1 / 3));
      }

   }

   void KaraokeView::LayoutLyricLines()
   {

      rect rectClient;
      GetClientRect(rectClient);

      int iRadius = max(2, (min(rectClient.width(), rectClient.height()) / 84));
      int iRadiusSmall = max(2, (min(rectClient.width(), rectClient.height()) / 284));
//
//      ::ca::pen_sp pen(get_app(), PS_SOLID | PS_GEOMETRIC | PS_ENDCAP_ROUND | PS_JOIN_ROUND, iRadius * 2, &lb);
//
      LOGBRUSH lb;
      lb.lbStyle = BS_SOLID;
      lb.lbHatch = 0;


      lb.lbColor = m_crLeftOutline;
      m_penLeft->CreatePen(PS_SOLID | PS_GEOMETRIC | PS_ENDCAP_ROUND | PS_JOIN_ROUND, iRadius, &lb);

      lb.lbColor = m_crRightOutline;
      m_penRight->CreatePen(PS_SOLID | PS_GEOMETRIC | PS_ENDCAP_ROUND | PS_JOIN_ROUND, iRadius, &lb);

      lb.lbColor = m_crLeftOutline;
      m_penLeftSmall->CreatePen(PS_SOLID | PS_GEOMETRIC | PS_ENDCAP_ROUND | PS_JOIN_ROUND, iRadiusSmall, &lb);

      lb.lbColor = m_crRightOutline;
      m_penRightSmall->CreatePen(PS_SOLID | PS_GEOMETRIC | PS_ENDCAP_ROUND | PS_JOIN_ROUND, iRadiusSmall, &lb);

      lb.lbColor = RGB(0, 0, 200);
      m_penLinkLeft->CreatePen(PS_SOLID | PS_GEOMETRIC | PS_ENDCAP_ROUND | PS_JOIN_ROUND, iRadius, &lb);

      lb.lbColor = RGB(150, 150, 200);
      m_penLinkRight->CreatePen(PS_SOLID | PS_GEOMETRIC | PS_ENDCAP_ROUND | PS_JOIN_ROUND, iRadius, &lb);

      lb.lbColor = RGB(0, 0, 255);
      m_penHoverLeft->CreatePen(PS_SOLID | PS_GEOMETRIC | PS_ENDCAP_ROUND | PS_JOIN_ROUND, iRadius, &lb);

      lb.lbColor = RGB(200, 200, 255);
      m_penHoverRight->CreatePen(PS_SOLID | PS_GEOMETRIC | PS_ENDCAP_ROUND | PS_JOIN_ROUND, iRadius, &lb);



      visual::font * pfont = GetLyricFont();
      ASSERT(pfont != NULL);


      LyricViewLines & lyriclines = GetLyricLines();
      for(int i = 0; i < lyriclines.GetLineCount(); i++)
      {
         LyricViewLine * pviewline = &lyriclines.get(i);
         pviewline->SetPendingLayoutUpdate();
      }



   }

   void KaraokeView::LayoutTitleLines()
   {


      visual::font * pfont = System.font_central().GetTitleFont();
      ASSERT(pfont != NULL);

      rect rectClient;
      GetClientRect(&rectClient);

      TEXTMETRIC & textmetric = pfont->m_tm;
      int iFontMegaHeight =
         (textmetric.tmHeight +
         textmetric.tmInternalLeading +
         textmetric.tmExternalLeading);

      int pos = 100;
      for(int i = 0; i < m_plinesTitle->get_size(); i++)
      {
         XfplayerViewLine & rline = m_plinesTitle->element_at(i);
         rect rect(
            rectClient.left,
            rectClient.top + pos,
            rectClient.right,
            rectClient.top + pos + iFontMegaHeight);
         rline.SetPlacement(rect);
         pos += iFontMegaHeight;
      }

   }


   void KaraokeView::OnShowBouncingBall(bool bShow)
   {
      UNREFERENCED_PARAMETER(bShow);
   }

   void KaraokeView::KaraokeRender(::ca::graphics *pdc)
   {

      if(!is_ok_for_rendering())
      {
         // may draw temporary things
         return;
      }

      rect rectClient;
      GetClientRect(rectClient);

      //      int count = 0;

      ikar::data & data = GetKaraokeData();
      if(&data == NULL)
         return;
      ikar::dynamic_data & dynamicdata = data.GetDynamicData();
      ikar::static_data & staticdata = data.GetStaticData();

      /*   if(m_bBouncingBall)
      {
      GetLyricLines().BBArrange(
      pdc,
      NULL,
      false,
      m_lpbouncingball->GetBBArrangeLine(),
      &rectClient,
      *((rect_array *)NULL),
      data,
      &count,
      pfont,
      m_lpbouncingball->GetBBArrangeRate());
      }*/

      EventsTracksV1 * peventsarray
         = &staticdata.m_eventsTracksForPositionCB;

      LyricEventsV2 * pevents;
      if(peventsarray == NULL ||
         peventsarray->get_size() <= 0)
         pevents = NULL;
      else
      {
         //      pevents = (LyricEventsV2 *) peventsarray->operator [](staticdata.m_str2aRawTokens.get_size());
         pevents = (LyricEventsV2 *) peventsarray->element_at(0);
         ASSERT(pevents != NULL);
         ASSERT(pevents->GetClassOrder() == 2);
      }

      LyricEventsV2 * peventsBB = NULL;
      if(staticdata.m_eventsTracksForBouncingBall.get_size() > 0)
      {
         peventsBB = (LyricEventsV2 *) staticdata.m_eventsTracksForBouncingBall.operator [](0);
         ASSERT(peventsBB != NULL);
         ASSERT(peventsBB->GetClassOrder() == 2);
      }

      if(m_bTitle && m_bTitleVisible)
      {
         ASSERT(m_plinesTitle != NULL);
         for(int i =0; i < m_plinesTitle->get_size(); i++)
         {
            XfplayerViewLine & line = m_plinesTitle->element_at(i);
            line.to(
               get_app(),
               pdc,
               true,
               rectClient,
               NULL_REF(rect_array),
               false);
         }
      }

      LyricViewLines & lyriclines = GetLyricLines();
      lyriclines.RenderEnable();
      if(m_lyrictemplatelines.get_size() > 0 &&
         true)
      {
      if(m_bBouncingBall &&
         pevents != NULL)
      {
         ASSERT(m_lpbouncingball != NULL);

         int iTokenIndexA = dynamicdata.m_iPlayingTokenIndex;

         if(iTokenIndexA < 0)
         {
            iTokenIndexA = 0;
         }
         LyricViewLine * pviewline = NULL;
         int iLineIndex;
         //         int iNoteIndexA = pevents->GetTokenFirstNote(iTokenIndexA);
         if(0 <= (iLineIndex =
            lyriclines.GetTokenLine(iTokenIndexA, &pviewline)))
         {
            m_lpbouncingball->to(
               pdc,
               true,
               pviewline,
               peventsBB,
               true);
         }
      }
         /*for(int i = 0; i < m_lyrictemplatelines.get_size(); i++)
         {
         LyricViewLineTemplate & linetemplate = m_lyrictemplatelines.element_at(i);
         LyricViewLine * pline = linetemplate.GetLine();
         if(pline != NULL
         && pline->IsVisible())
         {
         m_bTitleVisible = false;
         //            pline->GetPlacement(rectLine);
         pline->to(
         pdc,
         rectClient,
         rect_array::GetNullReference(),
         data,
         &count,
         false);
         }
         }*/
         int i;
         LyricViewLine * pviewline = NULL;
         rect rectLine;

         int iStartIndex, iEndIndex;
         iStartIndex = lyriclines.GetFirstVisibleLine();
         if(iStartIndex >= lyriclines.GetLineCount())
            iStartIndex = lyriclines.GetLineCount() - 1;
         if(iStartIndex < 0)
            iStartIndex = 0;
         iEndIndex = lyriclines.GetLastVisibleLine();
         if(iEndIndex < iStartIndex)
            iEndIndex = iStartIndex;
         if(iEndIndex >= lyriclines.GetLineCount())
            iEndIndex = lyriclines.GetLineCount() - 1;

         for( i = iStartIndex; i <= iEndIndex; i++)
         {
            pviewline = &lyriclines.get(i);
            if(pviewline->IsVisible())
            {
               SetTitleVisible(false);
               pviewline->GetPlacement(rectLine);
               pviewline->to(
                  pdc,
                  data,
                  false);
            }
         }

      }

      if(m_bBouncingBall &&
         pevents != NULL)
      {
         ASSERT(m_lpbouncingball != NULL);

         int iTokenIndexA = dynamicdata.m_iPlayingTokenIndex;

         if(iTokenIndexA < 0)
         {
            iTokenIndexA = 0;
         }
         LyricViewLine * pviewline = NULL;
         int iLineIndex;
         //         int iNoteIndexA = pevents->GetTokenFirstNote(iTokenIndexA);
         if(0 <= (iLineIndex =
            lyriclines.GetTokenLine(iTokenIndexA, &pviewline)))
         {
            m_lpbouncingball->to(
               pdc,
               true,
               pviewline,
               peventsBB,
               false);
         }
      }
   }

   DWORD KaraokeView::KaraokeGetDefaultCodePage(const char * lpsz)
   {
      UNREFERENCED_PARAMETER(lpsz);
      return GetKaraokeData().GetStaticData().m_dwDefaultCodePage;
   }

   DWORD KaraokeView::KaraokeGetOverridingCodePage(const char * lpsz)
   {
      UNREFERENCED_PARAMETER(lpsz);
      return CP_OEMCP;
   }

   bool KaraokeView::KaraokeDoesOverrideCodePage()
   {
      return false;
   }


   void KaraokeView::SetScale(double dScale)
   {
      m_dScale = dScale;
      LOGFONT lf;
      System.font_central().GetLyricViewFont()->GetFont()->GetLogFont(&lf);
      lf.lfHeight = (int) (((double)lf.lfHeight) * dScale);
      lf.lfWidth = 0;
      GetLyricFont()->GetFont()->CreateFontIndirect(&lf);
      GetLyricFont()->OnUpdateFont();
      m_dLyricFontEmbossWidth = (int) (4.5 * dScale);

   }

   bool KaraokeView::is_ok_for_rendering()
   {
      return !m_bHardWriting;
   }

   HCURSOR KaraokeView::KaraokeGetHandCursor()
   {
      return ::LoadCursor(NULL, IDC_ARROW);
   }

   void KaraokeView::KaraokeGetPosition(imedia::position &position)
   {
      ikar::data & data = GetKaraokeData();
      ikar::karaoke * pinterface = data.GetInterface();
      pinterface->get_position(position);
      imedia::time time = pinterface->PositionToTime(position);
      imedia::time timeDuration;
      timeDuration = KaraokeGetLyricsDelay();
      time = time - timeDuration;
      position = pinterface->TimeToPosition(time);

   }


   int KaraokeView::KaraokeGetLyricsDelay()
   {
      return 0;
   }

   void KaraokeView::SetTitleVisible(bool bVisible)
   {
      if(m_bTitleVisible != bVisible)
      {
         m_bTitleVisible = bVisible;
         _001RedrawWindow();
      }
   }

   DWORD KaraokeView::KaraokeGetCodePage(const char * lpsz)
   {
      if(KaraokeDoesOverrideCodePage())
      {
         return KaraokeGetOverridingCodePage(lpsz);
      }
      else
      {
         return KaraokeGetDefaultCodePage(lpsz);
      }
   }

   void KaraokeView::_TranslateAndFormat(string &str, const char * lpszFormat, string_array &wstraArg)
   {
      string_array stra;
      stra = wstraArg;
      _Translate(stra);
      str = lpszFormat;
      PcreUtil::Format(str, stra);
   }

   void KaraokeView::_Translate(string_array &stra)
   {
      for(int i = 0; i < stra.get_size(); i++)
      {
         _Translate(stra[i]);
      }
   }

   void KaraokeView::_Translate(string &str)
   {
      DWORD dwCodePage = KaraokeGetCodePage(str);
      gen::international::MultiByteToMultiByte(
         CP_UTF8,
         str,
         dwCodePage,
         str);
   }

   void KaraokeView::clear()
   {
      //GetKaraokeData().delete_contents(); // commented out : views preferentially should defer changing data to the document, or calling document
      m_lyrictemplatelines.clear_lines();
   }

   bool KaraokeView::IsKaraokeBouncingBall()
   {
      return m_bBouncingBall;
   }

} // namespace kar
