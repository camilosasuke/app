#pragma once


namespace user
{


   class split_bar;


   class CLASS_DECL_CORE split_layout :
      virtual public ::user::place_holder_container
   {
   public:


      enum
      {

         stateInitial = 1,
         stateDragging = 2

      };

      class CLASS_DECL_CORE Pane :
         virtual public object
      {
      public:


         id                      m_id;
         rect                    m_rect;
         rect                    m_rectClient;
         size                    m_sizeFixed;
         bool                    m_bFixedSize;
         sp(::user::place_holder)    m_pholder;


         Pane(::aura::application * papp);


      };


      spa(split_bar)                               m_splitbara;
      spa(Pane)                                    m_panea;
      e_orientation                                m_eorientationSplit;
      index                                        m_iIndex;
      int32_t                                      m_iState;
      bool                                         m_bInitialized;
      ::critical_section                           m_mutex;
      static const int32_t                         m_iMarging;
      int32_t                                      m_cxBorder;
      int32_t                                      m_cyBorder;

      ::count                                      m_iPaneCount;

      split_layout();
      split_layout(::aura::application * papp);
      virtual ~split_layout();


      virtual void install_message_handling(::message::dispatch * pinterface);

      virtual int32_t get_normal_dimension();
      virtual int32_t get_ortogonal_dimension();

      void RelayEventSplitBar(index iSplitBar, UINT message, WPARAM wParam, LPARAM lParam);
      e_orientation GetSplitOrientation();
      int32_t GetMaxPos(index iPane);
      int32_t GetMinPos(index iPane);
      bool InsertPaneAt(index iIndex, sp(::user::interaction)pwindow, bool bFixedSize, id idPane = id());
      bool SetPane(index iIndex, sp(::user::interaction)pwindow, bool bFixedSize, id idPane = id());
      bool RemovePaneAt(index iIndex);
      void SetPaneFixedSize(index iIndex, SIZE * pSize);
      void CalcSplitBarRect(index iIndex, LPRECT lpRect);
      void CalcPaneRect(int32_t nMinPos, int32_t nMaxPos, LPRECT lpRect);
      void CalcPaneRect(index iIndex, LPRECT lpRect);

      virtual sp(::user::interaction) get_pane_window(index iPane);
      virtual sp(::user::place_holder) get_pane_holder(index iPane);
      virtual rect & get_pane_rect(index iPane);
      virtual id get_pane_id(index iPane);
      virtual index get_pane_by_id(::id id);


      virtual ::count get_pane_count();
      virtual ::count get_visible_pane_count();
      virtual bool is_pane_visible(int iPane);

      ::count get_split_count();
      int32_t get_position(index iIndex);
      void set_position(index iIndex, int32_t nPos);
      void set_position_rate(index iIndex, double dRate, double dMinimumRate = 0.1, double dMaximumRate = 0.9);
      void on_layout();
      int32_t GetMaxPos();
      int32_t GetMinPos();
      int32_t GetPos(int32_t xPos, int32_t yPos);
      void RelayChildEvent(index iIndex, const MESSAGE *  lpMsg);

      void SetSplitOrientation(e_orientation eorientation);

      bool SetPaneCount(::count iPaneCount);

      virtual bool initialize_split_layout();

      DECL_GEN_SIGNAL(_001OnShowWindow);

      virtual void draw_control_background(::draw2d::graphics * pgraphics) override;

      virtual void _001OnDraw(::draw2d::graphics * pgraphics);


   };


} // namespace user



