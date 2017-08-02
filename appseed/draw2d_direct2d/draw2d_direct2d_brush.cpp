#include "framework.h"




namespace draw2d_direct2d
{



   brush::brush(::aura::application * papp) :
      ::object(papp)
   { 

   }

   brush::~brush()
   { 

      destroy();

   }


   // bool brush::CreateHatchBrush(int nIndex, COLORREF crColor)
   // { 
   //    //return Attach(::CreateHatchBrush(nIndex, crColor));
   //    return FALSE;
   // }
   //    bool brush::CreateBrushIndirect(const LOGBRUSH* lpLogBrush)
   //  {
   //return Attach(::CreateBrushIndirect(lpLogBrush)); 
   //   return FALSE;
   //}
   //bool brush::CreatePatternBrush(::draw2d::bitmap* pBitmap)
   //{ 
   //   //return Attach(::CreatePatternBrush((HBITMAP)pBitmap->get_os_data()));
   //   return FALSE;
   //}
   //bool brush::CreateDIBPatternBrush(const void * lpPackedDIB, UINT nUsage)
   //{ 
   //   //return Attach(::CreateDIBPatternBrushPt(lpPackedDIB, nUsage));
   //   return FALSE;
   //}
   //bool brush::CreateSysColorBrush(int nIndex)
   //{ 
   //   //return Attach(::GetSysColorBrush(nIndex));
   //   return FALSE;

   //}
   /*    int brush::GetLogBrush(LOGBRUSH* pLogBrush)
   { 
   //ASSERT(get_os_data() != NULL);
   //return ::GetObject(get_os_data(), sizeof(LOGBRUSH), pLogBrush); 
   return FALSE;
   }*/


   //   void brush::construct(COLORREF crColor)
   //   {
   ////      if (!Attach(::CreateSolidBrush(crColor)))
   //  //       throw resource_exception();
   //   }
   //
   //   void brush::construct(int nIndex, COLORREF crColor)
   //   {
   //    //  if (!Attach(::CreateHatchBrush(nIndex, crColor)))
   //      //   throw resource_exception();
   //   }
   //
   //   void brush::construct(::draw2d::bitmap* pBitmap)
   //   {
   //      //ASSERT_VALID(pBitmap);
   //
   //      //if (!Attach(::CreatePatternBrush((HBITMAP)pBitmap->get_os_data())))
   //        // throw resource_exception();
   //   }
   //
   //   bool brush::CreateDIBPatternBrush(HGLOBAL hPackedDIB, UINT nUsage)
   //   {
   //      //ASSERT(hPackedDIB != NULL);
   //      //const void * lpPackedDIB = ::GlobalLock(hPackedDIB);
   //      //ASSERT(lpPackedDIB != NULL);
   //      //bool bResult = Attach(::CreateDIBPatternBrushPt(lpPackedDIB, nUsage));
   //      //::GlobalUnlock(hPackedDIB);
   //      //return bResult;
   //      return FALSE;
   //   }


   void brush::dump(dump_context & dumpcontext) const
   {
      ::draw2d::object::dump(dumpcontext);

      //if (get_os_data() == NULL)
      // return;

      /*      LOGBRUSH lb;
      VERIFY(GetObject(sizeof(lb), &lb));
      dumpcontext << "lb.lbStyle = " << lb.lbStyle;
      dumpcontext << "\nlb.lbHatch = " << lb.lbHatch;
      dumpcontext << "\nlb.lbColor = " << (void *)(dword_ptr)lb.lbColor;

      dumpcontext << "\n";
      */
   }



   ID2D1Brush * brush::get_os_brush(::draw2d_direct2d::graphics * pgraphics) const
   {

      synch_lock sl(&draw2d_mutex());


      if(pgraphics->m_prendertarget == NULL)
         return NULL;

      if(m_etype == type_solid)
      {

         if(!m_bUpdated || m_psolidbrush == NULL)
         {

            if(m_pbrush != NULL)
            {

               ((brush *)this)->destroy();

            }

            D2D1_COLOR_F ca;

            ca.a = argb_get_a_value(m_cr) / 255.0f;
            ca.r = argb_get_r_value(m_cr) / 255.0f;
            ca.g = argb_get_g_value(m_cr) / 255.0f;
            ca.b = argb_get_b_value(m_cr) / 255.0f;


            pgraphics->m_prendertarget->CreateSolidColorBrush(ca, &((brush *) this)->m_psolidbrush);

            if(m_psolidbrush != NULL)
            {
               ((font *) this)->m_bUpdated   = true;
               ((font *) this)->m_pgraphics  = pgraphics;
            }

         }

         return m_psolidbrush.Get();

      }
      else if(m_etype == type_linear_gradient_point_color)
      {

         if(!m_bUpdated || m_plineargradientbrush == NULL)
         {

            if(m_pbrush != NULL)
            {

               ((brush *)this)->destroy();

            }

            D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES prop;

            prop.startPoint.x    = (FLOAT) m_pt1.x;
            prop.startPoint.y    = (FLOAT) m_pt1.y;
            prop.endPoint.x      = (FLOAT) m_pt2.x;
            prop.endPoint.y      = (FLOAT) m_pt2.y;

            // Create an array of gradient stops to put in the gradient stop
            // collection that will be used in the gradient brush.
            ID2D1GradientStopCollection * pgradientstops = NULL;

            D2D1_GRADIENT_STOP gradientstops[2];

            gradientstops[0].color.a = argb_get_a_value(m_cr1) / 255.0f;
            gradientstops[0].color.r = argb_get_r_value(m_cr1) / 255.0f;
            gradientstops[0].color.g = argb_get_g_value(m_cr1) / 255.0f;
            gradientstops[0].color.b = argb_get_b_value(m_cr1) / 255.0f;
            gradientstops[0].position = 0.0f;
            gradientstops[1].color.a = argb_get_a_value(m_cr2) / 255.0f;
            gradientstops[1].color.r = argb_get_r_value(m_cr2) / 255.0f;
            gradientstops[1].color.g = argb_get_g_value(m_cr2) / 255.0f;
            gradientstops[1].color.b = argb_get_b_value(m_cr2) / 255.0f;
            gradientstops[1].position = 1.0f;

            // Create the ID2D1GradientStopCollection from a previously
            // declared array of D2D1_GRADIENT_STOP structs.
            HRESULT hr = pgraphics->m_prendertarget->CreateGradientStopCollection(gradientstops, 2, D2D1_GAMMA_2_2, D2D1_EXTEND_MODE_CLAMP, &pgradientstops);

            hr = pgraphics->m_prendertarget->CreateLinearGradientBrush(prop, pgradientstops, &((brush *) this)->m_plineargradientbrush);

            pgradientstops->Release();

            if(m_plineargradientbrush != NULL)
            {
               ((font *) this)->m_bUpdated   = true;
               ((font *) this)->m_pgraphics  = pgraphics;
            }


         }


         return (ID2D1Brush *) m_plineargradientbrush.Get();

      }

      else if(m_etype == type_radial_gradient_color)
      {

         if(!m_bUpdated || m_plineargradientbrush == NULL)
         {

            if(m_pbrush != NULL)
            {

               ((brush *)this)->destroy();

            }

            // Create an array of gradient stops to put in the gradient stop
            // collection that will be used in the gradient brush.
            ID2D1GradientStopCollection *pgradientstops = NULL;

            D2D1_GRADIENT_STOP gradientstops[2];
            gradientstops[0].color.a = argb_get_a_value(m_cr1) / 255.0f;
            gradientstops[0].color.r = argb_get_r_value(m_cr1) / 255.0f;
            gradientstops[0].color.g = argb_get_g_value(m_cr1) / 255.0f;
            gradientstops[0].color.b = argb_get_b_value(m_cr1) / 255.0f;
            gradientstops[0].position = 0.0f;
            gradientstops[1].color.a = argb_get_a_value(m_cr2) / 255.0f;
            gradientstops[1].color.r = argb_get_r_value(m_cr2) / 255.0f;
            gradientstops[1].color.g = argb_get_g_value(m_cr2) / 255.0f;
            gradientstops[1].color.b = argb_get_b_value(m_cr2) / 255.0f;
            gradientstops[1].position = 1.0f;

            // Create the ID2D1GradientStopCollection from a previously
            // declared array of D2D1_GRADIENT_STOP structs.
            HRESULT hr = pgraphics->m_prendertarget->CreateGradientStopCollection(
               gradientstops,
               2,
               D2D1_GAMMA_2_2,
               D2D1_EXTEND_MODE_CLAMP,
               &pgradientstops
               );



            // The center of the gradient is in the center of the box.
            // The gradient origin offset was set to zero(0, 0) or center in this case.
            if(SUCCEEDED(hr))
            {
               
               double centerx = m_size.cx - m_pt.x;
               double centery = m_size.cy - m_pt.y;
               double originx = m_size.cx / 2 -m_pt.x;
               double originy = m_size.cy / 2 -m_pt.y;
               double radiusx = m_size.cx / 2;
               double radiusy = m_size.cy / 2;

               hr = pgraphics->m_prendertarget->CreateRadialGradientBrush(
                  D2D1::RadialGradientBrushProperties(
                  D2D1::Point2F(centerx,centery),
                  D2D1::Point2F(originx, originy),
                  (float) radiusx, (float) radiusy),
                  pgradientstops,
                  & ((brush *) this)->m_pradialgradientbrush
                  );
            }

            pgradientstops->Release();

            if(m_pradialgradientbrush != NULL)
            {
               ((font *) this)->m_bUpdated   = true;
               ((font *) this)->m_pgraphics  = pgraphics;
            }


         }


         return (ID2D1Brush *)m_pradialgradientbrush.Get();

      }

      else
      {

         return (ID2D1Brush *) m_pbrush.Get();

      }

      return NULL;


   }

   bool brush::destroy()
   {

      if(m_psolidbrush != NULL)
      {

         m_psolidbrush = nullptr;

      }
      
      if(m_plineargradientbrush != NULL)
      {

         m_plineargradientbrush = nullptr;

      }
      
      if(m_pbrush != NULL)
      {

         m_pbrush = nullptr;

      }

      return true;

   }


} // namespace draw2d_direct2d




