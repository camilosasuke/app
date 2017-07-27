#include "framework.h"

#define TIMER_HOVER 321654

class SimpleToolCmdUI : public cmd_ui        // class private to this file !
{
public: // re-implementations only

   SimpleToolCmdUI(::aura::application * papp);
   virtual void Enable(bool bOn = TRUE, ::action::context actioncontext = ::action::source_system);
   //   virtual void _001SetCheck(bool bCheck, ::action::context = ::action::source_system);   // 0, 1 or 2 (indeterminate)
   virtual void _001SetCheck(check::e_check echeck, ::action::context = ::action::source_system);   // 0, 1 or 2 (indeterminate)
//   virtual void SetRadio(bool bOn = TRUE, ::action::context actioncontext = ::action::source_system);
   virtual void SetText(const char * lpszText, ::action::context actioncontext = ::action::source_system);
};

#define EXTRA_TEXT_CX 3
#define EXTRA_TEXT_CY 1

//#define ITEM_SEP_CX 2
//#define ITEM_SEP_CY 2
//#define TOOL_SEP_CX 8

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//#define ITEMHOVERCX 0
//#define ITEMHOVERCY 0
/*#define ITEMHOVERPADLEFT 2
#define ITEMHOVERPADTOP 2
#define ITEMHOVERPADRIGHT 2
#define ITEMHOVERPADBOTTOM 2*/
//#define ITEMPRESSCX 3
//#define ITEMPRESSCY 3
/*#define ITEMPRESSPADLEFT 2
#define ITEMPRESSPADTOP 2
#define ITEMPRESSPADRIGHT 2
#define ITEMPRESSPADBOTTOM 2*/
//#define IMAGE_SEP_CX 2
//#define PAD_CX 1
//#define PAD_CX 1


simple_toolbar::simple_toolbar(::aura::application * papp) :
   object(papp),
   ::user::control_bar(papp),
   ::user::toolbar(papp),
   m_dibDraft(allocer())
{

   m_iImageSpacing = -1;

   m_sizePress.set(-1);

   m_sizeSpacing.set(-1);

   m_sizeSeparator.set(-1);

   m_rectBorder.set(-1);

   m_rectItemPad.set(-1);

   m_dFontSize = 1.0;

   // lakic
   m_sizeSpacing.cx = 8;

   m_rectItemPad.set(9, 9);

   m_rectBorder.set(9, 9);
   // end lakic

   m_iHover = 0x80000000;

   m_iButtonPressItem = -1;

   m_bTransparentBackground = true;

   m_sizeImage.cx = 16;
   m_sizeImage.cy = 15;

   m_sizeButton.cx = 23;
   m_sizeButton.cy = 22;

   m_bDelayedButtonLayout = true;

   

}


simple_toolbar::~simple_toolbar()
{

}


void simple_toolbar::install_message_handling(::message::dispatch * pdispatch)
{

   ::user::toolbar::install_message_handling(pdispatch);

   IGUI_WIN_MSG_LINK(WM_CREATE, pdispatch, this, &simple_toolbar::_001OnCreate);
   IGUI_WIN_MSG_LINK(WM_MOUSEMOVE, pdispatch, this, &simple_toolbar::_001OnMouseMove);
   IGUI_WIN_MSG_LINK(WM_LBUTTONDOWN, pdispatch, this, &simple_toolbar::_001OnLButtonDown);
   IGUI_WIN_MSG_LINK(WM_LBUTTONUP, pdispatch, this, &simple_toolbar::_001OnLButtonUp);
   IGUI_WIN_MSG_LINK(WM_NCHITTEST, pdispatch, this, &simple_toolbar::_001OnNcHitTest);
   IGUI_WIN_MSG_LINK(WM_MOUSELEAVE, pdispatch, this, &simple_toolbar::_001OnMouseLeave);

}


bool simple_toolbar::create(sp(::user::interaction) pParentWnd, uint32_t dwStyle, id nID)
{

   return create_window_ex(pParentWnd, 0, dwStyle, rect(m_cxLeftBorder, m_cyTopBorder, m_cxRightBorder, m_cyBottomBorder), nID);

}


bool simple_toolbar::create_window_ex(sp(::user::interaction) pParentWnd, uint32_t dwCtrlStyle, uint32_t dwStyle, const RECT & rectBorders, id nID)
{

   ASSERT_VALID(pParentWnd);   // must have a parent
   ASSERT(!((dwStyle & CBRS_SIZE_FIXED) && (dwStyle & CBRS_SIZE_DYNAMIC)));

   SetBorders(rectBorders);

   // save the style
   m_dwStyle = (dwStyle & CBRS_ALL);
   if (nID == __IDW_TOOLBAR)
      m_dwStyle |= CBRS_HIDE_INPLACE;

   dwStyle &= ~CBRS_ALL;
   dwStyle |= CCS_NOPARENTALIGN | CCS_NOMOVEY | CCS_NODIVIDER | CCS_NORESIZE;
   dwStyle |= dwCtrlStyle & 0xffff;
   m_dwCtrlStyle = dwCtrlStyle & (0xffff0000 | TBSTYLE_FLAT);

   if (!::user::interaction::create_window(NULL, NULL, dwStyle, null_rect(), pParentWnd, nID))
   {

      return FALSE;

   }

   SetSizes(m_sizeButton, m_sizeImage);

   return true;

}


size simple_toolbar::CalcFixedLayout(bool bStretch, bool bHorz)
{

   uint32_t dwMode = bStretch ? LM_STRETCH : 0;

   dwMode |= bHorz ? LM_HORZ : 0;

   return CalcLayout(dwMode);

}


size simple_toolbar::CalcSimpleLayout()
{

   ASSERT_VALID(this);

   ASSERT(IsWindow());

   ::count nCount;

   size sizeResult(0, 0);

   nCount = m_itema.get_count();

   if (nCount > 0)
   {

      rect rectItem;

      rect rectSize(0, 0, 0, 0);

      for (int32_t i = 0; i < nCount; i++)
      {

         _001GetItemRect(i, rectItem);

         rectSize.unite(rectSize, rectItem);

      }

      sizeResult = rectSize.size();

   }

#ifdef WINDOWSEX

   if (GetStyle() & TBSTYLE_FLAT)
   {

      sizeResult.cy += 2;

   }

#else

   throw todo(get_app());

#endif

   return sizeResult;

}


void simple_toolbar::_001OnDraw(::draw2d::graphics * pgraphics)
{

   sp(::user::tab) ptab = GetTypedParent < ::user::tab >();

   if (ptab.is_set())
   {

   }

   if (m_bDelayedButtonLayout)
   {

      on_layout();

   }

   rect rectClient;

   GetClientRect(rectClient);

   pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);

   pgraphics->FillSolidRect(rectClient, _001GetColor(::user::color_toolbar_background));

   //::user::interaction::_001OnDraw(pgraphics);

   select_font(pgraphics, ::user::font_toolbar, this);

   m_dFontSize = pgraphics->m_spfont->m_dFontSize;

   int iHover = _001GetHoverItem();

   for (int32_t iItem = 0; iItem < m_itema.get_size(); iItem++)
   {

      if (iItem != iHover)
      {

         _001DrawItem(pgraphics, iItem);

      }

   }

   if (iHover >= 0)
   {

      _001DrawItem(pgraphics, iHover);

   }


}


void simple_toolbar::SetTransparentBackground(bool bSet)
{

   m_bTransparentBackground = bSet;

}


void simple_toolbar::TransparentEraseNonClient(::draw2d::graphics * pgraphics)
{



   m_dibDraft->get_graphics()->BitBlt(0, 0, 7, 7, pgraphics, 0, 0, SRCCOPY);

   rect rectWindow;
   GetWindowRect(rectWindow);
   rectWindow.offset(-rectWindow.top_left());
   if (m_bTransparentBackground)
   {
      class imaging & imaging = System.visual().imaging();
      if (m_iHover >= -1)
      {
         imaging.color_blend(
            pgraphics,
            rectWindow.left,
            rectWindow.top,
            rectWindow.width(),
            rectWindow.height(),
            RGB(215, 215, 210),
            220);
      }
      else
      {
         imaging.color_blend(
            pgraphics,
            rectWindow.left,
            rectWindow.top,
            rectWindow.width(),
            rectWindow.height(),
            RGB(215, 215, 210),
            180);
      }
   }
   else
   {
#ifdef WINDOWSEX
      pgraphics->FillSolidRect(rectWindow, Session.get_default_color(COLOR_3DFACE));
#else
      pgraphics->FillSolidRect(rectWindow, ARGB(255, 184, 184, 177));
#endif
   }

   // Desenha chanfro
   COLORREF cr = m_dibDraft->GetPixel(0, 0);
   pgraphics->SetPixel(0, 0, cr);
   pgraphics->SetPixel(0, 1, m_dibDraft->GetPixel(0, 1));
   pgraphics->SetPixel(0, 2, m_dibDraft->GetPixel(0, 2));
   pgraphics->SetPixel(0, 3, m_dibDraft->GetPixel(0, 3));
   pgraphics->SetPixel(0, 4, m_dibDraft->GetPixel(0, 4));
   pgraphics->SetPixel(0, 5, m_dibDraft->GetPixel(0, 5));
   pgraphics->SetPixel(0, 6, m_dibDraft->GetPixel(0, 6));
   pgraphics->SetPixel(1, 0, m_dibDraft->GetPixel(1, 0));
   pgraphics->SetPixel(1, 1, m_dibDraft->GetPixel(1, 1));
   pgraphics->SetPixel(1, 2, m_dibDraft->GetPixel(1, 2));
   pgraphics->SetPixel(1, 3, m_dibDraft->GetPixel(1, 3));
   pgraphics->SetPixel(1, 4, m_dibDraft->GetPixel(1, 4));
   pgraphics->SetPixel(1, 5, m_dibDraft->GetPixel(1, 5));
   pgraphics->SetPixel(2, 0, m_dibDraft->GetPixel(2, 0));
   pgraphics->SetPixel(2, 1, m_dibDraft->GetPixel(2, 1));
   pgraphics->SetPixel(2, 2, m_dibDraft->GetPixel(2, 2));
   pgraphics->SetPixel(2, 3, m_dibDraft->GetPixel(2, 3));
   pgraphics->SetPixel(2, 4, m_dibDraft->GetPixel(2, 4));
   pgraphics->SetPixel(3, 0, m_dibDraft->GetPixel(3, 0));
   pgraphics->SetPixel(3, 1, m_dibDraft->GetPixel(3, 1));
   pgraphics->SetPixel(3, 2, m_dibDraft->GetPixel(3, 2));
   pgraphics->SetPixel(3, 3, m_dibDraft->GetPixel(3, 3));
   pgraphics->SetPixel(4, 0, m_dibDraft->GetPixel(4, 0));
   pgraphics->SetPixel(4, 1, m_dibDraft->GetPixel(4, 1));
   pgraphics->SetPixel(4, 2, m_dibDraft->GetPixel(4, 2));
   pgraphics->SetPixel(5, 0, m_dibDraft->GetPixel(5, 0));
   pgraphics->SetPixel(5, 1, m_dibDraft->GetPixel(5, 1));
   pgraphics->SetPixel(6, 0, m_dibDraft->GetPixel(6, 0));


   //pgraphics->BitBlt(0, 0, 7, 7, &m_dcDraft, 0, 0, SRCCOPY);

   //

   // draw borders in non-client area
   DrawBorders(pgraphics, rectWindow);
   /*   pgraphics->FillSolidRect(
   rectWindow.left + 7, rectWindow.top,
   rectWindow.right, rectWindow.top + 1,
   RGB(128, 128, 123));*/

   // erase parts not drawn
   //pgraphics->IntersectClipRect(rectWindow);
   //SendMessage(WM_ERASEBKGND, (WPARAM)pgraphics->get_handle1());

   // draw gripper in non-client area
   DrawGripper(pgraphics, rectWindow);

   //pgraphics->SetViewportOrg(ptViewport);
}


void simple_toolbar::_001OnCreate(signal_details * pobj)
{
   if (pobj->previous())
      return;


   //m_puserschemaSchema = Session.m_puserschemaSchema;
   m_dibDraft->create(20, 20);
}


void simple_toolbar::OnUpdateCmdUI(sp(::user::frame_window) pTarget, bool bDisableIfNoHndler)
{

   SimpleToolCmdUI state(get_app());

   state.m_pOther = (this);

   state.m_iCount = _001GetItemCount();

   for (state.m_iIndex = 0; state.m_iIndex < state.m_iCount; state.m_iIndex++)
   {

      // ignore separators
      if (m_itema[state.m_iIndex]->m_id != "separator")
      {

         state.m_id = m_itema[state.m_iIndex]->m_id;

         state.m_bEnableIfHasCommandHandler = m_itema[state.m_iIndex]->m_bEnableIfHasCommandHandler;

         // allow reflections
         //if (::user::interaction::on_simple_action(0,
         //   MAKELONG((int32_t)CN_UPDATE_COMMAND_UI, WM_COMMAND+WM_REFLECT_BASE),
         //   &state, NULL))
         //   continue;

         // allow the toolbar itself to have update handlers
         if (_001SendUpdateCmdUi(&state))
            continue;

         // allow the owner to process the update
         state.DoUpdate(pTarget, bDisableIfNoHndler);
      }

   }

   // update the dialog controls added to the toolbar
   UpdateDialogControls(pTarget, bDisableIfNoHndler);

}

#define CX_OVERLAP 0

size simple_toolbar::CalcSize(int32_t nCount)
{

   ASSERT(nCount > 0);

   ::draw2d::memory_graphics pgraphics(allocer());

   select_font(pgraphics, ::user::font_toolbar, this);

   m_dFontSize = pgraphics->m_spfont->m_dFontSize;

   int iImageSpacing = get_image_spacing();

   rect rBorder = get_bar_border();

   size sPress = get_press_shift();

   size sSpacing = get_item_spacing();

   size sSeparator = get_separator_size();

   point cur(rBorder.left, rBorder.top);

   size sizeResult(rBorder.left, rBorder.top);

   int32_t buttonx, buttony;

   int iC = MIN(nCount, m_itema.get_size());

   rect rItemPad = get_item_pad();

   string str;

   bool bFirstInRow = true;

   int iRowStart = 0;

   int i;

   for (i = 0; i < iC; i++)
   {

      if (!bFirstInRow)
      {

         cur.x += sSpacing.cx;

      }

      bFirstInRow = false;

      ::user::toolbar_item & item = m_itema(i);

      int32_t cySep = item.m_iImage;

      if (m_itema[i]->m_fsStyle & TBSTYLE_SEP)
      {

         buttonx = sSeparator.cx;

         buttony = rItemPad.top + rItemPad.bottom; //  +sPress.cy;

      }
      else
      {

         if (item.m_spdib.is_set())
         {

            buttonx = item.m_spdib->m_size.cx + iImageSpacing + rItemPad.left + rItemPad.right;

            buttony = item.m_spdib->m_size.cy + rItemPad.top + rItemPad.bottom;

         }
         else
         {

            buttonx = rItemPad.left + rItemPad.right;

            buttony = rItemPad.top + rItemPad.bottom;

         }

         sized size;

         GetButtonText(i, str);

         size = pgraphics->GetTextExtent(str);

         buttonx += size.cx + EXTRA_TEXT_CX;

         buttony += size.cy + EXTRA_TEXT_CY;

      }

      if (m_itema[i]->m_fsState & TBSTATE_HIDDEN)
      {

         continue;

      }

      m_itema[i]->m_rect.left = cur.x;

      m_itema[i]->m_rect.right = cur.x + buttonx;

      cur.x += buttonx; //  +sPress.cx;

      sizeResult.cx = MAX(cur.x, sizeResult.cx);

      cur.y = MAX(cur.y, buttony);

      if (m_itema[i]->m_fsState & TBSTATE_WRAP)
      {


         for (int j = iRowStart; j <= i; j++)
         {

            m_itema[i]->m_rect.top = sizeResult.cy;

            m_itema[i]->m_rect.bottom = sizeResult.cy + cur.y;

         }

         if (sizeResult.cy > rBorder.top)
         {

            cur.y += sSpacing.cy;

         }

         // cur.y += sPress.cy;

         iRowStart = i + 1;

         sizeResult.cy += cur.y;

         cur.x = rBorder.left;

         cur.y = 0;

         if (m_itema[i]->m_fsStyle & TBSTYLE_SEP)
         {

            sizeResult.cy += sSeparator.cy;

         }

         bFirstInRow = true;

      }

   }

   for (int j = iRowStart; j < i; j++)
   {

      m_itema[j]->m_rect.top = sizeResult.cy;

      m_itema[j]->m_rect.bottom = sizeResult.cy + cur.y;

   }

   // sizeResult.cy += sPress.cy;

   sizeResult.cy += cur.y;

   sizeResult.cx += rBorder.right;

   sizeResult.cy += rBorder.bottom;

   return sizeResult;

}




void simple_toolbar::_001DrawItem(::draw2d::graphics * pgraphics, int32_t iItem)
{

   if (m_puserschemaSchema != NULL)
   {

      if (m_puserschemaSchema->_001DrawToolbarItem(pgraphics, iItem, this))
      {

         return;

      }

   }

   return _001DrawSimpleToolbarItem(pgraphics, iItem);


}


void simple_toolbar::_001DrawSimpleToolbarItem(::draw2d::graphics * pgraphics, int32_t iItem)
{

   pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);

   rect rectItem;

   rect rectImage;

   select_font(pgraphics, ::user::font_toolbar, this);

   m_dFontSize = pgraphics->m_spfont->m_dFontSize;

   ::user::toolbar_item & item = m_itema(iItem);

   UINT nStyle = GetButtonStyle(iItem);

   bool bHover = iItem == _001GetHoverItem();

   BaseMenuCentral * pmenucentral = BaseMenuCentral::GetMenuCentral(get_app());

   UINT uiImage = pmenucentral->CommandToImage(item.m_id);

   e_element eelement = element_item;
   e_element eelementImage = element_image;
   e_element eelementText = element_text;
   if ((nStyle & TBBS_SEPARATOR) == 0)
   {
      if ((nStyle & TBBS_DISABLED) == 0)
      {
         // item is enabled
         if (m_iButtonPressItem >= 0)
         {
            if (iItem == m_iButtonPressItem)
            {
               if (bHover)
               {
                  eelement = ElementItemPress;
                  eelementImage = ElementImagePress;
                  eelementText = ElementTextPress;
               }
               else
               {
                  eelement = ElementItemHover;
                  eelementImage = ElementImageHover;
                  eelementText = ElementTextHover;
               }
            }
         }
         else if (bHover)
         {
            eelement = ElementItemHover;
            eelementImage = ElementImageHover;
            eelementText = ElementTextHover;
         }
      }
      else
      {
         // item is disabled
         eelement = element_item;
         eelementImage = element_image;
         eelementText = element_text;
      }
   }
   else
   {
      eelement = element_item;
      eelementImage = element_image;
      eelementText = element_text;
   }


   int iOffsetX = 0;
   int iOffsetY = 0;

   _001GetElementRect(iItem, rectItem, eelement);

   _001GetElementRect(iItem, rectImage, eelementImage);

   if ((nStyle & TBBS_SEPARATOR) != 0)
   {
      rect rectSeparator;
      rectSeparator.left = (rectImage.left + rectImage.right) / 2 - 1;
      rectSeparator.right = rectSeparator.left + 2;
      rectSeparator.top = rectImage.top;
      rectSeparator.bottom = rectImage.bottom;
      pgraphics->Draw3dRect(rectSeparator, ARGB(255, 92, 92, 92), ARGB(255, 255, 255, 255));
   }
   else
   {
      if (eelement == ElementItemHover)
      {
         if ((nStyle & TBBS_CHECKED) != 0)
         {

            _001GetElementRect(iItem, rectItem, element_item);

            _001GetElementRect(iItem, rectImage, element_image);

            if ((m_dwCtrlStyle & TBSTYLE_FLAT) == TBSTYLE_FLAT)
            {
               System.visual().imaging().color_blend(
                  pgraphics,
                  rectItem.left,
                  rectItem.top,
                  rectItem.width(),
                  rectItem.height(),
                  RGB(255, 255, 250), 208);

               pgraphics->Draw3dRect(rectItem, ARGB(255, 127, 127, 127), ARGB(255, 255, 255, 255));
            }
            
            if (uiImage != 0xffffffffu)
            {
               
               if ((nStyle & TBBS_DISABLED) == 0)
               {
                  
                  // button is enabled
                  pmenucentral->MenuV033GetImageListBlend()->draw(pgraphics, uiImage, rectImage.top_left(), 0);

               }
               else
               {
               
                  // button is disabled
                  pmenucentral->MenuV033GetImageListHueLight()->draw(pgraphics, uiImage, rectImage.top_left(), 0);

               }

            }

         }
         else
         {

            rect rectShadow;

            _001GetElementRect(iItem, rectShadow, ElementItemHover);

            if ((m_dwCtrlStyle & TBSTYLE_FLAT) == TBSTYLE_FLAT)
            {

               ::draw2d::pen_sp pen(pgraphics, 1, ARGB(184, 92, 184, 92));
               ::draw2d::brush_sp brush(allocer(), ARGB(123, 177, 184, 255));
               ::draw2d::pen * ppenOld = pgraphics->SelectObject(pen);
               ::draw2d::brush * pbrushOld = pgraphics->SelectObject(brush);
               pgraphics->Rectangle(rectItem);
               pgraphics->SelectObject(ppenOld);
               pgraphics->SelectObject(pbrushOld);

            }

            if (item.m_spdib.is_set())
            {
               
               rect rect;
               
               _001GetElementRect(iItem, rect, ElementImageHover);
               
               System.visual().imaging().color_blend(pgraphics, rect.top_left(), rect.size(), item.m_spdib->get_graphics(), null_point(), 0.84);

            }
            else if (uiImage != 0xffffffffu)
            {
               
               rect rect;
               
               _001GetElementRect(iItem, rect, ElementItemHover);

               pmenucentral->MenuV033GetImageListHue()->draw(pgraphics, uiImage, rect.top_left(), 0);

               pmenucentral->MenuV033GetImageList()->draw(pgraphics, uiImage, rectImage.top_left(), 0);
            }

         }

      }
      else if (eelement == ElementItemPress)
      {

         if ((m_dwCtrlStyle & TBSTYLE_FLAT) == TBSTYLE_FLAT)
         {

            ::draw2d::pen_sp pen(pgraphics, 1, ARGB(255, 92, 92, 92));
            ::draw2d::brush_sp brush(allocer(), ARGB(255, 255, 255, 255));
            ::draw2d::pen * ppenOld = pgraphics->SelectObject(pen);
            ::draw2d::brush * pbrushOld = pgraphics->SelectObject(brush);
            pgraphics->Rectangle(rectItem);
            pgraphics->SelectObject(ppenOld);
            pgraphics->SelectObject(pbrushOld);

         }

         if (item.m_spdib.is_set())
         {
            
            rect rect;
            
            _001GetElementRect(iItem, rect, ElementImagePress);

            System.visual().imaging().color_blend(pgraphics, rect.top_left(), rect.size(), item.m_spdib->get_graphics(), null_point(), 1.0);

         }
         else if (uiImage != 0xffffffff)
         {
            
            pmenucentral->MenuV033GetImageList()->draw(pgraphics, uiImage, rectImage.top_left(), 0);

         }

      }
      else
      {

         if ((nStyle & TBBS_DISABLED) == 0)
         {
            
            _001GetElementRect(iItem, rectItem, element_item);

            pgraphics->FillSolidRect(rectItem, ARGB(184, 255, 255, 255));

         }

         if ((nStyle & TBBS_CHECKED) != 0)
         {

            pgraphics->Draw3dRect(rectItem, ARGB(255, 127, 127, 127), ARGB(255, 255, 255, 255));

         }

         if (item.m_spdib.is_set())
         {
            
            rect rect;
            
            _001GetElementRect(iItem, rect, element_image);

            System.visual().imaging().color_blend(pgraphics, rect.top_left(), rect.size(), item.m_spdib->get_graphics(), null_point(), 0.23);

         }
         else if (uiImage != 0xffffffff)
         {

            if ((nStyle & TBBS_DISABLED) == 0)
            {
               
               pmenucentral->MenuV033GetImageListBlend()->draw(pgraphics, uiImage, rectImage.top_left(), 0);

            }
            else
            {
               
               pmenucentral->MenuV033GetImageListHueLight()->draw(pgraphics, uiImage, rectImage.top_left(), 0);

            }

         }

      }

   }

   if (item.m_str.has_char())
   {

      select_font(pgraphics, ::user::font_toolbar, this);

      m_dFontSize = pgraphics->m_spfont->m_dFontSize;

      rect rectText;

      ::draw2d::brush_sp brushText(allocer());

      if ((nStyle & TBBS_DISABLED) == 0)
      {

         brushText->create_solid(ARGB(255, 0, 0, 0));

      }
      else
      {

         brushText->create_solid(ARGB(255, 123, 123, 118));

      }
      
      pgraphics->SelectObject(brushText);

      if (_001GetElementRect(iItem, rectText, eelementText) && rectText.right > 0)
      {

         pgraphics->_DrawText(item.m_str, item.m_str.get_length(), rectText, DT_BOTTOM | DT_LEFT | DT_NOPREFIX);

      }

   }

}


bool simple_toolbar::_001GetElementRect(int32_t iItem, LPRECT lprect, e_element eelement)
{

   if (iItem < 0 || iItem >= m_itema.get_size())
   {

      return false;

   }

   rect rItemPad = get_item_pad();

   size sPress = get_press_shift();

   int iImageSpacing = get_image_spacing();

   rect rect;

   ::user::toolbar_item & item = m_itema(iItem);

   BaseMenuCentral * pmenucentral = BaseMenuCentral::GetMenuCentral(get_app());

   UINT uiImage = pmenucentral->CommandToImage(item.m_id);

   if ((item.m_fsStyle & TBSTYLE_SEP) != 0)
   {

      rect = item.m_rect;

   }
   else
   {

      switch (eelement)
      {
      case element_item:
      case ElementItemHover:
      case ElementItemPress:

         rect = item.m_rect;

         break;
      case element_image:
      case ElementImageHover:
      case ElementImagePress:

         if (item.m_spdib.is_null() || item.m_spdib->area() <= 0)
         {

            return false;

         }

         rect = item.m_rect;

         rect.left += rItemPad.left;
         rect.bottom -= rItemPad.bottom;
         rect.top = rect.bottom - item.m_spdib->m_size.cy;
         rect.right = rect.left + item.m_spdib->m_size.cx;

         break;
      case element_text:
      case ElementTextHover:
      case ElementTextPress:
      {

         rect = item.m_rect;

         rect.left += rItemPad.left;

         if (item.m_spdib.is_set() && item.m_spdib->area() > 0)
         {

            rect.left += item.m_spdib->m_size.cx;

            rect.left += iImageSpacing;

         }

         rect.top += rItemPad.top;
         rect.right -= (rItemPad.right);
         rect.bottom -= (rItemPad.bottom);
         break;

      }
      }
      switch (eelement)
      {
      case element_item:
      case element_image:
      case element_text:
         //rect.offset(rItemPad.left, rItemPad.top);
         break;
      case ElementItemHover:
      case ElementImageHover:
      case ElementTextHover:
//         rect.offset(HOVER_CX, HOVER_CY);
         break;
      case ElementItemPress:
      case ElementImagePress:
      case ElementTextPress:
      {
         size sPress = get_press_shift();
         rect.offset(sPress.cx, sPress.cy);
      }
         break;
      default:
         break;
      }

   }
   *lprect = rect;

   return true;

}




void simple_toolbar::SetSizes(SIZE sizeButton, SIZE sizeImage)
{
   ASSERT_VALID(this);

   // sizes must be non-zero and positive
   ASSERT(sizeButton.cx > 0 && sizeButton.cy > 0);
   ASSERT(sizeImage.cx > 0 && sizeImage.cy > 0);

   // button must be big enough to hold image
   //   + 7 pixels on x
   //   + 6 pixels on y
   ASSERT(sizeButton.cx >= sizeImage.cx + 7);
   ASSERT(sizeButton.cy >= sizeImage.cy + 6);

   //   if (::IsWindow(get_handle()))
   if (false)
   {
      // set the sizes via TB_SETBITMAPSIZE and TB_SETBUTTONSIZE
#ifdef WINDOWSEX
      VERIFY(send_message(TB_SETBITMAPSIZE, 0, MAKELONG(sizeImage.cx, sizeImage.cy)));
      VERIFY(send_message(TB_SETBUTTONSIZE, 0, MAKELONG(sizeButton.cx, sizeButton.cy)));
#else
      throw todo(get_app());
#endif

      Invalidate();   // just to be nice if called when toolbar is visible
   }
   else
   {
      // just set our internal values for later
      m_sizeButton = sizeButton;
      m_sizeImage = sizeImage;
   }
}

/*
bool simple_toolbar::LoadBitmap(const char * lpszResourceName)
{
ASSERT_VALID(this);
ASSERT(lpszResourceName != NULL);

// determine location of the bitmap in resource fork
HINSTANCE hInstImageWell = ::core::FindResourceHandle(lpszResourceName, RT_BITMAP);
HRSRC hRsrcImageWell = ::FindResource(hInstImageWell, lpszResourceName, RT_BITMAP);
if (hRsrcImageWell == NULL)
return FALSE;

// load the bitmap
HBITMAP hbmImageWell;
//   hbmImageWell = ::core::LoadSysColorBitmap(hInstImageWell, hRsrcImageWell);
::draw2d::memory_graphics pgraphics(this);;
hbmImageWell = imaging::LoadSysColorBitmap(pgraphics, hInstImageWell, hRsrcImageWell);


// tell common control toolbar about the new bitmap
//   if (!AddReplaceBitmap(hbmImageWell))
//      return FALSE;

// remember the resource handles so the bitmap can be recolored if necessary
m_hInstImageWell = hInstImageWell;
m_hRsrcImageWell = hRsrcImageWell;

_001DiscardImageList();

   m_pimagelist = new image_list();
m_bInternalImageList = true;

m_pimagelist->create(
(char *) lpszResourceName,
m_sizeImage.cx,
1,
RGB(192, 192, 192));

_001OnImageListAttrib();*/

/*
return TRUE;
}
*/

void simple_toolbar::on_layout()
{
   //ASSERT(m_bDelayedButtonLayout);

   synch_lock ml(m_pmutex);

   if (m_puserschemaSchema == NULL)
   {

      m_puserschemaSchema = GetTopLevel();

   }

   if (m_puserschemaSchema == NULL)
   {

      m_puserschemaSchema = Application.userschema();

   }

   m_bDelayedButtonLayout = false;

   bool bHorz = (m_dwStyle & CBRS_ORIENT_HORZ) != 0;

   if ((m_dwStyle & CBRS_FLOATING) && (m_dwStyle & CBRS_SIZE_DYNAMIC))
      CalcDynamicLayout(0, LM_HORZ | LM_MRUWIDTH | LM_COMMIT);
   else if (bHorz)
      CalcDynamicLayout(0, LM_HORZ | LM_HORZDOCK | LM_COMMIT);
   else
      CalcDynamicLayout(0, LM_VERTDOCK | LM_COMMIT);


   if (m_dwCtrlStyle &TBSTYLE_ALIGN_CENTER)
   {

      output_debug_string("please_center_align");

      rect rectClient;

      GetClientRect(rectClient);

      size s = CalcSize(m_itema.get_count());

      for (index i = 0; i < m_itema.get_count(); i++)
      {

         if (m_itema[i]->m_fsStyle & TBSTATE_WRAP || i == m_itema.get_upper_bound())
         {

            int iTotalX = 0;
            for (index j = 0; j <= i; j++)
            {

               iTotalX += m_itema[j]->m_rect.width() + get_item_spacing().cx;


            }

            int offsetx = (rectClient.width() - iTotalX) / 2;

            for (index j = 0; j <= i; j++)
            {

               m_itema[j]->m_rect.offset(offsetx, 0);


            }

         }

      }


   }


}


void simple_toolbar::_001OnMouseMove(signal_details * pobj)
{
   SCAST_PTR(::message::mouse, pmouse, pobj);
   point pt = pmouse->m_pt;
   ScreenToClient(&pt);
   if (m_bDockTrack)
   {
      pobj->previous();
   }
   else
   {
      _001Hover(pt);
      if (m_iHover >= 0)
      {
         pmouse->m_bRet = true;
         pmouse->set_lresult(1);
      }
      pobj->previous();
   }
}

void simple_toolbar::_001OnLButtonDown(signal_details * pobj)
{
   SCAST_PTR(::message::mouse, pmouse, pobj);
   point pt = pmouse->m_pt;
   ScreenToClient(&pt);
   m_iButtonPressItem = _001HitTest(pt);

   if (m_iButtonPressItem >= 0)
   {
      pmouse->m_bRet = true;
      pmouse->set_lresult(1);
   }

   RedrawWindow();

   pobj->previous();
}

void simple_toolbar::_001OnLButtonUp(signal_details * pobj)
{
   SCAST_PTR(::message::mouse, pmouse, pobj);
   point pt = pmouse->m_pt;
   ScreenToClient(&pt);
   if (m_bDockTrack)
   {
      pobj->previous();
   }
   else
   {
      int32_t iItem = _001HitTest(pt);
      if (iItem >= 0 && iItem == m_iButtonPressItem)
      {
         _001OnClick(iItem);
         pmouse->m_bRet = true;
         pmouse->set_lresult(1);
      }
      sp(::user::frame_window) pTarget = GetOwner();
      if (pTarget == NULL)
         pTarget = GetParentFrame();
      if (pTarget != NULL)
         pTarget->SendMessageToDescendants(WM_IDLEUPDATECMDUI);
      m_iButtonPressItem = -1;
      RedrawWindow();
      if (pobj->m_bRet)
      {
         return;
      }
      pobj->previous();
   }
}


int32_t simple_toolbar::_001HitTest(point pt)
{
   for (int32_t iItem = 0; iItem < m_itema.get_size(); iItem++)
   {
      if (m_itema[iItem]->m_rect.contains(pt))
         return iItem;
   }
   if (Session.GetCapture() == this)
      return -1;
   rect rectWindow;
   GetWindowRect(rectWindow);
   ScreenToClient(rectWindow);
   if (rectWindow.contains(pt))
      return -1;
   else
      return 0x80000000;
}


void simple_toolbar::_001Hover(point pt, bool bRedraw)
{
   int32_t iHover = _001HitTest(pt);
   if (m_iHover != iHover)
   {
      track_mouse_leave();
      m_iHover = iHover;
      OnUpdateHover();
      //      if(m_iHover < -1)
      //    {
      //     KillTimer(TIMER_HOVER);
      //      }
      //    else
      //  {
      //   SetTimer(TIMER_HOVER, 50, NULL);
      //}

      if (bRedraw)
      {
         RedrawWindow();
      }
   }
}

void simple_toolbar::_001Hover(bool bRedraw)
{
   point pt;
   GetCursorPos(&pt);
   ScreenToClient(&pt);
   _001Hover(pt, bRedraw);
}

void simple_toolbar::_001OnTimer(::timer * ptimer)
{
   ::user::toolbar::_001OnTimer(ptimer);
   if (ptimer->m_nIDEvent == TIMER_HOVER)
   {
      _001Hover();
   }

   // trans ::user::control_bar::OnTimer(ptimer->m_nIDEvent);
}

void simple_toolbar::_001OnClick(index iItem)
{
   sp(::user::interaction) pwnd = GetOwner();
   pwnd->_001SendCommand(m_itema[iItem]->m_id);
}

void simple_toolbar::_001DiscardImageList()
{
   /*   if(m_bInternalImageList)
   {
   if(m_pimagelist != NULL)
   {
   delete m_pimagelist;
   m_pimagelist = NULL;
   }
   }
   else
   {
   m_pimagelist = NULL;
   }*/
}

void simple_toolbar::_001SetImageList(sp(image_list) pimagelist)
{
   UNREFERENCED_PARAMETER(pimagelist);
   /*   m_pimagelist = pimagelist;
   m_bInternalImageList = false;
   _001OnImageListAttrib();*/
}


void simple_toolbar::_001OnImageListAttrib()
{
   /*
   if(m_pimagelistHue == NULL)
   {
   m_pimagelistHue = new image_list();
   }
   ::draw2d::graphics_sp spgraphics(allocer());
   spgraphics->CreateDC("DISPLAY", NULL, NULL, NULL);
   System.visual().imaging().CreateHueImageList(
   &spgraphics,
   m_pimagelistHue,
   m_pimagelist,
   RGB(192, 192, 180),
   0.50);

   if(m_pimagelistBlend == NULL)
   {
   m_pimagelistBlend = new image_list();
   }
   System.visual().imaging().Createcolor_blend_ImageList(
   m_pimagelistBlend,
   m_pimagelist,
   RGB(255, 255, 240),
   64);

   if(m_pimagelistHueLight == NULL)
   {
   m_pimagelistHueLight = new image_list();
   }
   System.visual().imaging().CreateHueImageList(
   &spgraphics,
   m_pimagelistHueLight,
   m_pimagelist,
   RGB(220, 220, 215),
   0.50);
   */
}


/////////////////////////////////////////////////////////////////////////////
// simple_toolbar idle update through SimpleToolCmdUI class

SimpleToolCmdUI::SimpleToolCmdUI(::aura::application * papp) :
   object(papp),
   cmd_ui(papp)
{
}
void SimpleToolCmdUI::Enable(bool bOn, ::action::context actioncontext)
{
   m_bEnableChanged = TRUE;
   simple_toolbar* pToolBar = dynamic_cast <simple_toolbar *> (m_pOther);
   ASSERT(pToolBar != NULL);
   //   ASSERT_KINDOF(simple_toolbar, pToolBar);
   ASSERT(m_iIndex < m_iCount);
   UINT nNewStyle = pToolBar->GetButtonStyle((int32_t)m_iIndex) & ~TBBS_DISABLED;
   if (!bOn)
   {
      nNewStyle |= TBBS_DISABLED;
      // WINBUG: If a button is currently pressed and then is disabled
      // COMCTL32.DLL does not unpress the button, even after the mouse
      // button goes up!  We work around this bug by forcing TBBS_PRESSED
      // off when a button is disabled.
      nNewStyle &= ~TBBS_PRESSED;
   }
   ASSERT(!(nNewStyle & TBBS_SEPARATOR));
   pToolBar->SetButtonStyle((int32_t)m_iIndex, nNewStyle);
}

void SimpleToolCmdUI::_001SetCheck(check::e_check echeck, ::action::context actioncontext)
{
   ASSERT(echeck == check::checked
      || echeck == check::unchecked
      || echeck == check::tristate); // 0=>off, 1=>on, 2=>indeterminate
   simple_toolbar* pToolBar = dynamic_cast <simple_toolbar *> (m_pOther);
   ASSERT(pToolBar != NULL);
   ASSERT_KINDOF(simple_toolbar, pToolBar);
   ASSERT(m_iIndex < m_iCount);

   UINT nNewStyle = pToolBar->GetButtonStyle((int32_t)m_iIndex) &
      ~(TBBS_CHECKED | TBBS_INDETERMINATE);
   if (echeck == check::checked)
      nNewStyle |= TBBS_CHECKED;
   else if (echeck == check::tristate)
      nNewStyle |= TBBS_INDETERMINATE;
   ASSERT(!(nNewStyle & TBBS_SEPARATOR));
   pToolBar->SetButtonStyle((int32_t)m_iIndex, nNewStyle | TBBS_CHECKBOX);
}

void SimpleToolCmdUI::SetText(const char *, ::action::context)
{
   // ignore it
}

int32_t simple_toolbar::GetItemStyle(int32_t iItem)
{
   return m_itema[iItem]->m_fsStyle;
}

bool simple_toolbar::SetItemStyle(int32_t iItem, BYTE bStyle)
{
   UNREFERENCED_PARAMETER(iItem);
   UNREFERENCED_PARAMETER(bStyle);
   return false;
}


UINT simple_toolbar::GetButtonStyle(int32_t nIndex)
{

   sp(::user::toolbar_item) pitem = m_itema[nIndex];

   if (pitem.is_null())
      return 0;

   return MAKELONG(pitem->m_fsStyle, pitem->m_fsState);

}


void simple_toolbar::SetButtonStyle(int32_t nIndex, UINT nStyle)
{

   sp(::user::toolbar_item) pitem = m_itema[nIndex];

   if (pitem.is_null())
      return;

   if (pitem->m_fsStyle != (BYTE)LOWORD(nStyle) || pitem->m_fsState != (BYTE)HIWORD(nStyle))
   {
      pitem->m_fsStyle = (BYTE)LOWORD(nStyle);
      pitem->m_fsState = (BYTE)HIWORD(nStyle);
      m_bDelayedButtonLayout = TRUE;
   }

}


void simple_toolbar::_001OnNcCalcSize(signal_details * pobj)
{
#if defined(WINDOWSEX) //|| defined(LINUX)
   SCAST_PTR(::message::nc_calc_size, pnccalcsize, pobj);
   // calculate border space (will add to top/bottom, subtract from right/bottom)
   class rect rect;
   rect.null();
   bool bHorz = (m_dwStyle & CBRS_ORIENT_HORZ) != 0;
   ::user::control_bar::CalcInsideRect(rect, bHorz);

   // adjust non-client area for border space
   pnccalcsize->m_pparams->rgrc[0].left += rect.left;
   pnccalcsize->m_pparams->rgrc[0].top += rect.top;
   pnccalcsize->m_pparams->rgrc[0].right += rect.right;
   pnccalcsize->m_pparams->rgrc[0].bottom += rect.bottom;
#else
   throw todo(get_app());
#endif
}


void simple_toolbar::_001OnNcHitTest(signal_details * pobj)
{
   SCAST_PTR(::message::nchittest, pnchittest, pobj);
   pnchittest->set_lresult(HTCLIENT);
}


#define CX_OVERLAP  0

int32_t simple_toolbar::WrapToolBar(int32_t nCount, int32_t nWidth)
{

   int32_t nResult = 0;

   ASSERT(nCount > 0);

   ::draw2d::memory_graphics pgraphics(allocer());

   select_font(pgraphics, ::user::font_toolbar, this);

   m_dFontSize = pgraphics->m_spfont->m_dFontSize;

   int iImageSpacing = get_image_spacing();

   size sSeparator = get_separator_size();

   size sSpacing = get_item_spacing();

   size sPress = get_press_shift();

   rect rItemPad = get_item_pad();

   rect rBorder = get_bar_border();

   int32_t x = rBorder.left;

   string str;

   int iC = MIN(nCount, m_itema.get_count());

   bool bFirstInRow = true;

   for (int32_t i = 0; i < iC; i++)
   {

      m_itema[i]->m_fsState &= ~TBSTATE_WRAP;

      if (m_itema[i]->m_fsState & TBSTATE_HIDDEN)
         continue;

      GetButtonText(i, str);

      int32_t dx, dxNext;

      if (m_itema[i]->m_fsStyle & TBSTYLE_SEP)
      {

         dx = sSeparator.cx;

         dxNext = dx;

      }
      else
      {

         dx = rItemPad.left;

         if (m_itema[i]->m_spdib.is_set())
         {

            dx += m_itema[i]->m_spdib->m_size.cy;

         }

         if (str.has_char() && m_itema[i]->m_spdib.is_set())
         {

            dx += iImageSpacing;

         }

         if (str.has_char())
         {

            sized size = pgraphics->GetTextExtent(str);

            dx += size.cx + EXTRA_TEXT_CX;

         }

         dx += rItemPad.right; // +sPress.cx;

         dxNext = dx - CX_OVERLAP;

      }

      bool bFound = FALSE;

      if (x + dx > nWidth - rBorder.right)
      {

         for (int32_t j = i; j >= 0 && !(m_itema[j]->m_fsState & TBSTATE_WRAP); j--)
         {
            // find last separator that isn't hidden
            // a separator that has a command ID is not
            // a separator, but a custom control.
            if ((m_itema[j]->m_fsStyle & TBSTYLE_SEP) &&
               (m_itema[j]->m_id == "separator") &&
               !(m_itema[j]->m_fsState & TBSTATE_HIDDEN))
            {
               bFound = TRUE;
               i = j;
               x = rBorder.left;
               bFirstInRow = true;
               m_itema[j]->m_fsState |= TBSTATE_WRAP;
               nResult++;
               break;
            }
         }

         if (!bFound)
         {
            for (int32_t j = i - 1; j >= 0 && !(m_itema[j]->m_fsState & TBSTATE_WRAP); j--)
            {
               // Never wrap anything that is hidden,
               // or any custom controls
               if ((m_itema[j]->m_fsState & TBSTATE_HIDDEN) ||
                  ((m_itema[j]->m_fsStyle & TBSTYLE_SEP) &&
                  (m_itema[j]->m_id != "separator")))
                  continue;

               bFound = TRUE;
               i = j;
               x = rBorder.left;
               bFirstInRow = true;
               m_itema[j]->m_fsState |= TBSTATE_WRAP;
               nResult++;
               break;
            }

         }

      }

      if (!bFound)
      {

         if (!bFirstInRow)
         {

            x += sSpacing.cx;

         }

         bFirstInRow = false;

         x += dxNext;

      }

   }

   return nResult + 1;

}


void  simple_toolbar::SizeToolBar(int32_t nCount, int32_t nLength, bool bVert)
{
   ASSERT(nCount > 0);

   if (!bVert)
   {
      int32_t nMin, nMax, nTarget, nCurrent, nMid;

      // Wrap ToolBar as specified
      nMax = nLength;
      nTarget = WrapToolBar(nCount, nMax);

      // Wrap ToolBar vertically
      nMin = 0;
      nCurrent = WrapToolBar(nCount, nMin);

      if (nCurrent != nTarget)
      {
         while (nMin < nMax)
         {
            nMid = (nMin + nMax) / 2;
            nCurrent = WrapToolBar(nCount, nMid);

            if (nCurrent == nTarget)
               nMax = nMid;
            else
            {
               if (nMin == nMid)
               {
                  WrapToolBar(nCount, nMax);
                  break;
               }
               nMin = nMid;
            }
         }
      }
      size size = CalcSize(nCount);
      WrapToolBar(nCount, size.cx);
      CalcSize(nCount);
   }
   else
   {
      size sizeMax, sizeMin, sizeMid;

      // Wrap ToolBar vertically
      WrapToolBar(nCount, 0);
      sizeMin = CalcSize(nCount);

      // Wrap ToolBar horizontally
      WrapToolBar(nCount, 32767);
      sizeMax = CalcSize(nCount);

      while (sizeMin.cx < sizeMax.cx)
      {
         sizeMid.cx = (sizeMin.cx + sizeMax.cx) / 2;
         WrapToolBar(nCount, sizeMid.cx);
         sizeMid = CalcSize(nCount);

         if (nLength < sizeMid.cy)
         {
            if (sizeMin == sizeMid)
            {
               WrapToolBar(nCount, sizeMax.cx);
               return;
            }
            sizeMin = sizeMid;
         }
         else if (nLength > sizeMid.cy)
            sizeMax = sizeMid;
         else
            return;
      }
   }
}

struct ___CONTROLPOS
{
   int32_t nIndex;
   string strId;
   rect rectOldPos;
};


size simple_toolbar::CalcLayout(uint32_t dwMode, int32_t nLength)
{
   sp(::user::tab) ptab = GetTypedParent < ::user::tab >();

   if (ptab.is_set())
   {

      if (!ptab->m_bShowTabs)
      {
         return size(0, 0);
      }

   }

   ASSERT_VALID(this);
   ASSERT(IsWindow());
   if (dwMode & LM_HORZDOCK)
      ASSERT(dwMode & LM_HORZ);

   int32_t nCount;
   //TBBUTTON* m_itema = NULL;
   size sizeResult(0, 0);

   nCount = _001GetItemCount();


   if (nCount > 0)
   {
      if (!(m_dwStyle & CBRS_SIZE_FIXED))
      {
         bool bDynamic = (m_dwStyle & CBRS_SIZE_DYNAMIC) != 0;

         if (bDynamic && (dwMode & LM_MRUWIDTH))
            SizeToolBar(nCount, m_nMRUWidth);
         else if (bDynamic && (dwMode & LM_HORZDOCK))
            SizeToolBar(nCount, 32767);
         else if (bDynamic && (dwMode & LM_VERTDOCK))
            SizeToolBar(nCount, 0);
         else if (bDynamic && (nLength != -1))
         {
            class rect rect;
            rect.null();
            CalcInsideRect(rect, (dwMode & LM_HORZ) != 0);
            bool bVert = (dwMode & LM_LENGTHY) != 0;
            int32_t nLen = nLength + (bVert ? rect.height() : rect.width());

            SizeToolBar(nCount, nLen, bVert);
         }
         else if (bDynamic && (m_dwStyle & CBRS_FLOATING))
            SizeToolBar(nCount, m_nMRUWidth);
         else
            SizeToolBar(nCount, (dwMode & LM_HORZ) ? 32767 : 0);
      }

      sizeResult = CalcSize(nCount);

      if (dwMode & LM_COMMIT)
      {
         ___CONTROLPOS* pControl = NULL;
         int32_t nControlCount = 0;
         bool bIsDelayed = m_bDelayedButtonLayout;
         m_bDelayedButtonLayout = FALSE;

         int32_t i;
         for (i = 0; i < nCount; i++)
            if ((m_itema[i]->m_fsStyle & TBSTYLE_SEP) && (m_itema[i]->m_id != "separator"))
               nControlCount++;

         if (nControlCount > 0)
         {
            pControl = new ___CONTROLPOS[nControlCount];
            nControlCount = 0;

            for (int32_t i = 0; i < nCount; i++)
            {
               if ((m_itema[i]->m_fsStyle & TBSTYLE_SEP) && (m_itema[i]->m_id != "separator"))
               {
                  pControl[nControlCount].nIndex = i;
                  pControl[nControlCount].strId = m_itema[i]->m_id;

                  rect rect;
                  _001GetItemRect(i, &rect);
                  ClientToScreen(&rect);
                  pControl[nControlCount].rectOldPos = rect;

                  nControlCount++;
               }
            }
         }

         //::draw2d::memory_graphics pgraphics(this);
         string str;
         if ((m_dwStyle & CBRS_FLOATING) && (m_dwStyle & CBRS_SIZE_DYNAMIC))
            m_nMRUWidth = sizeResult.cx;
         for (i = 0; i < nCount; i++)
         {
            //            _SetButton(i, &m_itema[i]);
            //            GetButtonText(i, str);
            //            size size;
            //            if(!str.is_empty())
            //            {
            //               ::GetTextExtentPointW(
            //                  (HDC)pgraphics->get_os_data(),
            //                  str,
            //                  str.get_length(),
            //                  &size);
            //               size.cx += m_sizeButton.cx;
            //            }
            //            else
            //            {
            //               size = m_sizeImage;
            //            }
            //            TBBUTTONINFOW button;
            //            memset(&button, 0, sizeof(button));
            //            button.cbSize = sizeof(button);
            //            button.dwMask =
            //               TBIF_COMMAND
            //               | TBIF_SIZE  ;
            //            button.cx = size.cx;
            //            button.m_id = i;
            //            GetToolBarCtrl().SetButtonInfo(i, &button);
         }
         //for (i = 0; i < nCount; i++)
         {
            //            TBBUTTONINFOW buttona;
              //          memset(&buttona, 0, sizeof(buttona));
                //        buttona.cbSize = sizeof(buttona);
                  //      buttona.dwMask =
                    //       TBIF_COMMAND
                      //     | TBIF_STYLE
                        //   | TBIF_SIZE;
                        //            UINT uiID = GetItemID(i);
                        ///            GetToolBarCtrl().GetButtonInfo(uiID, &buttona);
                        //         TRACE("BUTTON.m_id = %d\n", buttona.m_id  );
                        //         TRACE("BUTTON.m_fsStyle = %d\n", buttona.m_fsStyle  );
                        //         TRACE("BUTTON.cx = %d\n", buttona.cx );
         }

         if (nControlCount > 0)
         {
            for (int32_t i = 0; i < nControlCount; i++)
            {
               /* xxx sp(::user::interaction) pwindow = get_child_by_id(pControl[i]->strId);
               if (pwindow != NULL)
               {
               rect rect;
               pwindow->GetWindowRect(&rect);
               point pt = rect.top_left() - pControl[i]->rectOldPos.top_left();
               _001GetElementRect(pControl[i]->nIndex, &rect);
               pt = rect.top_left() + pt;
               pwindow->SetWindowPos(0, pt.x, pt.y, 0, 0, SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOZORDER);
               }*/
            }
            delete[] pControl;
         }
         m_bDelayedButtonLayout = bIsDelayed != 0;
      }
      //delete[] m_itema;
   }

   //BLOCK: Adjust Margins
   {
      class rect rect;
      rect.null();
      CalcInsideRect(rect, (dwMode & LM_HORZ) != 0);
      sizeResult.cy -= rect.height();
      sizeResult.cx -= rect.width();

      size size = ::user::control_bar::CalcFixedLayout((dwMode & LM_STRETCH) != 0, (dwMode & LM_HORZ) != 0);
      sizeResult.cx = MAX(sizeResult.cx, size.cx);
      sizeResult.cy = MAX(sizeResult.cy, size.cy);
   }

   return sizeResult;

}


void simple_toolbar::GetButtonText(int32_t i, string &str)
{

   if (i < 0 || i >= m_itema.get_count())
   {

      str.Empty();

      return;

   }

   if (m_itema[i].is_null())
   {

      str.Empty();

      return;

   }

   str = m_itema[i]->m_str;

}


void simple_toolbar::OnBarStyleChange(uint32_t dwOldStyle, uint32_t dwNewStyle)
{
   // a dynamically resizeable toolbar can not have the CBRS_FLOAT_MULTI
   ASSERT(!((dwNewStyle & CBRS_SIZE_DYNAMIC) &&
      (m_dwDockStyle & CBRS_FLOAT_MULTI)));

   // a toolbar can not be both dynamic and fixed in size
   ASSERT(!((dwNewStyle & CBRS_SIZE_FIXED) &&
      (dwNewStyle & CBRS_SIZE_DYNAMIC)));

   // CBRS_SIZE_DYNAMIC can not be disabled once it has been enabled
   ASSERT(((dwOldStyle & CBRS_SIZE_DYNAMIC) == 0) ||
      ((dwNewStyle & CBRS_SIZE_DYNAMIC) != 0));

   if (((dwOldStyle & CBRS_BORDER_ANY) != (dwNewStyle & CBRS_BORDER_ANY)))
   {
      // recalc non-client area when border styles change
      SetWindowPos(0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_DRAWFRAME);
   }
   m_bDelayedButtonLayout = TRUE;
}

size simple_toolbar::CalcDynamicLayout(int32_t nLength, uint32_t dwMode)
{
   if ((nLength == -1) && !(dwMode & LM_MRUWIDTH) && !(dwMode & LM_COMMIT) &&
      ((dwMode & LM_HORZDOCK) || (dwMode & LM_VERTDOCK)))
   {
      return CalcFixedLayout((dwMode & LM_STRETCH) != 0, (dwMode & LM_HORZDOCK) != 0);
   }
   return CalcLayout(dwMode, nLength);
}


void simple_toolbar::_001OnMouseLeave(signal_details * pobj)
{
   SCAST_PTR(::message::base, pbase, pobj);
   m_iHover = 0x80000000;
   OnUpdateHover();
   RedrawWindow();
   pbase->set_lresult(0);
}

void simple_toolbar::SetItemImage(int32_t iItem, int32_t iImage)
{
   m_itema[iItem]->m_iImage = iImage;
}

void simple_toolbar::OnUpdateHover()
{

}

int32_t simple_toolbar::_001GetHoverItem()
{
   return m_iHover;
}

void simple_toolbar::RemoveAllTools()
{
   m_itema.remove_all();
}


rect simple_toolbar::get_item_pad()
{

   rect r;

   r = m_rectItemPad;

   if (r.left < 0)
   {

      r.left = m_dFontSize/ 3;

   }
   if (r.right < 0)
   {

      r.right = m_dFontSize / 3;

   }
   if (r.top < 0)
   {

      r.top = m_dFontSize / 3;

   }
   if (r.bottom < 0)
   {

      r.bottom = m_dFontSize / 3;

   }


   return r;





}


rect simple_toolbar::get_bar_border()
{

   rect r;

   r = m_rectBorder;

   if (r.left < 0)
   {

      r.left = m_dFontSize / 3;

   }
   if (r.right < 0)
   {

      r.right = m_dFontSize / 3;

   }
   if (r.top < 0)
   {

      r.top = m_dFontSize / 3;

   }
   if (r.bottom < 0)
   {

      r.bottom = m_dFontSize / 3;

   }


   return r;

}


size simple_toolbar::get_press_shift()
{

   size s;

   s = m_sizePress;

   if (s.cx < 0)
   {

      s.cx = get_item_pad().right - 1 + get_item_spacing().cx;

   }
   if (s.cy < 0)
   {

      s.cy = get_item_pad().bottom - 1 + get_item_spacing().cy;

   }

   return s;

}

size simple_toolbar::get_item_spacing()
{

   size s;

   s = m_sizeSpacing;

   if (s.cx < 0)
   {

      s.cx = 0;

   }
   if (s.cy < 0)
   {

      s.cy = 0;

   }

   return s;

}


size simple_toolbar::get_separator_size()
{

   size s;

   s = m_sizeSeparator;

   if (s.cx < 0)
   {

      s.cx = m_dFontSize * 2 / 3;

   }

   if (s.cy < 0)
   {

      s.cy = m_dFontSize * 2 / 3;

   }

   return s;

}



int simple_toolbar::get_image_spacing()
{

   int i;

   i = m_iImageSpacing;

   if (i < 0)
   {

      i = m_dFontSize / 4;

   }

   return i;

}


