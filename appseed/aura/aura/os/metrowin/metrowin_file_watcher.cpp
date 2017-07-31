/**
	Copyright (c) 2009 James Wynn (james@jameswynn.com)

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in
	all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
	THE SOFTWARE.
*/
//#include "framework.h"
//#include "metrowin.h"


CLASS_DECL_AURA::Windows::Storage::StorageFolder ^ winrt_get_folder(const string & strFolder);


namespace file_watcher
{

	/// Internal watch data
   ref class watch_struct sealed
	{
   public:
      
/*		OVERLAPPED mOverlapped;
		HANDLE mDirHandle;
		BYTE mBuffer[32 * 1024];
		LPARAM lParam;
		DWORD mNotifyFilter;
		bool mStopNow;
		
		id mWatchid;*/


      property Windows::Storage::StorageFolder ^                     m_folder;
      property Windows::Storage::Search::StorageItemQueryResult ^    m_queryresult;
      property Windows::Foundation::EventRegistrationToken           m_evtoken;
      property String ^                                              m_strDirName;
      property size_t                                                m_pwatcher;   // should be exactly file_watcher_impl *
      property size_t                                                m_plistener;  // should be exactly file_watch_listener *
      property uint64_t                                              m_id;
      property bool                                                  m_bRecursive;
      property bool                                                  m_bOwn;

      static watch_struct ^ create_watch(String ^ strDirectory, bool bRecursive);

      void ContentsChanged(::Windows::Storage::Search::IStorageQueryResultBase ^ r, ::Object ^ o)
      {
         file_watcher_impl::action action;
         action.watch = this;
         action.r = r;
         ((file_watcher_impl *) m_pwatcher)->handle_action(&action);
      }

	};


#pragma region Internal Functions

	// forward decl
	bool RefreshWatch(watch_struct ^ pWatch, bool _clear = false);

	/// Unpacks events and passes them to a user defined callback.
	/*void CALLBACK WatchCallback(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped)
	{
		TCHAR szFile[MAX_PATH];
		PFILE_NOTIFY_INFORMATION pNotify;
		watch_struct* pWatch = (watch_struct*) lpOverlapped;
		size_t offset = 0;

		if(dwNumberOfBytesTransfered == 0)
			return;

		if (dwErrorCode == ERROR_SUCCESS)
		{
			do
			{
				pNotify = (PFILE_NOTIFY_INFORMATION) &pWatch->m_buffer[offset];
				offset += pNotify->NextEntryOffset;

#			if defined(UNICODE)
				{
					lstrcpynW(szFile, pNotify->FileName,
						min(MAX_PATH, pNotify->FileNameLength / sizeof(WCHAR) + 1));
				}
#			else
				{
					int count = WideCharToMultiByte(CP_ACP, 0, pNotify->FileName,
						pNotify->FileNameLength / sizeof(WCHAR),
						szFile, MAX_PATH - 1, NULL, NULL);
					szFile[count] = TEXT('\0');
				}
#			endif

				pWatch->m_pwatcher->handle_action(pWatch, szFile, pNotify->Action);

			} while (pNotify->NextEntryOffset != 0);
		}

		if (!pWatch->m_bStop)
		{
			RefreshWatch(pWatch);
		}
	}

	/// Refreshes the directory monitoring.
	bool RefreshWatch(watch_struct* pWatch, bool _clear)
	{
		return ReadDirectoryChangesW(
			pWatch->m_hDirectory, pWatch->m_buffer, sizeof(pWatch->m_buffer), FALSE,
			pWatch->m_dwNotify, NULL, &pWatch->m_overlapped, _clear ? 0 : WatchCallback) != 0;
	}*/

	/// Stops monitoring a directory.
	void DestroyWatch(watch_struct ^ pwatch)
	{
		if(pwatch != nullptr)
		{
         //delete pWatch;
		}
	}
	/// Starts monitoring a directory.
	watch_struct ^ watch_struct::create_watch(String ^ strDirectory, bool bRecursive)
	{

		watch_struct ^ pwatch = ref new watch_struct;


      Windows::Storage::Search::QueryOptions ^ options = ref new Windows::Storage::Search::QueryOptions();

      pwatch->m_bRecursive = bRecursive;

      if(pwatch->m_bRecursive)
      {

         options->FolderDepth = ::Windows::Storage::Search::FolderDepth::Deep;

      }
      else
      {

         options->FolderDepth = ::Windows::Storage::Search::FolderDepth::Shallow;

      }

      options->IndexerOption = ::Windows::Storage::Search::IndexerOption::DoNotUseIndexer;

      string strPrefix;

      pwatch->m_folder = winrt_get_folder(strDirectory, strPrefix);

      if (pwatch->m_folder == nullptr)
      {

         return nullptr;

      }

      pwatch->m_queryresult = pwatch->m_folder->CreateItemQuery();

		if(pwatch->m_queryresult == nullptr)
         return nullptr;

      pwatch->m_evtoken = pwatch->m_queryresult->ContentsChanged += ref new ::Windows::Foundation::TypedEventHandler < ::Windows::Storage::Search::IStorageQueryResultBase ^, 
         ::Object ^ >(pwatch, &watch_struct::ContentsChanged);

		return pwatch;

	}

#pragma endregion


	os_file_watcher::os_file_watcher(::aura::application * papp) :
      ::object(papp),
		m_idLast(0)
	{

	}


	os_file_watcher::~os_file_watcher()
	{
      watch_map::pair * ppair = m_watchmap.PGetFirstAssoc();
		for(; ppair != NULL; m_watchmap.PGetNextAssoc(ppair))
		{
         DestroyWatch(ppair->m_element2.w);
		}
		m_watchmap.remove_all();

	}


	file_watch_id os_file_watcher::add_watch(const string & directory, file_watch_listener * watcher, bool bRecursive, bool bOwn)
	{

      synch_lock sl(m_pmutex);

		file_watch_id watchid = ++m_idLast;

		//watch_struct ^ pwatch = watch_struct::create_watch(m_str(directory), FILE_NOTIFY_CHANGE_CREATION | FILE_NOTIFY_CHANGE_SIZE | FILE_NOTIFY_CHANGE_FILE_NAME);
      watch_struct ^ pwatch = watch_struct::create_watch(directory, bRecursive);

		if(pwatch == nullptr)
			throw file_not_found_exception(directory);

      pwatch->m_id            = watchid;
		pwatch->m_pwatcher      = (size_t) (int_ptr) (file_watcher_impl *) this;
		pwatch->m_plistener     = (size_t) (int_ptr) (file_watch_listener *) watcher;
		pwatch->m_strDirName    = directory;
      pwatch->m_bOwn          = bOwn;

      watch_holder holder;

      holder.w = pwatch;
      
		m_watchmap.set_at(watchid, holder);

		return watchid;

	}


	void os_file_watcher::remove_watch(const string & directory)
	{
      watch_map::pair * ppair = m_watchmap.PGetFirstAssoc();
      String ^ strDir = directory;
      for(; ppair != NULL; m_watchmap.PGetNextAssoc(ppair))
		{
         if(strDir == ppair->m_element2.w->m_strDirName)
			{
				remove_watch(ppair->m_element1);
				return;
			}
		}
	}

	void os_file_watcher::remove_watch(file_watch_id watchid)
	{
      watch_map::pair * ppair = m_watchmap.PLookup(watchid);

		if(ppair == NULL)
			return;

      watch_struct ^ pwatch = ppair->m_element2.w;
      m_watchmap.remove_key(ppair->m_element1);

		DestroyWatch(pwatch);
	}

   string os_file_watcher::watch_path(file_watch_id watchid)
   {
      return begin(m_watchmap[watchid].w->m_strDirName);
   }

	bool os_file_watcher::update()
	{
		//MsgWaitForMultipleObjectsEx(0, NULL, 0, QS_ALLINPUT, MWMO_ALERTABLE);

      return true;

	}

	void os_file_watcher::handle_action(action * paction)
	{
		/*Action fwAction;

		switch(action)
		{
		case FILE_ACTION_RENAMED_NEW_NAME:
		case FILE_ACTION_ADDED:
			fwAction = Actions::Add;
			break;
		case FILE_ACTION_RENAMED_OLD_NAME:
		case FILE_ACTION_REMOVED:
			fwAction = Actions::Delete;
			break;
		case FILE_ACTION_MODIFIED:
			fwAction = Actions::Modified;
			break;
		};

		watch->mFileWatchListener->handleFileAction(watch->mWatchid, watch->m_strDirName, filename, fwAction);*/
	}

} // namespace file_watcher




