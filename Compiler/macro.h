#pragma once
//关键字
#define		BREAK		0
#define		CONTINUE	1
#define		ELSE		2
#define		FOR			3
#define		IF			4
#define		RETURN		5
#define		WHILE		6
#define		CHAR		7
#define		INT			8
//标识符
#define		ID			9
//运算符
#define		PLUS		10 // +
#define		MINUS		11 // -
#define		MULTI		12 // *
#define		RDIV		13 // /
#define		MOD			14 // %
#define		INC			15 // ++
#define		DEC			16 // --
#define		AND			17 // &&
#define		OR			18 // ||
#define		NOT			19 // !
//比较符
#define		EQ			20 // ==
#define		LT			21 // <
#define		GT			22 // >
#define		LE			23 // <=
#define		GE			24 // >=
#define		NE			25 // != <>
//括号
#define		LS_BRAC		26 // (
#define		RS_BRAC		27 // )
#define		LM_BRAC		28 // [
#define		RM_BRAC		29 // ]
#define		LL_BRAC		30 // {
#define		RL_BRAC		31 // }
//其他符号
#define		ASSIGN		32 // =
#define		SEMIC		33 // ;
#define		Q_MARK		34 // '
#define		DQ_MARK		35 // "
#define		B_SLANT		36 /* \ */
#define		COMMA		37 // ,
/*#define		POUND		38 // #
#define		ADDRESS		39 // &
#define		FSTOP		40 // .*/
//常量
#define		B4			38 // int类型的常量
#define		B1			39 // char类型的常量
#define		STR			40 // char[]类型的常量
//#define		HEADER		41 // 头文件的文件名
//关键字补充
#define		MAIN		41
#define		PRINTF		42
//终结符数量
#define		MACRONUM	43
//三地址码中的符号
#define		JMP			50
#define		IFISGO		51
#define		GOTO		52
#define		PRT			53
#define		INASS		54
//临时变量的起始地址
#define		TEMPOFFSET	1000
//action表中的ACC
#define		ACC			10000
