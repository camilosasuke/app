//#include "framework.h"



#ifdef VARIADIC_TEMPLATE_FORMAT2


#ifdef APPLEOS

////#include <stdlib.h>

#endif

#ifdef ANDROID

#include <stdlib.h>

#endif

string_format::string_format()
{
   construct();
}

void string_format::construct()
{

   m_pszBuffer                   = NULL;
   m_iAllocation                 = 0;
   m_iLength                     = 0;

   m_estate                      = state_initial;

   m_bLeftJustify                = false;
   m_bForceShowSign              = false;
   m_bSharp                      = false;
   m_bZeroPadding                = false;


   m_iWidth                      = -1;
   m_iPrecision                  = -1;
   m_chLength                    = '\0';
   m_chSpec                      = '\0';

}


string_format::~string_format()
{

   if(m_pszBuffer != NULL)
   {

      memory_free(m_pszBuffer);

   }

}


void string_format::allocate_add_up(strsize iLenAddUp)
{

   strsize iNewSize;

   if(m_iAllocation <= 0)
   {

      iNewSize = ((iLenAddUp + 1 + 1023) & ~1023);

   }
   else
   {

      iNewSize = ((m_iLength + iLenAddUp + 1 + 1023) & ~1023);

      if(iNewSize < m_iAllocation)
      {

         return;

      }

   }

   if(iNewSize != m_iAllocation)
   {

      if(m_pszBuffer == NULL)
      {

         m_pszBuffer = (char *) memory_alloc(iNewSize);

      }
      else
      {

         m_pszBuffer = (char *) memory_realloc(m_pszBuffer, iNewSize);

      }

      m_iAllocation = iNewSize;

   }

   if(m_pszBuffer == NULL)
      throw memory_exception(get_thread_app());


   memset(&m_pszBuffer[m_iLength], 0, m_iAllocation - m_iLength);

}




bool string_format::parse(const char * & s)
{
   if(m_estate == state_initial)
   {
      while(*s != '\0')
      {
         if(*s == '-')
         {
            m_bLeftJustify = true;
         }
         else if(*s == '+')
         {
            m_bForceShowSign = true;
         }
         else if(*s == '#')
         {
            m_bSharp = true;
         }
         else if(*s == '0')
         {
            m_bZeroPadding = true;
         }
         else
         {
            break;
         }
         s++;
      }
      if(*s == '\0')
      {
         throw simple_exception(get_thread_app(), "unfineshed argument specifier");
      }
      const char * start = s;
      while(*s != '\0')
      {
         if(*s == '*')
         {
            s++;
            m_estate = state_waiting_width;
            return true;
         }
         else if(isdigit_dup(*s))
         {
         }
         else
         {
            break;
         }
         s++;
      }
      if(*s == '\0')
      {
         throw simple_exception(get_thread_app(), "unfineshed argument specifier");
      }
      if(s > start)
      {
         m_iWidth = (int32_t) natoi_dup(start, s - start);
      }
      m_estate = state_parse_precision;
   }
   if(m_estate == state_parse_precision)
   {
      if(*s == '.')
      {
         s++;
      }
      const char * start = s;
      while(*s != '\0')
      {
         if(*s == '*')
         {
            s++;
            m_estate = state_waiting_precision;
            return true;
         }
         else if(isdigit_dup(*s))
         {
         }
         else
         {
            break;
         }
         s++;
      }
      if(*s == '\0')
      {
         throw simple_exception(get_thread_app(), "unfineshed argument specifier");
      }
      if(s > start)
      {
         m_iPrecision = natoi_dup(start, s - start);
      }
      m_estate = state_parse_length;
   }
   if(m_estate == state_parse_length)
   {
      if(*s == 'l' || *s == 'L' || *s == 'h')
      {
         m_chLength = *s;
         s++;
      }
      if(*s == '\0')
      {
         throw simple_exception(get_thread_app(), "unfineshed argument specifier");
      }
      if(*s == 'c' || *s == 'd' || *s == 'i'
      || *s == 'e' || *s == 'E' || *s == 'f'
      || *s == 'g' || *s == 'G' || *s == 'o'
      || *s == 'g' || *s == 'G' || *s == 's' || *s == 'S'
      || *s == 'u' || *s == 'x' || *s == 'X'
      || *s == 'p')
      {
         m_chLength = *s;
         s++;
      }
      else if(*s == 'I' && *(s + 1) == '6' && *(s + 2) == '4' && *(s + 3) == 'd')
      {
         m_chLength = *s;
         s+=4;
      }
      else
      {
         throw simple_exception(get_thread_app(), "unfineshed format specifier");
      }
      return false;
   }
   else
   {
      throw "invalid state";
   }


}

void string_format::format(const char * & s)
{

   while (*s)
   {

      if (*s == '%' && *(++s) != '%')
         throw simple_exception(get_thread_app(), "invalid format string: missing arguments");

      append(*s++);

   }

}


   namespace str
   {



      void format(string_format * pformat, char const & ch)
      {

         if(pformat->m_chLength == 'X')
         {

            string str = ::hex::upper_from((void *) &ch, 1);

            while(pformat->m_iWidth > str.get_length())
            {
               str = "0" + str;
            }

            pformat->append(str);

         }
         else if (pformat->m_chLength == 'x')
         {

            string str = ::hex::upper_from((void *)&ch, 1);

            while (pformat->m_iWidth > str.get_length())
            {
               str = "0" + str;
            }

            pformat->append(str);

         }
         else
         {

            pformat->append(string((char) ch, 1));

         }

      }



      void format(string_format * pformat, uchar const & uch)
      {


         string str;
         if(pformat->m_chLength == 'x')
         {
            str = ::hex::lower_from((int) uch);
         }
         else if(pformat->m_chLength == 'X')
         {
            str = ::hex::upper_from((int)uch);
         }
         else
         {
            pformat->append(string((char)uch,1));
         }
         if(pformat->m_bZeroPadding)
         {
            while(str.get_length() < pformat->m_iWidth)
            {
               str = "0" + str;
            }
         }
         pformat->append(str);

      }



      void format(string_format * pformat, int16_t const & sh)
      {

         string str = ::str::from((int32_t)sh);
         if(pformat->m_bZeroPadding)
         {
            while(str.get_length() < pformat->m_iWidth)
            {
               str = "0" + str;
            }
         }
         pformat->append(str);

      }



      void format(string_format * pformat, uint16_t const & ush)
      {

         string str = ::str::from((uint32_t) ush);
         if(pformat->m_bZeroPadding)
         {
            while(str.get_length() < pformat->m_iWidth)
            {
               str = "0" + str;
            }
         }
         pformat->append(str);

      }



      void format(string_format * pformat, int32_t const & i)
      {
         // TODO: use specs
         string str;
         if (pformat->m_chLength == 'X')
         {

            str = ::hex::upper_from(i);

         }
         else if (pformat->m_chLength == 'x')
         {

            str = ::hex::upper_from(i);

         }
         else
         {

            str = ::str::from(i);

         }
         if(pformat->m_bZeroPadding)
         {
            while(str.get_length() < pformat->m_iWidth)
            {
                str = "0" + str;
            }
         }
         else
         {
            while(str.get_length() < pformat->m_iWidth)
            {
               str = " " + str;
            }
         }
         pformat->append(str);

      }


      void format(string_format * pformat, uint32_t const & ui)
      {
         // TODO: use specs
         string str;
         if(pformat->m_chLength == 'x')
         {
            str = ::hex::lower_from(ui);
         }
         else if(pformat->m_chLength == 'X')
         {
            str = ::hex::upper_from(ui);
         }
         else
         {
            str = ::str::from(ui);
         }
         if(pformat->m_bZeroPadding)
         {
            while(str.get_length() < pformat->m_iWidth)
            {
               str = "0" + str;
            }
         }
         pformat->append(str);

      }


      void format(string_format * pformat, int64_t const & i)
      {
         // TODO: use specs
         pformat->append(::str::from(i));

      }


//#ifdef ANDROID
//
//
//      void format(string_format * pformat, int64_t const long & i)
//      {
//         // TODO: use specs
//         pformat->append(::str::from(i));
//
//      }
//
//
//#endif
#ifdef ANDROID

      CLASS_DECL_AURA void format(string_format * pformat, long int const & i)
      {

         format(pformat, (long int)i);

      }

#endif

      void format(string_format * pformat, uint64_t const & ui)
      {
         // TODO: use specs
         // TODO: use specs
         string str;
         if (pformat->m_chLength == 'x')
         {
            str = ::hex::lower_from(ui);
         }
         else if (pformat->m_chLength == 'X')
         {
            str = ::hex::upper_from(ui);
         }
         else
         {
            str = ::str::from(ui);
         }
         if (pformat->m_bZeroPadding)
         {
            while (str.get_length() < pformat->m_iWidth)
            {
               str = "0" + str;
            }
         }
         pformat->append(str);

      }


      void format(string_format * pformat, float const & f)
      {
         // TODO: use specs
         pformat->append(::str::from(f));

      }



      void format(string_format * pformat, double const & d)
      {
         // TODO: use specs
         #ifdef WINDOWSEX
         char sz[_CVTBUFSIZE];
#elif defined(__APPLE__) || defined(LINUX) || defined(VSNORD)
         char sz[500];
         #else
         char sz[CVTBUFSIZE];
         #endif
         int decimal_point;
         int negative = d < 0.0;
         if(pformat->m_iPrecision >= 0)
         {
             if(fcvt_dup(sz, sizeof(sz), d, pformat->m_iPrecision, &decimal_point, &negative) != 0)
             {
                pformat->append(::str::from(d));
                return;
             }

             string str(sz);
             string strResult;

             if(negative)
             {
                 strResult = "-";
             }
             else if(pformat->m_bForceShowSign)
             {
                strResult = "+";
             }

             if(decimal_point == 0)
             {
                strResult += "0";
             }
             else
             {
                strResult += str.substr(0,decimal_point);
             }
             if(pformat->m_iPrecision > 0)
             {
                strResult += ".";
                strResult += str.substr(decimal_point);
             }



            pformat->append(strResult);

         }
         else
         {

            if(d == 0.0)
            {
               pformat->append("0.0");
               return;
            }

            int i = 0;

            int digits = MIN(10,sizeof(sz) - 2);

#ifdef WINDOWS
            if(_gcvt_s(sz,sizeof(sz),fabs(d),digits))
            {
               pformat->append(::str::from(d));
               return;
            }
#elif defined(LINUX) || defined(MACOS) 
            if(gcvt(fabs(d),digits, sz))
            {
               pformat->append(::str::from(d));
               return;
            }

#else
            int iSize = sizeof(sz) - 1;
            if(max_cvt_dup(sz,sizeof(sz),d,1024,&decimal_point,&negative,&iSize) != 0)
            {
               pformat->append(::str::from(d));
               return;
            }
#endif

             string str(sz);
             string strResult;

             if(negative)
             {
                 strResult = "-";
             }
             else if(pformat->m_bForceShowSign)
             {
                strResult = "+";
             }

             ::str::ends_eat(str,".");

             /*strResult += str.substr(0, decimal_point);
             if(decimal_point < str.get_length())
             {
                strResult += ".";
                strResult += str.substr(decimal_point);
             }*/
             strResult += str;


            pformat->append(strResult);

         }



      }




      void format(string_format * pformat, const void * const & p)
      {
         // TODO: use specs
         pformat->append(::str::from((uint_ptr)p));

      }



      void format(string_format * pformat, void * const & p)
      {
         // TODO: use specs

         string str;

         if(pformat->m_chLength == 'x')
         {
            str = ::hex::lower_from((uint_ptr)p);
         }
         else if (pformat->m_chLength == 'x')
         {
            str = ::hex::upper_from((uint_ptr)p);
         }
         else
         {
            str = ::str::from((uint_ptr)p);
         }

         if (pformat->m_bZeroPadding)
         {

            ::zero_pad(str, pformat->m_iWidth);

         }

         pformat->append(str);

      }



      void format(string_format * pformat, const char * const & psz)
      {

         pformat->append(psz);

      }



      void format(string_format * pformat, unsigned char * const & psz)
      {

         pformat->append((const char *) psz);

      }




      void format(string_format * pformat, string const & str)
      {

         pformat->append(str);

      }




      void format(string_format * pformat, lparam const & lparam)
      {

          ::str::format(pformat, (int_ptr)lparam.m_lparam);

      }

   } // namespace str




#endif


#ifdef DEBUG

   CLASS_DECL_AURA void debug_ca2_variadic_template_base_formatting_integer_zero_padding()
   {

      string str1;

      str1.Format("%d", 184);
      str1.Format("%01d", 184);
      str1.Format("%02d", 184);
      str1.Format("%03d", 184);
      str1.Format("%04d", 184);
      str1.Format("%05d", 1984);
      str1.Format("%06d", 1984);

      short sh = 84;

      short sh1 = 2;

      str1.Format("%02d %03d", sh1, sh);

   }

#endif


