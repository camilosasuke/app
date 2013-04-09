#include "framework.h"
#undef new
#include <gdiplus.h>


simple_brush::simple_brush()
{

   m_pbrush    = ::null();

   m_bDelete   = true;

}


simple_brush::~simple_brush()
{
   
   if(m_pbrush != ::null())
   {

      destroy();

   }

}


bool simple_brush::create_solid(simple_graphics & g, COLORREF cr)
{

   UNREFERENCED_PARAMETER(g);

   if(m_pbrush != ::null())
   {
    
      destroy();

   }

   m_pbrush = new Gdiplus::SolidBrush(Gdiplus::Color(GetAValue(cr), GetRValue(cr), GetGValue(cr), GetBValue(cr)));

   if(m_pbrush == ::null())
      return false;

   m_bDelete = true;

   return true;

}


bool simple_brush::create_linear_gradient(simple_graphics & g, POINT np1, POINT np2, COLORREF cr1, COLORREF cr2)
{

   if(m_pbrush != ::null())
   {

      destroy();

   }

   Gdiplus::Point p1;
   Gdiplus::Point p2;

   p1.X = np1.x;
   p1.Y = np1.y;
   
   p2.X = np2.x;
   p2.Y = np2.y;

   m_pbrush = new Gdiplus::LinearGradientBrush(
      p1, 
      p2,
      Gdiplus::Color(GetAValue(cr1), GetRValue(cr1), GetGValue(cr1), GetBValue(cr1)), 
      Gdiplus::Color(GetAValue(cr2), GetRValue(cr2), GetGValue(cr2), GetBValue(cr2)));

   if(m_pbrush == ::null())
      return false;

   m_bDelete = true;

   return true;

}


bool simple_brush::from_stock(int32_t iId)
{

   if(m_pbrush != ::null())
   {

      destroy();

   }

   if(iId == NULL_BRUSH)
   {

      m_pbrush = ::null();

   }
   else
   {
      return false;

   }

   return true;

}



bool simple_brush::destroy()
{

   if(m_pbrush == ::null())
      return true;

   bool bOk = true;


   if(m_bDelete)
   {

      try
      {

         delete m_pbrush;

      }
      catch(...)
      {

         bOk = false;

      }

   }

   m_bDelete = false;
   
   m_pbrush = ::null();

   if(!bOk)
      return false;

   return true;

}



simple_brush & simple_brush::operator = (const simple_brush & brush)
{

   if(m_pbrush != ::null())
   {

      destroy();

   }

   m_bDelete = false;

   m_pbrush = brush.m_pbrush;

   return *this;

}