#include "framework.h"


namespace datetime
{


   const char * const szInvalidDateTime = "Invalid DateTime";
   const char * const szInvalidDateTimeSpan = "Invalid DateTimeSpan";


   date_span::date_span()
   {
      m_iMonth = 0;
      m_iYear = 0;
      m_bConstraintMonth = 0;

   }
   /////////////////////////////////////////////////////////////////////////////
   // time_span
   /////////////////////////////////////////////////////////////////////////////

   time_span::time_span() throw() :
      m_timeSpan(0)
   {
   }

   time_span::time_span( __time64_t time ) throw() :
      m_timeSpan( time )
   {
   }

   time_span::time_span(LONG lDays, int nHours, int nMins, int nSecs) throw()
   {
      m_timeSpan = nSecs + 60* (nMins + 60* (nHours + int64_t(24) * lDays));
   }

   LONGLONG time_span::GetDays() const throw()
   {
      return( m_timeSpan/(24*3600) );
   }

   LONGLONG time_span::GetTotalHours() const throw()
   {
      return( m_timeSpan/3600 );
   }

   LONG time_span::GetHours() const throw()
   {
      return( LONG( GetTotalHours()-(GetDays()*24) ) );
   }

   LONGLONG time_span::GetTotalMinutes() const throw()
   {
      return( m_timeSpan/60 );
   }

   LONG time_span::GetMinutes() const throw()
   {
      return( LONG( GetTotalMinutes()-(GetTotalHours()*60) ) );
   }

   LONGLONG time_span::GetTotalSeconds() const throw()
   {
      return( m_timeSpan );
   }

   LONG time_span::GetSeconds() const throw()
   {
      return( LONG( GetTotalSeconds()-(GetTotalMinutes()*60) ) );
   }

   __time64_t time_span::GetTimeSpan() const throw()
   {
      return( m_timeSpan );
   }

   time_span time_span::operator+( time_span span ) const throw()
   {
      return( time_span( m_timeSpan+span.m_timeSpan ) );
   }

   time_span time_span::operator-( time_span span ) const throw()
   {
      return( time_span( m_timeSpan-span.m_timeSpan ) );
   }

   time_span& time_span::operator+=( time_span span ) throw()
   {
      m_timeSpan += span.m_timeSpan;
      return( *this );
   }

   time_span& time_span::operator-=( time_span span ) throw()
   {
      m_timeSpan -= span.m_timeSpan;
      return( *this );
   }

   bool time_span::operator==( time_span span ) const throw()
   {
      return( m_timeSpan == span.m_timeSpan );
   }

   bool time_span::operator!=( time_span span ) const throw()
   {
      return( m_timeSpan != span.m_timeSpan );
   }

   bool time_span::operator<( time_span span ) const throw()
   {
      return( m_timeSpan < span.m_timeSpan );
   }

   bool time_span::operator>( time_span span ) const throw()
   {
      return( m_timeSpan > span.m_timeSpan );
   }

   bool time_span::operator<=( time_span span ) const throw()
   {
      return( m_timeSpan <= span.m_timeSpan );
   }

   bool time_span::operator>=( time_span span ) const throw()
   {
      return( m_timeSpan >= span.m_timeSpan );
   }


   string time_span::Format(const char * pFormat) const
      // formatting timespans is a little trickier than formatting CTimes
      //  * we are only interested in relative time formats, ie. it is illegal
      //      to format anything dealing with absolute time (i.e. years, months,
      //         day of week, day of year, timezones, ...)
      //  * the only valid formats:
      //      %D - # of days
      //      %H - hour in 24 hour format
      //      %M - minute (0-59)
      //      %S - seconds (0-59)
      //      %% - percent sign
      //   %#<any_of_mods> - skip leading zeros
   {
      ASSERT( pFormat != NULL );
      if( pFormat == NULL )
         throw hresult_exception( E_INVALIDARG );

      string strBuffer;
      // string hmsFormats [_CTIMESPANFORMATS] = {"%c","%02ld","%d"};
      //   string dayFormats [_CTIMESPANFORMATS] = {"%c","%I64d","%I64d"};
      strBuffer.Preallocate(maxTimeBufferSize);
      char ch;

      while ((ch = *pFormat++) != '\0')
      {
         enum _CTIMESPANFORMATSTEP formatstep = _CTFS_NONE;
         if(ch == '%')
         {
            formatstep = _CTFS_FORMAT;
            ch = *pFormat++;
            if(ch == '#')
            {
               formatstep = _CTFS_NZ;
               ch = *pFormat++;
            }
         }
         switch (ch)
         {
         case '%':
            strBuffer += ch;
            break;
         case 'D':
            //            strBuffer.AppendFormat(dayFormats[formatstep], formatstep ? GetDays()    : ch);
            break;
         case 'H':
            //            strBuffer.AppendFormat(hmsFormats[formatstep], formatstep ? GetHours()   : ch);
            break;
         case 'M':
            //            strBuffer.AppendFormat(hmsFormats[formatstep], formatstep ? GetMinutes() : ch);
            break;
         case 'S':
            //            strBuffer.AppendFormat(hmsFormats[formatstep], formatstep ? GetSeconds() : ch);
            break;
         default:
            if(formatstep)
            {
#pragma warning (push)
#pragma warning (disable: 4127)  // conditional expression constant
               ENSURE(FALSE);      // probably a bad format character
#pragma warning (pop)
            }
            else
            {
               strBuffer += ch;
#ifdef _MBCS
               if (_istlead(ch))
               {
                  strBuffer += *pFormat++;
               }
#endif
            }
            break;
         }
      }

      return strBuffer;
   }


   ::datetime::time WINAPI time::get_current_time() throw()
   {

#ifdef WINDOWS

      return time( ::_time64( NULL ) ) ;

#else

      return time( ::time( NULL ) );

#endif

   }

   bool WINAPI time::is_valid_FILETIME(const FILETIME& fileTime) throw()
   {
      FILETIME localTime;
      if (!FileTimeToLocalFileTime(&fileTime, &localTime))
      {
         return FALSE;
      }

      // then convert that time to system time
      SYSTEMTIME sysTime;
      if (!FileTimeToSystemTime(&localTime, &sysTime))
      {
         return FALSE;
      }

      return TRUE;
   }

   time::time() throw() :
      m_time(0)
   {
   }

   time::time( __time64_t time )  throw():
      m_time( time )
   {
   }

   time::time(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec,
      int nDST)
   {
#pragma warning (push)
#pragma warning (disable: 4127)  // conditional expression constant


#pragma warning (pop)

      struct tm atm;

      atm.tm_sec = nSec;
      atm.tm_min = nMin;
      atm.tm_hour = nHour;
      atm.tm_mday = nDay;
      atm.tm_mon = nMonth - 1;        // tm_mon is 0 based
      atm.tm_year = nYear - 1900;     // tm_year is 1900 based
      atm.tm_isdst = nDST;

#ifdef WINDOWS

      m_time = _mktime64(&atm);

#else

      m_time = mktime(&atm);

#endif

      /*
      Remember that:
      ENSURE( nYear >= 1900 );
      ENSURE( nMonth >= 1 && nMonth <= 12 );
      ENSURE( nDay >= 1 && nDay <= 31 );
      ENSURE( nHour >= 0 && nHour <= 23 );
      ENSURE( nMin >= 0 && nMin <= 59 );
      ENSURE( nSec >= 0 && nSec <= 59 );
      ASSUME(m_time != -1);   */    // indicates an illegal input time
      if(m_time == -1)
      {
         throw invalid_argument_exception();
      }
   }

   time::time(WORD wDosDate, WORD wDosTime, int nDST)
   {

      struct tm atm;
      atm.tm_sec = (wDosTime & ~0xFFE0) << 1;
      atm.tm_min = (wDosTime & ~0xF800) >> 5;
      atm.tm_hour = wDosTime >> 11;

      atm.tm_mday = wDosDate & ~0xFFE0;
      atm.tm_mon = ((wDosDate & ~0xFE00) >> 5) - 1;
      atm.tm_year = (wDosDate >> 9) + 80;
      atm.tm_isdst = nDST;

#ifdef WINDOWS

      m_time = _mktime64(&atm);

#else

      m_time = mktime(&atm);

#endif

      ASSUME(m_time != -1);       // indicates an illegal input time

      if(m_time == -1)
         throw invalid_argument_exception();

   }

   time::time(const SYSTEMTIME& sysTime, int nDST)
   {
      if (sysTime.wYear < 1900)
      {
         __time64_t time0 = 0L;
         time timeT(time0);
         *this = timeT;
      }
      else
      {
         time timeT(
            (int)sysTime.wYear, (int)sysTime.wMonth, (int)sysTime.wDay,
            (int)sysTime.wHour, (int)sysTime.wMinute, (int)sysTime.wSecond,
            nDST);
         *this = timeT;
      }
   }

   time::time(const FILETIME& fileTime, int nDST)
   {
      // first convert file time (UTC time) to local time
      FILETIME localTime;
      if (!FileTimeToLocalFileTime(&fileTime, &localTime))
      {
         m_time = 0;
         throw invalid_argument_exception();
         return;
      }

      // then convert that time to system time
      SYSTEMTIME sysTime;
      if (!FileTimeToSystemTime(&localTime, &sysTime))
      {
         m_time = 0;
         throw invalid_argument_exception();
         return;
      }

      // then convert the system time to a time_t (C-runtime local time)
      time timeT(sysTime, nDST);
      *this = timeT;
   }

   ::datetime::time & time::operator=( __time64_t time ) throw()
   {
      m_time = time;

      return( *this );
   }

   ::datetime::time & time::operator+=( time_span span ) throw()
   {
      m_time += span.GetTimeSpan();

      return( *this );
   }

   ::datetime::time & time::operator-=( time_span span ) throw()
   {
      m_time -= span.GetTimeSpan();

      return( *this );
   }

   ::datetime::time & time::operator+=( date_span span )
   {
      UNREFERENCED_PARAMETER(span);
      throw not_implemented_exception(get_app());

      return( *this );
   }

   ::datetime::time& time::operator-=( date_span span )
   {
      UNREFERENCED_PARAMETER(span);
      throw not_implemented_exception(get_app());

      return( *this );
   }

   time_span time::operator-( time time ) const throw()
   {
      return( time_span( m_time-time.m_time ) );
   }

   ::datetime::time time::operator-( time_span span ) const throw()
   {
      return( time( m_time-span.GetTimeSpan() ) );
   }

   ::datetime::time time::operator+( time_span span ) const throw()
   {
      return( time( m_time+span.GetTimeSpan() ) );
   }

   ::datetime::time time::operator-( date_span span ) const
   {
      UNREFERENCED_PARAMETER(span);
      throw not_implemented_exception(get_app());
   }

   ::datetime::time time::operator+( date_span span ) const
   {
      UNREFERENCED_PARAMETER(span);
      throw not_implemented_exception(get_app());
   }

   bool time::operator==( time time ) const throw()
   {
      return( m_time == time.m_time );
   }

   bool time::operator!=( time time ) const throw()
   {
      return( m_time != time.m_time );
   }

   bool time::operator<( time time ) const throw()
   {
      return( m_time < time.m_time );
   }

   bool time::operator>( time time ) const throw()
   {
      return( m_time > time.m_time );
   }

   bool time::operator<=( time time ) const throw()
   {
      return( m_time <= time.m_time );
   }

   bool time::operator>=( time time ) const throw()
   {
      return( m_time >= time.m_time );
   }


   struct tm* time::GetGmtTm(struct tm* ptm) const
   {
      if (ptm != NULL)
      {


#ifdef WINDOWS

         struct tm tmTemp;

         errno_t err = _gmtime64_s(&tmTemp, &m_time);

         if (err != 0)
         {
            return NULL;    // indicates that m_time was not initialized!
         }

         *ptm = tmTemp;

         return ptm;

#else

         struct tm * ptmTemp;

         ptmTemp = gmtime(&m_time);

         if(ptmTemp == NULL)
            return NULL;

         if(errno != 0)
            return NULL;

         *ptm = *ptmTemp;

         return ptm;

#endif

      }
      else
      {

         return NULL;

      }


   }

   struct tm* time::GetLocalTm(struct tm* ptm) const
   {

      if (ptm != NULL)
      {


#ifdef WINDOWS

         struct tm tmTemp;

         errno_t err = _localtime64_s(&tmTemp, &m_time);

         if (err != 0)
         {
            return NULL;    // indicates that m_time was not initialized!
         }

         *ptm = tmTemp;

         return ptm;

#else

         struct tm * ptmTemp;

         ptmTemp = localtime(&m_time);

         if(ptmTemp == NULL)
            return NULL;

         if(errno != 0)
            return NULL;

         *ptm = *ptmTemp;

         return ptm;

#endif

      }
      else
      {

         return NULL;

      }

   }

   bool time::get_as_system_time(SYSTEMTIME& timeDest) const throw()
   {
      struct tm ttm;
      struct tm* ptm;

      ptm = GetLocalTm(&ttm);

      if(!ptm) { return false; }

      timeDest.wYear = (WORD) (1900 + ptm->tm_year);
      timeDest.wMonth = (WORD) (1 + ptm->tm_mon);
      timeDest.wDayOfWeek = (WORD) ptm->tm_wday;
      timeDest.wDay = (WORD) ptm->tm_mday;
      timeDest.wHour = (WORD) ptm->tm_hour;
      timeDest.wMinute = (WORD) ptm->tm_min;
      timeDest.wSecond = (WORD) ptm->tm_sec;
      timeDest.wMilliseconds = 0;

      return true;
   }

   __time64_t time::get_time() const NOTHROW
   {
      return( m_time );
   }

   int time::GetYear() const NOTHROW
   {
      struct tm ttm;
      struct tm * ptm;

      ptm = GetLocalTm(&ttm);
      return ptm ? (ptm->tm_year) + 1900 : 0 ;
   }

   int time::GetMonth() const NOTHROW
   {
      struct tm ttm;
      struct tm * ptm;

      ptm = GetLocalTm(&ttm);
      return ptm ? ptm->tm_mon + 1 : 0;
   }

   int time::GetDay() const NOTHROW
   {
      struct tm ttm;
      struct tm * ptm;

      ptm = GetLocalTm(&ttm);
      return ptm ? ptm->tm_mday : 0 ;
   }

   int time::GetHour() const NOTHROW
   {
      struct tm ttm;
      struct tm * ptm;

      ptm = GetLocalTm(&ttm);
      return ptm ? ptm->tm_hour : -1 ;
   }

   int time::GetMinute() const NOTHROW
   {
      struct tm ttm;
      struct tm * ptm;

      ptm = GetLocalTm(&ttm);
      return ptm ? ptm->tm_min : -1 ;
   }

   int time::GetSecond() const NOTHROW
   {
      struct tm ttm;
      struct tm * ptm;

      ptm = GetLocalTm(&ttm);
      return ptm ? ptm->tm_sec : -1 ;
   }

   int time::GetDayOfWeek() const NOTHROW
   {
      struct tm ttm;
      struct tm * ptm;

      ptm = GetLocalTm(&ttm);
      return ptm ? ptm->tm_wday + 1 : 0 ;
   }

   int time::GetGmtYear() const NOTHROW
   {
      struct tm ttm;
      struct tm * ptm;

      ptm = GetGmtTm(&ttm);
      return ptm ? (ptm->tm_year) + 1900 : 0 ;
   }

   int time::GetGmtMonth() const NOTHROW
   {
      struct tm ttm;
      struct tm * ptm;

      ptm = GetGmtTm(&ttm);
      return ptm ? ptm->tm_mon + 1 : 0;
   }

   int time::GetGmtDay() const NOTHROW
   {
      struct tm ttm;
      struct tm * ptm;

      ptm = GetGmtTm(&ttm);
      return ptm ? ptm->tm_mday : 0 ;
   }

   int time::GetGmtHour() const NOTHROW
   {
      struct tm ttm;
      struct tm * ptm;

      ptm = GetGmtTm(&ttm);
      return ptm ? ptm->tm_hour : -1 ;
   }

   int time::GetGmtMinute() const NOTHROW
   {
      struct tm ttm;
      struct tm * ptm;

      ptm = GetGmtTm(&ttm);
      return ptm ? ptm->tm_min : -1 ;
   }

   int time::GetGmtSecond() const NOTHROW
   {
      struct tm ttm;
      struct tm * ptm;

      ptm = GetGmtTm(&ttm);
      return ptm ? ptm->tm_sec : -1 ;
   }

   int time::GetGmtDayOfWeek() const NOTHROW
   {
      struct tm ttm;
      struct tm * ptm;

      ptm = GetGmtTm(&ttm);
      return ptm ? ptm->tm_wday + 1 : 0 ;
   }

   /////////////////////////////////////////////////////////////////////////////
   // file_time_span
   /////////////////////////////////////////////////////////////////////////////

   file_time_span::file_time_span() throw() :
      m_nSpan( 0 )
   {
   }

   file_time_span::file_time_span( const file_time_span& span ) throw() :
      m_nSpan( span.m_nSpan )
   {
   }

   file_time_span::file_time_span( LONGLONG nSpan ) throw() :
      m_nSpan( nSpan )
   {
   }

   file_time_span& file_time_span::operator=( const file_time_span& span ) throw()
   {
      m_nSpan = span.m_nSpan;

      return( *this );
   }

   file_time_span& file_time_span::operator+=( file_time_span span ) throw()
   {
      m_nSpan += span.m_nSpan;

      return( *this );
   }

   file_time_span& file_time_span::operator-=( file_time_span span ) throw()
   {
      m_nSpan -= span.m_nSpan;

      return( *this );
   }

   file_time_span file_time_span::operator+( file_time_span span ) const throw()
   {
      return( file_time_span( m_nSpan+span.m_nSpan ) );
   }

   file_time_span file_time_span::operator-( file_time_span span ) const throw()
   {
      return( file_time_span( m_nSpan-span.m_nSpan ) );
   }

   bool file_time_span::operator==( file_time_span span ) const throw()
   {
      return( m_nSpan == span.m_nSpan );
   }

   bool file_time_span::operator!=( file_time_span span ) const throw()
   {
      return( m_nSpan != span.m_nSpan );
   }

   bool file_time_span::operator<( file_time_span span ) const throw()
   {
      return( m_nSpan < span.m_nSpan );
   }

   bool file_time_span::operator>( file_time_span span ) const throw()
   {
      return( m_nSpan > span.m_nSpan );
   }

   bool file_time_span::operator<=( file_time_span span ) const throw()
   {
      return( m_nSpan <= span.m_nSpan );
   }

   bool file_time_span::operator>=( file_time_span span ) const throw()
   {
      return( m_nSpan >= span.m_nSpan );
   }

   LONGLONG file_time_span::GetTimeSpan() const throw()
   {
      return( m_nSpan );
   }

   void file_time_span::SetTimeSpan( LONGLONG nSpan ) throw()
   {
      m_nSpan = nSpan;
   }


   /////////////////////////////////////////////////////////////////////////////
   // file_time
   /////////////////////////////////////////////////////////////////////////////

   file_time::file_time() throw()
   {
      dwLowDateTime = 0;
      dwHighDateTime = 0;
   }

   file_time::file_time( const FILETIME& ft ) throw()
   {
      dwLowDateTime = ft.dwLowDateTime;
      dwHighDateTime = ft.dwHighDateTime;
   }

   file_time::file_time( ULONGLONG nTime ) throw()
   {
      dwLowDateTime = DWORD( nTime );
      dwHighDateTime = DWORD( nTime>>32 );
   }

   file_time& file_time::operator=( const FILETIME& ft ) throw()
   {
      dwLowDateTime = ft.dwLowDateTime;
      dwHighDateTime = ft.dwHighDateTime;

      return( *this );
   }

   file_time WINAPI file_time::get_current_time() throw()
   {
      file_time ft;
      GetSystemTimeAsFileTime(&ft);
      return ft;
   }

   file_time& file_time::operator+=( file_time_span span ) throw()
   {
      SetTime( get_time()+span.GetTimeSpan() );

      return( *this );
   }

   file_time& file_time::operator-=( file_time_span span ) throw()
   {
      SetTime( get_time()-span.GetTimeSpan() );

      return( *this );
   }

   file_time file_time::operator+( file_time_span span ) const throw()
   {
      return( file_time( get_time()+span.GetTimeSpan() ) );
   }

   file_time file_time::operator-( file_time_span span ) const throw()
   {
      return( file_time( get_time()-span.GetTimeSpan() ) );
   }

   file_time_span file_time::operator-( file_time ft ) const throw()
   {
      return( file_time_span( get_time()-ft.get_time() ) );
   }

   bool file_time::operator==( file_time ft ) const throw()
   {
      return( get_time() == ft.get_time() );
   }

   bool file_time::operator!=( file_time ft ) const throw()
   {
      return( get_time() != ft.get_time() );
   }

   bool file_time::operator<( file_time ft ) const throw()
   {
      return( get_time() < ft.get_time() );
   }

   bool file_time::operator>( file_time ft ) const throw()
   {
      return( get_time() > ft.get_time() );
   }

   bool file_time::operator<=( file_time ft ) const throw()
   {
      return( get_time() <= ft.get_time() );
   }

   bool file_time::operator>=( file_time ft ) const throw()
   {
      return( get_time() >= ft.get_time() );
   }

   ULONGLONG file_time::get_time() const throw()
   {
      return( (ULONGLONG( dwHighDateTime )<<32)|dwLowDateTime );
   }

   void file_time::SetTime( ULONGLONG nTime ) throw()
   {
      dwLowDateTime = DWORD( nTime );
      dwHighDateTime = DWORD( nTime>>32 );
   }

   file_time file_time::UTCToLocal() const throw()
   {
      file_time ftLocal;

      ::FileTimeToLocalFileTime( this, &ftLocal );

      return( ftLocal );
   }

   file_time file_time::LocalToUTC() const throw()
   {
      file_time ftUTC;

      ::LocalFileTimeToFileTime( this, &ftUTC );

      return( ftUTC );
   }



   const ULONGLONG file_time::Millisecond = 10000;
   const ULONGLONG file_time::Second = Millisecond * static_cast<ULONGLONG>(1000);
   const ULONGLONG file_time::Minute = Second * static_cast<ULONGLONG>(60);
   const ULONGLONG file_time::Hour = Minute * static_cast<ULONGLONG>(60);
   const ULONGLONG file_time::Day = Hour * static_cast<ULONGLONG>(24);
   const ULONGLONG file_time::Week = Day * static_cast<ULONGLONG>(7);





} // namespace datetime



dump_context& operator <<(dump_context & dumpcontext, ::datetime::time time)
{
   char psz[32];
   psz[0] = '\0';

   __time64_t tmp = time.get_time();
   errno_t err = _ctime64_s(psz, sizeof(psz), &tmp);

   if ((err != 0) || (psz[0] == '\0') || (time.get_time() == 0))
   {
      return dumpcontext << "::datetime::time(invalid #" << time.get_time() << ")";
   }

   // format it
   return dumpcontext << "::datetime::time(\"" << psz << "\")";
}


/*CArchive& ::datetime::time::Serialize64(CArchive& ar)
{
if (ar.IsStoring())
return ar << (int64_t) m_time;
else
return ar >> (int64_t) m_time;
}
const __int32 cTime64Mark = INT_MIN + 10;
CArchive& operator <<(CArchive& ar, ::datetime::time time)
{
// This version serializes 32 bits of invalid time and after 64 bits
// time64. New code will support both 64 serialization and 32/64 deserialization.
__time64_t ttime = time.get_time();
ar << (DWORD)cTime64Mark;
return ar << (int64_t) ttime;
}

CArchive& operator >>(CArchive& ar, ::datetime::time& rtime)
{
//his version  support both 32 and 64 deserialization.
//If the first 32 bits are cTime64Mark (which is invalid time32 value), then
//The next 64 bits are read to form time64 value, else, it must be a legacy (VC6-7.1) time32 value.
__time32_t timeOld;
CArchive& retRef = ar >> (DWORD&) timeOld;
if (static_cast<__int32>(timeOld)==cTime64Mark)
{
__time64_t t64;
ar >> (int64_t) t64;
rtime = t64;
} else
{
rtime = __time64_t(timeOld);
}
return retRef;
}*/

/////////////////////////////////////////////////////////////////////////////
// time_span - relative time


dump_context& operator <<(dump_context & dumpcontext, ::datetime::time_span timeSpan)
{
   return dumpcontext << "time_span(" << timeSpan.GetDays() << " days, " <<
      timeSpan.GetHours() << " hours, " <<
      timeSpan.GetMinutes() << " minutes and " <<
      timeSpan.GetSeconds() << " seconds)";
}

/*CArchive& time_span::Serialize64(CArchive& ar)
{
if (ar.IsStoring())
return ar << (int64_t) m_timeSpan;
else
return ar >> (int64_t) m_timeSpan;
}

CArchive& operator <<(CArchive& ar, time_span timeSpan)
{
// this version serializes only 32 bits
__time64_t timespan = timeSpan.GetTimeSpan();
ASSERT( timespan <= UINT_MAX);
if( timespan > UINT_MAX)
throw invalid_argument_exception();
time_t timeConverted = (time_t)timespan;
return ar << (DWORD) timeConverted;
}

CArchive& operator >>(CArchive& ar, time_span& rtimeSpan)
{
// this version serializes only 32 bits
__time32_t timeOld;
CArchive& retRef = ar >> (DWORD&) timeOld;
rtimeSpan = __time64_t(timeOld);
return retRef;
}*/


