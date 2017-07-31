


VOID
WINAPI
output_debug_string_a(
   _In_opt_ LPCSTR lpOutputString
   )
{

   ::OutputDebugStringW(wstring(lpOutputString));

}

typedef WINBASEAPI
VOID
WINAPI
FN_OutputDebugStringA(
   _In_opt_ LPCSTR lpOutputString
   );

typedef FN_OutputDebugStringA * PFN_OutputDebugStringA;

PFN_OutputDebugStringA g_pfnOutputDebugString = ::OutputDebugStringA;

void set_simple_output_debug_string()
{
   g_pfnOutputDebugString = ::OutputDebugStringA;
}

void set_extended_output_debug_string()
{
   g_pfnOutputDebugString = ::output_debug_string_a;
}

void output_debug_string(const char * psz)
{
   g_pfnOutputDebugString(psz);
}

