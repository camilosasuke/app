#pragma once


#ifdef WINDOWSEX


namespace windows
{


   class CLASS_DECL_AURA registry :
      virtual public ::object
   {
   public:


      class CLASS_DECL_AURA Key
      {
      public:


         HKEY m_hkey;


         Key();
         Key(HKEY hkey, const char * lpcszSubKey, bool bCreate);
         virtual ~Key();
      

         bool OpenKey(HKEY hkey, const char * lpcszSubKey, bool bCreate);
         void CloseKey();

         bool QueryValue(const char * lpcszValueName, DWORD & dwValue);
         
         bool QueryValue(const char * lpcszValueName, string &strValue);

         bool QueryValue(const char * lpcszValueName, memory & mem);

         bool SetValue(const char * lpcszValueName, DWORD dwValue);

         bool SetValue(const char * lpcszValueName, const string & strValue);

         bool SetValue(const char * lpcszValueName, const char * pszValue);

         bool SetValue(const char * lpcszValueName, const memory & mem);

         bool DeleteValue(const char * lpcszValueName);

         bool DeleteKey();

         ::count EnumKey(stringa & stra);

         ::count EnumValueName(stringa & stra);
         
         operator HKEY();

      };

      registry();
      virtual ~registry();
      /*bool RegQueryValue(
         HKEY hKey,       // handle to key to query
         const unichar * lpSubKey,
                      // name of subkey to query
         CWStr &str);*/

      bool RegQueryValue(
         HKEY hKey,       // handle to key to query
         const char * lpSubKey,
                      // name of subkey to query
         string &str);


      
   };


} // namespace windows



#endif



