#pragma once


namespace fs
{


// fs::interface - file system interface


   class CLASS_DECL_ca2 data :
      virtual public ::data
   {
   public:

      data(sp(base_application) papp);


      // optional if ls_dir is implemented
      virtual bool has_subdir(const char * pszPath);
      virtual bool ls(const char * pszDir, stringa * pstraPath, stringa * pstraTitle);
      virtual bool is_dir(const char * pszPath);
      virtual void root_ones(stringa & stra);
      virtual void get_ascendants_path(const char * pszPath, stringa & stra);
      virtual void get_ascendants_name(const char * lpcsz, stringa & straParam);
      virtual string eat_end_level(const char * pszPath, int32_t iCount);


      virtual string file_name(const char * pszPath);
      virtual string dir_path(const char * psz1, const char * psz2);

      virtual bool file_exists(const char * pszPath);


      virtual bool file_move(const char * pszDst, const char * pszSrc);

      virtual ::file::binary_buffer_sp get_file(var varFile, UINT nOpenFlags);
      virtual ::ca2::byte_stream get_byte_stream(var varFile, UINT nOpenFlags);


      virtual bool fast_has_subdir(const char * pszPath);

   };



} // namespace fs