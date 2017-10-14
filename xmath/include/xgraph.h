#pragma once
#include <FTGL/ftgl.h>
#include <XStdLib.h>
#include <float.h>

enum MARKER_TYPES	{FILL_SQUARE,EMPTY_SQUARE,FILL_TRIANGLE,EMPTY_TRIANGLE,FILL_CIRCLE,EMPTY_CIRCLE};
enum	GRAPH_TYPE	{XG_SOLID_LINE,XG_DASH_LINE,XG_MARKER,XG_SOLID_CONTOUR,XG_SURFACE};
enum	SURFACE_COLOR_SCHEME {XG_SFCCLR_BW,XG_SFCCLR_REDS,XG_SFCCLR_BLUES,XG_SFCCLR_GREENS,XG_SFCCLR_BBDY,XG_SFCCLR_INVBW,XG_SFCCLR_INVREDS,XG_SFCCLR_INVBLUES,XG_SFCCLR_INVGREENS,XG_SFCCLR_INVBBDY,XG_SFCCLR_CUSTOM};
class	ERRORBAR
{
public:
	double	m_dUpper;
	double	m_dLower;
	ERRORBAR(void) { m_dUpper = m_dLower = 0.0;}
};

class COORD3
{
public:
	double	m_dX;
	double	m_dY;
	double	m_dZ;

	COORD3 operator -(const COORD3 & i_cRHO) const
	{
		COORD3 cRet;
		cRet.m_dX = m_dX - i_cRHO.m_dX;
		cRet.m_dY = m_dY - i_cRHO.m_dY;
		cRet.m_dZ = m_dZ - i_cRHO.m_dZ;
		return cRet;
	}
};
class XGRAPHDATA
{
public:
	unsigned int	m_uiNum_Datapoints;
	unsigned int	m_uiAllocated_Size;
	COORD3 *		m_cData;
	ERRORBAR *		m_cErrorbars;
	double			m_dAxis_Ranges[3][2];
	double			m_dMin_Max_Values[3][2];
	double			m_dMin_Max_Values_Error[2];

	XGRAPHDATA(void)
	{
		m_uiNum_Datapoints = 0;
		m_uiAllocated_Size = 0;
		m_cData = NULL;
		m_cErrorbars = NULL;
		Reset_Axes();
		Reset_Min_Max();
	}
	void	Reset_Axes(void)
	{
		m_dAxis_Ranges[0][0] = 0.0;
		m_dAxis_Ranges[0][1] = 1.0;

		m_dAxis_Ranges[1][0] = 0.0;
		m_dAxis_Ranges[1][1] = 1.0;

		m_dAxis_Ranges[2][0] = 0.0;
		m_dAxis_Ranges[2][1] = 1.0;
	}
	void	Reset_Min_Max(void)
	{
		m_dMin_Max_Values[0][0] = DBL_MAX;
		m_dMin_Max_Values[1][0] = DBL_MAX;
		m_dMin_Max_Values[2][0] = DBL_MAX;

		m_dMin_Max_Values[0][1] = -DBL_MAX;
		m_dMin_Max_Values[1][1] = -DBL_MAX;
		m_dMin_Max_Values[2][1] = -DBL_MAX;

		m_dMin_Max_Values_Error[0] = DBL_MAX;

		m_dMin_Max_Values_Error[1] = -DBL_MAX;
	}
	void	Allocate(unsigned int i_uiSize, bool i_bAllocate_Errorbars = false, bool i_bReset_Axes = true, bool i_bReset_MinMax = true)
	{
		if (i_uiSize > m_uiAllocated_Size)
		{
			if (m_cData)
				delete [] m_cData;
			if (m_cErrorbars)
				delete [] m_cErrorbars;
			m_cData = new COORD3[i_uiSize];
			if (m_cData)
			{
				if (i_bAllocate_Errorbars)
					m_cErrorbars = new ERRORBAR[i_uiSize];
				m_uiAllocated_Size = i_uiSize;
			}
			else
				m_uiAllocated_Size = 0;
		}
		if (m_uiAllocated_Size > 0)
		{
			m_uiNum_Datapoints = i_uiSize;
			if (i_bReset_Axes)
				Reset_Axes();
			if (i_bReset_MinMax)
				Reset_Min_Max();
		}
	}
	void IncY(unsigned int i_uiIndex, const double &i_dIncrement = 1.0)
	{
		if (i_uiIndex < m_uiNum_Datapoints)
		{
			m_cData[i_uiIndex].m_dY += i_dIncrement;
			if (m_dMin_Max_Values[1][0] > m_cData[i_uiIndex].m_dY)
				m_dMin_Max_Values[1][0] = m_cData[i_uiIndex].m_dY;
			if (m_dMin_Max_Values[1][1] < m_cData[i_uiIndex].m_dY)
				m_dMin_Max_Values[1][1] = m_cData[i_uiIndex].m_dY;
		}
	}
	void IncZ(unsigned int i_uiIndex, const double &i_dIncrement = 1.0)
	{
		if (i_uiIndex < m_uiNum_Datapoints)
		{
			m_cData[i_uiIndex].m_dZ += i_dIncrement;
			if (m_dMin_Max_Values[2][0] > m_cData[i_uiIndex].m_dZ)
				m_dMin_Max_Values[2][0] = m_cData[i_uiIndex].m_dZ;
			if (m_dMin_Max_Values[2][1] < m_cData[i_uiIndex].m_dZ)
				m_dMin_Max_Values[2][1] = m_cData[i_uiIndex].m_dZ;
		}
	}
	void Set(unsigned int i_uiIndex, const double & i_dX, const double &i_dY, const double & i_dZ, const double &i_dErr_Upper = 0.0, const double & i_dErr_Lower = 0.0)
	{
		if (i_uiIndex < m_uiNum_Datapoints)
		{
			m_cData[i_uiIndex].m_dX = i_dX;
			m_cData[i_uiIndex].m_dY = i_dY;
			m_cData[i_uiIndex].m_dZ = i_dZ;
			if (m_dMin_Max_Values[0][0] > i_dX)
				m_dMin_Max_Values[0][0] = i_dX;
			if (m_dMin_Max_Values[1][0] > i_dY)
				m_dMin_Max_Values[1][0] = i_dY;
			if (m_dMin_Max_Values[2][0] > i_dZ)
				m_dMin_Max_Values[2][0] = i_dZ;

			if (m_dMin_Max_Values[0][1] < i_dX)
				m_dMin_Max_Values[0][1] = i_dX;
			if (m_dMin_Max_Values[1][1] < i_dY)
				m_dMin_Max_Values[1][1] = i_dY;
			if (m_dMin_Max_Values[2][1] < i_dZ)
				m_dMin_Max_Values[2][1] = i_dZ;
			if (m_cErrorbars)
			{
				m_cErrorbars[i_uiIndex].m_dUpper = i_dErr_Upper;
				m_cErrorbars[i_uiIndex].m_dLower = i_dErr_Lower;

				if (m_dMin_Max_Values_Error[0] > (i_dY - i_dErr_Lower))
					m_dMin_Max_Values_Error[0] = i_dY - i_dErr_Lower;

				if (m_dMin_Max_Values_Error[1] < (i_dY + i_dErr_Upper))
					m_dMin_Max_Values_Error[1] = i_dY + i_dErr_Upper;
			}
			else
			{
				m_dMin_Max_Values_Error[0] = m_dMin_Max_Values[1][0];
				m_dMin_Max_Values_Error[1] = m_dMin_Max_Values[1][1];
			}
		}
	}
	enum AXISLIMIT {LOWER=0,UPPER=1};
	enum AXISTOLIMIT {XAXIS=0,YAXIS=1,ZAXIS=2};
	enum LIMITTYPE {NORMAL=0,INVERTED=1,CUSTOM=2,SYMMETRIC=3,NORMAL_W_ERROR=0x020,INVERTED_W_ERROR=0x021,SYMMETRIC_W_ERROR=0x023};
	void	SetAxisLimit(AXISTOLIMIT i_eAxis_To_Limit, AXISLIMIT i_eLimit, LIMITTYPE i_eLimit_Type = NORMAL, const double &i_dCustom_Value = 0.0)
	{

		double	dValue = i_dCustom_Value;
		unsigned int uiMax_Max_Idx = (i_eLimit + i_eLimit_Type) % 2;
		switch (i_eLimit_Type)
		{
		case NORMAL_W_ERROR:
		case INVERTED_W_ERROR:
			dValue =  m_dMin_Max_Values_Error[uiMax_Max_Idx];
			break;
		case NORMAL:
		case INVERTED:
			dValue =  m_dMin_Max_Values[i_eAxis_To_Limit][uiMax_Max_Idx];
			break;
		case SYMMETRIC:
			{
				double	dRef = __max(fabs(m_dMin_Max_Values[i_eAxis_To_Limit][0]),fabs(m_dMin_Max_Values[i_eAxis_To_Limit][1]));
				if (i_eLimit == LOWER)
					dValue = -dRef;
				else
					dValue = dRef;
			}
		case SYMMETRIC_W_ERROR:
			{
				double	dRef = __max(fabs(m_dMin_Max_Values_Error[0]),fabs(m_dMin_Max_Values_Error[1]));
				if (i_eLimit == LOWER)
					dValue = -dRef;
				else
					dValue = dRef;
			}
		}
		m_dAxis_Ranges[i_eAxis_To_Limit][i_eLimit] = dValue;
	}

};

class AXES
{
public:
	bool	m_bLog_Axis[3];
	double	m_dAxis_Ranges[3][2];
	double	m_dSlopes[3];
	double	m_dDelta_Ranges[3];
	double	m_dSlopes_Norm[3];


	AXES(void)
	{
		for (unsigned int uiIndex = 0; uiIndex < 3; uiIndex++)
		{
			m_bLog_Axis[uiIndex] = false;
			m_dAxis_Ranges[uiIndex][0] = 0.0;
			m_dAxis_Ranges[uiIndex][1] = 1.0;
			m_dSlopes[uiIndex] = 1024.0;
			m_dSlopes_Norm[uiIndex] = 1.0;
			m_dDelta_Ranges[uiIndex] = 1.0;
		}
	}
	void InitSlopes(void)
	{
		for (unsigned int uiIndex = 0; uiIndex < 3; uiIndex++)
		{
			if (m_bLog_Axis[uiIndex])
			{
				if (m_dAxis_Ranges[uiIndex][0] <= 0.0 || m_dAxis_Ranges[uiIndex][1] <= 0.0)
					m_bLog_Axis[uiIndex] = false;
				else
				{
					m_dAxis_Ranges[uiIndex][0] = log10(m_dAxis_Ranges[uiIndex][0]);
					m_dAxis_Ranges[uiIndex][1] = log10(m_dAxis_Ranges[uiIndex][1]);
				}
			}
		}

		for (unsigned int uiIndex = 0; uiIndex < 3; uiIndex++)
		{
			m_dDelta_Ranges[uiIndex] = (m_dAxis_Ranges[uiIndex][1] - m_dAxis_Ranges[uiIndex][0]);
			m_dSlopes_Norm[uiIndex] = 1.0 / m_dDelta_Ranges[uiIndex];
			m_dSlopes[uiIndex] = 1024.0 * m_dSlopes_Norm[uiIndex];
		}
	}
	void TrxCoords(COORD3 &io_cCoord) const
	{
		COORD3	cCoordLcl = io_cCoord;
		if (m_bLog_Axis[0])
			cCoordLcl.m_dX = log10(cCoordLcl.m_dX);
		if (m_bLog_Axis[1])
			cCoordLcl.m_dY = log10(cCoordLcl.m_dY);
		if (m_bLog_Axis[2])
			cCoordLcl.m_dZ = log10(cCoordLcl.m_dZ);

		io_cCoord.m_dX = (cCoordLcl.m_dX - m_dAxis_Ranges[0][0]) * m_dSlopes[0];
		io_cCoord.m_dY = (cCoordLcl.m_dY - m_dAxis_Ranges[1][0]) * m_dSlopes[1];
		io_cCoord.m_dZ = (cCoordLcl.m_dZ - m_dAxis_Ranges[2][0]) * m_dSlopes[2];
	};
	void Vertex(const COORD3 &io_cCoord) const
	{
		COORD3 cCoord = io_cCoord;
		TrxCoords(cCoord);
		glVertex3d(cCoord.m_dX,cCoord.m_dY,cCoord.m_dZ);
	}

};
class COLOR
{
public:
	double	m_dRed;
	double	m_dGreen;
	double	m_dBlue;
	double	m_dAlpha;

	void Use(const double & i_dScalar = 1.0) const
	{
		glColor4d(m_dRed * i_dScalar,m_dGreen * i_dScalar,m_dBlue * i_dScalar,m_dAlpha);
	}

};
typedef COLOR xgfnCustomColor (const double &i_dColor);


extern	short		g_sTheta;
extern	short		g_sPhi;
extern	short		g_sScale;
extern	double		g_dPi;
extern	double		g_d1_Log10;
extern	int			g_iOffsetX;
extern	int			g_iOffsetY;

extern	COLOR	XG_BLACK;
extern	COLOR	XG_LT_GRAY;
extern	COLOR	XG_MD_GRAY;
extern	COLOR	XG_DK_GRAY;
extern	COLOR	XG_MAGENTA;
extern	COLOR	XG_MAGENTA_W75;
extern	COLOR	XG_MAGENTA_W50;
extern	COLOR	XG_MAGENTA_W25;
extern	COLOR	XG_YELLOW;
extern	COLOR	XG_YELLOW_W75;
extern	COLOR	XG_YELLOW_W50;
extern	COLOR	XG_YELLOW_W25;
extern	COLOR	XG_CYAN;
extern	COLOR	XG_CYAN_W75;
extern	COLOR	XG_CYAN_W50;
extern	COLOR	XG_CYAN_W25;
extern	COLOR	XG_DK_BLUE;
extern	COLOR	XG_DK_BLUE_W75;
extern	COLOR	XG_DK_BLUE_W50;
extern	COLOR	XG_DK_BLUE_W25;
extern	COLOR	XG_BLUE;
extern	COLOR	XG_BLUE_W75;
extern	COLOR	XG_BLUE_W50;
extern	COLOR	XG_BLUE_W25;
extern	COLOR	XG_WHITE;
extern	COLOR	XG_LT_GRAY_TRX;
extern	COLOR	XG_TRANSPARENT;
extern	COLOR	XG_RED;
extern	COLOR	XG_RED_W75;
extern	COLOR	XG_RED_W50;
extern	COLOR	XG_RED_W25;
extern	COLOR	XG_GREEN;
extern	COLOR	XG_GREEN_W75;
extern	COLOR	XG_GREEN_W50;
extern	COLOR	XG_GREEN_W25;
extern	COLOR	XG_LT_GREEN;
extern	COLOR	XG_LT_GREEN_W75;
extern	COLOR	XG_LT_GREEN_W50;
extern	COLOR	XG_LT_GREEN_W25;



class CONTROL_ITEM
{
public:
	enum	FONTTYPE {NORMAL,GREEK};
	CONTROL_ITEM *	m_lpLeafs[2];
	XSTRING			m_nameCode;
	FONTTYPE		m_eFontType;
	XSTRING			m_nameDraw;
	CONTROL_ITEM(void){m_lpLeafs[0] = m_lpLeafs[1] = NULL;m_eFontType = NORMAL;}

	void AddLeaf(const XSTRING &i_cName, const XSTRING &i_cDrawName, FONTTYPE i_eFont)
	{
		int iTest = m_nameCode.Compare(i_cName);
		unsigned int uiIndex = 2;
		if (iTest < 0)
			uiIndex = 0;
		else if (iTest > 0)
			uiIndex = 1;

		if (uiIndex < 2)
		{
			if (m_lpLeafs[uiIndex] == NULL)
			{
				m_lpLeafs[uiIndex] = new CONTROL_ITEM;
				m_lpLeafs[uiIndex]->m_eFontType = i_eFont;
				m_lpLeafs[uiIndex]->m_nameCode = i_cName;
				m_lpLeafs[uiIndex]->m_nameDraw = i_cDrawName;
			}
			else
				m_lpLeafs[uiIndex]->AddLeaf(i_cName, i_cDrawName, i_eFont);
		}
	}
	CONTROL_ITEM * Find(const XSTRING &i_cName)
	{
		CONTROL_ITEM * lpRet = NULL;
		int iTest = m_nameCode.Compare(i_cName);
		if (iTest == 0)
			lpRet = this;
		else if (iTest < 0 && m_lpLeafs[0])
			lpRet = m_lpLeafs[0]->Find(i_cName);
		else if (iTest > 0 && m_lpLeafs[1])
			lpRet = m_lpLeafs[1]->Find(i_cName);
		return lpRet;
	}
};
	
class CONTROL_ITEM_TREE
{
private:
	CONTROL_ITEM * m_lpControl_Set_Tree_Stem;
public:
	CONTROL_ITEM_TREE(void){m_lpControl_Set_Tree_Stem = NULL;}
	CONTROL_ITEM * Find(const XSTRING &i_cName)
	{
		CONTROL_ITEM * lpRet = NULL;
		if (m_lpControl_Set_Tree_Stem)
			lpRet = m_lpControl_Set_Tree_Stem->Find(i_cName);
		return lpRet;
	}
	void Add(const XSTRING &i_cName, const XSTRING &i_cDrawName, CONTROL_ITEM::FONTTYPE i_eFont)
	{
		if (m_lpControl_Set_Tree_Stem)
			m_lpControl_Set_Tree_Stem->AddLeaf(i_cName, i_cDrawName,i_eFont);
		else
		{
			m_lpControl_Set_Tree_Stem = new CONTROL_ITEM;
			m_lpControl_Set_Tree_Stem->m_eFontType = i_eFont;
			m_lpControl_Set_Tree_Stem->m_nameCode = i_cName;
			m_lpControl_Set_Tree_Stem->m_nameDraw = i_cDrawName;
		}
	}
};


extern CONTROL_ITEM_TREE	g_cControl_Item_Tree;
extern const char *	Find_Control_End_Char(const char *i_lpszName);
extern const char *	Find_Control_Char(const char *i_lpszName);

class STRING_TOKEN_LIST_NODE
{
public:
	enum	TOKEN_TYPE	{ST_UNDEF,ST_NAME,ST_SUPER,ST_SUB,ST_NESTSTART,ST_NESTEND};
	TOKEN_TYPE					m_eToken_Type;
	XSTRING						m_name;
	CONTROL_ITEM::FONTTYPE		m_eFont;
	STRING_TOKEN_LIST_NODE*		m_lpNext;
	STRING_TOKEN_LIST_NODE *	m_lpPrev;

	STRING_TOKEN_LIST_NODE(void)
	{
		m_eToken_Type= ST_UNDEF;
		m_lpNext = m_lpPrev = NULL;
	}

};
class STRING_TOKEN_LIST
{
private:
	STRING_TOKEN_LIST_NODE *	RenderNest(const double &i_dOffset, unsigned int i_uiFont_Size,STRING_TOKEN_LIST_NODE * lpCurr);
	STRING_TOKEN_LIST_NODE *	GetBoundBoxNest(const double &i_dOffset, double &io_dX_Offset, unsigned int i_uiFont_Size,STRING_TOKEN_LIST_NODE * lpCurr,COORD3 &io_cTLN, COORD3 &io_cBRF);
public:
	STRING_TOKEN_LIST_NODE * m_lpHead;
	STRING_TOKEN_LIST_NODE * m_lpTail;
	STRING_TOKEN_LIST(void){m_lpHead = m_lpTail = NULL;}
	void ClearList(void)
	{
		while (m_lpHead)
		{
			STRING_TOKEN_LIST_NODE * lpCurr = m_lpHead;
			m_lpHead = lpCurr->m_lpNext;
			delete lpCurr;
		}
		m_lpHead = m_lpTail = NULL;
	}
	void Add(STRING_TOKEN_LIST_NODE::TOKEN_TYPE i_eToken, XSTRING * lpName = NULL, CONTROL_ITEM::FONTTYPE i_eFont_Type = CONTROL_ITEM::NORMAL)
	{
		STRING_TOKEN_LIST_NODE * lpNewNode = new STRING_TOKEN_LIST_NODE;
		lpNewNode->m_eToken_Type = i_eToken;
		lpNewNode->m_eFont = i_eFont_Type;
		if (lpName)
			lpNewNode->m_name = lpName[0];

		if (!m_lpHead)
			m_lpHead = lpNewNode;
		if (m_lpTail)
			m_lpTail->m_lpNext = lpNewNode;
		lpNewNode->m_lpPrev = m_lpTail;
		m_lpTail = lpNewNode;
	}
	void Tokenize(const char * i_lpszName)
	{
		STRING_TOKEN_LIST_NODE::TOKEN_TYPE	eLast_Token = STRING_TOKEN_LIST_NODE::ST_UNDEF;
		ClearList();
		while (i_lpszName[0] != 0)
		{
			switch (i_lpszName[0])
			{
			case '\\':
				{
					XSTRING cRenderName;
					i_lpszName++;
					const char * lpNext = Find_Control_End_Char(i_lpszName);
					unsigned int uiLen = lpNext - i_lpszName;
					cRenderName.Copy(i_lpszName,uiLen);
					CONTROL_ITEM * lpItem = g_cControl_Item_Tree.Find(cRenderName);
					if (lpItem)
					{
						cRenderName = lpItem->m_nameDraw;
						Add(STRING_TOKEN_LIST_NODE::ST_NAME,&cRenderName,lpItem->m_eFontType);
						i_lpszName = lpNext;
					}
					else
					{
						cRenderName = "\\";
						Add(STRING_TOKEN_LIST_NODE::ST_NAME,&cRenderName,CONTROL_ITEM::NORMAL);
					}
					eLast_Token = STRING_TOKEN_LIST_NODE::ST_NAME;
				}
				break;
			case '_':
				i_lpszName++;
				Add(STRING_TOKEN_LIST_NODE::ST_SUB);
				eLast_Token = STRING_TOKEN_LIST_NODE::ST_SUB;
				break;
			case '^':
				i_lpszName++;
				Add(STRING_TOKEN_LIST_NODE::ST_SUPER);
				eLast_Token = STRING_TOKEN_LIST_NODE::ST_SUPER;
				break;
			case '{':
				i_lpszName++;
				Add(STRING_TOKEN_LIST_NODE::ST_NESTSTART);
				eLast_Token = STRING_TOKEN_LIST_NODE::ST_NESTSTART;
				break;
			case '}':
				i_lpszName++;
				Add(STRING_TOKEN_LIST_NODE::ST_NESTEND);
				eLast_Token = STRING_TOKEN_LIST_NODE::ST_NESTEND;
				break;
			default:
				{
					XSTRING cRenderName;
					const char * lpNext = Find_Control_Char(i_lpszName);
					if (eLast_Token == STRING_TOKEN_LIST_NODE::ST_SUB || eLast_Token == STRING_TOKEN_LIST_NODE::ST_SUPER)
					{
						lpNext = i_lpszName + 1;
					}
					unsigned int uiLen = lpNext - i_lpszName;
					cRenderName.Copy(i_lpszName,uiLen);
					Add(STRING_TOKEN_LIST_NODE::ST_NAME,&cRenderName,CONTROL_ITEM::NORMAL);
					i_lpszName = lpNext;
				}
				break;
			}
		}
	}

	void	Render(unsigned int i_uiFont_Size);
	void	GetBoundBox(unsigned int i_uiFont_Size, COORD3 &o_cTLN, COORD3 &o_cBRF);
};


class GRAPH_DATA_LIST_NODE
{
public:
	COLOR						m_cColor;
	GRAPH_TYPE					m_eType;
	unsigned int				m_uiStyle;
	double						m_dSize;
	STRING_TOKEN_LIST			m_stName;
	unsigned int				m_uiNameSize;
	const COORD3	*			m_lpCoords;
	bool						m_bShow_Errors[3];
	const ERRORBAR *			m_lpcErrors[3];
	unsigned int				m_uiNum_Points; // also doubles as num points x for surface
	unsigned int				m_uiNum_Points_Y; // ignored for curves
	xgfnCustomColor *			m_fnColor;
	GRAPH_DATA_LIST_NODE *		m_lpNext;
	GRAPH_DATA_LIST_NODE *		m_lpPrev;
	GRAPH_DATA_LIST_NODE(void) : m_stName()
	{
		m_cColor = XG_BLACK;
		m_eType = XG_SOLID_LINE;
		m_uiStyle = 0;
		m_lpCoords = NULL;
		m_lpNext = m_lpPrev = NULL;
		m_dSize = 1.0;
		m_uiNum_Points_Y = m_uiNum_Points = 0;
		m_bShow_Errors[0] = m_bShow_Errors[1] = m_bShow_Errors[2] = false;
		m_lpcErrors[0] = m_lpcErrors[1] = m_lpcErrors[2] = NULL;
		m_uiNameSize = 0;
		m_fnColor = NULL;
	}
};

class GRAPH_DATA_LIST
{
public:
	GRAPH_DATA_LIST_NODE *		m_lpList_Head;
	GRAPH_DATA_LIST_NODE *		m_lpList_Tail;
	bool						m_bContains_Surfaces;

	GRAPH_DATA_LIST(void)
	{
		m_lpList_Head = m_lpList_Tail = NULL;
		m_bContains_Surfaces = false;
	}
	void AddHead(GRAPH_DATA_LIST_NODE * i_lpList_Node)
	{
		if (i_lpList_Node)
		{
			if (m_lpList_Tail)
				m_lpList_Head->m_lpPrev = i_lpList_Node;
			else
			{
				m_lpList_Head = m_lpList_Tail = i_lpList_Node;
			}
			i_lpList_Node->m_lpNext = m_lpList_Head;
			m_lpList_Head = i_lpList_Node;
			m_bContains_Surfaces |= (i_lpList_Node->m_eType == XG_SURFACE);
		}
	}
	void AddTail(GRAPH_DATA_LIST_NODE * i_lpList_Node)
	{
		if (i_lpList_Node)
		{
			if (m_lpList_Tail)
				m_lpList_Tail->m_lpNext = i_lpList_Node;
			else
			{
				m_lpList_Head = m_lpList_Tail = i_lpList_Node;
			}
			i_lpList_Node->m_lpPrev = m_lpList_Tail;
			m_lpList_Tail = i_lpList_Node;
			m_bContains_Surfaces |= (i_lpList_Node->m_eType == XG_SURFACE);
		}
	}
	void ClearList(void)
	{
		GRAPH_DATA_LIST_NODE * lpCurr = m_lpList_Head;
		while (lpCurr)
		{
			m_lpList_Head = lpCurr->m_lpNext;
			delete lpCurr;
			lpCurr = m_lpList_Head;
		}
		m_lpList_Head = m_lpList_Tail = NULL;
		m_bContains_Surfaces = false;
	}
};




class AXIS_DATA
{
public:
	COLOR			m_cColor;
	COLOR			m_cGridline_Color;
	STRING_TOKEN_LIST	m_cName_String;
	unsigned int	m_uiNum_Ticks;
	bool			m_bLog_Scale;
	unsigned int	m_uiNum_Labels;
	XSTRING			m_nameLabel_Format;
};

class LEGEND_DATA
{
public:
	bool		m_bShow_Legend;
	double		m_dBorder_Width;
	COLOR		m_cBorder;
	COLOR		m_cFill;
	double		m_dCoords[2];
	unsigned int		m_uiFont_Size;

	LEGEND_DATA(void)
	{
		m_bShow_Legend = false;
		m_dBorder_Width = 1.0;
		m_cBorder = XG_BLACK;
		m_cFill = XG_LT_GRAY_TRX;
		m_dCoords[0] = 700.0;
		m_dCoords[1] = 700.0;
		m_uiFont_Size = 24;
	}
};

extern void Initialize(void);
extern void Draw(void);


void	SetBackgroundColor(const COLOR &i_cColor);
void	AddGraph(const COLOR &i_cColor,  GRAPH_TYPE i_eType, unsigned int i_uiStyle, const double &i_dSize, COORD3 * i_lpCoords, unsigned int i_uiNum_Points,const char * i_lpszName, bool i_bShow_Errors[3], const ERRORBAR * i_lpX_Errors = NULL, const ERRORBAR * i_lpY_Errors = NULL, const ERRORBAR * i_lpZ_Errors = NULL);
void	AddSurface(SURFACE_COLOR_SCHEME i_eColor_Scheme, COORD3 * i_lpCoords, unsigned int i_uiNum_Points_X, unsigned int i_uiNum_Points_Y,const char * i_lpszName,xgfnCustomColor *i_fnColor = NULL);
void	SetLegend(const double &i_dX, const double &i_dY, const double & i_dBorder_Width, COLOR i_cFill_Color, COLOR i_cBorder_Color, unsigned int i_uiFont_Size = 24);
void	SetAxisInfo(unsigned int i_uiAxis, const COLOR &i_cColor, const COLOR &i_cGridline_Color, const char * i_lpszTitle, const char * i_lpszLabel_Format, unsigned int i_uiNum_Ticks, unsigned int i_uiNum_Labels, bool i_bLog_Scale, const double &i_dMin, const double &i_dMax);
void	RequestSave(const char * i_lpszFileName);
void	SetTitle(const char * i_lpszTitle, unsigned int i_uiFont_Size = 48, const COLOR &i_cColor = XG_BLACK);
void	ClearGraphs(void);