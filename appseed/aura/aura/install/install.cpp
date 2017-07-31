//#include "framework.h"


#if defined(INSTALL_SUBSYSTEM)

#ifdef LINUX

//#include <unistd.h>

#endif




int32_t LangFromOS();





int32_t run_file(const char * pszFile, int32_t nCmdShow);









#ifdef SUPORTA_MATEMATICA_AVANCADA
double g_dPi = asin_dup(1.0) * 4;
#endif





namespace install
{

//   interaction_impl g_window;

} // namespace install









/*
string Login()
{
   if(stricmp_dup(g_pedit->m_str, "ca2") == 0
      && stricmp_dup(g_pedit->m_pnext->m_str, "ca2") == 0)
   {
      return "OK";
   }
   string document;
   string strPost;
   strPost = "entered_login=";
   strPost += g_pedit->m_str;
   strPost += "&entered_password=";
   strPost += g_pedit->m_pnext->m_str;
   document = ms_post("https://ca2os.com/ca2api/auth", strPost);
	return document;
}
*/





/*bool Get(const string& url_in,string & document)
{
   bool https = !strcasecmp(url.substr(0,8), "https://");
	int32_t port = https ? 443 : 80;
	string url = url_in;
	string file; // get filename at end of url
	string dir; // get filename at end of url
	{
		Parse pa(url,"/");
		string tmp = pa.getword();
      string lastfile;
		while (tmp.size())
		{
			file = tmp;
         dir = "/" + lastfile;
         lastfile = file;
			tmp = pa.getword();
		}
	}
	SocketHandler h;
	bool complete = false;

	if (!strcasecmp(protocol,"http") || https)
	{
		HttpGetSocket s(h, host, port, dir, file);
		uchar *p = new uchar[32000]; // MAX doc size
		memset_dup(p, 0, 32000);
		s.SetDataPtr(p, 31999); // preserve zero at end of document
/*		if (https)
		{
			s.EnableSSL();
		}*/
/*		h.add(&s);
		h.Select(240,0);
		while (h.get_count())
		{
			h.Select(240,0);
		}
		complete = s.Complete();
		if (complete)
		{
			document = static_cast<string>( (char *)s.GetDataPtr());
		}
		delete[] s.GetDataPtr();
	}
	else
	{
		printf_dup("Unknown protocol: '%s'\n",protocol);
	}
	if (complete)
	{
		printf_dup("%s\n", document);
	}
	return complete;
}*/
























string url_query_param(int32_t & iParam, const char * pszParam)
{
   string str;
   if(iParam == 0)
   {
      iParam++;
      str = "?";
      return str + pszParam;
   }
   else
   {
      iParam++;
      str = "&";
      return str + pszParam;
   }
}






#endif


