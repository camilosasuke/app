
#include <math.h>

#define BOX 0
#define BOX_SEL 1
#define BOX_HOVER 2



namespace visual
{


   font_list_data::font_list_data(::aura::application * papp) :
      object(papp)
   {

      m_rectMargin = rect(5, 5, 5, 5);
      m_iSel = -1;
      m_iHover = -1;

      defer_create_mutex();

      m_dwaFg.add(ARGB(255, 80, 80, 80));
      m_dwaFg.add(ARGB(255, 45, 45, 45));
      m_dwaFg.add(ARGB(255, 255, 255, 255));

      m_dwaBg.add(ARGB(0, 0, 0, 0));
      m_dwaBg.add(ARGB(255, 128, 200, 152));
      m_dwaBg.add(ARGB(255, 80, 80, 80));

   }


   font_list_data::~font_list_data()
   {


   }

   font_list_data::text_box::text_box()
   {

      m_bOk = false;

   }

   void font_list_data ::text_box::update(font_list_data * pdata, int iBox, string strText)
   {

      m_dib->create(m_size);

      m_dib->get_graphics()->FillSolidRect(0, 0, m_size.cx, m_size.cy, pdata->m_dwaBg[iBox]);

      m_dib->get_graphics()->selectFont(m_font);

      m_dib->get_graphics()->set_text_color(pdata->m_dwaFg[iBox]);

      m_dib->get_graphics()->text_out(pdata->m_rectMargin.left, pdata->m_rectMargin.top, strText);

      m_bOk = true;

   }

   void font_list_data::_001OnDraw(::draw2d::graphics * pgraphics)
   {

      synch_lock sl(m_pmutex);

      pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);

      rect rectClient = m_rectClient;



      for (int i = 0; i < m_itemptra.get_count(); i++)
      {

         if (m_itemptra[i] == NULL)
         {

            continue;

         }
         
         int iBox;

         if (i == m_iSel)
         {
            continue;
            iBox = BOX_SEL;
         }
         else if (i == m_iHover)
         {
            continue;
            iBox = BOX_HOVER;
         }
         else
         {
            iBox = BOX;
         }
         
         text_box * pbox = &m_itemptra[i]->m_box[iBox];



         if (!pbox->m_bOk)
         {
            
            pbox->update(this, iBox, m_itemptra[i]->m_strSample);

         }

         pgraphics->BitBlt(pbox->m_rect, pbox->m_dib->get_graphics());

      }

      if (m_iSel >= 0)
      {

         text_box * pbox = &m_itemptra[m_iSel]->m_box[BOX_SEL];

         if (!pbox->m_bOk)
         {

            pbox->update(this, BOX_SEL, m_itemptra[m_iSel]->m_strSample);

         }
         pgraphics->BitBlt(pbox->m_rect, pbox->m_dib->get_graphics());
      }
      if (m_iHover >= 0 && m_iHover != m_iSel)
      {

         text_box * pbox = &m_itemptra[m_iHover]->m_box[BOX_HOVER];

         if (!pbox->m_bOk)
         {

            pbox->update(this, BOX_HOVER, m_itemptra[m_iHover]->m_strSample);

         }
         pgraphics->BitBlt(pbox->m_rect, pbox->m_dib->get_graphics());
      }


   }


   void font_list_data::update()
   {

      {

         synch_lock sl(m_pmutex);

         auto * penum = System.visual().fonts().m_pfontenumeration;

         synch_lock slEnum(penum->m_pmutex);

         if (penum->m_itema == m_itema)
            return;

         m_itema = penum->m_itema;

      }

      {

         synch_lock sl(m_pmutex);

         m_itemptra.remove_all();

      }

      int nextx;

      index i = 0;
      index iCount = m_itema.get_count();
      index iScan = 1;
      index iOrder = 0;

      m_itemptra.set_size(iCount);

      output_debug_string("Middle");

      ::fork_count(get_app(), m_itema.get_count(), [&](index iOrder, index i, index iCount, index iScan)
      {

         ::draw2d::graphics_sp g(allocer());

         g->CreateCompatibleDC(NULL);

         ::draw2d::graphics * pgraphics = g;

         string strText = m_strTextLayout;

         size s;

         rect r;

         int_array iaSize;

         iaSize.add(18);
         iaSize.add(24);
         iaSize.add(30);

         for (; i < iCount; i += iScan)
         {

            synch_lock sl(m_pmutex);

            item * pitem = new item();

            m_itemptra[i] = pitem;

            //         pitem->m_dibSel.alloc(allocer());

            pitem->m_strFont = m_itema[i].m_strFile;

            pitem->m_strName = m_itema[i].m_strName;

            string str = pitem->m_strFont;

            for (index j = 0; j < iaSize.get_count(); j++)
            {

               text_box * pbox = &pitem->m_box[j];

               pbox->m_font.alloc(allocer());

               pbox->m_dib.alloc(allocer());

               if (str.compare_ci("GOUDY STOUT") == 0)
               {

                  output_debug_string("test05");

               }

               pbox->m_font->create_pixel_font(str, iaSize[j]);

               pgraphics->SelectFont(pbox->m_font);

               pbox->m_font->m_ecs = m_itema[i].m_ecs;

               if (j == 0)
               {

                  strText = m_strTextLayout;

                  if (strText.is_empty() || (pbox->m_font->m_ecs != ::draw2d::font::cs_ansi && pbox->m_font->m_ecs != ::draw2d::font::cs_default))
                  {

                     strText = ::draw2d::font::get_sample_text(pbox->m_font->m_ecs);

                     if (strText.is_empty())
                     {

                        strText = pitem->m_strName;

                     }

                  }

                  s = pgraphics->GetTextExtent(strText);

                  if (strText.has_char() && s.area() <= 0)
                  {

                     string strSample;
                     int maxarea = 0;
                     ::draw2d::font::e_cs ecs;
                     ::draw2d::font::e_cs ecsFound = pbox->m_font->m_ecs;
                     size sSample;


                     if (maxarea <= 0)
                     {

                        strSample = m_strTextLayout;

                        if (strSample.has_char())
                        {

                           sSample = pgraphics->GetTextExtent(strSample);

                           if (sSample.area() > maxarea)
                           {

                              maxarea = sSample.area();

                              strText = strSample;

                              s = sSample;

                           }

                        }

                     }

                     if (maxarea <= 0)
                     {

                        strSample = pitem->m_strName[i];

                        if (strSample.has_char())
                        {

                           sSample = pgraphics->GetTextExtent(strSample);

                           if (sSample.area() > maxarea)
                           {

                              maxarea = sSample.area();

                              strText = strSample;

                              s = sSample;

                           }

                        }

                     }

                     pbox->m_font->m_ecs = ecsFound;


                  }
                  pitem->m_strSample = strText;

               }
               else
               {

                  pbox->m_font->m_ecs = pitem->m_box[j - 1].m_font->m_ecs;

                  s = pgraphics->GetTextExtent(pitem->m_strSample);

               }

               s.cx += m_rectMargin.left + m_rectMargin.right;
               s.cy += m_rectMargin.top + m_rectMargin.bottom;


               pbox->m_size = s;


               pbox->m_bOk = false;
               //pitem->m_box[1].m_bOk = false;
               //pitem->m_box[2].m_bOk = false;



            }


            sl.unlock();

         }

      }
            );

      output_debug_string("End");
      /*
      ,
      [&]()
      {

      update();

      }

      );
      */


         }

         void font_list_data::on_layout()
         {

            synch_lock sl(m_pmutex);

            string strText = m_strTextLayout;

            size s;

            int x = 0;

            int y = 0;

            int h = 0;

            sl.lock();

            int nextx;

            for (int i = 0; i < m_itemptra.get_count(); i++)
            {

               item * pitem = m_itemptra[i];

               if (pitem == NULL)
               {

                  continue;

               }

               size & s = pitem->m_box[0].m_size;

               rect & r = pitem->m_box[0].m_rect;

               nextx = x + s.cx;

               if (nextx >= m_rectClient.width())
               {
                  x = 0;
                  nextx = s.cx;
                  y += h;
                  h = 0;
               }

               //pgraphics->text_out(x + m_rectMargin.left,y + m_rectMargin.top,strText);

               r.left = x;
               r.top = y;
               r.right = r.left + s.cx;
               r.bottom = r.top + s.cy;

               x = nextx;

               h = MAX(h, s.cy);

               for (index j = 1; j < 3; j++)
               {

                  size & s2 = pitem->m_box[j].m_size;
                  rect & r2 = pitem->m_box[j].m_rect;

                  int dw = (s2.cx - s.cx) / 2;
                  int dh = (s2.cy - s.cy) / 2;

                  r2.left = r.left - dw;
                  r2.top = r.top - dh;
                  r2.right = r2.left + s2.cx;
                  r2.bottom = r2.top + s2.cy;

               }

            }

         }




         index font_list_data::hit_test(point pt)
         {

            synch_lock sl(m_pmutex);

            for (index i = 0; i < m_itemptra.get_size(); i++)
            {

               if (m_itemptra[i] == NULL)
               {

                  continue;

               }

               if (i == m_iSel)
               {
                  if (m_itemptra[i]->m_box[BOX_SEL].m_rect.contains(pt))
                  {

                     return i;
                  }
               }
               else if(i == m_iHover)
               {
                  if (m_itemptra[i]->m_box[BOX_HOVER].m_rect.contains(pt))
                  {

                     return i;
                  }
               }
               else
               {
                  if (m_itemptra[i]->m_box[BOX].m_rect.contains(pt))
                  {

                     return i;
                  }
               }

            }

            return -1;
         }


         index font_list_data::find_name(string str)
         {

            return m_itemptra.pred_find_first([&](item *pitem) {return pitem->m_strName.compare_ci(str) == 0;  });

         }



      } // namespace user

















