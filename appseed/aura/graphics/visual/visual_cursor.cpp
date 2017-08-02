

namespace visual
{

   cursor::cursor(::aura::application * papp) :
      object(papp),
      m_dib(allocer())
   {

#ifdef WINDOWSEX

      m_hcursor = NULL;

#endif

   }


   bool cursor::to(::draw2d::graphics * pgraphics, point pt)
   {

      pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);

      return pgraphics->BitBlt(pt - m_szHotspotOffset, m_dib->m_size, m_dib->get_graphics(), null_point());

   }
#ifdef WINDOWSEX

   bool cursor::initialize_system_default()
   {

      const char * psz;

      if (m_ecursor == cursor_arrow)
      {

         psz = IDC_ARROW;

      }
      else if (m_ecursor == cursor_size_top || m_ecursor == cursor_size_bottom)
      {

         psz = IDC_SIZENS;

      }
      else if (m_ecursor == cursor_size_left || m_ecursor == cursor_size_right)
      {

         psz = IDC_SIZEWE;

      }
      else if (m_ecursor == cursor_size_top_left || m_ecursor == cursor_size_bottom_right)
      {

         psz = IDC_SIZENWSE;

      }
      else if (m_ecursor == cursor_size_top_right || m_ecursor == cursor_size_bottom_left)
      {

         psz = IDC_SIZENESW;

      }
      else if (m_ecursor == cursor_text_select)
      {

         psz = IDC_IBEAM;

      }
      else if (m_ecursor == cursor_hand)
      {

         psz = IDC_HAND;

      }
      else
      {

         return false;

      }

      m_hcursor = LoadCursor(NULL, psz);
            
      if (m_hcursor == NULL)
      {
         
         return false;

      }

      return true;

   }
#else
   bool cursor::initialize_system_default()
   {

      return true;

   }

#endif
#ifdef WINDOWSEX

   HCURSOR cursor::get_HCURSOR()
   {

      if(m_hcursor == NULL)
      {

         m_hcursor = ::CreateAlphaCursor(m_dib,m_szHotspotOffset.cx,m_szHotspotOffset.cy);

      }

      return m_hcursor;

   }

#endif


   void cursor_alloc(::aura::application * papp,cursor * & pdib,int xHotspot,int yHotspot)
   {

      pdib = new cursor(papp);

      pdib->m_szHotspotOffset.cx = xHotspot;
      pdib->m_szHotspotOffset.cy = yHotspot;

   }


   ::draw2d::dib * cursor_get_dib(cursor * pcursor)
   {

      return pcursor->m_dib;

   }


} // namespace visual










