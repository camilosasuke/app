#include "framework.h"

#define GRIP_CORNER_LARGE_CX 16
#define GRIP_CORNER_SMALL_CX 5
#define GRIP_CORNER_LARGE_CY 16
#define GRIP_CORNER_SMALL_CY 5

#undef GRIP_CENTER_LARGE_CX
#undef GRIP_CENTER_SMALL_CX
#undef GRIP_CENTER_LARGE_CY
#undef GRIP_CENTER_SMALL_CY


#define GRIP_CENTER_LARGE_CX 18
#define GRIP_CENTER_SMALL_CX 5
#define GRIP_CENTER_LARGE_CY 18
#define GRIP_CENTER_SMALL_CY 5


namespace user
{


   namespace wndfrm
   {


      namespace frame
      {


         namespace wndfrm_core
         {


            FrameSchemaHardCodedMinimal001::FrameSchemaHardCodedMinimal001(::aura::application * papp) :
               object(papp),
               frame_schema(papp)
            {

               set_style(StyleLightBlue);

               m_rectMarginNormal.set(0,0,0,0);


            }


            FrameSchemaHardCodedMinimal001::~FrameSchemaHardCodedMinimal001()
            {

            }


            EHitTest FrameSchemaHardCodedMinimal001::_000HitTest(point ptCursor)
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





            void FrameSchemaHardCodedMinimal001::DrawBorderSide(::draw2d::graphics * pgraphics, const RECT & lpcrectClient, EBorder eside)
            {

               if(eside != BorderTop)
                  return;

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

//               EDock edock = m_pworkset->GetDockingManager()->GetDockState();

               rect rectA(lpcrectClient);

               if(is_translucid_style(m_estyle))
               {

                  rect rect;

                  GetBorderRect(lpcrectClient, rect, eside);

                  class imaging & imaging = System.visual().imaging();

                  imaging.color_blend(pgraphics, rect, crMoveableBorder, 127);

               }
               /*else if(m_estyle == StyleLightBlue)
               {

                  rect rect;

                  GetBorderRect(lpcrectClient, rect, eside);

                  class imaging & imaging = System.visual().imaging();

                  imaging.color_blend(pgraphics, rect, crMoveableBorder, 127);



                  rectA.deflate(2, 2, 2, 2);
                  Draw3dRectSide(pgraphics, rectA, eside, crMoveableBorderHilight, crMoveableBorderShadow);

                  rectA.deflate(1, 1, 1, 1);
                  Draw3dRectSide(pgraphics, rectA, eside, crMoveableBorder, crMoveableBorder);

                  rectA.deflate(1, 1, 1, 1);
                  Draw3dRectSide(pgraphics, rectA, eside, crMoveableBorder, crMoveableBorder);


               }*/
               else
               {

                  rect rectClient(lpcrectClient);

                  rectClient.deflate(2, 2, 2, 2);

                  rect rect;

                  GetBorderRect(rectClient, rect, eside);

                  class imaging & imaging = System.visual().imaging();

                  imaging.color_blend(pgraphics, rect, crMoveableBorder, 127);

               }

            }


            void FrameSchemaHardCodedMinimal001::on_draw_frame(::draw2d::graphics * pgraphics)
            {

               sp(::user::interaction) pwndDraw = get_draw_window();

               if(pwndDraw == NULL)
                  return;

               appearance * pappearance = get_appearance();

               if(!pappearance->IsEnabled())
                  return;

               sp(::user::interaction) pwnd = get_window();

               bool bZoomed = pwnd->WfiIsZoomed() != 0;

               string str;

               rect rectWindow;

               pwndDraw->GetWindowRect(rectWindow);

               rectWindow -= rectWindow.top_left();

               if(!pappearance->IsFullScreen())
               {

                  DrawBorder(pgraphics, rectWindow);

               }

               if(!bZoomed && !pappearance->IsFullScreen())
               {

                  DrawGripSet(pgraphics, rectWindow);

               }

            }


            void FrameSchemaHardCodedMinimal001::DrawBorder(::draw2d::graphics * pgraphics, const RECT & lpcrectClient)
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

            void FrameSchemaHardCodedMinimal001::GetBorderRect(const RECT & lpcrectClient, LPRECT lprect, EBorder eside)
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


            void FrameSchemaHardCodedMinimal001::on_style_change()
            {

               on_style_change_001_and_002();

            }


            void FrameSchemaHardCodedMinimal001::DrawGrip(::draw2d::graphics * pgraphics, const RECT & lpcrectClient, EGrip egrip)
            {

               return;

               

               rect rectC(lpcrectClient);

               rect rectClient(lpcrectClient);

               rectClient.right--;

               rectClient.bottom--;

               rect rectClientB(rectClient);

               rect rectA;

               point ptA;

               point ptB;

               point ptC;

               rect rect(rectClient);

               switch(egrip)
               {
               case GripTopLeft:
               {
                                  pgraphics->SelectObject(m_penHilight1);

                                  rectA = rectClient;

                                  rectA.top++;
                                  rectA.left++;
                                  rectA.bottom--;
                                  rectA.right--;

                                  ptA = rectA.top_left();
                                  ptA.y += 14;
                                  ptB = rectA.top_left();
                                  ptC = rectA.top_left();
                                  ptC.x += 15;

                                  pgraphics->MoveTo(ptA);
                                  pgraphics->LineTo(ptB);
                                  pgraphics->LineTo(ptC);

                                  pgraphics->SelectObject(m_penFace1);

                                  // Most external rectangle

                                  rectA = rectClient;

                                  ptA = rectA.top_left();
                                  ptA.y += 15;
                                  ptB = rectA.top_left();
                                  ptC = rectA.top_left();
                                  ptC.x += 16;
                                  pgraphics->MoveTo(ptA);
                                  pgraphics->LineTo(ptB);
                                  pgraphics->LineTo(ptC);

                                  // Midle Rectangle

                                  rectA = rectClient;

                                  rectA.top += 2;
                                  rectA.left += 2;
                                  rectA.bottom -= 2;
                                  rectA.right -= 2;

                                  ptA = rectA.top_left();
                                  ptA.y += 13;
                                  ptB = rectA.top_left();
                                  ptC = rectA.top_left();
                                  ptC.x += 14;
                                  pgraphics->MoveTo(ptA);
                                  pgraphics->LineTo(ptB);
                                  pgraphics->LineTo(ptC);

                                  pgraphics->SelectObject(m_penShadow1);

                                  rectA = rectClient;

                                  rectA.top++;
                                  rectA.left++;
                                  rectA.bottom--;
                                  rectA.right--;

                                  rectA.top += 2;
                                  rectA.left += 2;
                                  rectA.bottom -= 2;
                                  rectA.right -= 2;


                                  ptA = rectA.top_left();
                                  ptA.y += 12;
                                  ptB = rectA.top_left();
                                  ptC = rectA.top_left();
                                  ptC.x += 13;
                                  pgraphics->MoveTo(ptA);
                                  pgraphics->LineTo(ptB);
                                  pgraphics->LineTo(ptC);

                                  pgraphics->SelectObject(m_penDkShadow1);

                                  rectA = rectClient;

                                  rectA.top += 4;
                                  rectA.left += 4;
                                  rectA.bottom -= 4;
                                  rectA.right -= 4;

                                  ptA = rectA.top_left();
                                  ptA.y += 11;
                                  ptB = rectA.top_left();
                                  ptC = rectA.top_left();
                                  ptC.x += 12;
                                  pgraphics->MoveTo(ptA);
                                  pgraphics->LineTo(ptB);
                                  pgraphics->LineTo(ptC);

                                  pgraphics->SelectObject(m_penShadow1);

                                  // Details

                                  ptA.x = rectClientB.left + 14;
                                  ptA.y = rectClientB.top + 1;
                                  ptB.x = rectClientB.left + 14;
                                  ptB.y = rectClientB.top + 3;
                                  pgraphics->MoveTo(ptA);
                                  pgraphics->LineTo(ptB);


                                  // Details

                                  ptA.x = rectClientB.left + 1;
                                  ptA.y = rectClientB.top + 14;
                                  ptB.x = rectClientB.left + 3;
                                  ptB.y = rectClientB.top + 14;
                                  pgraphics->MoveTo(ptA);
                                  pgraphics->LineTo(ptB);

                                  pgraphics->SelectObject(m_penDkShadow1);

                                  // Details

                                  ptA.x = rectClientB.left + 15;
                                  ptA.y = rectClientB.top + 1;
                                  ptB.x = rectClientB.left + 15;
                                  ptB.y = rectClientB.top + 5;
                                  pgraphics->MoveTo(ptA);
                                  pgraphics->LineTo(ptB);

                                  // Details

                                  ptA.x = rectClientB.left + 1;
                                  ptA.y = rectClientB.top + 15;
                                  ptB.x = rectClientB.left + 5;
                                  ptB.y = rectClientB.top + 15;
                                  pgraphics->MoveTo(ptA);
                                  pgraphics->LineTo(ptB);
               }
                  break;
               case GripTopRight:
               {
                                   pgraphics->SelectObject(m_penHilight1);

                                   rectA = rectClient;

                                   rectA.top++;
                                   rectA.left++;
                                   rectA.bottom--;
                                   rectA.right--;

                                   ptA = rectA.top_right();
                                   ptA.x -= 15;
                                   ptB = rectA.top_right();
                                   pgraphics->MoveTo(ptA);
                                   pgraphics->LineTo(ptB);

                                   rectA.top += 2;
                                   rectA.left += 2;
                                   rectA.bottom -= 2;
                                   rectA.right -= 2;

                                   ptB = rectA.top_right();
                                   ptC = rectA.top_right();
                                   ptC.y += 13;
                                   pgraphics->MoveTo(ptB);
                                   pgraphics->LineTo(ptC);

                                   pgraphics->SelectObject(m_penFace1);

                                   // Most external rectangle

                                   rectA = rectClient;

                                   ptA = rectA.top_right();
                                   ptA.x -= 16;
                                   ptB = rectA.top_right();
                                   pgraphics->MoveTo(ptA);
                                   pgraphics->LineTo(ptB);


                                   // Most internal rectangle

                                   rectA.top += 4;
                                   rectA.left += 4;
                                   rectA.bottom -= 4;
                                   rectA.right -= 4;

                                   ptB = rectA.top_right();
                                   ptC = rectA.top_right();
                                   ptC.y += 12;
                                   pgraphics->MoveTo(ptB);
                                   pgraphics->LineTo(ptC);

                                   // Midle Rectangle

                                   rectA = rectClient;

                                   rectA.top += 2;
                                   rectA.left += 2;
                                   rectA.bottom -= 2;
                                   rectA.right -= 2;

                                   ptA = rectA.top_right();
                                   ptA.x -= 13;
                                   ptB = rectA.top_right();
                                   ptC = rectA.top_right();
                                   ptC.y += 14;
                                   pgraphics->MoveTo(ptA);
                                   pgraphics->LineTo(ptB);
                                   pgraphics->LineTo(ptC);

                                   pgraphics->SelectObject(m_penShadow1);

                                   rectA = rectClient;

                                   rectA.top++;
                                   rectA.left++;
                                   rectA.bottom--;
                                   rectA.right--;

                                   ptB = rectA.top_right();
                                   ptC = rectA.top_right();
                                   ptC.y += 15;
                                   pgraphics->MoveTo(ptB);
                                   pgraphics->LineTo(ptC);

                                   rectA.top += 2;
                                   rectA.left += 2;
                                   rectA.bottom -= 2;
                                   rectA.right -= 2;

                                   ptA = rectA.top_right();
                                   ptA.x -= 12;
                                   ptB = rectA.top_right();
                                   pgraphics->MoveTo(ptA);
                                   pgraphics->LineTo(ptB);

                                   pgraphics->SelectObject(m_penDkShadow1);

                                   rectA = rectClient;

                                   ptB = rectA.top_right();
                                   ptC = rectA.top_right();
                                   ptC.y += 16;
                                   pgraphics->MoveTo(ptB);
                                   pgraphics->LineTo(ptC);

                                   rectA.top += 4;
                                   rectA.left += 4;
                                   rectA.bottom -= 4;
                                   rectA.right -= 4;

                                   ptA = rectA.top_right();
                                   ptA.x -= 11;
                                   ptB = rectA.top_right();
                                   pgraphics->MoveTo(ptA);
                                   pgraphics->LineTo(ptB);

                                   pgraphics->SelectObject(m_penHilight1);

                                   ptA.x = rectClientB.right - 14;
                                   ptA.y = rectClientB.top + 1;
                                   ptB.x = rectClientB.right - 14;
                                   ptB.y = rectClientB.top + 4;
                                   pgraphics->MoveTo(ptA);
                                   pgraphics->LineTo(ptB);

                                   pgraphics->SelectObject(m_penFace1);

                                   // Details

                                   ptA.x = rectClientB.right - 15;
                                   ptA.y = rectClientB.top;
                                   ptB.x = rectClientB.right - 15;
                                   ptB.y = rectClientB.top + 5;
                                   pgraphics->MoveTo(ptA);
                                   pgraphics->LineTo(ptB);

                                   pgraphics->SelectObject(m_penShadow1);

                                   // Details

                                   ptA.x = rectClientB.right - 3;
                                   ptA.y = rectClientB.top + 14;
                                   ptB.x = rectClientB.right - 1;
                                   ptB.y = rectClientB.top + 14;
                                   pgraphics->MoveTo(ptA);
                                   pgraphics->LineTo(ptB);

                                   pgraphics->SelectObject(m_penDkShadow1);

                                   // Details

                                   ptA.x = rectClientB.right - 4;
                                   ptA.y = rectClientB.top + 15;
                                   ptB.x = rectClientB.right;
                                   ptB.y = rectClientB.top + 15;
                                   pgraphics->MoveTo(ptB);
                                   pgraphics->LineTo(ptA);
               }
                  break;
               case GripBottomLeft:
               {

                                     pgraphics->SelectObject(m_penHilight1);

                                     rectA = rectClient;

                                     rectA.left++;
                                     rectA.bottom--;

                                     // left 1
                                     ptA = rectA.bottom_left();
                                     ptA.y -= 16;
                                     ptB = rectA.bottom_left();
                                     pgraphics->MoveTo(ptB);
                                     pgraphics->LineTo(ptA);

                                     rectA = rectClient;

                                     rectA.left += 3;
                                     rectA.bottom -= 3;

                                     // bottom 2
                                     ptB = rectA.bottom_left();
                                     ptC = rectA.bottom_left();
                                     ptC.x += 12;
                                     pgraphics->MoveTo(ptC);
                                     pgraphics->LineTo(ptB);

                                     pgraphics->SelectObject(m_penFace1);

                                     // Most external rectangle 0

                                     rectA = rectClient;

                                     ptA = rectA.bottom_left();
                                     ptA.y -= 15;
                                     ptB = rectA.bottom_left();
                                     pgraphics->MoveTo(ptB);
                                     pgraphics->LineTo(ptA);

                                     // Most internal rectangle 4

                                     rectA.top += 4;
                                     rectA.left += 4;
                                     rectA.bottom -= 4;
                                     rectA.right -= 4;

                                     ptB = rectA.bottom_left();
                                     ptC = rectA.bottom_left();
                                     ptC.x += 12;
                                     pgraphics->MoveTo(ptB);
                                     pgraphics->LineTo(ptC);

                                     // Midle Rectangle 2

                                     rectA = rectClient;

                                     rectA.top += 2;
                                     rectA.left += 2;
                                     rectA.bottom -= 2;
                                     rectA.right -= 2;

                                     ptA = rectA.bottom_left();
                                     ptA.y -= 13;
                                     ptB = rectA.bottom_left();
                                     ptC = rectA.bottom_left();
                                     ptC.x += 14;
                                     pgraphics->MoveTo(ptA);
                                     pgraphics->LineTo(ptB);
                                     pgraphics->LineTo(ptC);

                                     pgraphics->SelectObject(m_penShadow1);

                                     rectA = rectClient;

                                     rectA.left += 2;
                                     rectA.bottom--;

                                     ptB = rectA.bottom_left();
                                     ptC = rectA.bottom_left();
                                     ptC.x += 14;
                                     pgraphics->MoveTo(ptB);
                                     pgraphics->LineTo(ptC);

                                     rectA = rectClient;

                                     rectA.left += 3;
                                     rectA.bottom -= 2;

                                     ptA = rectA.bottom_left();
                                     ptA.y -= 12;
                                     ptB = rectA.bottom_left();
                                     pgraphics->MoveTo(ptA);
                                     pgraphics->LineTo(ptB);

                                     pgraphics->SelectObject(m_penDkShadow1);

                                     rectA = rectClient;

                                     ptB = rectA.bottom_left();
                                     ptB.x++;
                                     ptC = rectA.bottom_left();
                                     ptC.x += 16;
                                     pgraphics->MoveTo(ptB);
                                     pgraphics->LineTo(ptC);

                                     rectA.left += 4;
                                     rectA.bottom -= 3;

                                     ptA = rectA.bottom_left();
                                     ptA.y -= 11;
                                     ptB = rectA.bottom_left();
                                     pgraphics->MoveTo(ptA);
                                     pgraphics->LineTo(ptB);

                                     // Details - top most

                                     pgraphics->SelectObject(m_penHilight1);

                                     ptA.x = rectClientB.left + 1;
                                     ptA.y = rectClientB.bottom - 15;
                                     ptB.x = rectClientB.left + 4;
                                     ptB.y = rectClientB.bottom - 15;
                                     pgraphics->MoveTo(ptA);
                                     pgraphics->LineTo(ptB);

                                     // Details - top most

                                     pgraphics->SelectObject(m_penFace1);

                                     ptA.x = rectClientB.left;
                                     ptA.y = rectClientB.bottom - 15;
                                     ptB.x = rectClientB.left + 5;
                                     ptB.y = rectClientB.bottom - 15;
                                     pgraphics->MoveTo(ptA);
                                     pgraphics->LineTo(ptB);

                                     pgraphics->SelectObject(m_penShadow1);

                                     // Details - right most

                                     ptA.x = rectClientB.left + 14;
                                     ptA.y = rectClientB.bottom - 4;
                                     ptB.x = rectClientB.left + 14;
                                     ptB.y = rectClientB.bottom - 1;
                                     pgraphics->MoveTo(ptB);
                                     pgraphics->LineTo(ptA);

                                     pgraphics->SelectObject(m_penDkShadow1);

                                     // Details - right most

                                     ptA.x = rectClientB.left + 15;
                                     ptA.y = rectClientB.bottom - 4;
                                     ptB.x = rectClientB.left + 15;
                                     ptB.y = rectClientB.bottom;
                                     pgraphics->MoveTo(ptB);
                                     pgraphics->LineTo(ptA);
               }
                  break;
               case GripBottomRight:
               {
                                      pgraphics->SelectObject(m_penHilight1);

                                      rectA = rectClient;

                                      rectA.top++;
                                      rectA.left++;
                                      rectA.bottom--;
                                      rectA.right--;

                                      rectA.top += 2;
                                      rectA.left += 2;
                                      rectA.bottom -= 2;
                                      rectA.right -= 2;

                                      ptA = rectA.bottom_right();
                                      ptA.y -= 12;
                                      ptB = rectA.bottom_right();
                                      ptC = rectA.bottom_right();
                                      ptC.x -= 13;
                                      pgraphics->MoveTo(ptA);
                                      pgraphics->LineTo(ptB);
                                      pgraphics->LineTo(ptC);

                                      pgraphics->SelectObject(m_penFace1);

                                      rectA = rectClient;

                                      // Most internal rectangle

                                      rectA.top += 4;
                                      rectA.left += 4;
                                      rectA.bottom -= 4;
                                      rectA.right -= 4;

                                      ptA = rectA.bottom_right();
                                      ptA.y -= 11;
                                      ptB = rectA.bottom_right();
                                      ptC = rectA.bottom_right();
                                      ptC.x -= 12;
                                      pgraphics->MoveTo(ptA);
                                      pgraphics->LineTo(ptB);
                                      pgraphics->LineTo(ptC);

                                      // Midle Rectangle

                                      rectA = rectClient;

                                      rectA.top += 2;
                                      rectA.left += 2;
                                      rectA.bottom -= 2;
                                      rectA.right -= 2;

                                      ptA = rectA.bottom_right();
                                      ptA.y -= 13;
                                      ptB = rectA.bottom_right();
                                      ptC = rectA.bottom_right();
                                      ptC.x -= 14;
                                      pgraphics->MoveTo(ptA);
                                      pgraphics->LineTo(ptB);
                                      pgraphics->LineTo(ptC);
                                      pgraphics->SelectObject(m_penShadow1);

                                      rectA = rectClient;

                                      rectA.top++;
                                      rectA.left++;
                                      rectA.bottom--;
                                      rectA.right--;

                                      ptA = rectA.bottom_right();
                                      ptA.y -= 14;
                                      ptB = rectA.bottom_right();
                                      ptC = rectA.bottom_right();
                                      ptC.x -= 15;
                                      pgraphics->MoveTo(ptA);
                                      pgraphics->LineTo(ptB);
                                      pgraphics->LineTo(ptC);

                                      pgraphics->SelectObject(m_penDkShadow1);

                                      rectA = rectClient;

                                      ptA = rectA.bottom_right();
                                      ptA.y -= 15;
                                      ptB = rectA.bottom_right();
                                      ptC = rectA.bottom_right();
                                      ptC.x -= 15;
                                      pgraphics->MoveTo(ptA);
                                      pgraphics->LineTo(ptB);
                                      pgraphics->LineTo(ptC);

                                      pgraphics->SelectObject(m_penHilight1);

                                      ptA.x = rectClientB.right - 3;
                                      ptA.y = rectClientB.bottom - 14;
                                      ptB.x = rectClientB.right - 1;
                                      ptB.y = rectClientB.bottom - 14;
                                      pgraphics->MoveTo(ptB);
                                      pgraphics->LineTo(ptA);

                                      ptA.x = rectClientB.right - 14;
                                      ptA.y = rectClientB.bottom - 3;
                                      ptB.x = rectClientB.right - 14;
                                      ptB.y = rectClientB.bottom - 1;
                                      pgraphics->MoveTo(ptB);
                                      pgraphics->LineTo(ptA);

                                      pgraphics->SelectObject(m_penFace1);

                                      // Details

                                      ptA.x = rectClientB.right - 5;
                                      ptA.y = rectClientB.bottom - 15;
                                      ptB.x = rectClientB.right;
                                      ptB.y = rectClientB.bottom - 15;
                                      pgraphics->MoveTo(ptB);
                                      pgraphics->LineTo(ptA);

                                      ptA.x = rectClientB.right - 15;
                                      ptA.y = rectClientB.bottom - 5;
                                      ptB.x = rectClientB.right - 15;
                                      ptB.y = rectClientB.bottom;
                                      pgraphics->MoveTo(ptB);
                                      pgraphics->LineTo(ptA);
               }
                  break;
               case GripCenterTop:
               {
                                    point ptCenter = rect.center();

                                    int iMod;
                                    if(rect.width() % 2 == 1)
                                       iMod = 1;
                                    else
                                       iMod = 0;

                                    class rect rectB;

                                    rectB.top = rectC.top;
                                    rectB.left = ptCenter.x - GRIP_CENTER_LARGE_CX / 2 + iMod;
                                    rectB.right = ptCenter.x + GRIP_CENTER_LARGE_CX / 2 + iMod;
                                    rectB.bottom = rectC.top + GRIP_CENTER_SMALL_CY;

                                    DrawRectGrip(pgraphics, rectB);
               }
                  break;
               case GripCenterBottom:
               {
                                       point ptCenter = rect.center();

                                       class rect rectB;

                                       int iMod;
                                       if(rect.width() % 2 == 1)
                                          iMod = 1;
                                       else
                                          iMod = 0;

                                       rectB.bottom = rectC.bottom;
                                       rectB.left = ptCenter.x - GRIP_CENTER_LARGE_CX / 2 + iMod;
                                       rectB.right = ptCenter.x + GRIP_CENTER_LARGE_CX / 2 + iMod;
                                       rectB.top = rectC.bottom - GRIP_CENTER_SMALL_CY;

                                       DrawRectGrip(pgraphics, rectB);
               }
                  break;
               case GripCenterLeft:
               {
                                     point ptCenter = rect.center();

                                     int iMod;
                                     if(rect.height() % 2 == 1)
                                        iMod = 1;
                                     else
                                        iMod = 0;


                                     class rect rectB;

                                     rectB.top = ptCenter.y - GRIP_CENTER_LARGE_CY / 2 + iMod;
                                     rectB.left = rectC.left;
                                     rectB.right = rectC.left + GRIP_CENTER_SMALL_CX;
                                     rectB.bottom = ptCenter.y + GRIP_CENTER_LARGE_CY / 2 + iMod;

                                     DrawRectGrip(pgraphics, rectB);
               }
                  break;
               case GripCenterRight:
               {
                                      point ptCenter = rect.center();

                                      int iMod;
                                      if(rect.height() % 2 == 1)
                                         iMod = 1;
                                      else
                                         iMod = 0;


                                      class rect rectB;

                                      rectB.top = ptCenter.y - GRIP_CENTER_LARGE_CY / 2 + iMod;
                                      rectB.right = rectC.right;
                                      rectB.left = rectC.right - GRIP_CENTER_SMALL_CX;
                                      rectB.bottom = ptCenter.y + GRIP_CENTER_LARGE_CY / 2 + iMod;

                                      DrawRectGrip(pgraphics, rectB);
               }
                  break;
               default:
                  break;
               }


            }

            void FrameSchemaHardCodedMinimal001::DrawRectGrip(::draw2d::graphics * pgraphics,const RECT & rectParam)
            {

               

               rect rect(rectParam);

               //rect.right++;

               //rect.bottom++;

               pgraphics->Draw3dRect(rect, Session.get_default_color(COLOR_BTNFACE) | 0xff000000, Session.get_default_color(COLOR_3DDKSHADOW) | 0xff000000);

               rect.deflate(1, 1);

               pgraphics->Draw3dRect(rect, Session.get_default_color(COLOR_BTNHILIGHT) | 0xff000000, Session.get_default_color(COLOR_BTNSHADOW) | 0xff000000);

               rect.deflate(1, 1);

               //rect.right--;

               //rect.bottom--;

               pgraphics->FillSolidRect(rect, Session.get_default_color(COLOR_BTNFACE) | 0xff000000);

            }

            void FrameSchemaHardCodedMinimal001::DrawGripSet(::draw2d::graphics * pgraphics, const RECT & lpcrectClient)
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


         } // namespace wndfrm_core


      } // namespace frame


   } // namespace wndfrm


} // namespace user






























