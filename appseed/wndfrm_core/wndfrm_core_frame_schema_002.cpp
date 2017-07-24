#include "framework.h"


#define GRIP_CORNER_LARGE_CX 16
#define GRIP_CORNER_SMALL_CX 5
#define GRIP_CORNER_LARGE_CY 16
#define GRIP_CORNER_SMALL_CY 5

#undef GRIP_CENTER_LARGE_CX
#undef GRIP_CENTER_SMALL_CX
#undef GRIP_CENTER_LARGE_CY
#undef GRIP_CENTER_SMALL_CY

#define GRIP_CENTER_LARGE_CX 36
#define GRIP_CENTER_SMALL_CX 7
#define GRIP_CENTER_LARGE_CY 36
#define GRIP_CENTER_SMALL_CY 7


COLORREF SetAValue(BYTE a, COLORREF cr)
{

   return ARGB(a, argb_get_r_value(cr), argb_get_g_value(cr), argb_get_b_value(cr));

}


namespace user
{


   namespace wndfrm
   {


      namespace frame
      {


         namespace wndfrm_core
         {



            FrameSchemaHardCoded002::FrameSchemaHardCoded002(::aura::application * papp) :
               object(papp),
               frame_schema(papp)
            {

               set_style(StyleLightGreen);

               m_rectControlBoxMarginNormal.top = 0;


            }


            FrameSchemaHardCoded002::~FrameSchemaHardCoded002()
            {

            }


            EHitTest FrameSchemaHardCoded002::_000HitTest(point ptCursor)
            {
               EHitTest etest = HitTestClient;
               {
                  //      m_pworkset->GetEventWindow()->ScreenToClient(pt);
                  rect rectEvent;
                  m_pworkset->GetRegionWindow()->GetWindowRect(rectEvent);
                  rect rect;
                  point ptCenter = rectEvent.center();
                  EGrip egrip = m_pworkset->GetSizingManager()->GetGripMask();

                  if(egrip & GripTopLeft)
                  {
                     rect = rectEvent;
                     rect.right = rect.left + 16;
                     rect.bottom = rect.top + 5;
                     if(rect.contains(ptCursor))
                     {
                        etest = HitTestSizingTopLeft;
                        goto SizingSuccess;
                     }
                     rect = rectEvent;
                     rect.right = rect.left + 5;
                     rect.bottom = rect.top + 16;
                     if(rect.contains(ptCursor))
                     {
                        etest = HitTestSizingTopLeft;
                        goto SizingSuccess;
                     }
                  }
                  if(egrip & GripTopRight)
                  {
                     rect = rectEvent;
                     rect.left = rect.right - 16;
                     rect.bottom = rect.top + 5;
                     if(rect.contains(ptCursor))
                     {
                        etest = HitTestSizingTopRight;
                        goto SizingSuccess;
                     }
                     rect = rectEvent;
                     rect.left = rect.right - 5;
                     rect.bottom = rect.top + 16;
                     if(rect.contains(ptCursor))
                     {
                        etest = HitTestSizingTopRight;
                        goto SizingSuccess;
                     }
                  }
                  if(egrip & GripBottomRight)
                  {
                     rect = rectEvent;
                     rect.left = rect.right - 16;
                     rect.top = rect.bottom - 5;
                     if(rect.contains(ptCursor))
                     {
                        etest = HitTestSizingBottomRight;
                        goto SizingSuccess;
                     }
                     rect = rectEvent;
                     rect.left = rect.right - 5;
                     rect.top = rect.bottom - 16;
                     if(rect.contains(ptCursor))
                     {
                        etest = HitTestSizingBottomRight;
                        goto SizingSuccess;
                     }
                  }
                  if(egrip & GripBottomLeft)
                  {
                     rect = rectEvent;
                     rect.right = rect.left + 16;
                     rect.top = rect.bottom - 5;
                     if(rect.contains(ptCursor))
                     {
                        etest = HitTestSizingBottomLeft;
                        goto SizingSuccess;
                     }
                     rect = rectEvent;
                     rect.right = rect.left + 5;
                     rect.top = rect.bottom - 16;
                     if(rect.contains(ptCursor))
                     {
                        etest = HitTestSizingBottomLeft;
                        goto SizingSuccess;
                     }
                  }
                  if(egrip & GripCenterTop)
                  {
                     rect.top = rectEvent.top;
                     rect.left = ptCenter.x - 8;
                     rect.right = ptCenter.x + 8;
                     rect.bottom = rectEvent.top + 5;
                     if(rect.contains(ptCursor))
                     {
                        etest = HitTestSizingTop;
                        goto SizingSuccess;
                     }
                  }
                  if(egrip & GripCenterBottom)
                  {
                     rect.top = rectEvent.bottom - 5;
                     rect.left = ptCenter.x - 8;
                     rect.right = ptCenter.x + 8;
                     rect.bottom = rectEvent.bottom;
                     if(rect.contains(ptCursor))
                     {
                        etest = HitTestSizingBottom;
                        goto SizingSuccess;
                     }
                  }
                  if(egrip & GripCenterLeft)
                  {
                     rect.top = ptCenter.y - 8;
                     rect.left = rectEvent.left;
                     rect.right = rectEvent.left + 5;
                     rect.bottom = ptCenter.y + 8;
                     if(rect.contains(ptCursor))
                     {
                        etest = HitTestSizingLeft;
                        goto SizingSuccess;
                     }
                  }
                  if(egrip & GripCenterRight)
                  {
                     rect.top = ptCenter.y - 8;
                     rect.left = rectEvent.right - 5;
                     rect.right = rectEvent.right;
                     rect.bottom = ptCenter.y + 8;
                     if(rect.contains(ptCursor))
                     {
                        etest = HitTestSizingRight;
                        goto SizingSuccess;
                     }
                  }
                  goto SizingNone;
               SizingSuccess:
                  return etest;
               SizingNone:;
               }
               return HitTestClient;
            }




            void FrameSchemaHardCoded002::DrawBorderSide(::draw2d::graphics * pgraphics, const RECT & lpcrectClient, EBorder eside)
            {

               

               appearance * pappearance = get_appearance();

               COLORREF    crMoveableBorder;
               COLORREF    crMoveableBorderHilight;
               COLORREF    crMoveableBorderShadow;

               if(pappearance->m_fActive)
               {
                  crMoveableBorder = m_crMoveableBorder;
                  crMoveableBorderHilight = m_crMoveableBorderHilight;
                  crMoveableBorderShadow = m_crMoveableBorderShadow;
               }
               else
               {
                  crMoveableBorder = Session.get_default_color(COLOR_BTNFACE);
                  crMoveableBorderHilight = Session.get_default_color(COLOR_BTNHILIGHT);
                  crMoveableBorderShadow = Session.get_default_color(COLOR_BTNSHADOW);
               }

               EDock edock = m_pworkset->GetDockingManager()->GetDockState();
               rect rectA(lpcrectClient);

               if(m_estyle == StyleTranslucidWarmGray
                  || m_estyle == StyleTranslucidLightBlue
                  || m_estyle == StyleTranslucidLightGreen)
               {
                  rect rect;
                  GetBorderRect(lpcrectClient, rect, eside);
                  class imaging & imaging = System.visual().imaging();
                  imaging.color_blend(pgraphics,
                     rect,
                     crMoveableBorder,
                     127);
               }
               else if(m_estyle == StyleLightBlue)
               {
                  rectA.deflate(1, 1, 1, 1);
                  Draw3dRectSide(pgraphics, rectA, eside, crMoveableBorder, 0);//m_crMoveableBorderDkShadow);

                  rectA.deflate(1, 1, 1, 1);
                  Draw3dRectSide(pgraphics, rectA, eside, crMoveableBorderHilight, crMoveableBorderShadow);

                  rectA.deflate(1, 1, 1, 1);
                  Draw3dRectSide(pgraphics, rectA, eside, crMoveableBorder, crMoveableBorder);

                  rectA.deflate(1, 1, 1, 1);
                  Draw3dRectSide(pgraphics, rectA, eside, crMoveableBorder, crMoveableBorder);

                  rect rect;
                  GetBorderRect(lpcrectClient, rect, eside);
                  class imaging & imaging = System.visual().imaging();
                  imaging.color_blend(pgraphics,
                     rect,
                     crMoveableBorder,
                     127);
               }
               else
               {
                  rect rect;
                  ::rect rectClient = lpcrectClient;
                  rectClient.deflate(1, 1);
                  GetBorderRect(rectClient, rect, eside);
                  class imaging & imaging = System.visual().imaging();
                  imaging.color_blend(pgraphics,
                     rect,
                     crMoveableBorder,
                     200);

                  GetBorderRect(lpcrectClient, rect, eside);

                  class rect rectClientB = rectA;

                  rectClientB.bottom--;
                  rectClientB.right--;

                  rectA.top++;
                  rectA.bottom--;
                  rectA.left++;
                  rectA.right--;
                  if(edock == DockNone)
                  {
                     Draw3dRectSide(pgraphics, rectA, eside, SetAValue(255, m_crDkShadow), SetAValue(255, m_crDkShadow));
                  }

                  rectA.top++;
                  rectA.bottom--;
                  rectA.left++;
                  rectA.right--;
                  Draw3dRectSide(pgraphics, rectA, eside, SetAValue(255, m_crDkShadow), SetAValue(255, m_crDkShadow));

                  rectA.top++;
                  rectA.bottom--;
                  rectA.left++;
                  rectA.right--;
                  if(edock == DockNone)
                  {
                     Draw3dRectSide(pgraphics, rectA, eside, SetAValue(255, m_crDkShadow), SetAValue(255, m_crDkShadow));
                  }

               }

            }

            void FrameSchemaHardCoded002::on_draw_frame(::draw2d::graphics * pgraphics)
            {

               sp(::user::interaction) pwndDraw = get_draw_window();

               if(pwndDraw == NULL)
                  return;

               appearance * pappearance = get_appearance();

               if(!pappearance->IsEnabled())
                  return;

               sp(::user::interaction) pwnd = get_window();

               bool bZoomed = pwnd->WfiIsZoomed() != 0;

               //    CVMSApp * pApp = (CVMSApp *) &System;
               //::aura::savings & savings = Session.savings();


               rect rectClient;
               pwndDraw->GetClientRect(rectClient);

               string str;


               rect rectNClient;

               pwndDraw->GetWindowRect(rectNClient);

               rectNClient -= rectNClient.top_left();

               ////////////////////
               //
               // Routine:
               // draw frame border
               //
               ////////////////////


               if(!pappearance->IsFullScreen())
               {
                  DrawBorder(pgraphics, rectNClient);
               }

               ///////////////////////
               //
               // Routine:
               // draw resize grip set
               //
               ///////////////////////

               if(!bZoomed && !pappearance->IsFullScreen())
               {
                  DrawGripSet(pgraphics, rectNClient);
               }

               //          pgraphics->SetBkMode(iOriginalBkMode);
               //            pgraphics->set_text_color(crOriginalTextColor);

            }


            void FrameSchemaHardCoded002::DrawBorder(::draw2d::graphics * pgraphics, const RECT & lpcrectClient)
            {


               MoveManager * pwmm = m_pworkset->GetMovingManager();

               EBorder eborder = pwmm->GetBorderMask();

               if(get_appearance()->IsZoomed())
               {
                  eborder = (EBorder)
                     (eborder &
                     ~(BorderRight
                     | BorderBottom
                     | BorderLeft));
               }

               if(eborder & BorderTop)
               {
                  DrawBorderSide(pgraphics, lpcrectClient, BorderTop);
               }
               if(eborder & BorderRight)
               {
                  DrawBorderSide(pgraphics, lpcrectClient, BorderRight);
               }
               if(eborder & BorderBottom)
               {
                  DrawBorderSide(pgraphics, lpcrectClient, BorderBottom);
               }
               if(eborder & BorderLeft)
               {
                  DrawBorderSide(pgraphics, lpcrectClient, BorderLeft);
               }

            }

            void FrameSchemaHardCoded002::GetBorderRect(
               const RECT & lpcrectClient,
               LPRECT lprect,
               EBorder eside)
            {
               rect rectBig(lpcrectClient);
               rect rectSmall;
               get_window_client_rect(rectSmall);
               rect rect;
               if(eside == BorderTop)
               {
                  rect.left = rectBig.left;
                  rect.right = rectBig.right;
                  rect.top = rectBig.top;
                  rect.bottom = rectSmall.top;
               }
               else if(eside == BorderLeft)
               {
                  rect.left = rectBig.left;
                  rect.right = rectSmall.left;
                  rect.top = rectSmall.top;
                  rect.bottom = rectSmall.bottom;
               }
               else if(eside == BorderRight)
               {
                  rect.left = rectSmall.right;
                  rect.right = rectBig.right;
                  rect.top = rectSmall.top;
                  rect.bottom = rectSmall.bottom;
               }
               else if(eside == BorderBottom)
               {
                  rect.left = rectBig.left;
                  rect.right = rectBig.right;
                  rect.top = rectSmall.bottom;
                  rect.bottom = rectBig.bottom;
               }
               *lprect = rect;
            }


            void FrameSchemaHardCoded002::on_style_change()
            {

               on_style_change_001_and_002();

            }

#define BEVEL_SMALL 2
#define BEVEL_LARGE 5
#define GRIP_SMALL 8
#define GRIP_LARGE 30

            void FrameSchemaHardCoded002::DrawGrip(
               ::draw2d::graphics * pgraphics,
               const RECT & lpcrectClient,
               EGrip egrip)
            {
               //            const int32_t size1 = 14;
               //      const int32_t size2 = 15;

               rect rectClient(lpcrectClient);
               //            rect rectClientB(rectClient);
               rect rectA;
               point ptA;
               point ptB;
               point ptC;
               rect rect(rectClient);

               

               switch(egrip)
               {
               case GripTopLeft:
               {

                                  rectClient.bottom--;
                                  rectClient.right--;

                                  rectA = rectClient;

                                  pgraphics->SelectObject(m_penHilight1);

                                  ptA = rectA.top_left();
                                  ptB = ptA;
                                  ptB.x += 16;
                                  pgraphics->MoveTo(ptA);
                                  pgraphics->LineTo(ptB);


                                  ptA = rectA.top_left();
                                  ptB = ptA;
                                  ptB.y += 16;
                                  pgraphics->MoveTo(ptA);
                                  pgraphics->LineTo(ptB);

                                  pgraphics->SelectObject(m_penFace1);

                                  ptA = rectA.top_left();
                                  ptA.x++;
                                  ptA.y++;
                                  ptB = ptA;
                                  ptB.x += 15;
                                  pgraphics->MoveTo(ptA);
                                  pgraphics->LineTo(ptB);

                                  ptA = rectA.top_left();
                                  ptA.x += 2;
                                  ptA.y += 2;
                                  ptB = ptA;
                                  ptB.x += 14;
                                  pgraphics->MoveTo(ptA);
                                  pgraphics->LineTo(ptB);


                                  ptA = rectA.top_left();
                                  ptA.x++;
                                  ptA.y++;
                                  ptB = ptA;
                                  ptB.y += 15;
                                  pgraphics->MoveTo(ptA);
                                  pgraphics->LineTo(ptB);

                                  ptA = rectA.top_left();
                                  ptA.x += 2;
                                  ptA.y += 2;
                                  ptB = ptA;
                                  ptB.y += 14;
                                  pgraphics->MoveTo(ptA);
                                  pgraphics->LineTo(ptB);

                                  pgraphics->SelectObject(m_penShadow1);

                                  ptA = rectA.top_left();
                                  ptA.x += 3;
                                  ptA.y += 3;
                                  ptB = ptA;
                                  ptB.x += 13;
                                  pgraphics->MoveTo(ptA);
                                  pgraphics->LineTo(ptB);


                                  ptA = rectA.top_left();
                                  ptA.x += 3;
                                  ptA.y += 3;
                                  ptB = ptA;
                                  ptB.y += 13;
                                  pgraphics->MoveTo(ptA);
                                  pgraphics->LineTo(ptB);

                                  pgraphics->SelectObject(m_penDkShadow1);

                                  ptA = rectA.top_left();
                                  ptA.x += 4;
                                  ptA.y += 4;
                                  ptB = ptA;
                                  ptB.y += 12;
                                  ptC = ptB;
                                  ptC.x -= 3;
                                  pgraphics->MoveTo(ptA);
                                  pgraphics->LineTo(ptB);
                                  pgraphics->LineTo(ptC);

                                  ptA = rectA.top_left();
                                  ptA.x += 4;
                                  ptA.y += 4;
                                  ptB = ptA;
                                  ptB.x += 12;
                                  ptC = ptB;
                                  ptC.y -= 3;
                                  pgraphics->MoveTo(ptA);
                                  pgraphics->LineTo(ptB);
                                  pgraphics->LineTo(ptC);
               }
                  break;
               case GripTopRight:
               {

                                   rectClient.bottom--;
                                   rectClient.right--;

                                   rectA = rectClient;

                                   pgraphics->SelectObject(m_penHilight1);

                                   ptA = rectA.top_right();
                                   ptB = ptA;
                                   ptB.x -= 16;
                                   ptC = ptB;
                                   ptC.y += 4;
                                   pgraphics->MoveTo(ptA);
                                   pgraphics->LineTo(ptB);
                                   pgraphics->LineTo(ptC);

                                   ptB = rectA.top_right();
                                   ptB.x -= 4;
                                   ptB.y += 4;
                                   ptC = ptB;
                                   ptC.y += 12;
                                   pgraphics->MoveTo(ptB);
                                   pgraphics->LineTo(ptC);

                                   pgraphics->SelectObject(m_penFace1);

                                   ptA = rectA.top_right();
                                   ptA.x--;
                                   ptA.y++;
                                   ptB = ptA;
                                   ptB.x -= 14;
                                   pgraphics->MoveTo(ptA);
                                   pgraphics->LineTo(ptB);

                                   ptA = rectA.top_right();
                                   ptA.x -= 2;
                                   ptA.y += 2;
                                   ptB = ptA;
                                   ptB.x -= 13;
                                   pgraphics->MoveTo(ptA);
                                   pgraphics->LineTo(ptB);

                                   ptA = rectA.top_right();
                                   ptA.x -= 2;
                                   ptA.y += 2;
                                   ptB = ptA;
                                   ptB.y += 13;
                                   pgraphics->MoveTo(ptA);
                                   pgraphics->LineTo(ptB);

                                   ptA = rectA.top_right();
                                   ptA.x -= 3;
                                   ptA.y += 3;
                                   ptB = ptA;
                                   ptB.y += 12;
                                   pgraphics->MoveTo(ptA);
                                   pgraphics->LineTo(ptB);

                                   pgraphics->SelectObject(m_penShadow1);

                                   ptA = rectA.top_right();
                                   ptA.x -= 3;
                                   ptA.y += 3;
                                   ptB = ptA;
                                   ptB.x -= 12;
                                   pgraphics->MoveTo(ptA);
                                   pgraphics->LineTo(ptB);

                                   ptA = rectA.top_right();
                                   ptA.x--;
                                   ptA.y++;
                                   ptB = ptA;
                                   ptB.y += 14;
                                   pgraphics->MoveTo(ptA);
                                   pgraphics->LineTo(ptB);

                                   pgraphics->SelectObject(m_penDkShadow1);

                                   ptB = rectA.top_right();
                                   ptB.x -= 4;
                                   ptB.y += 4;
                                   ptC = ptB;
                                   ptC.x -= 12;
                                   pgraphics->MoveTo(ptB);
                                   pgraphics->LineTo(ptC);


                                   ptA = rectA.top_right();
                                   ptB = ptA;
                                   ptB.y += 16;
                                   ptC = ptB;
                                   ptC.x -= 4;
                                   pgraphics->MoveTo(ptA);
                                   pgraphics->LineTo(ptB);
                                   pgraphics->LineTo(ptC);

               }
                  break;
               case GripBottomLeft:
               {
                                     pgraphics->SelectObject(m_penHilight1);

                                     rectClient.bottom--;
                                     rectClient.right--;

                                     rectA = rectClient;


                                     ptA = rectA.bottom_left();
                                     ptB = rectA.bottom_left();
                                     ptB.y -= 16;
                                     ptC = ptB;
                                     ptC.x += 4;
                                     pgraphics->MoveTo(ptA);
                                     pgraphics->LineTo(ptB); // vertical left hilite
                                     pgraphics->LineTo(ptC); // horizontal top hilite


                                     ptB = rectA.bottom_left();
                                     ptB.y -= 4;
                                     ptB.x += 4;
                                     ptC = ptB;
                                     ptC.x += 12;
                                     pgraphics->MoveTo(ptB);
                                     pgraphics->LineTo(ptC); // horizontal bottom hillite

                                     pgraphics->SelectObject(m_penFace1);

                                     ptA = rectA.bottom_left();
                                     ptA.y--;
                                     ptA.x++;
                                     ptB = ptA;
                                     ptB.y -= 14;
                                     pgraphics->MoveTo(ptA);
                                     pgraphics->LineTo(ptB);

                                     ptA = rectA.bottom_left();
                                     ptA.y -= 2;
                                     ptA.x += 2;
                                     ptB = ptA;
                                     ptB.y -= 13;
                                     pgraphics->MoveTo(ptA);
                                     pgraphics->LineTo(ptB);

                                     ptA = rectA.bottom_left();
                                     ptA.y -= 2;
                                     ptA.x += 2;
                                     ptB = ptA;
                                     ptB.x += 13;
                                     pgraphics->MoveTo(ptA);
                                     pgraphics->LineTo(ptB);

                                     ptA = rectA.bottom_left();
                                     ptA.y -= 3;
                                     ptA.x += 3;
                                     ptB = ptA;
                                     ptB.x += 12;
                                     pgraphics->MoveTo(ptA);
                                     pgraphics->LineTo(ptB);

                                     pgraphics->SelectObject(m_penShadow1);

                                     ptA = rectA.bottom_left();
                                     ptA.y -= 3;
                                     ptA.x += 3;
                                     ptB = ptA;
                                     ptB.y -= 12;
                                     pgraphics->MoveTo(ptA);
                                     pgraphics->LineTo(ptB);

                                     ptA = rectA.bottom_left();
                                     ptA.y--;
                                     ptA.x++;
                                     ptB = ptA;
                                     ptB.x += 14;
                                     pgraphics->MoveTo(ptA);
                                     pgraphics->LineTo(ptB);

                                     pgraphics->SelectObject(m_penDkShadow1);

                                     ptB = rectA.bottom_left();
                                     ptB.y -= 4;
                                     ptB.x += 4;
                                     ptC = ptB;
                                     ptC.y -= 12;
                                     pgraphics->MoveTo(ptB);
                                     pgraphics->LineTo(ptC);

                                     ptA = rectA.bottom_left();
                                     ptB = ptA;
                                     ptB.x += 16;
                                     ptC = ptB;
                                     ptC.y -= 4;
                                     pgraphics->MoveTo(ptA);
                                     pgraphics->LineTo(ptB);
                                     pgraphics->LineTo(ptC);

               }
                  break;
               case GripBottomRight:
               {
                                      rectClient.bottom--;
                                      rectClient.right--;

                                      rectA = rectClient;


                                      pgraphics->SelectObject(m_penDkShadow1);

                                      ptA = rectA.bottom_right();
                                      ptB = ptA;
                                      ptB.x -= 16;
                                      pgraphics->MoveTo(ptA);
                                      pgraphics->LineTo(ptB);

                                      ptA = rectA.bottom_right();
                                      ptB = ptA;
                                      ptB.y -= 16;
                                      pgraphics->MoveTo(ptA);
                                      pgraphics->LineTo(ptB);

                                      pgraphics->SelectObject(m_penShadow1);

                                      ptA = rectA.bottom_right();
                                      ptA.y--;
                                      ptA.x--;
                                      ptB = ptA;
                                      ptB.x -= 15;
                                      pgraphics->MoveTo(ptA);
                                      pgraphics->LineTo(ptB);

                                      ptA = rectA.bottom_right();
                                      ptA.y--;
                                      ptA.x--;
                                      ptB = ptA;
                                      ptB.y -= 15;
                                      pgraphics->MoveTo(ptA);
                                      pgraphics->LineTo(ptB);


                                      pgraphics->SelectObject(m_penFace1);

                                      ptA = rectA.bottom_right();
                                      ptA.y -= 3;
                                      ptA.x -= 3;
                                      ptB = ptA;
                                      ptB.x -= 13;
                                      pgraphics->MoveTo(ptA);
                                      pgraphics->LineTo(ptB);

                                      ptA = rectA.bottom_right();
                                      ptA.y -= 2;
                                      ptA.x -= 2;
                                      ptB = ptA;
                                      ptB.x -= 14;
                                      pgraphics->MoveTo(ptA);
                                      pgraphics->LineTo(ptB);

                                      ptA = rectA.bottom_right();
                                      ptA.y -= 3;
                                      ptA.x -= 3;
                                      ptB = ptA;
                                      ptB.y -= 13;
                                      pgraphics->MoveTo(ptA);
                                      pgraphics->LineTo(ptB);

                                      ptA = rectA.bottom_right();
                                      ptA.y -= 2;
                                      ptA.x -= 2;
                                      ptB = ptA;
                                      ptB.y -= 14;
                                      pgraphics->MoveTo(ptA);
                                      pgraphics->LineTo(ptB);


                                      pgraphics->SelectObject(m_penHilight1);

                                      ptA = rectA.bottom_right();
                                      ptA.x -= 4;
                                      ptA.y -= 4;
                                      ptB = ptA;
                                      ptB.x -= 12;
                                      ptC = ptB;
                                      ptC.y += 4;

                                      pgraphics->MoveTo(ptA);
                                      pgraphics->LineTo(ptB);
                                      pgraphics->LineTo(ptC);

                                      ptA = rectA.bottom_right();
                                      ptA.x -= 4;
                                      ptA.y -= 4;
                                      ptB = ptA;
                                      ptB.y -= 12;
                                      ptC = ptB;
                                      ptC.x += 4;

                                      pgraphics->MoveTo(ptA);
                                      pgraphics->LineTo(ptB);
                                      pgraphics->LineTo(ptC);

               }
                  break;
               case GripCenterTop:
               {
                                    point ptCenter = rect.center();

                                    class rect rectB;

                                    rectB.top = rectClient.top;
                                    rectB.left = ptCenter.x - GRIP_CENTER_LARGE_CX / 2;
                                    rectB.right = ptCenter.x + GRIP_CENTER_LARGE_CX / 2;
                                    rectB.bottom = rectB.top + GRIP_CENTER_SMALL_CY;

                                    DrawRectGrip(pgraphics, rectB);
               }
                  break;
               case GripCenterBottom:
               {
                                       point ptCenter = rect.center();

                                       class rect rectB;

                                       rectB.bottom = rectClient.bottom;
                                       rectB.left = ptCenter.x - GRIP_CENTER_LARGE_CX / 2;
                                       rectB.right = ptCenter.x + GRIP_CENTER_LARGE_CX / 2;
                                       rectB.top = rectB.bottom - GRIP_CENTER_SMALL_CY;

                                       DrawRectGrip(pgraphics, rectB);
               }
                  break;
               case GripCenterLeft:
               {
                                     point ptCenter = rect.center();

                                     class rect rectB;

                                     rectB.top = ptCenter.y - GRIP_CENTER_LARGE_CY / 2;
                                     rectB.left = rectClient.left;
                                     rectB.right = rectClient.left + GRIP_CENTER_SMALL_CX;
                                     rectB.bottom = ptCenter.y + GRIP_CENTER_LARGE_CY / 2;

                                     DrawRectGrip(pgraphics, rectB);
               }
                  break;
               case GripCenterRight:
               {
                                      point ptCenter = rect.center();

                                      class rect rectB;

                                      rectB.top = ptCenter.y - GRIP_CENTER_LARGE_CY / 2;
                                      rectB.right = rectClient.right;
                                      rectB.left = rectClient.right - GRIP_CENTER_SMALL_CX;
                                      rectB.bottom = ptCenter.y + GRIP_CENTER_LARGE_CY / 2;

                                      DrawRectGrip(pgraphics, rectB);
               }
                  break;
               default:
                  break;
               }


            }

            void FrameSchemaHardCoded002::DrawRectGrip(::draw2d::graphics * pgraphics,const RECT & rectParam)
            {

               

               rect rect(rectParam);

               pgraphics->Draw3dRect(rect, Session.get_default_color(COLOR_BTNHILIGHT), Session.get_default_color(COLOR_3DDKSHADOW));

               rect.top++;
               rect.bottom--;
               rect.left++;
               rect.right--;

               pgraphics->Draw3dRect(rect, Session.get_default_color(COLOR_BTNFACE), Session.get_default_color(COLOR_BTNSHADOW));

               rect.top++;
               rect.bottom--;
               rect.left++;
               rect.right--;

               pgraphics->FillSolidRect(rect, Session.get_default_color(COLOR_BTNFACE));

            }

            void FrameSchemaHardCoded002::DrawGripSet(::draw2d::graphics * pgraphics, const RECT & lpcrectClient)
            {

               SizeManager * pwsm = m_pworkset->GetSizingManager();

               EGrip egrip = pwsm->GetGripMask();

               if(egrip & GripCenterTop)
               {
                  DrawGrip(pgraphics, lpcrectClient, GripCenterTop);
               }

               if(egrip & GripTopRight)
               {
                  DrawGrip(pgraphics, lpcrectClient, GripTopRight);
               }

               if(egrip & GripRight)
               {
                  DrawGrip(pgraphics, lpcrectClient, GripCenterRight);
               }

               if(egrip & GripBottomRight)
               {
                  DrawGrip(pgraphics, lpcrectClient, GripBottomRight);
               }

               if(egrip & GripCenterBottom)
               {
                  DrawGrip(pgraphics, lpcrectClient, GripCenterBottom);
               }

               if(egrip & GripBottomLeft)
               {
                  DrawGrip(pgraphics, lpcrectClient, GripBottomLeft);
               }

               if(egrip & GripCenterLeft)
               {
                  DrawGrip(pgraphics, lpcrectClient, GripCenterLeft);
               }

               if(egrip & GripTopLeft)
               {
                  DrawGrip(pgraphics, lpcrectClient, GripTopLeft);
               }

            }


            COLORREF FrameSchemaHardCoded002::get_border_main_body_color()
            {

               return m_crMoveableBorder;

            }


         } // namespace wndfrm_core


      } // namespace frame


   } // namespace wndfrm


} // namespace user





































