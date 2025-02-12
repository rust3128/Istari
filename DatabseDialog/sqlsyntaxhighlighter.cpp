#include "sqlsyntaxhighlighter.h"

SqlSyntaxHighlighter::SqlSyntaxHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    // Формат для SQL ключових слів
    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    const QString keywordPatterns[] = {
        "\\bABS\\b", "\\bACCENT\\b", "\\bACOS\\b", "\\bADD\\b", "\\bADMIN\\b", "\\bAFTER\\b",
        "\\bALL\\b", "\\bALTER\\b", "\\bAND\\b", "\\bANY\\b", "\\bAS\\b", "\\bASC\\b", "\\bASCII_CHAR\\b",
        "\\bASCII_VAL\\b", "\\bASIN\\b", "\\bAT\\b", "\\bATAN\\b", "\\bATAN2\\b", "\\bAUTO\\b",
        "\\bAUTODDL\\b", "\\bAVG\\b", "\\bBACKUP\\b", "\\bBEFORE\\b", "\\bBEGIN\\b", "\\bBETWEEN\\b",
        "\\bBIGINT\\b", "\\bBIN_AND\\b", "\\bBIN_OR\\b", "\\bBIN_SHL\\b", "\\bBIN_SHR\\b", "\\bBIN_XOR\\b",
        "\\bBIT_LENGTH\\b", "\\bBLOB\\b", "\\bBLOBEDIT\\b", "\\bBOOLEAN\\b", "\\bBOTH\\b", "\\bBREAK\\b",
        "\\bBY\\b", "\\bCASCADE\\b", "\\bCASE\\b", "\\bCAST\\b", "\\bCEIL\\b", "\\bCEILING\\b",
        "\\bCHAR\\b", "\\bCHAR_LENGTH\\b", "\\bCHAR_TO_UUID\\b", "\\bCHARACTER\\b", "\\bCHARACTER_LENGTH\\b",
        "\\bCHECK\\b", "\\bCLOSE\\b", "\\bCOALESCE\\b", "\\bCOLLATE\\b", "\\bCOLLATION\\b", "\\bCOLUMN\\b",
        "\\bCOMMENT\\b", "\\bCOMMIT\\b", "\\bCOMMITTED\\b", "\\bCOMMON\\b", "\\bCOMPARE_DECFLOAT\\b", "\\bCOMPUTED\\b",
        "\\bCONDITIONAL\\b", "\\bCONNECT\\b", "\\bCONSTRAINT\\b", "\\bCONTAINING\\b", "\\bCOS\\b", "\\bCOSH\\b",
        "\\bCOUNT\\b", "\\bCREATE\\b", "\\bCROSS\\b", "\\bCSTRING\\b", "\\bCURRENT\\b", "\\bCURRENT_CONNECTION\\b",
        "\\bCURRENT_DATE\\b", "\\bCURRENT_ROLE\\b", "\\bCURRENT_TIME\\b", "\\bCURRENT_TIMESTAMP\\b", "\\bCURRENT_TRANSACTION\\b",
        "\\bCURRENT_USER\\b", "\\bCURSOR\\b", "\\bDATABASE\\b", "\\bDATE\\b", "\\bDATEADD\\b", "\\bDATEDIFF\\b",
        "\\bDAY\\b", "\\bDECFLOAT\\b", "\\bDECLARE\\b", "\\bDECODE\\b", "\\bDEFAULT\\b", "\\bDELETE\\b",
        "\\bDELETING\\b", "\\bDEPRECATED\\b", "\\bDESC\\b", "\\bDESCRIBE\\b", "\\bDESCRIPTOR\\b", "\\bDIFFERENCE\\b",
        "\\bDISCONNECT\\b", "\\bDISTINCT\\b", "\\bDO\\b", "\\bDOMAIN\\b", "\\bDOUBLE\\b", "\\bDROP\\b",
        "\\bE\\b", "\\bECHO\\b", "\\bEDIT\\b", "\\bELSE\\b", "\\bEND\\b", "\\bENTRY_POINT\\b",
        "\\bESCAPE\\b", "\\bEVENT\\b", "\\bEXCEPT\\b", "\\bEXCEPTION\\b", "\\bEXECUTE\\b", "\\bEXISTS\\b",
        "\\bEXIT\\b", "\\bEXP\\b", "\\bEXTERNAL\\b", "\\bEXTRACT\\b", "\\bFALSE\\b", "\\bFETCH\\b",
        "\\bFILE\\b", "\\bFILTER\\b", "\\bFIRST\\b", "\\bFIRSTNAME\\b", "\\bFIRST_DAY\\b", "\\bFIRST_VALUE\\b",
        "\\bFLOAT\\b", "\\bFLOOR\\b", "\\bFOR\\b", "\\bFOREIGN\\b", "\\bFREE_IT\\b", "\\bFROM\\b",
        "\\bFULL\\b", "\\bFUNCTION\\b", "\\bGDSCODE\\b", "\\bGENERATED\\b", "\\bGENERATOR\\b", "\\bGEN_ID\\b",
        "\\bGEN_UUID\\b", "\\bGLOBAL\\b", "\\bGRANT\\b", "\\bGROUP\\b", "\\bHASH\\b", "\\bHAVING\\b",
        "\\bHELP\\b", "\\bHOUR\\b", "\\bIF\\b", "\\bIGNORE\\b", "\\bIN\\b", "\\bINACTIVE\\b",
        "\\bINDEX\\b", "\\bINNER\\b", "\\bINPUT_TYPE\\b", "\\bINSENSITIVE\\b", "\\bINSERT\\b", "\\bINSERTING\\b",
        "\\bINT\\b", "\\bINTEGER\\b", "\\bINTO\\b", "\\bIS\\b", "\\bISOLATION\\b", "\\bJOIN\\b",
        "\\bKEY\\b", "\\bLCASE\\b", "\\bLEADING\\b", "\\bLEAVE\\b", "\\bLEFT\\b", "\\bLENGTH\\b",
        "\\bLEV\\b", "\\bLEVEL\\b", "\\bLIKE\\b", "\\bLIMI\\b", "\\bLIMIT\\b", "\\bLN\\b",
        "\\bLOCAL\\b", "\\bLOG\\b", "\\bLOG10\\b", "\\bLONG\\b", "\\bLOWER\\b", "\\bLPAD\\b",
        "\\bLTRIM\\b", "\\bMANUAL\\b", "\\bMAPPING\\b", "\\bMATCHES\\b", "\\bMAX\\b", "\\bMAXIMUM_SEGMENT\\b",
        "\\bMAXVALUE\\b", "\\bMERGE\\b", "\\bMIDDLENAME\\b", "\\bMIDDLENAME\\b", "\\bMIN\\b", "\\bMINUTE\\b",
        "\\bMINVALUE\\b", "\\bMOD\\b", "\\bMODULE_NAME\\b", "\\bMONTH\\b", "\\bNAME\\b", "\\bNAMES\\b",
        "\\bNATIONAL\\b", "\\bNATURAL\\b", "\\bNCHAR\\b", "\\bNO\\b", "\\bNOT\\b", "\\bNULL\\b",
        "\\bNULLIF\\b", "\\bNUMERIC\\b", "\\bOCTET_LENGTH\\b", "\\bOF\\b", "\\bOFFSET\\b", "\\bON\\b",
        "\\bONLY\\b", "\\bOPEN\\b", "\\bOPTION\\b", "\\bOR\\b", "\\bORDER\\b", "\\bOUTER\\b",
        "\\bOUTPUT_TYPE\\b", "\\bOVER\\b", "\\bOVERLAY\\b", "\\bPAD\\b", "\\bPAGE\\b", "\\bPAGE_SIZE\\b",
        "\\bPAGES\\b", "\\bPARAMETER\\b", "\\bPASSWORD\\b", "\\bPI\\b", "\\bPLACING\\b", "\\bPLAN\\b",
        "\\bPOSITION\\b", "\\bPOST_EVENT\\b", "\\bPOWER\\b", "\\bPRECISION\\b", "\\bPRIMARY\\b", "\\bPRIVILEGES\\b",
        "\\bPROCEDURE\\b", "\\bPROTECTED\\b", "\\bRAND\\b", "\\bRDB$DB_KEY\\b", "\\bRDB$GET_CONTEXT\\b",
        "\\bRDB$GET_TRANSACTION_ID\\b", "\\bRDB$RECORD_VERSION\\b", "\\bRDB$SET_CONTEXT\\b", "\\bREAD\\b",
        "\\bREAL\\b", "\\bRECORD_VERSION\\b", "\\bRECREATE\\b", "\\bRECURSIVE\\b", "\\bREFERENCES\\b",
        "\\bREGR_AVGX\\b", "\\bREGR_AVGY\\b", "\\bREGR_COUNT\\b", "\\bREGR_INTERCEPT\\b", "\\bREGR_R2\\b",
        "\\bREGR_SLOPE\\b", "\\bREGR_SXX\\b", "\\bREGR_SXY\\b", "\\bREGR_SYY\\b", "\\bRELEASE\\b",
        "\\bREPLACE\\b", "\\bREQUESTS\\b", "\\bRESERV\\b", "\\bRESERVED\\b", "\\bRESTART\\b", "\\bRESTRICT\\b",
        "\\bRETAIN\\b", "\\bRETURNING\\b", "\\bRETURNING_VALUES\\b", "\\bRETURNS\\b", "\\bREVOKE\\b",
        "\\bRIGHT\\b", "\\bROLE\\b", "\\bROLLBACK\\b", "\\bROUND\\b", "\\bROW_COUNT\\b", "\\bROWS\\b",
        "\\bRPAD\\b", "\\bRTRIM\\b", "\\bSAVEPOINT\\b", "\\bSCALAR_ARRAY\\b", "\\bSCHEMA\\b", "\\bSECOND\\b",
        "\\bSECONDARY\\b", "\\bSEGMENT\\b", "\\bSELECT\\b", "\\bSENSITIVE\\b", "\\bSEQUENCE\\b", "\\bSET\\b",
        "\\bSHADOW\\b", "\\bSHARED\\b", "\\bSIGN\\b", "\\bSIMILAR\\b", "\\bSIN\\b", "\\bSING\\b",
        "\\bSINGULAR\\b", "\\bSOME\\b", "\\bSORT\\b", "\\bSOURCE\\b", "\\bSPACE\\b", "\\bSQL\\b",
        "\\bSQRT\\b", "\\bSTABILITY\\b", "\\bSTARTING\\b", "\\bSTARTS\\b", "\\bSTATEMENT\\b", "\\bSTATIC\\b",
        "\\bSTATISTICS\\b", "\\bSTDDEV_POP\\b", "\\bSTDDEV_SAMP\\b", "\\bSUBSTRING\\b", "\\bSUB_TYPE\\b",
        "\\bSUM\\b", "\\bSUSPEND\\b", "\\bTAN\\b", "\\bTANH\\b", "\\bTABLE\\b", "\\bTABLESPACE\\b",
        "\\bTEMPORARY\\b", "\\bTERMINATOR\\b", "\\bTHEN\\b", "\\bTIES\\b", "\\bTIME\\b", "\\bTIMESTAMP\\b",
        "\\bTO\\b", "\\bTRAILING\\b", "\\bTRANSACTION\\b", "\\bTRIGGER\\b", "\\bTRIM\\b", "\\bTRUE\\b",
        "\\bTRUNC\\b", "\\bTRUNCATE\\b", "\\bTYPE\\b", "\\bUNCOMMITTED\\b", "\\bUNDO\\b", "\\bUNION\\b",
        "\\bUNIQUE\\b", "\\bUNKNOWN\\b", "\\bUPDATE\\b", "\\bUPDATING\\b", "\\bUPPER\\b", "\\bUSER\\b",
        "\\bUSING\\b", "\\bUUID_TO_CHAR\\b", "\\bVALUE\\b", "\\bVALUES\\b", "\\bVAR_POP\\b", "\\bVAR_SAMP\\b",
        "\\bVARCHAR\\b", "\\bVARIABLE\\b", "\\bVARYING\\b", "\\bVIEW\\b", "\\bWAIT\\b", "\\bWEEK\\b",
        "\\bWEEKDAY\\b", "\\bWHEN\\b", "\\bWHERE\\b", "\\bWHILE\\b", "\\bWITH\\b", "\\bWORD\\b",
        "\\bWRITE\\b", "\\bYEAR\\b", "\\bYEARDAY\\b"
    };
    for (const QString &pattern : keywordPatterns) {
        rule.pattern = QRegularExpression(pattern, QRegularExpression::CaseInsensitiveOption);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    // Формат для операторів
    operatorFormat.setForeground(Qt::red);
    const QString operatorPatterns[] = {
        "=", "<", ">", "<=", ">=", "<>", "!="
    };
    for (const QString &pattern : operatorPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = operatorFormat;
        highlightingRules.append(rule);
    }

    // Формат для чисел
    numberFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegularExpression("\\b[0-9]+\\b");
    rule.format = numberFormat;
    highlightingRules.append(rule);

    // Формат для однорядкових коментарів
    singleLineCommentFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegularExpression("--[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    // Формат для рядкових літералів
    quotationFormat.setForeground(Qt::darkRed);
    rule.pattern = QRegularExpression("\".*\"|'.*'");
    rule.format = quotationFormat;
    highlightingRules.append(rule);
}

void SqlSyntaxHighlighter::highlightBlock(const QString &text)
{
    for (const HighlightingRule &rule : qAsConst(highlightingRules)) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
    setCurrentBlockState(0);
}
