/*********************************************************************
*                                                                    *
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                                                                    *
**********************************************************************
*                                                                    *
* C-file generated by:                                               *
*                                                                    *
*        GUI_Builder for emWin version 5.32                          *
*        Compiled Oct  8 2015, 11:59:02                              *
*        (c) 2015 Segger Microcontroller GmbH & Co. KG               *
*                                                                    *
**********************************************************************
*                                                                    *
*        Internet: www.segger.com  Support: support@segger.com       *
*                                                                    *
**********************************************************************
*/

// USER START (Optionally insert additional includes)
#include <MainWindowDLG.h>
#include "DIALOG.h"
#include "WM.h"

#define BCKG_COLOR GUI_WHITE
#define FRNT_COLOR GUI_BLACK
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "MainWindow", WIN_MAIN, 0, 0, 800, 480, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Title", TXT_TITLE, 15, 5, 400, 30, 0, 0x64, 0 },
  { IMAGE_CreateIndirect, "Sensor", IMG_SENSOR, 435, 50, 200, 218, 0, 0, 0 },
  { IMAGE_CreateIndirect, "STM32", IMG_STM32, 15, 340, 191, 70, 0, 0, 0 },
  { IMAGE_CreateIndirect, "Segger", IMG_SEGGER, 250, 360, 107, 70, 0, 0, 0 },
  { IMAGE_CreateIndirect, "Mikroe", IMG_MIKROE, 400, 360, 264, 70, 0, 0, 0 },
  { BUTTON_CreateIndirect, "btnStream", BTN_STREAM, 730, 32, 60, 60, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Stream", TXT_STREAM, 730, 100, 60, 20, 0, 0x64, 0 },
  { BUTTON_CreateIndirect, "btnSettings", BTN_SETTINGS, 730, 132, 60, 60, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Settings", TXT_SETTINGS, 730, 200, 60, 20, 0, 0x64, 0 },
  { BUTTON_CreateIndirect, "btnCapture", BTN_CAPTURE, 730, 232, 60, 60, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Capture", TXT_CAPTURE, 730, 300, 60, 20, 0, 0x64, 0 },
  { BUTTON_CreateIndirect, "btnGallery", BTN_GALLERY, 730, 332, 60, 60, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Gallery", TXT_GALLERY, 730, 400, 60, 20, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "JpgInfo", TXT_JPG, 720, 450, 80, 20, 0, 0x64, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

// USER START (Optionally insert additional static code)

extern GUI_CONST_STORAGE GUI_BITMAP bmplay;
extern GUI_CONST_STORAGE GUI_BITMAP bmsettings;
extern GUI_CONST_STORAGE GUI_BITMAP bmcapture;
extern GUI_CONST_STORAGE GUI_BITMAP bmgallery;
extern GUI_CONST_STORAGE GUI_BITMAP bmcamera;
extern GUI_CONST_STORAGE GUI_BITMAP bmstm32;
extern GUI_CONST_STORAGE GUI_BITMAP bmsegger;
extern GUI_CONST_STORAGE GUI_BITMAP bmmikroe;

// USER END

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN      hItem;
  int          NCode;
  int          Id;
  // USER START (Optionally insert additional variables)
  // USER END

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    //
    // Initialization of 'MainWindow'
    //
    hItem = pMsg->hWin;
    WINDOW_SetBkColor(hItem, BCKG_COLOR);
    //
    // Initialization of 'Images'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, IMG_SENSOR);
    IMAGE_SetBitmap(hItem, &bmcamera);
    hItem = WM_GetDialogItem(pMsg->hWin, IMG_STM32);
    IMAGE_SetBitmap(hItem, &bmstm32);
    hItem = WM_GetDialogItem(pMsg->hWin, IMG_SEGGER);
    IMAGE_SetBitmap(hItem, &bmsegger);
    hItem = WM_GetDialogItem(pMsg->hWin, IMG_MIKROE);
    IMAGE_SetBitmap(hItem, &bmmikroe);
    //
    // Initialization of 'Buttons'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, BTN_STREAM);
    BUTTON_SetText(hItem, "");
    BUTTON_SetTextColor(hItem, BUTTON_CI_UNPRESSED, GUI_BLACK);
    BUTTON_SetBitmapEx(hItem, BUTTON_CI_UNPRESSED,&bmplay, 5, 5);

    hItem = WM_GetDialogItem(pMsg->hWin, BTN_SETTINGS);
    BUTTON_SetText(hItem, "");
    BUTTON_SetTextColor(hItem, BUTTON_CI_UNPRESSED, GUI_BLACK);
    BUTTON_SetBitmapEx(hItem, BUTTON_CI_UNPRESSED,&bmsettings, 5, 5);

    hItem = WM_GetDialogItem(pMsg->hWin, BTN_CAPTURE);
    BUTTON_SetText(hItem, "");
    BUTTON_SetTextColor(hItem, BUTTON_CI_UNPRESSED, GUI_BLACK);
    BUTTON_SetBitmapEx(hItem, BUTTON_CI_UNPRESSED,&bmcapture, 5, 5);

    hItem = WM_GetDialogItem(pMsg->hWin, BTN_GALLERY);
    BUTTON_SetText(hItem, "");
    BUTTON_SetTextColor(hItem, BUTTON_CI_UNPRESSED, GUI_BLACK);
    BUTTON_SetBitmapEx(hItem, BUTTON_CI_UNPRESSED,&bmgallery, 5, 5);
    //
    // Initialization of 'Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, TXT_TITLE);
    TEXT_SetTextColor(hItem, FRNT_COLOR);
    TEXT_SetFont(hItem, GUI_FONT_20B_1);
    TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    TEXT_SetBkColor(hItem, BCKG_COLOR);
    TEXT_SetText(hItem, "OmniVision OV7670 Camera test using DCMI");

    hItem = WM_GetDialogItem(pMsg->hWin, TXT_STREAM);
    TEXT_SetFont(hItem, GUI_FONT_16B_1);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_TOP);
    TEXT_SetText(hItem, "Stream");

    hItem = WM_GetDialogItem(pMsg->hWin, TXT_SETTINGS);
    TEXT_SetFont(hItem, GUI_FONT_16B_1);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_TOP);
    TEXT_SetText(hItem, "Settings");

    hItem = WM_GetDialogItem(pMsg->hWin, TXT_CAPTURE);
    TEXT_SetFont(hItem, GUI_FONT_16B_1);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_TOP);
    TEXT_SetText(hItem, "Save JPG");

    hItem = WM_GetDialogItem(pMsg->hWin, TXT_GALLERY);
    TEXT_SetFont(hItem, GUI_FONT_16B_1);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_TOP);
    TEXT_SetText(hItem, "Load JPG");

    hItem = WM_GetDialogItem(pMsg->hWin, TXT_JPG);
    TEXT_SetFont(hItem, GUI_FONT_16B_1);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_TOP);
    TEXT_SetText(hItem, "");

    //

    // USER START (Optionally insert additional code for further widget initialization)
    // USER END
    break;

  case WM_PAINT:
  	 break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case BTN_CAPTURE: // Notifications sent by 'btnCapture'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
	case BTN_GALLERY: // Notifications sent by 'btnGallery'
		switch(NCode) {
		case WM_NOTIFICATION_CLICKED:
		  // USER START (Optionally insert code for reacting on notification message)
		  // USER END
		  break;
		case WM_NOTIFICATION_RELEASED:
		  // USER START (Optionally insert code for reacting on notification message)
		  // USER END
		  break;
		// USER START (Optionally insert additional code for further notification handling)
		// USER END
	}
	break;
    // USER START (Optionally insert additional code for further Ids)
    // USER END
    }
    break;
  // USER START (Optionally insert additional message handling)
  // USER END
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       CreateMainWindow
*/

WM_HWIN CreateMainWindow(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return hWin;
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/
