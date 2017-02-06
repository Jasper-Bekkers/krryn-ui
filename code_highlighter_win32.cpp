#include "code_highlighter_win32.h"
#include "code_highlighter.h"
#include "widget_helper_win32.h"
#include "Scintilla.h"
#include "SciLexer.h"

using namespace gui_imp;
using namespace gui;

// TODO: Some of this stuff is /not/ platform dependant, move that out :-)
// TODO: Make highlighting colors configurable

// Some words are actually case-insensitive according to the spec
// however, scintilla only supports everything case-sensitive or
// everything case-insensitive. There doesn't seem to be a "some
// words should be case-insensitive" option for the CPP lexer.
static const char g_CgKeywords[] = "asm asm_fragment auto break case catch char class column_major"
	"compile const const_cast continue decl default delete discard do double dword dynamic_cast else "
	"emit enum explicit extern false fixed for friend get goto if in inline inout int "
	"interface long matrix mutable namespace new operator out packed pass pixelfragment pixelshader "
	"private protected public register reinterpret_cast return row_major sampler sampler_state sampler1D "
	"sampler2D sampler3D samplerCUBE shared short signed sizeof static static_cast string struct switch "
	"technique template texture texture1D texture2D texture3D textureCUBE textureRECT this throw true "
	"try typedef typeid typename uniform union unsigned using vector vertexfragment vertexshader "
	"virtual void volatile while "
	
	"bool bool2 bool3 bool4 bool2x2 bool3x3 bool4x4 "
	"float float2 float3 float4 float2x2 float3x3 float4x4 "
	"half half2 half3 half4 half2x2 half3x3 half4x4 "
	"fixed fixed2 fixed3 fixed4 fixed2x2 fixed3x3 fixed4x4 ";

static const char g_CgFunctions[] = 
	// mathematical functions
	"abs acos all any asin atan atan2 ceil clamp cos cosh cross "
	"degrees determinant dot exp exp2 floor fmod frac frexp isfinite isinf isnan ldexp lerp lit "
	"log log2 log10 max min modf mul mul mul noise pow radians round rsqrt saturate sign sin sincos "
	"sinh smoothstep step sqrt tan tanh transpose "
	
	// geometric functions
	"distance faceforward length normalize reflect refract "

	// texture map functions
	"tex1D tex1Dproj tex2D tex2Dproj texRECT texRECTproj tex3D tex3Dproj texCUBE texCUBEproj "

	// derivative functions
	"ddx ddy "

	// debug functions
	"debug ";

void code_highlighter_impl_win32::make(const code_highlighter_initializer &a_Initializer){
	static HMODULE handle = 0;

	if(!handle){
		handle = LoadLibrary(TEXT("..\\scintilla\\scintilla.dll"));

		if(!handle){
			throw std::runtime_error("Can't load scintilla.dll!");
		}
	}

	HWND l_ParentWnd = (HWND)a_Initializer.get_parent()->handle();

	{
		//widget_hook_win32(this);

		m_hWnd = ::CreateWindow(
			TEXT("Scintilla"),
			"Source",
			WS_CHILD | WS_VSCROLL | WS_HSCROLL | WS_CLIPCHILDREN,
			a_Initializer.get_left(), a_Initializer.get_top(), 
			a_Initializer.get_width(), a_Initializer.get_height(), 
			l_ParentWnd, 0, 0, 0);

		::ShowWindow(m_hWnd, SW_SHOW);
		::SetFocus(m_hWnd);
	}

	const COLORREF black = RGB(0, 0, 0);
	const COLORREF white = RGB(0xFF, 0xFF, 0xFF);
	const COLORREF comment = RGB(0, 127, 0);
	const COLORREF number = RGB(56, 142, 142);
	const COLORREF keyword = RGB(0, 0, 239);
	const COLORREF string = RGB(127, 0, 127);
	const COLORREF function = RGB(112, 128, 255);

	send(SCI_SETLEXER, SCLEX_CPP);
	send(SCI_SETKEYWORDS, 0, reinterpret_cast<LPARAM>(g_CgKeywords));
	send(SCI_SETKEYWORDS, 1, reinterpret_cast<LPARAM>(g_CgFunctions));
	set_style(STYLE_DEFAULT, black, white, 9, "Courier New");
	send(SCI_STYLECLEARALL);

	set_style(SCE_C_DEFAULT, black, white);
	set_style(SCE_C_COMMENTLINE, comment, white);
	set_style(SCE_C_NUMBER, number, white);
	set_style(SCE_C_STRING, string, white);
	set_style(SCE_C_CHARACTER, string, white);
	set_style(SCE_C_WORD, keyword, white);
	set_style(SCE_C_OPERATOR, black, white);
	send(SCI_STYLESETBOLD, SCE_C_OPERATOR, 1);
	set_style(SCE_C_IDENTIFIER, black, white);
	set_style(SCE_C_STRINGEOL, string, white);

	set_style(SCE_C_WORD2, function, white);
	send(SCI_STYLESETBOLD, SCE_C_WORD2, 1);

	// show line numbers in margin
	send(SCI_SETMARGINTYPEN, 1, SC_MARGIN_NUMBER);
	send(SCI_SETMARGINWIDTHN, 1, 40);
}

void code_highlighter_impl_win32::set_text(const std::string &a_Text){
	send(SCI_SETTEXT, 0, (LPARAM)a_Text.c_str());
}

void code_highlighter_impl_win32::send(UINT Msg, WPARAM wParam, LPARAM lParam){
	::SendMessage(m_hWnd, Msg, wParam, lParam);
}

void code_highlighter_impl_win32::set_style(int style, COLORREF fore, COLORREF back, int size, const char *face){
	send(SCI_STYLESETFORE, style, fore);
	send(SCI_STYLESETBACK, style, back);
	if (size >= 1) send(SCI_STYLESETSIZE, style, size);
	if (face) send(SCI_STYLESETFONT, style, reinterpret_cast<LPARAM>(face));
}
