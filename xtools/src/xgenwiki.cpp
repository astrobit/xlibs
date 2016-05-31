#include<cstdio>
#include<cstdlib>
#include <xcpp.h>
#include <iostream>

int main(int i_iArg_Count, const char * i_lpszArg_Values[])
{
	std::ifstream 	fStream;

	fStream.open(i_lpszArg_Values[1]);

	if (fStream.is_open())
	{
		cpp_tokenizer cTokenizer;
		cTokenizer << fStream;
		fStream.close();
		for (std::vector<cpp_tokenizer::element>::const_iterator iterI = cTokenizer.cbegin(); iterI != cTokenizer.cend(); iterI++)
		{
			switch (iterI->m_eType)
			{
			case cpp_tokenizer::symbol:
				std::cout << "symbol: " << iterI->m_szData << std::endl;
				break;
			case cpp_tokenizer::numeric:
				std::cout << "numeric: " << iterI->m_szData << std::endl;
				break;
			case cpp_tokenizer::string:
				std::cout << "string: " << iterI->m_szData << std::endl;
				break;
			case cpp_tokenizer::character:
				std::cout << "character: " << iterI->m_szData << std::endl;
				break;
			case cpp_tokenizer::preprocessor:
				std::cout << "proprocessor: " << iterI->m_szData << std::endl;
				break;
			case cpp_tokenizer::ccomment:
				std::cout << "c comment: " << iterI->m_szData << std::endl;
				break;
			case cpp_tokenizer::cppcomment:
				std::cout << "cpp comment: " << iterI->m_szData << std::endl;
				break;
			case cpp_tokenizer::token_openbrace:
				std::cout << "{" << std::endl;
				break;
			case cpp_tokenizer::token_closebrace:
				std::cout << "}" << std::endl;
				break;
			case cpp_tokenizer::token_openparen:
				std::cout << "(" << std::endl;
				break;
			case cpp_tokenizer::token_closeparen:
				std::cout << ")" << std::endl;
				break;
			case cpp_tokenizer::token_openbracket:
				std::cout << "[" << std::endl;
				break;
			case cpp_tokenizer::token_closebracket:
				std::cout << "]" << std::endl;
				break;
			case cpp_tokenizer::token_semicolon:
				std::cout << ";" << std::endl;
				break;
			default:
				std::cout << '.' << std::endl;
/*		key_alignas,
		key_alignof,
		key_and,
		key_andeq,
		key_asm,
		key_atomic_cancel,
		key_atomic_commit,
		key_atomic_noexcept,
		key_auto,
		key_bitand,
		key_bitor,
		key_bool,
		key_break,
		key_case,
		key_catch,
		key_char,
		key_char16_t,
		key_char32_t,
		key_class,
		key_compl,
		key_concept,
		key_const,
		key_constexpr,
		key_const_cast,
		key_continue,
		key_decltype,
		key_default,
		key_do,
		key_double,
		key_dynamic_cast,
		key_else,
		key_enum,
		key_explicit,
		key_export,
		key_extern,
		key_false,
		key_float,
		key_for,
		key_friend,
		key_goto,
		key_if,
		key_inline,
		key_int,
		key_mutable,
		key_namespace,
		key_noexcept,
		key_not,
		key_not_eq,
		key_nullptr,
		key_operator,
		key_or,
		key_or_eq,
		key_private,
		key_protected,
		key_public,
		key_register,
		key_reinterpret_cast,
		key_requires,
		key_return,
		key_short,
		key_signed,
		key_sizeof,
		key_static,
		key_static_assert,
		key_static_cast,
		key_struct,
		key_switch,
		key_synchronized,
		key_template,
		key_this,
		key_thread_local,
		key_throw,
		key_true,
		key_try,
		key_typedef,
		key_typeid,
		key_typename,
		key_union,
		key_unsigned,
		key_using,
		key_virtual,
		key_void,
		key_volatle,
		key_wchar_t,
		key_while,
		key_xor,
		key_xor_eq,
		oper_new,
		oper_delete,
		oper_inc,
		oper_dec,
		oper_plus,
		oper_pluseq,
		oper_minus,
		oper_minuseq,
		oper_times_ptrindirection,
		oper_timeseq,
		oper_div,
		oper_diveq,
		oper_mod,
		oper_modeq,
		oper_assign,
		oper_booleq,
		oper_boolnot,
		oper_boolnoteq,
		oper_booland,
		oper_boolor,
		oper_bitnot,
		oper_bitand_ptraddr,
		oper_bitandeq,
		oper_bitor,
		oper_bitoreq,
		oper_bitxor,
		oper_bitxoreq,
		oper_lt_openangle,
		oper_lteq,
		oper_gt_closeangle,
		oper_gteq,
		oper_lsh,
		oper_lsheq,
		oper_rsh,
		oper_rsheq,
		oper_comma,
		oper_pointer, //->
		oper_ptrmembr, //->*
		oper_ellipsis,
		oper_member, //.
		oper_if, //?
		oper_colon, // :
		oper_scope // ::*/
			}
		}
	}
	else
	{
		std::cout << "Usage: xgenwiki <path-to-header-file> <path-to-wiki>" << std::endl;
		std::cout << "Generates wiki pages for a header file." << std::endl;
	}

	return 0;
}

