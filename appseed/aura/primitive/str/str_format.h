#pragma once


#if defined(VARIADIC_TEMPLATE_FORMAT2)


class CLASS_DECL_AURA string_format_printer
{
public:



   typedef void (string_format_printer:: * PRINTER)(void *, const char *, strsize);


};



/*
format
String that contains the text to be written to stdout.
It can optionally contain embedded format tags that are substituted by the values specified in subsequent argument(s) and formatted as requested.
The number of arguments following the format parameters should at least be as much as the number of format tags.
The format tags follow this prototype:

%[flags][width][.precision][length]specifier
Where specifier is the most significant one and defines the type and the interpretation of the value of the coresponding argument:
specifier	Output	Example
ca	Character	a
d or i	Signed decimal integer	392
e	Scientific notation (mantissa/exponent) using e character	3.9265e+2
E	Scientific notation (mantissa/exponent) using E character	3.9265E+2
f	Decimal floating point	392.65
g	Use the shorter of %e or %f	392.65
G	Use the shorter of %E or %f	392.65
o	Unsigned octal	610
s	String of characters	sample
u	Unsigned decimal integer	7235
x	Unsigned hexadecimal integer	7fa
X	Unsigned hexadecimal integer (capital letters)	7FA
p	Pointer address	B800:0000
n	Nothing printed. The argument must be a pointer to a int32_t, where the number of characters written so far is stored.
%	A % followed by another % character will write % to stdout.	%

The tag can also contain flags, width, .precision and length sub-specifiers, which are optional and follow these specifications:

flags	description
-	Left-justify within the given field width; Right justification is the default (see width sub-specifier).
+	Forces to precede the result with a plus or minus sign (+ or -) even for positive numbers. By default, only negative numbers are preceded with a - sign.
(space)	If no sign is going to be written, a blank space is inserted before the value.
#	Used with o, x or X specifiers the value is preceeded with 0, 0x or 0X respectively for values different than zero.
Used with e, E and f, it forces the written output to contain a decimal point even if no digits would follow. By default, if no digits follow, no decimal point is written.
Used with g or G the result is the same as with e or E but trailing zeros are not removed.
0	Left-pads the number with zeroes (0) instead of spaces, where padding is specified (see width sub-specifier).

width	description
(number)	Minimum number of characters to be printed. If the value to be printed is shorter than this number, the result is padded with blank spaces. The value is not truncated even if the result is larger.
*	The width is not specified in the format string, but as an additional integer value argument preceding the argument that has to be formatted.

.precision	description
.number	For integer specifiers (d, i, o, u, x, X): precision specifies the minimum number of digits to be written. If the value to be written is shorter than this number, the result is padded with leading zeros. The value is not truncated even if the result is longer. A precision of 0 means that no character is written for the value 0.
For e, E and f specifiers: this is the number of digits to be printed after the decimal point.
For g and G specifiers: This is the maximum number of significant digits to be printed.
For s: this is the maximum number of characters to be printed. By default all characters are printed until the ending NULL character is encountered.
For ca type: it has no effect.
When no precision is specified, the default is 1. If the period is specified without an explicit value for precision, 0 is assumed.
.*	The precision is not specified in the format string, but as an additional integer value argument preceding the argument that has to be formatted.

length	description
h	The argument is interpreted as a short int32_t or unsigned short int32_t (only applies to integer specifiers: i, d, o, u, x and X).
l	The argument is interpreted as a long int32_t or unsigned long int32_t for integer specifiers (i, d, o, u, x and X), and as a wide character or wide character string for specifiers ca and s.
L	The argument is interpreted as a long double (only applies to floating point specifiers: e, E, f, g and G).

additional arguments
Depending on the format string, the function may expect a sequence of additional arguments, each containing one value to be inserted instead of each %-tag specified in the format parameter, if any. There should be the same number of these arguments as the number of %-tags that expect a value.


*/


class CLASS_DECL_AURA string_format
{
public:


   enum e_state
   {

      state_initial,
      state_waiting_width,
      state_parse_precision,
      state_waiting_precision,
      state_parse_length

   };


   char *                           m_pszBuffer;
   strsize                          m_iAllocation;
   strsize                          m_iLength;


   e_state                          m_estate;


   bool                             m_bLeftJustify;
   bool                             m_bForceShowSign;
   bool                             m_bSharp;
   bool                             m_bZeroPadding;

   int32_t                          m_iWidth;
   int32_t                          m_iPrecision;

   char                             m_chLength;
   char                             m_chSpec;



   string_format();
   ~string_format();



   void construct();

   void allocate_add_up(strsize iLenAddUp);

   inline void append(char ch)
   {

      allocate_add_up(1);

      m_pszBuffer[m_iLength] = ch;

      m_iLength++;

   }


   inline void append(const char * psz)
   {

      if(psz == NULL)
         return;

      try
      {

         strsize iLen = strlen(psz);

         allocate_add_up(iLen);

         strncpy(&m_pszBuffer[m_iLength], psz, iLen);

         m_iLength += iLen;

      }
      catch(...)
      {

      }

   }

   bool parse(const char * & s);


   inline bool defer_get_additional_argument(const char * & s)
   {

      throw "missing argument value";

   }


   template < typename T, typename... Args>
   inline void defer_get_additional_argument(const char * & s, const T & value, Args... args)
   {

      if(m_estate == state_initial || m_estate == state_parse_precision || m_estate == state_parse_length)
      {

         if(!parse(s))
         {

            ::str::format_type(this, value);

            m_estate = state_initial;

            format(s, args...);

            return;

         }

      }

      if(m_estate == state_waiting_width)
      {

         throw "width should plain int32_t";

      }
      else if(m_estate == state_waiting_precision)
      {

         throw "width should plain int32_t";

      }

      defer_get_additional_argument(s, args...);

   }

   template < typename T, typename... Args>
   inline void defer_get_additional_argument(const char * & s, const int32_t & value, Args... args)
   {

      if(m_estate == state_initial || m_estate == state_parse_precision || m_estate == state_parse_length)
      {

         if(!parse(s))
         {

            ::str::format(this, value);

            format(s, args...);

            return;

         }

      }

      if(m_estate == state_waiting_width)
      {

         m_iWidth = value;

         m_estate = state_parse_precision;

      }
      else if(m_estate == state_waiting_precision)
      {

         m_iPrecision = value;

         m_estate = state_parse_length;

      }

      defer_get_additional_argument(s, args...);

   }

   void format(const char * & s);

   template<typename T, typename... Args>
   inline void format(const char * & s, const T & value, Args... args);


   inline string to_string();

};

#endif


#ifdef DEBUG

void debug_ca2_variadic_template_base_formatting_integer_zero_padding()

#endif

