//#include "framework.h"

//////////////////////////////////////////////////////////////////////////
//
// FILE: TextSpeaker.h
//
// Defines a simple C++ class to speak some text.
//
// by Giovanni Dicanio <gdicanio@mvps.org>
//
// 2010, December 28th
//
//////////////////////////////////////////////////////////////////////////


//#include <sapi.h>               // SAPI

// Declare local identifiers:

inline HRESULT SpGetCategoryFromId(const WCHAR * pszCategoryId,ISpObjectTokenCategory ** ppCategory,BOOL fCreateIfNotExist = FALSE)
{
   HRESULT hr;

   ::windows::comptr < ISpObjectTokenCategory > pcategory;
   hr = pcategory.CoCreateInstance(CLSID_SpObjectTokenCategory);

   if(SUCCEEDED(hr))
   {
      hr = pcategory->SetId(pszCategoryId,fCreateIfNotExist);
   }

   if(SUCCEEDED(hr))
   {
      *ppCategory = pcategory.detach();
   }

   return hr;
}

inline HRESULT SpGetTokenFromId(const WCHAR * pszTokenId,ISpObjectToken ** ppToken,BOOL fCreateIfNotExist = FALSE)
{
   HRESULT hr;

   ::windows::comptr < ISpObjectToken > ptoken;
   hr = ptoken.CoCreateInstance(CLSID_SpObjectToken);

   if(SUCCEEDED(hr))
   {
      hr = ptoken->SetId(NULL,pszTokenId,fCreateIfNotExist);
   }

   if(SUCCEEDED(hr))
   {
      *ppToken = ptoken.detach();
   }


   return hr;

}

inline HRESULT SpGetDefaultTokenFromCategoryId(
   const WCHAR * pszCategoryId,
   ISpObjectToken ** ppToken,
   BOOL fCreateCategoryIfNotExist = TRUE)
{
   HRESULT hr;

   ::windows::comptr<ISpObjectTokenCategory> pcategory;
   hr = SpGetCategoryFromId(pszCategoryId,&pcategory,fCreateCategoryIfNotExist);

   if(SUCCEEDED(hr))
   {
      WCHAR * pszTokenId;
      hr = pcategory->GetDefaultTokenId(&pszTokenId);
      if(SUCCEEDED(hr))
      {
         hr = SpGetTokenFromId(pszTokenId,ppToken);
         ::CoTaskMemFree(pszTokenId);
      }
   }

   return hr;
}

inline bool SpGetDefaultTokenFromCategoryIdAndLang(
   ::aura::application * papp,
   const WCHAR * pszCategoryId,
   ISpObjectToken ** ppToken,
   const char * pszLang,
   BOOL fCreateCategoryIfNotExist = TRUE)
{
   HRESULT hr;

   ::windows::comptr<ISpObjectTokenCategory> pcategory;
   hr = SpGetCategoryFromId(pszCategoryId,&pcategory,fCreateCategoryIfNotExist);

   string str(pszLang);

   str.make_lower();

   string strLang;

   if(str == "en" || str == "us" || str == "english")
   {
      
      strLang = "409";

   }
   else if(str == "pt" || str == "br" || str == "portugues" || str == "português")
   {

      strLang = "416";

   }
   else
   {

      ::property_set set;

      string strJson = App(papp).file().as_string(App(papp).dir().matter("speech/windows/lang.json"));

      const char * pszJson = strJson;

      try
      {

         set.parse_json(pszJson);

      }
      catch(...)
      {


      }


      if(set[str].get_string().has_char())
      {

         strLang = set[str];

      }
      else
      {

         strLang = "409";

      }

   }

   if(SUCCEEDED(hr))
   {
//      WCHAR * pszTokenId;
      // Declare local identifiers:
      HRESULT                           hr = S_OK;
      ::windows::comptr<IEnumSpObjectTokens>      cpSpEnumTokens;

      if(SUCCEEDED(hr))
      {

         hr = pcategory->EnumTokens(NULL,NULL,&cpSpEnumTokens);

      }

      ULONG ulCount;

      if(SUCCEEDED(hr))
      {

         cpSpEnumTokens->GetCount(&ulCount);

      }

      for(ULONG i = 0; i < ulCount; i++)
      {

         ::windows::comptr<ISpObjectToken> ptoken;

         hr = cpSpEnumTokens->Item(i, &ptoken);

         if(SUCCEEDED(hr))
         {

            wstring wstrLang;

            wstrLang = L"Language=";
            wstrLang += wstring(strLang);
            BOOL bMatches;

            hr = ptoken->MatchesAttributes(wstrLang, &bMatches);

            if(SUCCEEDED(hr) && bMatches)
            {

               wstrLang = L"Gender=";
               wstrLang += L"Female";
               
               hr = ptoken->MatchesAttributes(wstrLang,&bMatches);

               if(SUCCEEDED(hr) && bMatches)
               {
                  *ppToken = ptoken;
                  ptoken.m_p = NULL;
                  return true;
               }

            }


         }

      }

      for(ULONG i = 0; i < ulCount; i++)
      {

         ::windows::comptr<ISpObjectToken> ptoken;

         hr = cpSpEnumTokens->Item(i,&ptoken);

         if(SUCCEEDED(hr))
         {

            wstring wstrLang;

            wstrLang = L"Language=";
            wstrLang += wstring(strLang);
            BOOL bMatches;

            hr = ptoken->MatchesAttributes(wstrLang,&bMatches);


            if(SUCCEEDED(hr) && bMatches)
            {
               *ppToken = ptoken;
               ptoken.m_p = NULL;
               return true;
            }


         }

      }

      return SUCCEEDED(SpGetDefaultTokenFromCategoryId(SPCAT_VOICES,ppToken,FALSE));

   }

   return SUCCEEDED(hr);
}


namespace windows
{


   namespace tts
   {


      //------------------------------------------------------------------------
      // Simple class to speak some text (using SAPI).
      //------------------------------------------------------------------------

      speaker::speaker(::aura::application * papp):
         object(papp),
         ::tts::speaker(papp)
      {

         m_strDefaultLang = "en";

      }


      speaker::~speaker()
      {

      }

      //--------------------------------------------------------------------
      // Initializes the text speaker.
      //--------------------------------------------------------------------
      bool speaker::initialize(string strLang)
      {

         finalize(strLang);

         //
         // Create text to speech engine
         //
         HRESULT hr = m_voice[strLang].CoCreateInstance(CLSID_SpVoice);
         if(FAILED(hr))
         {

            return false;

         }

         if(strLang.is_empty())
         {

            //
            // Get token corresponding to default voice 
            //
            hr = SpGetDefaultTokenFromCategoryId(SPCAT_VOICES,&m_token[strLang],FALSE);

            if(FAILED(hr))
            {

               return false;

            }

         }
         else
         {
            
            //
            // Get token corresponding to default voice 
            //
            hr = SpGetDefaultTokenFromCategoryIdAndLang(get_app(), L"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Speech_OneCore\\Voices",&m_token[strLang], strLang,FALSE);

            if(FAILED(hr))
            {

               return false;

            }

         }


         //
         // Set default voice
         //
         hr = m_voice[strLang]->SetVoice(m_token[strLang]);

         if(FAILED(hr))
         {

            return false;

         }

         return true;

      }
      bool speaker::initialize_translator(string strLang)
      {

         return false;
      
      }

      bool speaker::finalize(string strLang)
      {

         if(m_voice[strLang].is_set())
         {

            m_voice[strLang]->Pause();

         }

         m_token[strLang].Release();

         m_voice[strLang].Release();



         return true;
      }
      bool speaker::finalize_translator(string strLang)
      {

         return false;

      }


      //--------------------------------------------------------------------
      // Speaks some text.
      // (The input text must not be empty.)
      //--------------------------------------------------------------------
      bool speaker::speak(const string & text)
      {

         return speak(m_strDefaultLang,text);

      }


      bool speaker::is_lang_ok(string strLang)
      {

         bool bTts = false;

         //if((!m_tts.Lookup(strLang, bTts) || bTts) && m_voice[strLang].is_set() || (!is_speaking(strLang) && get_tick_count() - m_time[strLang] > 30 * 1000))
         if (!m_tts.Lookup(strLang, bTts))
         {

            if (initialize(strLang))
            {

               bTts = true;

            }
            else
            {

               bTts = false;

            }

            m_tts[strLang] = bTts;

         }

         return bTts;

      }

      //--------------------------------------------------------------------
      // Speaks some text.
      // (The input text must not be empty.)
      //--------------------------------------------------------------------
      bool speaker::speak(const string & strLang, const string & text, bool bSync)
      {
         
         //
         // Input text must not be empty
         //
         if(text.is_empty())
         {

            // nothing to speak?!?!
            return false;

         }

         if (!is_lang_ok(strLang))
         {

            return ::tts::speaker::speak(strLang, text, bSync);

         }

         string strT(text);

         UINT uiFlag = 0;

         strT.trim();

         //if (::str::begins_eat_ci(strT, "xml:"))
         {

            uiFlag |= SPF_IS_XML;
            
         }
         //else
         //{

           // uiFlag |= SPF_IS_NOT_XML;

         //}

         wstring wstr(strT);

         if(bSync)
         { 
            //
            // Speak input text
            //
            ULONG streamNumber;

            m_voice[strLang]->WaitUntilDone(5 * 60 * 60 * 1000);

            HRESULT hr = m_voice[strLang]->Speak(
               wstr,
               //SPF_IS_NOT_XML | SPF_ASYNC | SPF_PURGEBEFORESPEAK,
               uiFlag | (bSync ? 0 : SPF_ASYNC),
               &streamNumber);

            if (FAILED(hr))
            {

               return false;

            }

         }
         else
         {

            ::fork(get_app(), [=]()
            {

               defer_co_initialize_ex(false);
               //
               // Speak input text
               //
               ULONG streamNumber;

               m_voice[strLang]->WaitUntilDone(5 * 60 * 60 * 1000);

               HRESULT hr = m_voice[strLang]->Speak(
                  wstr,
                  //SPF_IS_NOT_XML | SPF_ASYNC | SPF_PURGEBEFORESPEAK,
                  uiFlag | (bSync ? 0 : SPF_ASYNC),
                  &streamNumber);

               if (FAILED(hr))
               {

                  return false;

               }

               return true;

            });

         }

         return true;

      }


      bool speaker::is_speaking(string strLang)
      {

         if (!is_lang_ok(strLang))
         {

            return ::tts::speaker::is_speaking(strLang);

         }


         if(m_voice[strLang].is_null())
         {

            return false;

         }

         SPVOICESTATUS status ={};

         HRESULT hr = m_voice[strLang]->GetStatus(&status,NULL);

         if(FAILED(hr))
         {

            return false;

         }

         return (status.dwRunningState & SPRS_IS_SPEAKING) != FALSE;

      }


      bool speaker::stop(string strLang)
      {

         if (!is_lang_ok(strLang))
         {

            return ::tts::speaker::stop(strLang);

         }

         if (m_tts[strLang])
         {
            
            finalize(strLang);

         }
         else
         {

            finalize_translator(strLang);

         }

         return true;

      }


   } // namespace tts


} // namespace windows










