#pragma once


class CXfplayerSong;


namespace visual
{
   
   class font;

}


typedef spa(visual::font) visual_font_spa;


class CLASS_DECL_AXIS font_department :
   virtual public ::aura::department
{
public:


   font_department(::aura::application * papp);
   virtual ~font_department();


   ::visual::font_enumeration *     m_pfontenumeration;

   ::draw2d::font_sp                m_fontCaption;
   ::draw2d::font_sp                m_fontMenu;
   ::draw2d::font_sp                m_fontStandard;
   ::draw2d::font_sp                m_font;
   ::draw2d::font_sp                m_fontListCtrl;
   ::draw2d::font_sp                m_fontMeshCtrl;
   ::draw2d::font_sp                m_fontGridCtrl;
   sp(visual::font)                 m_lpSongLabelFont;
   sp(visual::font)                 m_lpSongListFont;
   sp(visual::font)                 m_pfontLyric;
   sp(visual::font)                 m_pfontLyricCompact;
   sp(visual::font)                 m_pxffontMidiTrackName;
   visual_font_spa              m_pTitleFonts;
   visual_font_spa              m_pSubTitleFonts;
   visual_font_spa              m_pH3TitleFonts;
   bool                             m_bInitialized;
   bool                             m_bLyric;


   ::draw2d::font * GetListCtrlFont();
   ::draw2d::font * GetMeshCtrlFont();
   ::draw2d::font * GetGridCtrlFont();
   ::draw2d::font * GetCaptionFont();
   ::draw2d::font * GetStandartFont();
   visual::font * GetSongListFont();
   visual::font * GetSongLabelFont();
   visual::font * GetTitleFont();
   visual::font * GetSubTitleFont();
   visual::font * GetH3TitleFont();
   visual::font * GetMidiTrackNameFont();
   ::draw2d::font * GetMenuFont();
   visual::font * GetLyricViewFont();
   visual::font * GetLyricViewCompactFont();
   visual_font_spa & GetTitleFonts();
   visual_font_spa & GetSubTitleFonts();
   visual_font_spa & GetH3TitleFonts();

   void CreateLyricViewFonts();
   void DestroyLyricViewFonts();
   void Finalize();
   bool Initialize();

   bool IsInitialized();

   void defer_create_font_enumeration();

   void update_font_enumeration();

};

