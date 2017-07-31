//#include "framework.h"
#include <time.h>
//#include <math.h>


namespace math
{





   math::math(::aura::application * papp) :
      object(papp),
      ::aura::department(papp),
      m_mutex(papp)
   {
      // initial rng seed
      dPi = atan(1.0) * 4.0;

#ifdef WINDOWSEX
      hCryptProv = NULL;
      hOriginalKey = NULL;
      hDuplicateKey = NULL;

      if (!CryptAcquireContext(
         &hCryptProv,
         NULL,
         NULL,
         PROV_RSA_FULL,
         0))
      {
         if (CryptAcquireContext(
            &hCryptProv,
            NULL,
            NULL,
            PROV_RSA_FULL,
            CRYPT_NEWKEYSET))
         {
            //debug_print("CryptAcquireContext succeeded. \n");
         }
         else
         {
            //           TRACELASTERROR();
            //debug_print("Error during CryptAcquireContext!\n");

         }

      }
      else
      {
         //debug_print("CryptAcquireContext succeeded. \n");
      }
      //-------------------------------------------------------------------
      // Generate a key.
      if (CryptGenKey(
         hCryptProv,
         CALG_RC4,
         0,
         &hOriginalKey))
      {
         //debug_print("Original session key is created. \n");
      }
      else
      {
         //debug_print("ERROR - CryptGenKey.");
      }
      //-------------------------------------------------------------------
      // Duplicate the key.

      if (CryptDuplicateKey(
         hOriginalKey,
         NULL,
         0,
         &hDuplicateKey))
      {
         //debug_print("The session key has been duplicated. \n");
      }
      else
      {
         //debug_print("ERROR - CryptDuplicateKey");
      }
      //-------------------------------------------------------------------
      // set additional parameters on the original key.
      // First, set the cipher mode.

      dwMode = CRYPT_MODE_ECB;
      if (CryptSetKeyParam(
         hOriginalKey,
         KP_MODE,
         (BYTE*)&dwMode,
         0))
      {
         //debug_print("Key Parameters set. \n");
      }
      else
      {
         //debug_print("Error during CryptSetKeyParam.");
      }

      // Generate a random initialization vector.
      if (CryptGenRandom(
         hCryptProv,
         8,
         pbData))
      {
         //debug_print("Random sequence generated. \n");
      }
      else
      {
         //debug_print("Error during CryptGenRandom.");
      }
      //-------------------------------------------------------------------
      // set the initialization vector.
      if (CryptSetKeyParam(
         hOriginalKey,
         KP_IV,
         pbData,
         0))
      {
         //debug_print("Parameter set with random sequence as initialization vector. \n");
      }
      else
      {
         //debug_print("Error during CryptSetKeyParam.");
      }

#endif

      {
         //  m_chRngReSeedCountDown = -1;
         //unsigned long ulRnd = rnd();
      }

   }

   uint64_t math::gen_rand()
   {
#if defined(METROWIN)
      uint64_t uiLo = Windows::Security::Cryptography::CryptographicBuffer::GenerateRandomNumber();
      uint64_t uiHi = Windows::Security::Cryptography::CryptographicBuffer::GenerateRandomNumber();
      return uiLo | (uiHi << 32);
#else
      uint64_t ui = 0;
      gen_rand(&ui, sizeof(ui));
      return ui;
#endif

   }

   void math::random_bytes(void * buf, memory_size_t dwLen)
   {

      gen_rand(buf, dwLen);

   }

   void math::RandomBytes(void * buf, memory_size_t dwLen)
   {

      random_bytes(buf, dwLen);

   }


   void math::gen_rand(void * buf, memory_size_t dwLen)
   {
#ifdef WINDOWSEX
      CryptGenRandom(hCryptProv, (DWORD) dwLen, (BYTE *)buf);
#elif defined(METROWIN)
      Windows::Storage::Streams::IBuffer ^ buffer = Windows::Security::Cryptography::CryptographicBuffer::GenerateRandom(dwLen);
      memory mem;
      mem.set_os_buffer(buffer);
      memcpy(buf, mem.get_data(), mem.get_size());
#else

      byte * puch = (byte *)buf;
      while (dwLen > 0)
      {
         *puch = (byte)rnd() % 256;
         puch++;
         dwLen--;
      }
#endif
   }


   void math::gen_rand_alnum(char * buf, memory_size_t dwLen)
   {

      static unsigned int uiMemory = 0;

      gen_rand(buf,dwLen);

      for(uint32_t ui = 0 ; ui < dwLen; ui++)
      {

         unsigned int ch = (((unsigned int)buf[ui]) + uiMemory++) % 36;

         buf[ui] = (char) ((ch <= 9) ? (ch + '0') : (ch - 10 + 'a'));

      }

   }




   int32_t math::rand_max()
   {

      return 0xffffff;

   }


   uint32_t math::rnd()
   {

      single_lock lock(&m_mutex, true);

      static int32_t s_iRngReSeedCountDown = -1;

      if (s_iRngReSeedCountDown < 0)
      {

         s_iRngReSeedCountDown = random_context_entropy(19840, 8777);

         m_rng.seed(1984, random_context_entropy(19841511, 19770402));

      }
      else
      {

         s_iRngReSeedCountDown--;

      }

      return m_rng.get();

   }


   int32_t math::random_context_entropy(int32_t iMin, int32_t iMax, int32_t iLevel)
   {
      int32_t iValue;
      if (iLevel > 0)
      {
         iLevel = MIN(iLevel, 3);
         iValue = abs(random_context_entropy(iMin, iMax, iLevel - 1) + random_context_entropy(iMin, iMax, iLevel - 1)) % abs(iMax - iMin);
      }
      else
      {

         ::aura::chronometer chronometer;

         double d1 = chronometer.getTimeInMicroSec();

         sleep(millis(1));

         int32_t v1 = (int32_t)chronometer.getTimeInMicroSec();

         int32_t v2 = (int32_t)time(NULL);

         int32_t v3 = (int32_t) ((uint32_t)chronometer.getTimeInMicroSec() % 0xffffffff, (const byte *)&d1, sizeof(double));

         iValue = abs(v1 + v2 + v3) % (iMax - iMin);

      }
      if (iMax > iMin)
      {
         return iMin + iValue;
      }
      else
      {
         return iMin - iValue;
      }
   }

   math::~math()
   {
      /*if (hOriginalKey)
      if (!CryptDestroyKey(hOriginalKey))
      TRACE("Failed CryptDestroyKey\n");

      if (hDuplicateKey)
      if (!CryptDestroyKey(hDuplicateKey))
      TRACE("Failed CryptDestroyKey\n");

      if(hCryptProv)
      if (!CryptReleaseContext(hCryptProv, 0))
      TRACE("Failed CryptReleaseContext\n");
      */

   }

   double math::GetPi()
   {
      return dPi;
   }

   bool math::IsPowerOfTwo(
      uint32_t uiValue)
   {
      if (uiValue < 2)
         return FALSE;

      if (uiValue & (uiValue - 1)) // Thanks to 'byang' for this cute trick!
         return FALSE;

      return TRUE;
   }

   uint32_t math::ReverseBits(uint32_t index, uint32_t NumBits)
   {
      uint32_t i, rev;

      for (i = rev = 0; i < NumBits; i++)
      {
         rev = (rev << 1) | (index & 1);
         index >>= 1;
      }

      return rev;
   }

   /*      int32_t math::RandRange(int32_t i1, int32_t i2)
   {
   // dRandRange == 0.0 is impossible event due this next statement;
   if(i1 == i2)
   return i1;
   int32_t iMin = MIN(i1, i2);
   int32_t iMax = MAX(i1, i2);
   double dRand = 0.0;
   double dRange = (double) (iMax - iMin);
   double dRandRange = 1.0;
   int32_t i = 1;
   while((dRandRange - 1.0) < dRange)
   {
   dRand += dRandRange * gen_rand();
   dRandRange = pow((double) rand_max() + 1, (double) i);
   i++;
   }
   dRandRange -= 1.0;
   dRand *= (dRange) / (double) dRandRange;
   dRand += (double) iMin;
   dRand += 0.5;
   return (int32_t) dRand;
   }

   uint32_t math::RandRange(uint32_t ui1, uint32_t ui2)
   {
   // dRandRange == 0.0 is impossible event due this next statement;
   if(ui1 == ui2)
   return ui1;
   int32_t iMin = MIN(ui1, ui2);
   int32_t iMax = MAX(ui1, ui2);
   double dRand = 0.0;
   double dRange = (double) (iMax - iMin);
   double dRandRange = 1.0;
   int32_t i = 1;
   while((dRandRange - 1.0) < dRange)
   {
   dRand += dRandRange * gen_rand();
   dRandRange = pow((double) rand_max() + 1, (double) i);
   i++;
   }
   dRandRange -= 1.0;
   dRand *= (dRange) / (double) dRandRange;
   dRand += (double) iMin;
   dRand += 0.5;
   return (int32_t) dRand;
   }*/

   int64_t math::RandRange(int64_t i1, int64_t i2)
   {
      // dRandRange == 0.0 is impossible event due this next statement;
      if (i1 == i2)
         return i1;
      int64_t iMin = MIN(i1, i2);
      int64_t iMax = MAX(i1, i2);
      uint64_t uiRange = iMax - iMin;
      return (gen_rand() % (uiRange + 1)) + iMin;
   }

   double math::frand_range(double d1, double d2)
   {
      // dRandRange == 0.0 is impossible event due this next statement;
      if (d1 == d2)
         return d1;
      double dMin = MIN(d1, d2);
      double dMax = MAX(d1, d2);
      double dRange = dMax - dMin;
      return (((double)(gen_rand() % (1000000000 + 1)) / 1000000000.0) * dRange) + dMin;
   }

   /*

   uint64_t math::RandRange(uint64_t ui1, uint64_t ui2)
   {
   // dRandRange == 0.0 is impossible event due this next statement;
   if(ui1 == ui2)
   return ui1;
   uint64_t uiMin = MIN(ui1, ui2);
   uint64_t uiMax = MAX(ui1, ui2);
   double dRand = 0.0;
   double dRange = (double) (uiMax - uiMin);
   double dRandRange = 1.0;
   int32_t i = 1;
   while((dRandRange - 1.0) < dRange)
   {
   dRand += dRandRange * gen_rand();
   dRandRange = pow((double) rand_max() + 1, (double) i);
   i++;
   }
   dRandRange -= 1.0;
   dRand *= (dRange) / (double) dRandRange;
   dRand += (double) uiMin;
   dRand += 0.5;
   return (int32_t) dRand;
   }


   */

   int32_t math::LinearMap(int32_t iMin, int32_t iMax, int32_t iValue, int32_t iValueMin, int32_t iValueMax)
   {
      double d = iValue - iValueMin;
      d *= (iMax - iMin) / (double)(iValueMax - iValueMin);
      d += iMin;
      d += 0.5;
      return (int32_t)d;
   }

   double math::LinearMap(
      double dMin, double dMax,
      double dValue,
      double dValueMin,
      double dValueMax)
   {
      double d = dValue - dValueMin;
      d *= (dMax - dMin) / (double)(dValueMax - dValueMin);
      d += dMin;
      return d;
   }


   int32_t math::RangeRate(int32_t iMin, int32_t iMax, double dRate)
   {
      return (int32_t)((iMax - iMin) * dRate) + iMin;
   }



} // namespace math





namespace core
{


   namespace lemon
   {

      int32_t CLASS_DECL_AURA time_seed();

      void CLASS_DECL_AURA srand();

      int32_t CLASS_DECL_AURA time_seed()
      {
         return (time(NULL) % 5000 + (::get_tick_count() / 100) % 5000) % 1000;
      }


      void CLASS_DECL_AURA srand()
      {
         int32_t iSeed = time_seed();
         ::srand(iSeed);
         iSeed %= 64;
         for (int32_t i = 0; i < iSeed; i++)
         {
            ::srand(rand());
         }
         for (int32_t i = 0; i < iSeed; i++)
         {
            rand();
         }
      }


   } // lemon

} // namespace core
