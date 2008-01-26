/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 1995, 2004 IBM Corporation. All rights reserved.             */
/* Copyright (c) 2005-2006 Rexx Language Association. All rights reserved.    */
/*                                                                            */
/* This program and the accompanying materials are made available under       */
/* the terms of the Common Public License v1.0 which accompanies this         */
/* distribution. A copy is also available at the following address:           */
/* http://www.oorexx.org/license.html                          */
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
/******************************************************************************/
/* Object REXX OODialog                                             oovutil.h */
/*                                                                            */
/******************************************************************************/

#define MAXREXXNAME 128
#define MAXLENQUEUE 2056
#define NR_BUFFER 15
#define STR_BUFFER 256
#define LONGSTR_BUFFER 1024
#define DATA_BUFFER 4096*2      // was 256 before ES_MULTILINE
#define MAX_P 64
#define MAX_MT_ENTRIES  500
#define MAX_BT_ENTRIES  300
#define MAX_DT_ENTRIES  750
#define MAX_CT_ENTRIES 1000
#define MAX_IT_ENTRIES   20
#define MAXCHILDDIALOGS 20
#define MAXDIALOGS 20

/* User defined window messages used for RexxDlgProc() */
#define WM_USER_CREATECHILD         WM_USER + 0x0601
#define WM_USER_INTERRUPTSCROLL     WM_USER + 0x0602
#define WM_USER_GETFOCUS            WM_USER + 0x0603
#define WM_USER_GETSETCAPTURE       WM_USER + 0x0604
#define WM_USER_GETKEYSTATE         WM_USER + 0x0605
#define WM_USER_SUBCLASS            WM_USER + 0x0606
#define WM_USER_SUBCLASS_REMOVE     WM_USER + 0x0607
#define WM_USER_HOOK                WM_USER + 0x0608

#define VISDLL "OODIALOG.DLL"
#define DLLVER 2130

#define MSG_TERMINATE "1DLGDELETED1"

extern LONG HandleError(PRXSTRING r, CHAR * text);

/* Defines for the different possible versions of comctl32.dll up to Windows
 * XP SP2. These DWORD "packed version" numbers are calculated using the
 * following macro:
 */
#define MAKEVERSION(major,minor) MAKELONG(minor,major)

#define COMCTL32_4_0         262144
#define COMCTL32_4_7         262151
#define COMCTL32_4_71        262215
#define COMCTL32_4_72        262216
#define COMCTL32_5_8         327688
#define COMCTL32_5_81        327761
#define COMCTL32_5_82        327762
#define COMCTL32_6_0         393216

/* The version of comctl32.dll in use when oodialog.dll is loaded. */
extern DWORD ComCtl32Version;

/**
 *  A 'tag' is used in processing the mapping of Windows messages to user
 *  defined methods.  It allows the user mapping to dictate different processing
 *  of a Windows message based on the tag.
 *
 *  The least significant 2 bytes are used to define the type of control.  These
 *  bytes can be isolated using TAG_CTRLMASK.
 */
#define TAG_DIALOG                0x00000001
#define TAG_HELP                  0x00000100

#define TAG_TREEVIEW              0x00000006
#define TAG_LISTVIEW              0x00000007
#define TAG_TRACKBAR              0x00000008
#define TAG_TAB                   0x00000009
#define TAG_CTRLMASK              0x000000FF

#define TAG_STATECHANGED          0x00000100
#define TAG_CHECKBOXCHANGED       0x00000200
#define TAG_SELECTCHANGED         0x00000400
#define TAG_FOCUSCHANGED          0x00000800

/* macros to check the number of arguments */
#define CHECKARG(argexpct) { \
   if (argc != argexpct) \
      return HandleError(retstr, "Wrong number of arguments"); }


#define CHECKARGL(argexpct) { \
   if (argc < argexpct) \
      return HandleError(retstr, "Too few arguments"); }


#define CHECKARGLH(argexpctl, argexpcth) { \
   if (argc < argexpctl) return HandleError(retstr, "Too few arguments"); \
   if (argc > argexpcth) return HandleError(retstr, "Too many arguments"); }


/* macros for a easier return code */
#define RETC(retcode) { \
                   retstr->strlength = 1;\
                   if (retcode) retstr->strptr[0] = '1'; else retstr->strptr[0] = '0'; \
                   retstr->strptr[1] = '\0'; \
                   return 0; \
                }

#define RETERR  { \
                   retstr->strlength = 1;\
                   retstr->strptr[0] = '1'; \
                   retstr->strptr[1] = '\0'; \
                   return 40; \
                }


#define RETVAL(retvalue)  { \
                   ltoa(retvalue, retstr->strptr, 10); \
                   retstr->strlength = strlen(retstr->strptr);\
                   return 0; \
                }

/* macros for searching and checking the bitmap table */
#define SEARCHBMP(addr, ndx, id) \
   {                     \
      ndx = 0;\
      if (addr && addr->BmpTab)              \
      while ((ndx < addr->BT_size) && (addr->BmpTab[ndx].buttonID != (ULONG)id))\
         ndx++;                                                  \
   }

#define VALIDBMP(addr, ndx, id) \
   (addr && &addr->BmpTab[ndx] && (ndx < addr->BT_size) && (addr->BmpTab[ndx].buttonID == (ULONG)id))


/* macros for searching and checking the bitmap table */
#define SEARCHDATA(addr, ndx, id) \
   {                     \
      ndx = 0;              \
      if (addr && addr->DataTab)              \
      while ((ndx < addr->DT_size) && (addr->DataTab[ndx].id != (ULONG)id))\
         ndx++;                                                  \
   }

#define VALIDDATA(addr, ndx, id) \
   (addr && (ndx < addr->DT_size) && (addr->DataTab[ndx].id == (ULONG)id))


#define SEARCHBRUSH(addr, ndx, id, brush) \
   {                     \
      ndx = 0;\
      if (addr && addr->ColorTab) {              \
          while ((ndx < addr->CT_size) && (addr->ColorTab[ndx].itemID != (ULONG)id))\
             ndx++;                                                  \
          if (ndx < addr->CT_size) brush = addr->ColorTab[ndx].ColorBrush; \
      } \
   }


#define ISHEX(value) \
   ((value[0] == '0') && (toupper(value[1]) == 'X'))


//
// macros to access the fields in a BITMAPINFO struct
// field_value = macro(pBitmapInfo)
//

#define BI_WIDTH(pBI)       (int)((pBI)->bmiHeader.biWidth)
#define BI_HEIGHT(pBI)      (int)((pBI)->bmiHeader.biHeight)
#define BI_PLANES(pBI)      ((pBI)->bmiHeader.biPlanes)
#define BI_BITCOUNT(pBI)    ((pBI)->bmiHeader.biBitCount)

//
// macros to access BITMAPINFO fields in a DIB
// field_value = macro(pDIB)
//

#define DIB_WIDTH(pDIB)     (BI_WIDTH((LPBITMAPINFO)(pDIB)))
#define DIB_HEIGHT(pDIB)    (BI_HEIGHT((LPBITMAPINFO)(pDIB)))
#define DIB_PLANES(pDIB)    (BI_PLANES((LPBITMAPINFO)(pDIB)))
#define DIB_BITCOUNT(pDIB)  (BI_BITCOUNT((LPBITMAPINFO)(pDIB)))
//#define DIB_COLORS(pDIB)    (((LPBITMAPINFO)pDIB)->bmiHeader.biClrUsed)
#define DIB_COLORS(pDIB)    (NumDIBColorEntries((LPBITMAPINFO)(pDIB)))
#define DIB_BISIZE(pDIB)    (sizeof(BITMAPINFOHEADER) \
                            + DIB_COLORS(pDIB) * sizeof(RGBQUAD))
#define DIB_PBITS(pDIB)     (((LPSTR)((LPBITMAPINFO)(pDIB))) \
                            + DIB_BISIZE(pDIB))
#define DIB_PBI(pDIB)       ((LPBITMAPINFO)(pDIB))


#define SEEK_DLGADM_TABLE(hDlg, addressedTo) \
   if (topDlg && ((topDlg->TheDlg == hDlg) || (topDlg->AktChild == hDlg))) \
       addressedTo = topDlg; \
   else { register INT i=0; \
       while ((i<StoredDialogs) && (DialogTab[i]->TheDlg != hDlg) && (DialogTab[i]->AktChild != hDlg)) i++; \
       if (i<StoredDialogs) addressedTo = DialogTab[i]; else addressedTo = NULL;   } \

#define DEF_ADM     DIALOGADMIN * dlgAdm = NULL
#define GET_ADM     dlgAdm = (DIALOGADMIN *)atol(argv[0].strptr)

/* structures to manage the dialogs */
typedef struct {
   ULONG msg;
   ULONG filterM;
   ULONG wParam;
   ULONG filterP;
   ULONG lParam;
   ULONG filterL;
   ULONG tag;
   PCHAR rexxProgram;
} MESSAGETABLEENTRY;

typedef struct {
   ULONG id;
   USHORT typ;
   USHORT category;
} DATATABLEENTRY;

typedef struct {
   ULONG buttonID;
   HBITMAP bitmapID;
   HBITMAP bmpFocusID;
   HBITMAP bmpSelectID;
   HBITMAP bmpDisableID;
   SHORT Frame;
   SHORT Loaded;
   SHORT displacex;
   SHORT displacey;
} BITMAPTABLEENTRY;


typedef struct {
   ULONG itemID;
   INT ColorBk;
   INT ColorFG;
   HBRUSH ColorBrush;
} COLORTABLEENTRY;

typedef struct {
   ULONG iconID;
   PCHAR fileName;
} ICONTABLEENTRY;

/* Stuff for key press subclassing and keyboard hooks */
#define MAX_KEYPRESS_METHODS  63
#define COUNT_KEYPRESS_KEYS   256
#define CCH_METHOD_NAME       197

#define KEY_REALEASE          0x80000000
#define KEY_WASDOWN           0x40000000
#define KEY_TOGGLED           0x00000001
#define ISDOWN                    0x8000

/* Microsoft does not define these, just has this note:
 *
 * VK_0 - VK_9 are the same as ASCII '0' - '9' (0x30 - 0x39)
 * 0x40 : unassigned
 * VK_A - VK_Z are the same as ASCII 'A' - 'Z' (0x41 - 0x5A)
 */
#define VK_0   0x30
#define VK_1   0x31
#define VK_2   0x32
#define VK_3   0x33
#define VK_4   0x34
#define VK_5   0x35
#define VK_6   0x36
#define VK_7   0x37
#define VK_8   0x38
#define VK_9   0x39

#define VK_A   0x41
#define VK_B   0x42
#define VK_C   0x43
#define VK_D   0x44
#define VK_E   0x45
#define VK_F   0x46
#define VK_G   0x47
#define VK_H   0x48
#define VK_I   0x49
#define VK_J   0x4A
#define VK_K   0x4B
#define VK_L   0x4C
#define VK_M   0x4D
#define VK_N   0x4E
#define VK_O   0x4F
#define VK_P   0x50
#define VK_Q   0x51
#define VK_R   0x52
#define VK_S   0x53
#define VK_T   0x54
#define VK_U   0x55
#define VK_V   0x56
#define VK_W   0x57
#define VK_X   0x58
#define VK_Y   0x59
#define VK_Z   0x5A

typedef struct {
    BOOL none;          /* If none, neither of shift, control, or alt can be pressed */
    BOOL shift;
    BOOL alt;
    BOOL control;
    BOOL and;           /* If 'and' is false, filter is 'or' */
} KEYFILTER, *PKEYFILTER;

typedef struct {
    BYTE       key[COUNT_KEYPRESS_KEYS];            /* Value of key[x] is index to pMethods[]   */
    UINT       usedMethods;                         /* Count of used slots in  pMethods[]       */
    UINT       topOfQ;                              /* Top of next free queue, 0 if empty       */
    PCHAR      pMethods[MAX_KEYPRESS_METHODS + 1];  /* Index 0 intentionally left empty         */
    KEYFILTER *pFilters[MAX_KEYPRESS_METHODS + 1];  /* If null, no filter                       */
    UINT       nextFreeQ[MAX_KEYPRESS_METHODS];     /* Used only if existing connection removed */
} KEYPRESSDATA;

typedef struct {
    UINT          uID;
    HWND          hCtrl;
    PCHAR         pMessageQueue;
    KEYPRESSDATA *pKeyPressData;
} SUBCLASSDATA;


typedef struct
{
   void * previous;
   INT TableEntry;
   MESSAGETABLEENTRY * MsgTab;
   INT MT_size;
   DATATABLEENTRY * DataTab;
   INT DT_size;
   BITMAPTABLEENTRY * BmpTab;
   INT BT_size;
   COLORTABLEENTRY * ColorTab;
   INT CT_size;
   ICONTABLEENTRY * IconTab;
   INT IT_size;
   HWND TheDlg;
   HWND ChildDlg[MAXCHILDDIALOGS+1];
   HWND AktChild;
   HINSTANCE TheInstance;
   HANDLE TheThread;
   BOOL OnTheTop;
   ULONG LeaveDialog;
   BOOL Use3DControls;
   HBRUSH BkgBrush;
   HBITMAP BkgBitmap;
   HPALETTE ColorPalette;
   HMENU menu;
   HICON SysMenuIcon;
   HICON TitleBarIcon;
   BOOL  SharedIcon;
   BOOL  DidChangeIcon;
   HHOOK hHook;
   KEYPRESSDATA * pKeyPressData;
   DWORD threadID;
   WPARAM StopScroll;
   CHAR * pMessageQueue;
} DIALOGADMIN;



#ifdef EXTERNALFUNCS
typedef LONG REXXENTRY GETITEMDATAEXTERNALFN (HANDLE, ULONG, UINT, PCHAR, ULONG);
typedef LONG REXXENTRY SETITEMDATAEXTERNALFN (DIALOGADMIN *, HANDLE, ULONG, UINT, PCHAR);
typedef LONG REXXENTRY GETSTEMDATAEXTERNALFN (HANDLE, ULONG, ULONG, PCHAR, ULONG);
typedef LONG REXXENTRY SETSTEMDATAEXTERNALFN (DIALOGADMIN *, HANDLE, ULONG, ULONG, PCHAR);
#endif

#ifdef CREATEDLL
/* tools */
extern void rxstrlcpy(CHAR * tar, RXSTRING src);
extern void rxdatacpy(CHAR * tar, RXSTRING src);
extern BOOL IsYes(CHAR * s);

/* global variables */
#ifndef NOGLOBALVARIABLES
extern _declspec(dllexport) HINSTANCE MyInstance;
extern _declspec(dllexport) DIALOGADMIN * DialogTab[MAXDIALOGS];
extern _declspec(dllexport) DIALOGADMIN * topDlg;
extern _declspec(dllexport) INT StoredDialogs;
extern _declspec(dllexport) BOOL ReleaseMain;
#ifdef EXTERNALFUNCS
extern _declspec(dllexport) GETITEMDATAEXTERNALFN * GetItemDataExternal = NULL;
extern _declspec(dllexport) SETITEMDATAEXTERNALFN * SetItemDataExternal = NULL;
extern _declspec(dllexport) GETSTEMDATAEXTERNALFN * GetStemDataExternal = NULL;
extern _declspec(dllexport) SETSTEMDATAEXTERNALFN * SetStemDataExternal = NULL;
#endif

#endif

#else
extern _declspec(dllimport) HINSTANCE MyInstance;
extern _declspec(dllimport) DIALOGADMIN * DialogTab[MAXDIALOGS];
extern _declspec(dllimport) DIALOGADMIN * topDlg;
extern _declspec(dllimport) INT StoredDialogs;
extern _declspec(dllimport) BOOL ReleaseMain;
#ifdef EXTERNALFUNCS
extern _declspec(dllimport) GETITEMDATAEXTERNALFN * GetItemDataExternal;
extern _declspec(dllimport) SETITEMDATAEXTERNALFN * SetItemDataExternal;
extern _declspec(dllimport) GETSTEMDATAEXTERNALFN * GetStemDataExternal;
extern _declspec(dllimport) SETSTEMDATAEXTERNALFN * SetStemDataExternal;
#endif

#endif
