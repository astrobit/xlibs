#include <xcpp.h>

cpp_dictionary & cpp_dictionary::operator << (std::ifstream &i_fStream)
{
	cpp_tokenizer cTokens;
	cTokens << i_fStream;
	return (*this << cTokens);
}

cpp_dictionary & cpp_dictionary::operator << (const cpp_tokenizer &i_cppTokenlist)
{
	enum protection_status {ps_private, ps_protected, ps_public} eProtection_Status = ps_public;

	std::vector <std::string> vCurrent_Scope; // the overall scope of where we are in the code
	std::vector <std::string> vCurrent_Symbol_Scope; // the scope of the symbol currently being processed
	std::vector <cpp_tokenizer::token_type> veType_Qualifiers;
	for (cpp_tokenizer::element_iterator cIter = i_cppTokenlist.cbegin(); cIter != i_cppTokenlist.cend(); cIter++)
	{
		switch (cIter->m_eType)
		{
		case cpp_tokenizer::preprocessor:
			// ignore preprocessor directives for now
			break;
		case cpp_tokenizer::key_unsigned:
		case cpp_tokenizer::key_signed:
		case cpp_tokenizer::key_bool:
		case cpp_tokenizer::key_char:
		case cpp_tokenizer::key_enum:
		case cpp_tokenizer::key_int:
		case cpp_tokenizer::key_short:
		case cpp_tokenizer::key_char16_t:
		case cpp_tokenizer::key_char32_t:
		case cpp_tokenizer::key_float:
		case cpp_tokenizer::key_double:
		case cpp_tokenizer::key_class:
		case cpp_tokenizer::key_struct:
		case cpp_tokenizer::key_union:
		case cpp_tokenizer::key_namespace:
		case cpp_tokenizer::key_void:
		case cpp_tokenizer::key_wchar_t:

		case cpp_tokenizer::key_auto:
		case cpp_tokenizer::key_const:
		case cpp_tokenizer::key_explicit: // does not allow automatic typecasting, useful mostly for constructors, but also some operators
		case cpp_tokenizer::key_export:
		case cpp_tokenizer::key_extern:
		case cpp_tokenizer::key_friend:
		case cpp_tokenizer::key_inline:
		case cpp_tokenizer::key_noexcept: // function will not throw exceptions
		case cpp_tokenizer::key_operator:
		case cpp_tokenizer::key_register: // hint to place data in a register
		case cpp_tokenizer::key_static:
		case cpp_tokenizer::key_template: // probably going to need additional data processing for this
		case cpp_tokenizer::key_typedef:
		case cpp_tokenizer::key_volatile:
		case cpp_tokenizer::key_virtual:

		case cpp_tokenizer::special_override:
		case cpp_tokenizer::special_final:
			veType_Qualifiers.push_back(cIter->m_eType);
			break;
		case cpp_tokenizer::symbol:
			break;
		}
	}
	return *this;
}

cpp_dictionary & cpp_dictionary::operator << (const cpp_dictionary & i_cRHO)
{
	return *this;
}


