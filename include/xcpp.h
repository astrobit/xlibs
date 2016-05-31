#pragma once

#include <map>
#include <vector>
#include <set>
#include <string>
#include <fstream>

class cpp_tokenizer
{
public:

	enum	token_type 
	{
		symbol,
		numeric,
		string, 
		character,
		ccomment,
		cppcomment,
		preprocessor,
		token_openbrace,
		token_closebrace,
		token_openparen,
		token_closeparen,
		token_openbracket,
		token_closebracket,
		token_semicolon, 
		key_alignas,
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
		key_volatile,
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
		oper_scope, // ::
		special_override,
		special_final
	};
	class element
	{
	public:
		token_type	m_eType;
		std::string	m_szData;
	};
	typedef std::vector<cpp_tokenizer::element>::const_iterator element_iterator;
private:
	static bool	m_bData_Initialized;
	static std::map<std::string, token_type> m_mToken_Map;
	static std::set<char> m_setNon_Alpha_Characters;

	std::vector<element> m_vToken_List;
public:
	void clear(void) {m_vToken_List.clear();}
	bool empty(void)const {return m_vToken_List.empty();}
	size_t size(void)const {return m_vToken_List.size();}
	const element operator[](size_t i_uiIndex)const {return m_vToken_List[i_uiIndex];}

	std::vector<element>::const_iterator cbegin(void) const {return m_vToken_List.cbegin();}
	std::vector<element>::const_iterator cend(void) const {return m_vToken_List.cend();}
	std::vector<element>::const_reverse_iterator crbegin(void) const {return m_vToken_List.crbegin();}
	std::vector<element>::const_reverse_iterator crend(void) const {return m_vToken_List.crend();}

	cpp_tokenizer(void);

	cpp_tokenizer & operator << (const cpp_tokenizer & i_cRHO);
	cpp_tokenizer & operator << (std::ifstream &i_fStream);

}; // end of tokenizer class

class cpp_dictionary
{
private:
	std::map<std::string,std::string> m_mDictionary;
public:
	cpp_dictionary(void);

	cpp_dictionary & operator << (const cpp_dictionary & i_cRHO);
	cpp_dictionary & operator << (std::ifstream &i_fStream);
	cpp_dictionary & operator << (const cpp_tokenizer &i_cppTokenlist);
};

