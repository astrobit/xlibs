#include <map>
#include <vector>
#include <string>
#include <xstdlib.h>
#include <iostream>
#include <cmath>

using namespace xstdlib;

command_line_parse::command_line_parse(void)
{
}
command_line_parse::command_line_parse(int i_iArg_Count, const char * i_lpszArg_Values[])
{
	Read_Command_Line(i_iArg_Count,i_lpszArg_Values);
}

void command_line_parse::Add_Allowed_Option(const std::string &i_sOption , xstdlib::datatype i_eType, const std::string & i_sExplanation )
{
	if (!i_sOption.empty())
	{
		m_vAllowed_Options.push_back(i_sOption);
		m_mAllowed_Option_Types[i_sOption] = i_eType;
		if (!i_sExplanation.empty())
			m_mAllowed_Options_Explanations[i_sOption] = i_sExplanation;
	}
}
void command_line_parse::Add_Allowed_Options(const std::vector<std::string> & i_vsOption_List, const std::map<std::string,xstdlib::datatype> i_mOption_Types, const std::map<std::string, std::string> & i_msOption_Explanation_List)
{
	m_vAllowed_Options = i_vsOption_List;
	m_mAllowed_Options_Explanations = i_msOption_Explanation_List;
	m_mAllowed_Option_Types = i_mOption_Types;
}


int command_line_parse::Read_Command_Line(int i_iArg_Count, const char * i_lpszArg_Values[])
{
	int iRet = 0;
	vUnrecognized_Options.clear();
	vIncorrect_Type_Options.clear();
	m_sCall_Path.clear();
	mapValues.clear();
	mapTypes.clear();

	if (i_iArg_Count > 0)
		m_sCall_Path = i_lpszArg_Values[0];
	for (size_t tI = 1; tI < i_iArg_Count; tI++)
	{
		std::string sFull_Opt = i_lpszArg_Values[tI];
		std::string sKey;
		std::string sValue;
		size_t tEquals = sFull_Opt.find('=');
		if (tEquals != std::string::npos)
		{
			sKey = sFull_Opt.substr(0,tEquals);
			if (sFull_Opt.size() > (tEquals + 1)) // there is data past the equals
			{
				size_t tStart_Idx = tEquals + 1;
				sValue = sFull_Opt.substr(tStart_Idx,sFull_Opt.size() - tStart_Idx);
			}
			else
			{
				tI++;
				sValue = i_lpszArg_Values[tI];
			}
		}
		else
		{
			sKey = sFull_Opt;
			if (tI < (i_iArg_Count - 1) && i_lpszArg_Values[tI + 1][0] == '=')
			{
				tI++;
				if (i_lpszArg_Values[tI][1] == 0) ///
				{
					tI++;
					sValue = i_lpszArg_Values[tI];
				}
				else
				{
					sValue = &i_lpszArg_Values[tI][1];
				}
			}
		}

		if (!sValue.empty())
		{
			mapValues[sKey] = sValue;
			mapTypes[sKey] = xstdlib::identify_datatype(sValue);
		}
		else
		{
			mapValues[sKey] = std::string();
			mapTypes[sKey] = xstdlib::empty;
		}

	}

	if (!m_vAllowed_Options.empty())
	{
		for (auto iterI = mapTypes.begin(); iterI != mapTypes.end(); iterI++)
		{
			if (iterI->first.at(0) == '-') // only check options that start with '-' (or "--")
			{
				if (m_mAllowed_Option_Types.count(iterI->first) == 0)
				{
					iRet--;
					vUnrecognized_Options.push_back(iterI->first);
				}
				else
				{
					switch (m_mAllowed_Option_Types[iterI->first])
					{
					case xstdlib::floating_point:
						if (iterI->second == xstdlib::logical ||
							iterI->second == xstdlib::string  ||
							iterI->second == xstdlib::hex     ||
							iterI->second == xstdlib::octal   ||
							iterI->second == xstdlib::binary)
						{
							iRet--;
							vIncorrect_Type_Options.push_back(iterI->first);
						}
						break;
					case xstdlib::integer:
						if (iterI->second == xstdlib::logical ||
							iterI->second == xstdlib::string  ||
							iterI->second == xstdlib::hex     ||
							iterI->second == xstdlib::octal   ||
							iterI->second == xstdlib::binary  ||
							iterI->second == xstdlib::floating_point)
						{
							iRet--;
							vIncorrect_Type_Options.push_back(iterI->first);
						}
						break;
					case xstdlib::logical:
						if (iterI->second == xstdlib::integer ||
							iterI->second == xstdlib::string  ||
							iterI->second == xstdlib::hex     ||
							iterI->second == xstdlib::octal   ||
							iterI->second == xstdlib::binary  ||
							iterI->second == xstdlib::floating_point)
						{
							iRet--;
							vIncorrect_Type_Options.push_back(iterI->first);
						}
						break;
					case xstdlib::empty:
						if (iterI->second != xstdlib::empty)
						{
							iRet--;
							vIncorrect_Type_Options.push_back(iterI->first);
						}
						break;
					}
				}
			}
		}
	}
	return iRet;
}

bool command_line_parse::Key_Exists(const std::string & i_sOption)
{
	return (mapTypes.count(i_sOption) == 1);
}

double command_line_parse::Get_Key_Value_Double(const std::string & i_sOption)
{
	double dRet = nan("");
	if (mapTypes.count(i_sOption) == 1 && mapTypes[i_sOption] == xstdlib::floating_point || mapTypes[i_sOption] == xstdlib::integer)
		dRet = std::stod(mapValues[i_sOption]);
	return dRet;
}

std::string command_line_parse::Get_Key_Value_String(const std::string & i_sOption)
{
	std::string sRet;
	if (mapTypes.count(i_sOption) == 1)
		sRet = mapValues[i_sOption];
	return sRet;
}

int command_line_parse::Get_Key_Value_Int(const std::string & i_sOption)
{
	int iRet = 0;
	if (mapTypes.count(i_sOption) == 1 && mapTypes[i_sOption] == xstdlib::integer)
		iRet = std::stoi(mapValues[i_sOption]);
	return iRet;
}
unsigned int command_line_parse::Get_Key_Value_UInt(const std::string & i_sOption)
{
	unsigned int iRet = 0;
	if (mapTypes.count(i_sOption) == 1 && mapTypes[i_sOption] == xstdlib::integer)
		iRet = std::stoi(mapValues[i_sOption]);
	return iRet;
}

bool command_line_parse::Get_Key_Value_Bool(const std::string & i_sOption)
{
	bool bRet = false;
	if (mapTypes.count(i_sOption) == 1 && mapTypes[i_sOption] == xstdlib::logical)
	{
		std::string sValue = mapValues[i_sOption];
		bRet = (sValue == "yes" || sValue == "true" || sValue == ".true." || sValue == "y" || sValue == "t" || 
				sValue == "Yes" || sValue == "True" || sValue == "Y" || sValue == "T" || 
				sValue == "YES" || sValue == "TRUE");
	}
	return bRet;
}

std::vector<std::string> command_line_parse::Get_All_Keys(void)
{
	std::vector<std::string> vsRet;
	for (auto iterI = mapValues.begin(); iterI != mapValues.end(); iterI++)
	{
		vsRet.push_back(iterI->first);
	}
	return vsRet;

}

void command_line_parse::Set_Key_Value(const std::string & i_sKey, const std::string & i_sValue)
{
	if (!i_sKey.empty())
	{
		mapValues[i_sKey] = i_sValue;
		mapTypes[i_sKey] = xstdlib::identify_datatype(i_sValue);

	}
}
