#include<cstdio>
#include<cstdlib>
#include <string>
#include <fstream>
#include <sstream>
#include <utility>
#include <vector>
#include <iostream>

void Usage(std::string szCommand)
{
	std::cout << "Usage: " << szCommand << " [-n X] <file>" << std::endl;
	std::cout << "Reprocesses a text file to make it fit into a certain width." << std::endl;
	std::cout << "Options:" << std::endl << "-n : number of columns that output must fit into; default is 80." << std::endl;
	std::cout << "Output: reprocessed text file is placed in file 'out.txt'" << std::endl;
}

int main(int i_iArg_Count, const char * i_lpszArg_Values[])
{
	if (i_iArg_Count > 1)
	{
		std::string szFilename;
		unsigned int uiNum_Columns = 80;
		for (int iI = 1; iI < i_iArg_Count; iI++)
		{
			std::string szCommand = i_lpszArg_Values[iI];
			if (szCommand == "--help" || szCommand == "?" || szCommand == "help" || szCommand == "HELP")
			{
				Usage(i_lpszArg_Values[0]);
			}
			else if (szCommand == "-n")
			{
				iI++;
				if (iI < i_iArg_Count)
				{
					szCommand = i_lpszArg_Values[iI];
					uiNum_Columns = std::stoi(szCommand);
				}
			}
			else
			{
				szFilename = szCommand;
			}
		} 
		if (!szFilename.empty())
		{
			std::ifstream ifsFile;
			ifsFile.open(szFilename);
			unsigned int uiLast_Nest = 0;
			std::vector <std::pair<unsigned int, std::string> > vpuisData;
			if (ifsFile.is_open())
			{
				std::string szLine;
				while (std::getline(ifsFile,szLine))
				{
					if (!szLine.empty())
					{
						if (szLine[0] == 10 || szLine[0]  == 13)
						{
							vpuisData.push_back(std::pair<unsigned int, std::string>(-1, "") );
						}
						else
						{
							std::string szLineFinal;
							//line is part of reply text; nest it
							unsigned int uiNest = 0;
							bool bBegin_Copy = false;
							for (std::string::iterator iterI = szLine.begin(); iterI != szLine.end(); iterI++)
							{
								if (bBegin_Copy)
								{
									if (*iterI == '\t')
										szLineFinal.push_back(' ');
									else  if (*iterI != 10 && *iterI != 13)
										szLineFinal.push_back(*iterI);
								}
								else if (*iterI == '>')
									uiNest++;
								else if (*iterI != ' ' && *iterI != '\t')
								{
									bBegin_Copy = true;
									if (*iterI == '\t')
										szLineFinal.push_back(' ');
									else  if (*iterI != 10 && *iterI != 13)
										szLineFinal.push_back(*iterI);
								}
							}
							std::istringstream issLine(szLineFinal);
							std::string szToken;
							while (std::getline(issLine, szToken, ' '))
							{
								vpuisData.push_back(std::pair<unsigned int, std::string>(uiNest, szToken) );
							}
						}
					}
					else // if line is empty
						vpuisData.push_back(std::pair<unsigned int, std::string>(-1, "") );
				} // while
				ifsFile.close();
				std::ofstream ofsOut;
				ofsOut.open("out.txt");
				if (ofsOut.is_open())
				{
					unsigned int uiLine_Count = 0;
					unsigned int uiNest = 0;
					for (std::vector<std::pair<unsigned int, std::string> >::iterator iterI = vpuisData.begin(); iterI != vpuisData.end(); iterI++)
					{
						if (iterI->first == -1)
						{
							uiLine_Count = 0;
							ofsOut << std::endl;
						}
						else if (iterI->first != uiNest || (uiLine_Count + iterI->second.size()) >= uiNum_Columns)
						{
							uiLine_Count = 0;
							ofsOut << std::endl;
							uiNest = iterI->first;
							for (unsigned int uiI = 0; uiI < uiNest; uiI++)
							{
								ofsOut << "> ";
								uiLine_Count += 2;
							}
						}
						if (!iterI->second.empty())
						{
							ofsOut << iterI->second << " ";
							uiLine_Count += iterI->second.size() + 1;
						}
					}
					ofsOut.close();
				}
			} // if filename not empty
			
		}
	}
	else
		Usage(i_lpszArg_Values[0]);


	return 0;
}
