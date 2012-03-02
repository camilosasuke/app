/**
 **   \file http_client_socket.cpp
 **   \date  2006-04-20
 **   \author grymse@alhem.net
**/
/*
Copyright (C) 2007  Anders Hedstrom

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
#include "StdAfx.h"

namespace sockets
{

   http_client_socket::http_client_socket(socket_handler_base& h) :
      ::ca::ca(h.get_app()),
      socket(h),
      stream_socket(h),
      tcp_socket(h),
      http_tunnel(h),
      m_content_length((size_t)-1),
      m_fil(NULL),
      m_content_ptr(0),
      m_b_complete(false),
      m_b_close_when_complete(false)
   {
      m_pfile = NULL;
   }


   http_client_socket::http_client_socket(socket_handler_base& h,const string & url_in) :
      ::ca::ca(h.get_app()),
      socket(h),
      stream_socket(h),
      tcp_socket(h),
      http_tunnel(h),
      m_content_length((size_t)-1),
      m_fil(NULL),
      m_content_ptr(0),
      m_b_complete(false),
      m_b_close_when_complete(false)
   {
      string url;
      url_this(url_in, m_protocol, m_host, m_port, url, m_url_filename);
      m_request.attr("http_protocol") = m_protocol;
      outheader("host") = m_host;
      m_request.attr("request_uri") = url;
      m_response.attr("request_uri") = url;
      m_strUrl = url_in;
      m_pfile = NULL;
   }


   http_client_socket::~http_client_socket()
   {
      if (m_fil)
      {
         fclose(m_fil);
      }
   }


   void http_client_socket::OnFirst()
   {
      if (!IsResponse())
      {
         Handler().LogError(this, "OnFirst", 0, "Response expected but not received - aborting", ::gen::log::level::fatal);
         SetCloseAndDelete();
      }
      m_content = m_response.attr("http_version").get_string() + " " +
                  m_response.attr("http_status_code") + " " +
                  m_response.attr("http_status") + "\r\n";
   }


   void http_client_socket::OnHeader(const string & key,const string & value)
   {
      m_content += key + ": " + value + "\r\n";
      m_response.m_propertysetHeader[key] = value;
      if (!strcasecmp(key, "content-length"))
      {
         m_content_length = atoi(value);
      }
      else if(!strcasecmp(key, "content-type"))
      {
         m_content_type = value;
      }
      else if(!strcasecmp(key, "set-cookie"))
      {
         m_response.m_cookies.add(value);
      }
   }


   void http_client_socket::OnHeaderComplete()
   {
      if (m_filename.get_length())
      {
         m_fil = fopen(m_filename, "wb");
      }
      else if(m_pfile != NULL)
      {
         if(m_content_length != ((size_t) (-1)))
         {
            gen::memory_file * pmemoryfile = dynamic_cast < gen::memory_file * > (m_pfile);
            if(pmemoryfile != NULL)
            {
               pmemoryfile->allocate_internal(m_content_length);
            }
         }
      }
      else
      {
         if(m_content_length != ((size_t) (-1)))
         {
            //m_memoryData.allocate(m_content_length);
         }
      }

   }

   void http_client_socket::OnDataComplete()
   {
      SetCloseAndDelete();
   }

   void http_client_socket::OnData(const char *buf,size_t len)
   {
      OnDataArrived(buf, len);
      if(m_pfile)
      {
         m_pfile->write(buf, len);
      }
      else if (m_fil)
      {
         fwrite(buf, 1, len, m_fil);
      }
      else
      {
         gen::memory_file memfile(get_app(), &m_memoryData);
         memfile.seek_to_end();
         memfile.write(buf, len);
      }
      m_content_ptr += len;
      if (m_content_ptr == m_content_length && m_content_length && m_content_length != ((size_t) (-1)))
      {
         if (m_fil)
         {
            fclose(m_fil);
            m_fil = NULL;
         }
         m_b_complete = true;
         OnContent();
         if (m_b_close_when_complete)
         {
            SetCloseAndDelete();
         }
      }
   }

   void http_client_socket::OnDataArrived(const char * buf, size_t len)
   {
      UNREFERENCED_PARAMETER(buf);
      UNREFERENCED_PARAMETER(len);
   }


   void http_client_socket::OnDelete()
   {
      if (!m_b_complete)
      {
         if (m_fil)
         {
            fclose(m_fil);
            m_fil = NULL;
         }
         m_b_complete = true;
         OnContent();
      }
   }

   void http_client_socket::SetFilename(const string & x)
   {
      m_filename = x;
   }

   const string & http_client_socket::GetContent()
   {
      return m_content;
   }

   size_t http_client_socket::GetContentLength()
   {
      if(m_content_length == ((size_t)-1))
      {
         return 0;
      }
      else
      {
         return m_content_length;
      }
   }

   size_t http_client_socket::GetContentPtr()
   {
      return m_content_ptr;
   }

   size_t http_client_socket::GetPos()
   {
      return m_content_ptr;
   }

   bool http_client_socket::Complete()
   {
      return m_b_complete;
   }

   const unsigned char *http_client_socket::GetDataPtr() const
   {
      return m_memoryData.get_data();
   }

   void http_client_socket::OnContent()
   {
   }

   void http_client_socket::SetCloseOnComplete(bool x)
   {
      m_b_close_when_complete = x;
   }

   const string & http_client_socket::GetUrlProtocol()
   {
      return m_protocol;
   }

   const string & http_client_socket::GetUrlFilename()
   {
      return m_url_filename;
   }

   const string & http_client_socket::GetContentType()
   {
      return m_content_type;
   }

   void http_client_socket::Url(const string & url_in, string & host, port_t & port)
   {
      string url;
      url_this(url_in, m_protocol, m_host, m_port, url, m_url_filename);
      m_request.attr("url") = url;
      host = System.url().get_server(url);
      port = (port_t) System.url().get_port(url);
   }

} // namespace sockets
