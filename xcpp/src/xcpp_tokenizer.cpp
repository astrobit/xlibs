#include <xcpp.h>
#include <ios>
#include <iostream>


bool cpp_tokenizer::m_bData_Initialized = false;
std::map<std::string, cpp_tokenizer::token_type> cpp_tokenizer::m_mToken_Map;
std::set<char> cpp_tokenizer::m_setNon_Alpha_Characters;


inline bool is_name_char(char chChar)
{
	return ((chChar >= 'a' && chChar <= 'z') || (chChar >= 'A' && chChar <= 'Z') || chChar == '_');
}
inline bool is_whitespace(char chChar)
{
	return (chChar == ' ' || chChar == '\t');
}
inline bool is_cr_lf(char chChar)
{
	return (chChar == 10 || chChar == 13);
}
inline bool is_decimal(char chChar)
{
	return (chChar >= '0' && chChar <= '7');
}
inline bool is_octal(char chChar)
{
	return (chChar >= '0' && chChar <= '7');
}
inline bool is_hex(char chChar)
{
	return (chChar >= '0' && chChar <= '9') || (chChar >= 'a' && chChar <= 'f') || (chChar >= 'A' && chChar <= 'F');
}
inline bool is_binary(char chChar)
{
	return (chChar == '0' || chChar == '1');
}

cpp_tokenizer::cpp_tokenizer(void)
{
	if (!m_bData_Initialized)
	{
		m_vToken_List.clear();
		m_setNon_Alpha_Characters.clear();

		m_mToken_Map["{"] = token_openbrace;
		m_mToken_Map["}"] = token_closebrace;
		m_mToken_Map["("] = token_openparen;
		m_mToken_Map[")"] = token_closeparen;
		m_mToken_Map["["] = token_openbracket;
		m_mToken_Map["]"] = token_closebracket;
		m_mToken_Map[";"] = token_semicolon;

		m_mToken_Map["alignas"] = key_alignas;
		m_mToken_Map["alignof"] = key_alignof;
		m_mToken_Map["and"] = key_and;
		m_mToken_Map["andeq"] = key_andeq;
		m_mToken_Map["asm"] = key_asm;
		m_mToken_Map["atomic_cancel"] = key_atomic_cancel;
		m_mToken_Map["atomic_commit"] = key_atomic_commit;
		m_mToken_Map["atomic_noexcept"] = key_atomic_noexcept;
		m_mToken_Map["auto"] = key_auto;
		m_mToken_Map["bitand"] = key_bitand;
		m_mToken_Map["bitor"] = key_bitor;
		m_mToken_Map["bool"] = key_bool;
		m_mToken_Map["break"] = key_break;
		m_mToken_Map["case"] = key_case;
		m_mToken_Map["catch"] = key_catch;
		m_mToken_Map["char"] = key_char;
		m_mToken_Map["char16_t"] = key_char16_t;
		m_mToken_Map["char32_t"] = key_char32_t;
		m_mToken_Map["class"] = key_class;
		m_mToken_Map["compl"] = key_compl;
		m_mToken_Map["concept"] = key_concept;
		m_mToken_Map["const"] = key_const;
		m_mToken_Map["constexpr"] = key_constexpr;
		m_mToken_Map["const_cast"] = key_const_cast;
		m_mToken_Map["continue"] = key_continue;
		m_mToken_Map["decltype"] = key_decltype;
		m_mToken_Map["default"] = key_default;
		m_mToken_Map["do"] = key_do;
		m_mToken_Map["double"] = key_double;
		m_mToken_Map["dynamic_cast"] = key_dynamic_cast;
		m_mToken_Map["else"] = key_else;
		m_mToken_Map["enum"] = key_enum;
		m_mToken_Map["explicit"] = key_explicit;
		m_mToken_Map["export"] = key_export;
		m_mToken_Map["extern"] = key_extern;
		m_mToken_Map["false"] = key_false;
		m_mToken_Map["float"] = key_float;
		m_mToken_Map["for"] = key_for;
		m_mToken_Map["friend"] = key_friend;
		m_mToken_Map["goto"] = key_goto;
		m_mToken_Map["if"] = key_if;
		m_mToken_Map["inline"] = key_inline;
		m_mToken_Map["int"] = key_int;
		m_mToken_Map["mutable"] = key_mutable;
		m_mToken_Map["namespace"] = key_namespace;
		m_mToken_Map["noexcept"] = key_noexcept;
		m_mToken_Map["not"] = key_not;
		m_mToken_Map["not_eq"] = key_not_eq;
		m_mToken_Map["nullptr"] = key_nullptr;
		m_mToken_Map["operator"] = key_operator;
		m_mToken_Map["or"] = key_or;
		m_mToken_Map["or_eq"] = key_or_eq;
		m_mToken_Map["private"] = key_private;
		m_mToken_Map["protected"] = key_protected;
		m_mToken_Map["public"] = key_public;
		m_mToken_Map["register"] = key_register;
		m_mToken_Map["reinterpret_cast"] = key_reinterpret_cast;
		m_mToken_Map["requires"] = key_requires;
		m_mToken_Map["return"] = key_return;
		m_mToken_Map["short"] = key_short;
		m_mToken_Map["signed"] = key_signed;
		m_mToken_Map["sizeof"] = key_sizeof;
		m_mToken_Map["static"] = key_static;
		m_mToken_Map["static_assert"] = key_static_assert;
		m_mToken_Map["static_cast"] = key_static_cast;
		m_mToken_Map["struct"] = key_struct;
		m_mToken_Map["switch"] = key_switch;
		m_mToken_Map["synchronized"] = key_synchronized;
		m_mToken_Map["template"] = key_template;
		m_mToken_Map["this"] = key_this;
		m_mToken_Map["thread_local"] = key_thread_local;
		m_mToken_Map["key_throw"] = key_throw;
		m_mToken_Map["true"] = key_true;
		m_mToken_Map["try"] = key_try;
		m_mToken_Map["typedef"] = key_typedef;
		m_mToken_Map["typeid"] = key_typeid;
		m_mToken_Map["typename"] = key_typename;
		m_mToken_Map["union"] = key_union;
		m_mToken_Map["unsigned"] = key_unsigned;
		m_mToken_Map["using"] = key_using;
		m_mToken_Map["virtual"] = key_virtual;
		m_mToken_Map["void"] = key_void;
		m_mToken_Map["volatile"] = key_volatile;
		m_mToken_Map["wchar_t"] = key_wchar_t;
		m_mToken_Map["while"] = key_while;
		m_mToken_Map["xor"] = key_xor;
		m_mToken_Map["xor_eq"] = key_xor_eq;

		m_mToken_Map["override"] = special_override;
		m_mToken_Map["final"] = special_final;

		m_mToken_Map["new"] = oper_new;
		m_mToken_Map["delete"] = oper_delete;
		m_mToken_Map["++"] = oper_inc;
		m_mToken_Map["--"] = oper_dec;
		m_mToken_Map["+"] = oper_plus;
		m_mToken_Map["+="] = oper_pluseq;
		m_mToken_Map["-"] = oper_minus;
		m_mToken_Map["-="] = oper_minuseq;
		m_mToken_Map["*"] = oper_times_ptrindirection;
		m_mToken_Map["*="] = oper_timeseq;
		m_mToken_Map["/"] = oper_div;
		m_mToken_Map["/="] = oper_diveq;
		m_mToken_Map["%"] = oper_mod;
		m_mToken_Map["%="] = oper_modeq;
		m_mToken_Map["="] = oper_assign;
		m_mToken_Map["=="] = oper_booleq;
		m_mToken_Map["!"] = oper_boolnot;
		m_mToken_Map["!="] = oper_boolnoteq;
		m_mToken_Map["&&"] = oper_booland;
		m_mToken_Map["||"] = oper_boolor;
		m_mToken_Map["~"] = oper_bitnot;
		m_mToken_Map["&"] = oper_bitand_ptraddr;
		m_mToken_Map["&="] = oper_bitandeq;
		m_mToken_Map["|"] = oper_bitor;
		m_mToken_Map["|="] = oper_bitoreq;
		m_mToken_Map["^"] = oper_bitxor;
		m_mToken_Map["^="] = oper_bitxoreq;
		m_mToken_Map["<"] = oper_lt_openangle;
		m_mToken_Map["<="] = oper_lteq;
		m_mToken_Map[">"] = oper_gt_closeangle;
		m_mToken_Map[">="] = oper_gteq;
		m_mToken_Map["<<"] = oper_lsh;
		m_mToken_Map["<<="] = oper_lsheq;
		m_mToken_Map[">>"] = oper_rsh;
		m_mToken_Map[">>="] = oper_rsheq;
		m_mToken_Map[","] = oper_comma;
		m_mToken_Map["->"] = oper_pointer; //->
		m_mToken_Map["->*"] = oper_ptrmembr; //->*
		m_mToken_Map["..."] = oper_ellipsis;
		m_mToken_Map["."] = oper_member; //.
		m_mToken_Map["?"] = oper_if; //?}
		m_mToken_Map[":"] = oper_colon;
		m_mToken_Map["::"] = oper_scope;

		for (std::map<std::string,token_type>::iterator iterI = m_mToken_Map.begin(); iterI != m_mToken_Map.end(); iterI++)
		{
			for (std::string::const_iterator iterJ = iterI->first.cbegin(); iterJ != iterI->first.cend(); iterJ++)
			{
				if (!is_name_char(iterJ[0]) && !is_decimal(iterJ[0]))
				{
					m_setNon_Alpha_Characters.emplace(iterJ[0]);
				}
			}
		}
		m_bData_Initialized = true;
	}
}

cpp_tokenizer & cpp_tokenizer::operator << (const cpp_tokenizer & i_cRHO)
{
	for (std::vector<element>::const_iterator cI = i_cRHO.m_vToken_List.cbegin(); cI != i_cRHO.m_vToken_List.cend(); cI++)
		m_vToken_List.push_back(*cI);
	return *this;
}
cpp_tokenizer & cpp_tokenizer::operator << (std::ifstream &i_fStream)
{
	if (i_fStream.is_open())
	{
		std::string szLine;
		bool bC_Comment = false;
		bool bCpp_Comment = false;
		char	chString_Delimiter = 0;
		element	cElem;
		bool bContinue = true;
		bool bEOF = false;
		unsigned int uiLine_Number = 0;
		while (!bEOF)
		{
			szLine.clear();
			// read lines, stripping CR/LF and continuation characters
			// if there is a continuation character, read the next line and add it to the string to be processed
			bContinue = true;
			while (bContinue && !bEOF)
			{
				bContinue = false;
				std::string szLineLcl;
				bEOF = !getline (i_fStream,szLineLcl);
				uiLine_Number++;
				//std::cout << "Read line " << uiLine_Number << std::endl;
				if (!bEOF)
				{
					std::string::reverse_iterator riterI = szLineLcl.rbegin();
					while (riterI != szLineLcl.rend() && is_cr_lf(riterI[0]))
					{
						riterI++;
						szLineLcl.pop_back();
					}
					if (riterI[0] == '\\') // may be continuation
					{
						riterI++;
						if (riterI[0] != '\\') // anything but \\ is a continuation
						{
							szLineLcl.pop_back();
							bContinue = true;
						}
					}
					for (std::string::iterator iterI = szLineLcl.begin(); iterI != szLineLcl.end(); iterI++)
						szLine.push_back(iterI[0]);
				}
			}


			//std::cout << "Processing line " << uiLine_Number << std::endl;
			//std::cout << szLine << std::endl;
			// check for preprocessor directive
			std::string::iterator iterI = szLine.begin();
			bool bStart_Of_Line = true;
			while (iterI != szLine.end())
			{
				if (bC_Comment)
				{
					//std::cout << "Processing C comment" << std::endl;
					std::string::iterator iterJ = iterI;
					iterJ++;
					while (iterI != szLine.end() && (iterI[0] != '*' || (iterJ == szLine.end() || iterJ[1] != '/')))
					{
						cElem.m_szData.push_back(iterI[0]);
						iterI++;
						iterJ++;
					}
					if (iterI != szLine.end() && iterI[0] == '*' && iterJ != szLine.end() && iterJ[0] == '/')
					{
						cElem.m_eType = ccomment;
						m_vToken_List.push_back(cElem);
						cElem.m_szData.clear();
						bC_Comment = false;
						iterI += 2;
					}
					bStart_Of_Line = false;
				}
				else if (bCpp_Comment)
				{
					//std::cout << "Processing cpp comment" << std::endl;
					while (iterI != szLine.end())
					{
						cElem.m_szData.push_back(iterI[0]);
						iterI++;
					}
					cElem.m_eType = cppcomment;
					m_vToken_List.push_back(cElem);
					cElem.m_szData.clear();
					bCpp_Comment = false;
					bStart_Of_Line = false;
				}
				else if (chString_Delimiter == '\"')
				{
					//std::cout << "Processing string" << std::endl;
					while (iterI != szLine.end() && iterI[0] != chString_Delimiter)
					{
						cElem.m_szData.push_back(iterI[0]);
						iterI++;
					}
					if (iterI != szLine.end() && iterI[0] == chString_Delimiter)
					{ // we've gotten to the end of the string, add it
						cElem.m_eType = string;
						m_vToken_List.push_back(cElem);
						cElem.m_szData.clear();
						chString_Delimiter = 0;
						iterI++; // bypass delimiter
					}
					else
						std::cerr << "[" << uiLine_Number << "]: String contains end-of-line." << std::endl;
					bStart_Of_Line = false;
				}
				else
				{ // not in a comment or string, start by bypassing whitespace
					while (iterI != szLine.end() && is_whitespace(iterI[0]))
						iterI++;
					if (iterI != szLine.end())
					{
						if (iterI[0] == '#')
						{
							//std::cout << "Processing preprocesssor" << std::endl;
							cElem.m_eType = preprocessor;
							while (iterI != szLine.end())
							{
								cElem.m_szData.push_back(iterI[0]);
								iterI++;
							}
							m_vToken_List.push_back(cElem);
							cElem.m_szData.clear();
							if (!bStart_Of_Line)
								std::cerr << "[" << uiLine_Number << "]: Preprocessor directive not at beginning of line." << std::endl;
							bStart_Of_Line = false;
						}
						else if (is_name_char(iterI[0])) // keyword or symbol
						{
							//std::cout << "Processing keyword or symbol" << std::endl;
							while (iterI != szLine.end() && (is_name_char(iterI[0]) || is_decimal(iterI[0])))
							{
								cElem.m_szData.push_back(iterI[0]);
								iterI++;
							}
							if (m_mToken_Map.count(cElem.m_szData) > 0)
								cElem.m_eType = m_mToken_Map[cElem.m_szData];
							else
								cElem.m_eType = symbol;

							m_vToken_List.push_back(cElem);
							cElem.m_szData.clear();
							bStart_Of_Line = false;
						}
						else if (iterI[0] == '\'') // character const
						{
							//std::cout << "Processing character const" << std::endl;
							iterI++; // bypass '
							while (iterI != szLine.end() && iterI[0] != '\'')
							{
								cElem.m_szData.push_back(iterI[0]);
								iterI++;
							}
							if (iterI != szLine.end())
								iterI++; // bypass final '
							cElem.m_eType = character;
							m_vToken_List.push_back(cElem);
							cElem.m_szData.clear();
							bStart_Of_Line = false;
						}
						else if (iterI[0] == '\"') // character const
						{
							//std::cout << "Processing start of string" << std::endl;
							iterI++; // bypass "
							chString_Delimiter = '\"';
							bStart_Of_Line = false;
							// don't process it here - let the code above handle string processsing
						}
						else if (is_decimal(iterI[0])) // numeric data
						{
							//std::cout << "Processing number" << std::endl;
							enum encoding {binary,octal,decimal,hex};
							encoding eEncoding = decimal;
							if (iterI[0] == '-' || iterI[0] == '+')
							{// sign character
								cElem.m_szData.push_back(iterI[0]);
								iterI++;
							} 

							if (iterI[0] == '0') // first digit is 0, meaning hex, octal, or binary encoding
							{
								std::string::iterator iterJ = iterI;
								iterJ++;
								if (iterJ != szLine.end())
								{
									switch (iterJ[0])
									{
									case 'b':
									case 'B':
										eEncoding = binary;
										cElem.m_szData.push_back(iterI[0]);
										iterI++; // bypass b or B
										break;
									case 'x':
									case 'X':
										eEncoding = hex;
										cElem.m_szData.push_back(iterI[0]);
										iterI++; // bypass x or X
										break;
									default:
										// potenitally octal - test for a decimal point or exponent
										// get to end of numeric part
										while (iterJ != szLine.end() && is_decimal(iterJ[0]))
											iterJ++; 
										if (iterJ != szLine.end())
										{
											if (iterJ[0] == '.')
												eEncoding = decimal;
											else if (iterJ[0] == 'e' || iterJ[0] == 'E')
											{
												iterJ++; 
												if (iterJ != szLine.end() && (iterJ[0] == '-' || iterJ[0] == '+'))
													iterJ++; // bypass sign, legal character
												if (iterJ != szLine.end() && is_decimal(iterJ[0])) // there is a legal decimal character for the exponent
													eEncoding = decimal;
												else // this is probably a bad number
													eEncoding = octal;
											}
											else
												eEncoding = octal;
										}
										else
											eEncoding = octal;
										break;
									}
								}
								cElem.m_szData.push_back(iterI[0]);
								iterI++; // bypass 0
							}

							while (iterI != szLine.end() && 
								((eEncoding == decimal && is_decimal(iterI[0])) ||
								 (eEncoding == binary && is_binary(iterI[0])) ||
								 (eEncoding == hex && is_hex(iterI[0])) ||
								 (eEncoding == octal && is_octal(iterI[0]))))
							{
								cElem.m_szData.push_back(iterI[0]);
								iterI++; // add number
							}
							if (iterI != szLine.end())
							{
								if (iterI[0] == 'l' || iterI[0] == 'L' ||
									 iterI[0] == 'u' || iterI[0] == 'U')
								{
									cElem.m_szData.push_back(iterI[0]);
									iterI++; // add postfix
									if (iterI != szLine.end() && (iterI[0] == 'l' || iterI[0] == 'L'))
									{
										cElem.m_szData.push_back(iterI[0]);
										iterI++; // add postfix
									}
								}
								else if (eEncoding == decimal)
								{
									bool bMay_Be_Float = false;
									if (iterI[0] == '.')
									{
										bMay_Be_Float = true;
										cElem.m_szData.push_back(iterI[0]);
										iterI++; // add decimal
										while (iterI != szLine.end() &&  is_decimal(iterI[0]))
										{
											cElem.m_szData.push_back(iterI[0]);
											iterI++; // add fraction part
										}
									}
									if (iterI != szLine.end() && (iterI[0] == 'e' || iterI[0] == 'E'))
									{
										bMay_Be_Float = true;
										cElem.m_szData.push_back(iterI[0]);
										iterI++; // add exponent id
										if (iterI != szLine.end() && (iterI[0] == '+' || iterI[0] == '-'))
										{
											cElem.m_szData.push_back(iterI[0]);
											iterI++; // add exponent sign
										}
										while (iterI != szLine.end() && is_decimal(iterI[0]))
										{
											cElem.m_szData.push_back(iterI[0]);
											iterI++; // add exponent part
										}
									}
									// check for float or long double postfix
									if (iterI != szLine.end() && bMay_Be_Float && 
											(iterI[0] == 'f' || iterI[0] == 'F' ||
											 iterI[0] == 'l' || iterI[0] == 'L'))
									{
										cElem.m_szData.push_back(iterI[0]);
										iterI++; // add exponent id
									}
								}
							}
							if (iterI != szLine.end() && is_name_char(iterI[0]))
							{
								std::cerr << "[" << uiLine_Number << "]: Ill formed number." << std::endl;
							}
							bStart_Of_Line = false;
							cElem.m_eType = numeric;
							m_vToken_List.push_back(cElem);
							cElem.m_szData.clear();

						}
						else
						{ // operators, braces, etc
							//std::cout << "Processing operator or other special symbol" << std::endl;
							std::string::iterator iterJ = iterI;
							iterJ++;
							if (iterI[0] == '/' && iterJ != szLine.end() && iterJ[0] == '/')
							{
								bCpp_Comment = true;
								iterI += 2;
							}
							else if (iterI[0] == '/' && iterJ != szLine.end() && iterJ[0] == '*')
							{
								bC_Comment = true;
								iterI += 2;
							}
							else // no, it really is a symbol (or junk)
							{
								std::string szTest;
								szTest.clear(); // make sure test is empty
								szTest.push_back(iterI[0]);
								iterJ = iterI;
								while (iterJ != szLine.end() && m_mToken_Map.count(szTest) >= 1)
								{
									cElem.m_szData = szTest;
									iterJ++;
									szTest.push_back(iterJ[0]);
								}
								if (!cElem.m_szData.empty())
								{
									cElem.m_eType = m_mToken_Map[cElem.m_szData];
									cElem.m_szData.clear();
									m_vToken_List.push_back(cElem);
									iterI = iterJ;
								}
								else // failed to identify the symbol - fault
								{
									cElem.m_szData.clear(); // make sure to clear data 
									std::cerr << "[" << uiLine_Number << "]: Unrecognized symbol." << std::endl;
								}
							} // type processing
						} // if operators/brace
					} // if not end of line
				} // if not comment / string
			} // while not end of line
		} // while !eof
	} // fHeader open
	return *this;
} // end of function



