#include "framework.h"

namespace ftp
{


   output_stream::implementation::implementation(const string& strEolCharacterSequence, const string& strStreamName) :
      mc_strEolCharacterSequence(strEolCharacterSequence),
      m_iCurrentPos(0),
      m_strStreamName(strStreamName)
   {
   }

   bool output_stream::implementation::GetNextLine(string& strLine)// const
   {

      int64_t i = m_vBuffer.find(mc_strEolCharacterSequence, m_iCurrentPos);

      if (i < 0)
      {

         return false;

      }

      strLine = m_vBuffer.Mid((strsize) m_iCurrentPos, (strsize) (i - m_iCurrentPos));

      m_iCurrentPos = (strsize) (i + mc_strEolCharacterSequence.size());

      return true;

   }


   void output_stream::implementation::OnPreBytesSend(byte * pszBuffer, size_t bufferSize, size_t& bytesToSend)
   {
      
      for (bytesToSend = 0; m_iCurrentPos < m_vBuffer.get_length() && bytesToSend < bufferSize; ++m_iCurrentPos, ++bytesToSend)
      {

         pszBuffer[bytesToSend] = m_vBuffer[m_iCurrentPos];

      }

   }


   output_stream::output_stream(const string& strEolCharacterSequence, const string& strStreamName) :
      m_pimpl(new implementation(strEolCharacterSequence, strStreamName))
   {
   }

   output_stream::~output_stream() {}

   void output_stream::SetBuffer(const string& strBuffer)
   {
      m_pimpl->m_vBuffer = strBuffer;
   }

   const string& output_stream::GetBuffer()
   {
      return m_pimpl->m_vBuffer;
   }


   void output_stream::SetStartPosition()
   {

      m_pimpl->m_iCurrentPos = 0;

   }


   bool output_stream::GetNextLine(string& strLine)
   {

      return m_pimpl->GetNextLine(strLine);

   }


   string output_stream::GetLocalStreamName() const
   {

      return m_pimpl->m_strStreamName;

   }


   UINT output_stream::GetLocalStreamSize() const
   {
       return (UINT) natural(m_pimpl->m_vBuffer.size());
   }

   void output_stream::SetLocalStreamOffset(DWORD dwOffsetFromBeginOfStream)
   {
      m_pimpl->m_iCurrentPos = dwOffsetFromBeginOfStream;
   }

   void output_stream::OnBytesReceived(const memory& vBuffer, long lReceivedBytes)
   {
      m_pimpl->m_vBuffer +=  vBuffer.to_string(0, lReceivedBytes);
   }

   void output_stream::OnPreBytesSend(byte * pszBuffer, size_t bufferSize, size_t& bytesToSend)
   {
      m_pimpl->OnPreBytesSend(pszBuffer, bufferSize, bytesToSend);
   }


} // namespace ftp




