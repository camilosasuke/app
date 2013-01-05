namespace gen
{
   namespace math
   {

// {
      // http://www.alhem.net/
      /** from C++ Sockets Library \file Utility.h
       **   \date  2004-02-13
       **   \author grymse@alhem.net
      **/
      /** Conversion utilities. 
         \ingroup util */
         /**
            The Mersenne Twister
            http://www.math.keio.ac.jp/~matumoto/emt.html
         */
         class CLASS_DECL_ca rng
         {
         public:
            rng();

            void seed(int32_t iTwistLen, uint32_t seed);
            uint32_t get();

         private:
            int32_t m_value;
            uint32_array m_dwa;
         };
   
      // http://www.alhem.net/
      /** from C++ Sockets Library \file Utility.h
       **   \date  2004-02-13
       **   \author grymse@alhem.net
       **/ 
      // }

   } //namespace math
} //namespace gen
