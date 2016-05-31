#pragma once



struct CLASS_DECL_AURA blob
{


   size_t   m_sizet;
   char *   m_pchData;


   blob();
   ~blob();
   void read(HANDLE hfile);
   void write(HANDLE hfile);
   blob & operator = (const char * psz);
};

class CLASS_DECL_AURA machine_event_data
{
public:
   struct CLASS_DECL_AURA fixed
   {
      fixed();
      bool  m_bRequestCloseApplication;
      bool  m_bSpaUpgrade;
   };
   fixed m_fixed;
   blob m_blobCommand;

   void read(HANDLE hfile);
   void write(HANDLE hfile);
};



