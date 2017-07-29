#include "framework.h"

#define MAX_STOP (::numeric_info < file_position_t >::get_maximum_value())

namespace file
{


   edit_file::Item::Item()
   {

   }


   bool edit_file::Item::read_byte(byte * pbyte, ::file::edit_file * pfile)
   {

      throw interface_only_exception(pfile->get_app());

   }


   file_position_t edit_file::Item::get_position(bool bForward) { UNREFERENCED_PARAMETER(bForward); return m_dwPosition; };

   BYTE * edit_file::Item::get_data() { return NULL; }
   edit_file::EItemType edit_file::Item::get_type() { return ItemTypeUndefined; }
   memory_size_t edit_file::Item::get_extent() { return 0; }
   memory_size_t edit_file::Item::get_file_extent() { return 0; }
   BYTE * edit_file::Item::reverse_get_data() { return NULL; }
   edit_file::EItemType edit_file::Item::reverse_get_type() { return ItemTypeUndefined; }
   memory_size_t edit_file::Item::reverse_get_extent() { return 0; }
   memory_size_t edit_file::Item::reverse_get_file_extent() { return 0; }
   memory_size_t edit_file::Item::get_extent(bool bForward) { return bForward ? get_extent() : reverse_get_extent(); }
   edit_file::EItemType edit_file::Item::get_type(bool bForward) { return bForward ? get_type() : reverse_get_type(); }
   memory_size_t edit_file::Item::get_file_extent(bool bForward) { return bForward ? get_file_extent() : reverse_get_file_extent(); }
   BYTE * edit_file::Item::get_data(bool bForward) { return bForward ? get_data() : reverse_get_data(); }
   memory_offset_t edit_file::Item::get_delta_length()  { return 0; }



   edit_file::EItemType edit_file::DeleteItem::get_type()
   {
      return ItemTypeDelete;
   }

   memory_size_t edit_file::DeleteItem::get_extent()
   {
      return 0;
   }

   memory_size_t edit_file::DeleteItem::get_file_extent()
   {
      return m_memstorage.get_size();
   }

   BYTE * edit_file::DeleteItem::get_data()
   {
      return NULL;
   }

   edit_file::EItemType edit_file::DeleteItem::reverse_get_type()
   {
      return ItemTypeInsert;
   }

   memory_size_t edit_file::DeleteItem::reverse_get_extent()
   {

      return m_memstorage.get_size();

   }


   memory_size_t edit_file::DeleteItem::reverse_get_file_extent()
   {

      return 0;

   }


   BYTE * edit_file::DeleteItem::reverse_get_data()
   {

      return m_memstorage.get_data();

   }


   memory_offset_t edit_file::DeleteItem::get_delta_length()
   {

      return - (memory_offset_t) m_memstorage.get_size();

   }


   bool edit_file::DeleteItem::read_byte(byte * pbyte, ::file::edit_file * pfile)
   {

      if(pfile->m_bRootDirection)
      {

         if(pfile->m_dwIterationPosition >= m_dwPosition)
         {

            pfile->m_dwIterationPosition += m_memstorage.get_size();

            //if (pfile->m_dwStopPosition != MAX_STOP && m_dwPosition < pfile->m_dwStopPosition)
            //{

            //   pfile->m_dwStopPosition += m_memstorage.get_size();

            //}

         }
         //else if (pfile->m_dwStopPosition > m_dwPosition)
         //{

         //   pfile->m_dwStopPosition = m_dwPosition;

         //}

      }
      else
      {
         if(pfile->m_dwIterationPosition >= m_dwPosition)
         {

            if(pfile->m_dwIterationPosition < (m_dwPosition + m_memstorage.get_size()))
            {

               file_position_t iOffset = pfile->m_dwIterationPosition - m_dwPosition;

               if (pbyte != NULL)
               {

                  *pbyte = m_memstorage.get_data()[iOffset];

               }

               return true;

            }
            else
            {

               pfile->m_dwIterationPosition -= m_memstorage.get_size();

            }

         }

      }

      return false;

   }




   edit_file::EItemType edit_file::InsertItem::get_type()
   {
      return ItemTypeInsert;
   }

   memory_size_t edit_file::InsertItem::get_extent()
   {
      return m_memstorage.get_size();
   }

   memory_size_t edit_file::InsertItem::get_file_extent()
   {
      return 0;
   }

   BYTE * edit_file::InsertItem::get_data()
   {
      return m_memstorage.get_data();
   }

   edit_file::EItemType edit_file::InsertItem::reverse_get_type()
   {
      return ItemTypeDelete;
   }

   memory_size_t edit_file::InsertItem::reverse_get_extent()
   {
      return 0;
   }

   memory_size_t edit_file::InsertItem::reverse_get_file_extent()
   {
      return m_memstorage.get_size();
   }

   BYTE * edit_file::InsertItem::reverse_get_data()
   {
      return NULL;
   }

   memory_offset_t edit_file::InsertItem::get_delta_length()
   {
      return (memory_offset_t) m_memstorage.get_size();
   }



   bool edit_file::InsertItem::read_byte(byte * pbyte, ::file::edit_file * pfile)
   {

      if(pfile->m_bRootDirection)
      {

         if(pfile->m_dwIterationPosition >= m_dwPosition)
         {

            if(pfile->m_dwIterationPosition < (m_dwPosition + m_memstorage.get_size()))
            {

               file_position_t iOffset = pfile->m_dwIterationPosition - m_dwPosition;

               if (pbyte != NULL)
               {

                  *pbyte = m_memstorage.get_data()[iOffset];

               }

               return true;

            }
            else
            {

               pfile->m_dwIterationPosition -= m_memstorage.get_size();

            }

            //else
            //{

            //    pfile->m_dwIterationPosition -= m_memstorage.get_size();

            //    if (pfile->m_dwStopPosition != MAX_STOP && m_dwPosition  < pfile->m_dwStopPosition)
            //    {

            //       if (m_dwPosition + m_memstorage.get_size() < pfile->m_dwStopPosition)
            //       {

            //          pfile->m_dwStopPosition -= m_memstorage.get_size();

            //       }

            //    }

            //}

         }
         //else
         //{
         // 
         //   if (pfile->m_dwStopPosition > m_dwPosition)
         //   {

         //      pfile->m_dwStopPosition = m_dwPosition;

         //   }

         //}

      }
      else
      {

         //if(pfile->m_dwPosition + pfile->m_iOffset >= m_dwPosition)
         //{

         //   pfile->m_iOffset -= m_memstorage.get_size();

         //   pfile->m_dwLength -= m_memstorage.get_size();

         //}

         if (pfile->m_dwIterationPosition >= m_dwPosition)
         {

            pfile->m_dwIterationPosition += m_memstorage.get_size();

         }

      }

      return false;

   }






   edit_file::EItemType edit_file::EditItem::get_type()
   {
      return ItemTypeEdit;
   }

   memory_size_t edit_file::EditItem::get_extent()
   {
      return m_memstorage.get_size();
   }

   memory_size_t edit_file::EditItem::get_file_extent()
   {
      return get_extent();
   }

   BYTE * edit_file::EditItem::get_data()
   {
      return m_memstorage.get_data();
   }

   edit_file::EItemType edit_file::EditItem::reverse_get_type()
   {
      return ItemTypeEdit;
   }

   memory_size_t edit_file::EditItem::reverse_get_extent()
   {
      return m_memstorageReverse.get_size();
   }

   memory_size_t edit_file::EditItem::reverse_get_file_extent()
   {
      return get_extent();
   }

   BYTE * edit_file::EditItem::reverse_get_data()
   {
      return m_memstorageReverse.get_data();
   }

   memory_offset_t edit_file::EditItem::get_delta_length()
   {
      return 0;
   }












   edit_file::EItemType edit_file::GroupItem::get_type()
   {
      return ItemTypeGroup;
   }

   memory_size_t edit_file::GroupItem::get_extent()
   {
      return 0;
   }

   memory_size_t edit_file::GroupItem::get_file_extent()
   {
      return 0;
   }

   BYTE * edit_file::GroupItem::get_data()
   {
      return 0;
   }

   edit_file::EItemType edit_file::GroupItem::reverse_get_type()
   {
      return ItemTypeGroup;
   }

   memory_size_t edit_file::GroupItem::reverse_get_extent()
   {
      return 0;
   }

   memory_size_t edit_file::GroupItem::reverse_get_file_extent()
   {
      return 0;
   }

   BYTE * edit_file::GroupItem::reverse_get_data()
   {
      return 0;
   }

   memory_offset_t edit_file::GroupItem::get_delta_length()
   {
      memory_offset_t iLen = 0;
      for(int32_t i = 0; i < this->get_count(); i++)
      {
         iLen += this->element_at(i)->get_delta_length();
      }
      return iLen;
   }


   bool edit_file::GroupItem::read_byte(byte * pbyte, ::file::edit_file * pfile)
   {

      if(pfile->m_bRootDirection)
      {

         for(index i = get_upper_bound(); i >= 0; i--)
         {
            
            if (this->operator[](i)->read_byte(pbyte, pfile))
            {

               return true;

            }
               
         }

      }
      else
      {
         for(index i = 0; i < this->get_count(); i++)
         {

            if (this->operator[](i)->read_byte(pbyte, pfile))
            {

               return true;

            }

         }

      }

      return false;

   }


   edit_file::edit_file(::aura::application * papp):
      object(papp),
      ::data::data(papp),
      ::file::file_sp(papp),
      ::data::tree(papp)
      {

         m_iBranch = 0;
         m_pgroupitem = NULL;

         m_ptreeitem = get_base_item();
         m_ptreeitemFlush = get_base_item();

      }

   edit_file::~edit_file()
   {

   }


   void edit_file::SetFile(::file::file_sp  pfile)
   {

      if(pfile.cast < ::memory_file >() == NULL || pfile.cast < ::file::buffered_file >() == NULL)
      {

         pfile = canew(::file::buffered_file(get_app(),pfile));

      }

      if(pfile == NULL)
         throw invalid_argument_exception(get_app());

      m_pfile = pfile;

      m_dwFileLength = pfile->get_length();

      m_pfile->seek(0,::file::seek_begin);

      m_dwPosition = 0;

   }


   memory_size_t edit_file::read(void *lpBuf,memory_size_t nCount)
   {

      if (nCount <= 0)
      {

         return 0;

      }

      if (m_ptreeitem == m_ptreeitemFlush)
      {

         return (memory_size_t) (m_dwPosition = m_pfile->read(lpBuf, nCount));

      }
      
      byte * buf = (byte *)lpBuf;

      memory_size_t uiRead = 0;

      memory_size_t uiReadCount = 0;

      if(m_dwPosition >= m_dwFileLength)
      {

         return uiRead;

      }

      //      uint32_t dwPosition = m_dwPosition;
      //      uint32_t dwFilePosition = m_dwPosition;
      //      uint32_t dwMaxCount = m_dwFileLength;
      //      uint32_t dwUpperLimit = m_dwFileLength;
      //      int32_t iOffset =0;

      sp(::data::tree_item) ptreeitem;

      //      GroupItem * pitemgroup = NULL;

      int_array ia;

      m_bRootDirection = calc_root_direction();

      UINT uiReadItem = 0xffffffff;

      //uint64_t uiStopSize;

      m_iOffset = 0;

      if (m_bRootDirection)
      {

         m_ptreeitemBeg = m_ptreeitemFlush->get_child_next_or_parent();

         //m_ptreeitemBeg = m_ptreeitemEnd;

      }
      else
      {

         m_ptreeitemBeg = m_ptreeitemFlush;

      }

      byte b;

      m_dwLength = m_dwFileLength;

      m_dwIterationPosition = m_dwPosition;

      bool bRead;

      DWORD dwFileOffset = 0;

      do
      {

         m_dwIterationPosition = m_dwPosition;

         ptreeitem = m_ptreeitem;
         
         bRead = false;

         while (ptreeitem != NULL && (!m_bRootDirection || ptreeitem->m_pitem.is_set()))
         {

            if (!m_bRootDirection)
            {

               ptreeitem = ptreeitem->get_child_next_or_parent();

            }

            Item * pitem = (Item *) ptreeitem->m_pitem.m_p;

            bRead = pitem->read_byte(&b, this);

            if (bRead)
            {

               break;

            }

            if (ptreeitem == m_ptreeitemBeg)
            {

               break;

            }

            if (m_bRootDirection)
            {

               ptreeitem = ptreeitem->get_previous_or_parent();
               
            }

         } 

         if(!bRead)
         {
            
            m_pfile->seek_begin(m_dwIterationPosition);

            bRead = m_pfile->read(&b, 1) == 1;

         }

         if (!bRead)
         {

            break;

         }

         buf[uiRead] = b;

         nCount--;

         uiRead++;

         m_dwPosition++;

      } while(nCount > 0 && m_dwPosition < m_dwLength);

      return uiRead;

   }


   void edit_file::TreeInsert(Item * pitem)
   {
      
      if(m_pgroupitem != NULL && m_pgroupitem != pitem)
      {
         
         m_pgroupitem->add(pitem);

         return;

      }

      sp(::data::tree_item) pitemNew;

      if(m_ptreeitem != NULL && m_ptreeitem->get_next() != NULL)
      {

         pitemNew = insert_item(pitem,::data::RelativeFirstChild, m_ptreeitem);

      }
      else
      {

         pitemNew = insert_item(pitem,::data::RelativeLastSibling, m_ptreeitem);

      }

      if (pitemNew != NULL)
      {

         m_ptreeitem = pitemNew;

      }

   }


   void edit_file::write(const void * lpBuf,memory_size_t nCount)
   {
      sp(EditItem) pedit;
      pedit = canew(EditItem);
      pedit->m_dwPosition = m_dwPosition;
      pedit->m_memstorage.allocate(nCount);
      memcpy(pedit->m_memstorage.get_data(),lpBuf,nCount);
      TreeInsert(pedit);
      m_dwPosition += nCount;
   }

   edit_file::InsertItem * edit_file::Insert(const void * lpBuf,memory_size_t nCount)
   {
      sp(InsertItem) pinsert;
      pinsert = canew(InsertItem);
      pinsert->m_dwPosition = m_dwPosition;
      pinsert->m_memstorage.allocate(nCount);
      memcpy(pinsert->m_memstorage.get_data(),lpBuf,nCount);
      TreeInsert(pinsert);
      m_dwFileLength += nCount;
      return pinsert;

   }


   edit_file::DeleteItem * edit_file::Delete(memory_size_t uiCount)
   {

      sp(DeleteItem) pdelete;

      uiCount = MIN(uiCount,(memory_size_t) (get_length() - m_dwPosition));
      if(uiCount == 0)
         return NULL;

      pdelete = canew(DeleteItem);
      pdelete->m_dwPosition = m_dwPosition;
      pdelete->m_memstorage.allocate(uiCount);
      seek((file_offset_t)m_dwPosition,::file::seek_begin);
      read(pdelete->m_memstorage.get_data(),uiCount);
      TreeInsert(pdelete);
      m_dwFileLength -= uiCount;

      return pdelete;

   }


   file_position_t edit_file::get_position() const
   {
      return m_dwPosition;
   }

   bool edit_file::IsValid() const
   {
      return true;
   }

   file_position_t edit_file::seek(file_offset_t lOff,::file::e_seek nFrom)
   {

      if (m_ptreeitem == m_ptreeitemFlush)
      {

         return m_dwPosition = m_pfile->seek(lOff, nFrom);

      }

      ASSERT(IsValid());

      ASSERT(nFrom == ::file::seek_begin || nFrom == ::file::seek_end || nFrom == ::file::seek_current);

      ASSERT(::file::seek_begin == FILE_BEGIN && ::file::seek_end == FILE_END && ::file::seek_current == FILE_CURRENT);

      file_position_t dwNew = (uint32_t)-1;

      switch(nFrom)
      {
      case ::file::seek_begin:
         
         dwNew = (file_position_t)lOff;
         
         break;

      case ::file::seek_end:
         
         dwNew = get_length() - lOff;
         
         break;

      case ::file::seek_current:
         
         if(lOff < 0)
         {
         
            dwNew = m_dwPosition + lOff;

            if (dwNew > m_dwPosition)
            {

               dwNew = 0;

            }

         }
         else
         {

            dwNew = m_dwPosition + lOff;

         }

         break;
      default:
         return (file_position_t)-1;
      }

      if (dwNew > m_dwFileLength)
      {

         dwNew = m_dwFileLength;

      }

      m_dwPosition = dwNew;

      return dwNew;

   }


   file_size_t edit_file::get_length() const
   {

      return m_dwFileLength;

   }


   void edit_file::flush()
   {

      string strTimeFile;

      strTimeFile = Application.file().time_square();

      ::file::file_sp spfile = Application.file().get_file(strTimeFile,::file::type_binary | ::file::mode_read_write | ::file::mode_create | ::file::defer_create_directory);

      if(spfile.is_null())
      {

         throw ::file::exception(get_app(),::file::exception::none,-1,strTimeFile);

         return;

      }

      Save(*spfile);

      file_size_t dwLen = spfile->get_length();

      char buf[4096];

      memory_size_t uiRead;

      m_pfile->set_length(0);

      spfile->seek(0,::file::seek_begin);

      while((uiRead = spfile->read(buf,sizeof(buf))) > 0)
      {

         m_pfile->write(buf,uiRead);

      }

      m_pfile->flush();
      
      m_dwFileLength = m_pfile->get_length();

      ASSERT(m_dwFileLength == dwLen);
      
      m_ptreeitemFlush = m_ptreeitem;

   }


   bool edit_file::SaveTo(::file::ostream & ostream)
   {
      char buf[4096];
      memory_size_t uiRead;
      seek(0,::file::seek_begin);
      while((uiRead = read(buf,sizeof(buf))) > 0)
      {
         ostream.write(buf,uiRead);
      }
      ostream.flush();
      m_ptreeitemFlush = m_ptreeitem;
      //file.close();
      return true;
   }

   bool edit_file::Save(::file::file & file)
   {
      char buf[4096];
      memory_size_t uiRead;
      file.set_length(0);
      seek(0,::file::seek_begin);
      while((uiRead = read(buf,sizeof(buf))) > 0)
      {
         file.write(buf,uiRead);
      }
      file.flush();
      //file.close();
      return true;
   }

   bool edit_file::Save_N_to_CRLF(::file::file & file)
   {
      char buf[4096];
      string str;
      memory_size_t uiRead;
      file.set_length(0);
      seek(0,::file::seek_begin);
      while((uiRead = read(buf,sizeof(buf))) > 0)
      {
         buf[uiRead] = '\0';
         str = buf;
         str.replace("\n","\r\n");
         file.write(str,str.get_length());
      }
      //file.flush();
      file.close();
      return true;
   }


   bool edit_file::CanUndo()
   {
      return m_ptreeitem != get_base_item();
   }

   bool edit_file::CanRedo()
   {
      return m_iBranch < m_ptreeitem->get_expandable_children_count()
         || m_ptreeitem->get_next() != NULL;
   }

   ::count edit_file::GetRedoBranchCount()
   {
      if(m_ptreeitem == NULL)
         return 1;
      else
         return   m_ptreeitem->get_expandable_children_count()
         + (m_ptreeitem->get_next() != NULL ? 1 : 0)
         + (m_ptreeitem->get_children_count() > 0 ? 1 : 0);
   }


   bool edit_file::Undo()
   {

      if (!CanUndo())
      {

         return false;

      }

      m_dwFileLength -= ((sp(Item))m_ptreeitem->m_pitem)->get_delta_length();

      m_ptreeitem = m_ptreeitem->get_previous_or_parent();

      return true;

   }


   bool edit_file::Redo()
   {
      if(m_iBranch < 0 || m_iBranch >= GetRedoBranchCount())
      {
         return false;
      }
      //      sp(Item) pitem = NULL;
      sp(::data::tree_item) ptreeitem;
      if(m_iBranch < m_ptreeitem->get_expandable_children_count())
      {
         ptreeitem = m_ptreeitem->get_expandable_child(m_iBranch);
      }
      else
         ptreeitem = m_ptreeitem->get_child_next_or_parent();
      if(ptreeitem == NULL)
         return false;
      m_dwFileLength += ((sp(Item)) ptreeitem->m_pitem)->get_delta_length();
      m_ptreeitem = ptreeitem;
      return true;
   }

   void edit_file::MacroBegin()
   {
      GroupItem * pgroupitem = canew(GroupItem);
      pgroupitem->m_pgroupitem = m_pgroupitem;
      m_pgroupitem = pgroupitem;
   }

   void edit_file::MacroEnd()
   {
      if(m_pgroupitem == NULL)
      {
         ASSERT(FALSE);
         return;
      }
      if(m_pgroupitem->m_pgroupitem == NULL)
      {
         TreeInsert(m_pgroupitem);
      }
      m_pgroupitem = m_pgroupitem->m_pgroupitem;
   }



   bool edit_file::calc_root_direction()
   {
      sp(::data::tree_item) ptreeitem;
      if(m_ptreeitem == m_ptreeitemFlush)
         return false;
      for(ptreeitem  = m_ptreeitem;
         ptreeitem != m_ptreeitemFlush && ptreeitem != get_base_item() && ptreeitem != NULL;
         ptreeitem  = ptreeitem->get_previous_or_parent())
      {
      }
      return ptreeitem == m_ptreeitemFlush;
   }




} // namespace core



