/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 1995, 2004 IBM Corporation. All rights reserved.             */
/* Copyright (c) 2005-2009 Rexx Language Association. All rights reserved.    */
/*                                                                            */
/* This program and the accompanying materials are made available under       */
/* the terms of the Common Public License v1.0 which accompanies this         */
/* distribution. A copy is also available at the following address:           */
/* http://www.oorexx.org/license.html                                         */
/*                                                                            */
/* Redistribution and use in source and binary forms, with or                 */
/* without modification, are permitted provided that the following            */
/* conditions are met:                                                        */
/*                                                                            */
/* Redistributions of source code must retain the above copyright             */
/* notice, this list of conditions and the following disclaimer.              */
/* Redistributions in binary form must reproduce the above copyright          */
/* notice, this list of conditions and the following disclaimer in            */
/* the documentation and/or other materials provided with the distribution.   */
/*                                                                            */
/* Neither the name of Rexx Language Association nor the names                */
/* of its contributors may be used to endorse or promote products             */
/* derived from this software without specific prior written permission.      */
/*                                                                            */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS        */
/* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT          */
/* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS          */
/* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT   */
/* OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,      */
/* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED   */
/* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,        */
/* OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY     */
/* OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING    */
/* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS         */
/* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.               */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#ifndef oodCommon_Included
#define oodCommon_Included

// Map string keywords representing constant defines to their int values.  For
// translating things like "IDI_APPLICATION" from the user to the proper API
// value.
#include <string>
#include <map>
using namespace std;
typedef map<string, int, less<string> > String2Int;


#define DEFAULT_FONTNAME       "MS Shell Dlg"
#define DEFAULT_FONTSIZE       8


#define OOD_ID_EXCEPTION 0xFFFFFFF7   // -9

// Enum for the type of an ooDialog class.  Types to be added as needed.
typedef enum
{
    oodPlainBaseDialog, oodCategoryDialog, oodStaticControl, oodButtonControl, oodEditControl,
    oodListBox,         oodProgressBar,    oodUnknown
} oodClass_t;

// Enum for the type of Windows dialog control.  Note that a track bar is a
// SliderControl in ooDiaolg. ;-(
typedef enum
{
    winStatic, winButton, winTreeView, winListView, winTab, winEdit, winRadioButton, winCheckBox,
    winGroupBox, winListBox, winComboBox, winScrollBar, winProgressBar, winTrackBar, winMonthCalendar,
    winDateTimePicker, winUnknown
} oodControl_t;

/* Struct for the WindowBase class CSelf. */
typedef struct _wbCSelf {
    HWND              hwnd;
    RexxObjectPtr     rexxHwnd;
    wholenumber_t     initCode;
    uint32_t          sizeX;
    uint32_t          sizeY;
    double            factorX;
    double            factorY;
} CWindowBase;
typedef CWindowBase *pCWindowBase;

/* Struct for the PlainBaseDialog class CSelf. */
typedef struct _pbdCSelf {
    pCWindowBase   wndBase;
    RexxObjectPtr  rexxSelf;
    HWND           hDlg;
    DIALOGADMIN    *dlgAdm;

} CPlainBaseDialog;
typedef CPlainBaseDialog *pCPlainBaseDialog;

/* Struct for the DialogControl class CSelf. */
typedef struct _dcCSelf {
    pCWindowBase   wndBase;
    RexxObjectPtr  rexxSelf;
    uint32_t       id;
    HWND           hCtrl;    // Handle of the dialog control
    HWND           hDlg;     // Handle of the owner dialog
    RexxObjectPtr  oDlg;     // The Rexx owner dialog object
} CDialogControl;
typedef CDialogControl *pCDialogControl;

/* Struct for the DynamicDialog class CSelf. */
typedef struct _ddCSelf {
    pCPlainBaseDialog  pcpbd;
    RexxObjectPtr      rexxSelf;
    DLGTEMPLATE       *base;    // Base pointer to dialog template (basePtr)
    void              *active;  // Pointer to current location in dialog template (activePtr)
    uint32_t           count;   // Dialog item count (dialogItemCount)
} CDynamicDialog;
typedef CDynamicDialog *pCDynamicDialog;

extern LRESULT CALLBACK RexxDlgProc( HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam );
extern bool             dialogInAdminTable(DIALOGADMIN * Dlg);
extern bool             InstallNecessaryStuff(DIALOGADMIN* dlgAdm, CSTRING library);
extern int32_t          stopDialog(HWND hDlg);
extern int32_t          DelDialog(DIALOGADMIN * aDlg);
extern BOOL             GetDialogIcons(DIALOGADMIN *, INT, UINT, PHANDLE, PHANDLE);
extern void             rxstrlcpy(CHAR * tar, CONSTRXSTRING &src);
extern void             rxdatacpy(CHAR * tar, RXSTRING &src);
extern bool             isYes(const char *s);
extern bool             IsNo(const char * s);
extern void *           string2pointer(const char *string);
extern void *           string2pointer(RexxMethodContext *c, RexxStringObject string);
extern void             pointer2string(char *, void *pointer);
extern RexxStringObject pointer2string(RexxMethodContext *, void *);
extern RexxStringObject pointer2string(RexxThreadContext *c, void *pointer);
extern LONG             HandleError(PRXSTRING r, CHAR * text);
extern char *           strdupupr(const char *str);
extern char *           strdupupr_nospace(const char *str);
extern char *           strdup_2methodName(const char *str);
extern DIALOGADMIN *    rxGetDlgAdm(RexxMethodContext *, RexxObjectPtr);

extern LPWORD lpwAlign(LPWORD lpIn);
extern BOOL AddTheMessage(DIALOGADMIN *, UINT, UINT, WPARAM, ULONG_PTR, LPARAM, ULONG_PTR, CSTRING, ULONG);

extern void       ooDialogInternalException(RexxMethodContext *, char *, int, char *, char *);
extern oodClass_t oodClass(RexxMethodContext *, RexxObjectPtr, oodClass_t *, size_t);
extern uint32_t   oodResolveSymbolicID(RexxMethodContext *, RexxObjectPtr, RexxObjectPtr, int, int);
extern bool       oodSafeResolveID(uint32_t *, RexxMethodContext *, RexxObjectPtr, RexxObjectPtr, int, int);

extern DWORD oodGetSysErrCode(RexxThreadContext *);
extern void  oodSetSysErrCode(RexxThreadContext *, DWORD);
extern void  oodResetSysErrCode(RexxThreadContext *context);

extern PPOINT        rxGetPoint(RexxMethodContext *context, RexxObjectPtr p, int argPos);
extern RexxObjectPtr rxNewPoint(RexxMethodContext *c, long x, long y);
extern PRECT         rxGetRect(RexxMethodContext *context, RexxObjectPtr r, int argPos);
extern RexxObjectPtr rxNewRect(RexxMethodContext *context, long l, long t, long r, long b);
extern PSIZE         rxGetSize(RexxMethodContext *context, RexxObjectPtr s, int argPos);
extern RexxObjectPtr rxNewSize(RexxMethodContext *c, long cx, long cy);

extern bool rxGetWindowText(RexxMethodContext *c, HWND hwnd, RexxStringObject *pStringObj);

// TODO move to APICommon when ooDialog is converted to use .Pointer instead of
// pointer strings.
extern POINTER rxGetPointerAttribute(RexxMethodContext *context, RexxObjectPtr obj, CSTRING name);

extern bool checkControlClass(HWND, oodControl_t);
extern bool requiredComCtl32Version(RexxMethodContext *context, const char *methodName, DWORD minimum);

extern bool          initWindowBase(RexxMethodContext *c, HWND hwndObj, RexxObjectPtr self, pCWindowBase *ppCWB);
extern RexxObjectPtr setDlgHandle(RexxMethodContext *c, pCPlainBaseDialog pcpbd, RexxStringObject hDlg);
extern int           getKeywordValue(String2Int *cMap, const char * str);

// Shared button stuff.
typedef enum {push, check, radio, group, owner, notButton} BUTTONTYPE, *PBUTTONTYPE;
typedef enum {def, autoCheck, threeState, autoThreeState, noSubtype } BUTTONSUBTYPE, *PBUTTONSUBTYPE;

extern BUTTONTYPE getButtonInfo(HWND, PBUTTONSUBTYPE, DWORD *);

#define GET_HANDLE(p) string2pointer(p)
#define GET_HWND(p)   ((HWND)string2pointer(p))
#define GET_POINTER(p) string2pointer(p)

inline void failedToRetrieveDlgAdmException(RexxThreadContext *, RexxObjectPtr);

inline void *string2pointer(CONSTRXSTRING *string) { return string2pointer(string->strptr); }
inline void *string2pointer(CONSTRXSTRING &string) { return string2pointer(string.strptr); }

// TODO check whether these functions are really inlined.

inline void pointer2string(PRXSTRING result, void *pointer)
{
    pointer2string(result->strptr, pointer);
    result->strlength = strlen(result->strptr);
}

inline void safeLocalFree(void *p)
{
    if (p != NULL)
    {
        LocalFree(p);
    }
}

inline void safeFree(void *p)
{
    if (p != NULL)
    {
        free(p);
    }
}

inline void safeDeleteObject(HANDLE h)
{
    if (h != NULL)
    {
        DeleteObject(h);
    }
}

inline void oodSetSysErrCode(RexxThreadContext *context)
{
    oodSetSysErrCode(context, GetLastError());
}

inline HWND rxGetWindowHandle(RexxMethodContext * context, RexxObjectPtr windowObject)
{
    return (HWND)rxGetPointerAttribute(context, windowObject, "HWND");
}

/**
 * Returns the first character of the message name that invoked the current
 * method.
 *
 * @param context  The method context.
 *
 * @return The first charactere of the message name.
 */
inline char msgAbbrev(RexxMethodContext *context)
{
    return *(context->GetMessageName());
}

/**
 * Checks that the argument could be construed as 'true'.  This would be 1 or
 * yes, but for historical reasons the German ja must also be included.
 *
 * This will also work for an optional arg to an API method.  I.e., if s is
 * null, false is returned.
 *
 * @param s  The string to check.
 *
 * @return bool
 */
inline bool isYes(const char * s)
{
   if ( s == NULL || strlen(s) == 0 )
   {
       return FALSE;
   }

   char c = toupper(s[0]);
   return ( c == 'J' || c =='Y' || c == '1' );
}

#endif
