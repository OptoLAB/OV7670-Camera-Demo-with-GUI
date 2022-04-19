/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
#include "SettingsDLG.h"
#include "OV7670.h"
// USER START (Optionally insert additional static data)
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "Camera settings", WIN_SETTINGS, 12, 32, 390, 445, 0, 0x0, 0 },

  { BUTTON_CreateIndirect, "BtnCancel", BTN_CANCEL, 285, 355, 80, 25, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "BtnOk", BTN_OK, 285, 390, 80, 25, 0, 0x0, 0 },

  { TEXT_CreateIndirect, "Resolution", TXT_RESOLUTION, 5, 5, 90, 20, 0, 0x64, 0 },
  { DROPDOWN_CreateIndirect, "Resolution", DDWN_RESOLUTION, 5, 25, 90, 120, 0, 0x0, 0 },

  { TEXT_CreateIndirect, "Format", TXT_FORMAT, 105, 5, 90, 20, 0, 0x64, 0 },
  { DROPDOWN_CreateIndirect, "Format", DDWN_FORMAT, 105, 25, 90, 50, 0, 0x0, 0 },

  { TEXT_CreateIndirect, "Effect", TXT_EFFECT, 205, 5, 90, 20, 0, 0x64, 0 },
  { DROPDOWN_CreateIndirect, "Effect", DDWN_EFFECT, 205, 25, 90, 130, 0, 0x0, 0 },

  { TEXT_CreateIndirect, "Scale", TXT_SCALE, 305, 5, 90, 20, 0, 0x64, 0 },
  { SPINBOX_CreateIndirect, "Scale", SBOX_SCALE, 305, 25, 50, 23, 0, 0x0, 0 },

  { TEXT_CreateIndirect, "Exposure", TXT_EXPOSURE, 5, 55, 95, 20, 0, 0x64, 0 },
  { SLIDER_CreateIndirect, "Exposure", SLDR_EXPOSURE, 5, 75, 265, 20, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "AEC", CBOX_EXPOSURE, 285, 75, 80, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Exposure", LBL_EXPOSURE, 250, 55, 40, 20, 0, 0x64, 0 },

  { TEXT_CreateIndirect, "Gain", TXT_GAIN, 5, 95, 90, 20, 0, 0x64, 0 },
  { SLIDER_CreateIndirect, "Gain", SLDR_GAIN, 5, 115, 265, 20, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "AGC", CBOX_GAIN, 285, 105, 80, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Gain", LBL_GAIN, 250, 95, 40, 20, 0, 0x64, 0 },

  { TEXT_CreateIndirect, "Brightness", TXT_BRIGHTNESS, 5, 135, 90, 20, 0, 0x64, 0 },
  { SLIDER_CreateIndirect, "Brightness", SLDR_BRIGHTNESS, 5, 155, 265, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Brightness", LBL_BRIGHTNESS, 250, 135, 40, 20, 0, 0x64, 0 },

  { TEXT_CreateIndirect, "Contrast", TXT_CONTRAST, 5, 175, 90, 20, 0, 0x64, 0 },
  { SLIDER_CreateIndirect, "Contrast", SLDR_CONTRAST, 5, 195, 265, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Contrast", LBL_CONTRAST, 250, 175, 40, 20, 0, 0x64, 0 },

  { TEXT_CreateIndirect, "Saturation", TXT_SATURATION, 5, 215, 90, 20, 0, 0x64, 0 },
  { SLIDER_CreateIndirect, "Saturation", SLDR_SATURATION, 5, 235, 265, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Saturation", LBL_SATURATION, 250, 215, 40, 20, 0, 0x64, 0 },

  { TEXT_CreateIndirect, "GainCeiling", TXT_GAIN_CEIL, 5, 255, 90, 20, 0, 0x64, 0 },
  { SLIDER_CreateIndirect, "GainCeiling", SLDR_GAIN_CEIL, 5, 275, 265, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "GainCeiling", LBL_GAIN_CEIL, 250, 255, 40, 20, 0, 0x64, 0 },

  { TEXT_CreateIndirect, "RedGain", TXT_RCH_GAIN, 5, 295, 90, 20, 0, 0x64, 0 },
  { SLIDER_CreateIndirect, "RedGain", SLDR_RCH_GAIN, 5, 315, 265, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "RedGain", LBL_RCH_GAIN, 250, 295, 40, 20, 0, 0x64, 0 },

  { TEXT_CreateIndirect, "GreenGain", TXT_GCH_GAIN, 5, 335, 90, 20, 0, 0x64, 0 },
  { SLIDER_CreateIndirect, "GreenGain", SLDR_GCH_GAIN, 5, 355, 265, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "GreenGain", LBL_GCH_GAIN, 250, 335, 40, 20, 0, 0x64, 0 },

  { TEXT_CreateIndirect, "BlueGain", TXT_BCH_GAIN, 5, 375, 90, 20, 0, 0x64, 0 },
  { SLIDER_CreateIndirect, "BlueGain", SLDR_BCH_GAIN, 5, 395, 265, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "BlueGain", LBL_BCH_GAIN, 250, 375, 40, 20, 0, 0x64, 0 },

  { CHECKBOX_CreateIndirect, "AWB", CBOX_AWB, 285, 315, 100, 20, 0, 0x0, 0 },

  { CHECKBOX_CreateIndirect, "Hflip", CBOX_HFLIP, 285, 195, 100, 20, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "Vflip", CBOX_VFLIP, 285, 225, 80, 20, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "Colorbar", CBOX_CBAR, 285, 255, 80, 20, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "NightMode", CBOX_NIGHT_MODE, 285, 285, 80, 20, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "HistogramAEC", CBOX_HIST_AEC, 285, 135, 80, 20, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "Denoise", CBOX_DENOISE, 285, 165, 80, 20, 0, 0x0, 0 },

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

/*
*/
// USER END

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;
  // USER START (Optionally insert additional variables)
  char str[5];
  // USER END

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    //
    // Initialization of 'Texts'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, TXT_RESOLUTION);
    TEXT_SetFont(hItem, GUI_FONT_13_1);
    TEXT_SetText(hItem, "Resolution");

    hItem = WM_GetDialogItem(pMsg->hWin, TXT_FORMAT);
    TEXT_SetFont(hItem, GUI_FONT_13_1);
    TEXT_SetText(hItem, "Format");

    hItem = WM_GetDialogItem(pMsg->hWin, TXT_EFFECT);
    TEXT_SetFont(hItem, GUI_FONT_13_1);
    TEXT_SetText(hItem, "Effect");

    hItem = WM_GetDialogItem(pMsg->hWin, TXT_SCALE);
    TEXT_SetFont(hItem, GUI_FONT_13_1);
    TEXT_SetText(hItem, "Scale");

    hItem = WM_GetDialogItem(pMsg->hWin, TXT_EXPOSURE);
    TEXT_SetFont(hItem, GUI_FONT_13_1);
    TEXT_SetText(hItem, "Exposure time [ms]");

    hItem = WM_GetDialogItem(pMsg->hWin, TXT_GAIN);
    TEXT_SetFont(hItem, GUI_FONT_13_1);
    TEXT_SetText(hItem, "Gain");

    hItem = WM_GetDialogItem(pMsg->hWin, TXT_BRIGHTNESS);
    TEXT_SetFont(hItem, GUI_FONT_13_1);
    TEXT_SetText(hItem, "Brightness");

    hItem = WM_GetDialogItem(pMsg->hWin, TXT_CONTRAST);
    TEXT_SetFont(hItem, GUI_FONT_13_1);
    TEXT_SetText(hItem, "Contrast");

    hItem = WM_GetDialogItem(pMsg->hWin, TXT_SATURATION);
    TEXT_SetFont(hItem, GUI_FONT_13_1);
    TEXT_SetText(hItem, "Saturation");

    hItem = WM_GetDialogItem(pMsg->hWin, TXT_GAIN_CEIL);
    TEXT_SetFont(hItem, GUI_FONT_13_1);
    TEXT_SetText(hItem, "Gain Ceiling");

    hItem = WM_GetDialogItem(pMsg->hWin, TXT_RCH_GAIN);
    TEXT_SetFont(hItem, GUI_FONT_13_1);
    TEXT_SetText(hItem, "Red Ch gain");

    hItem = WM_GetDialogItem(pMsg->hWin, TXT_GCH_GAIN);
    TEXT_SetFont(hItem, GUI_FONT_13_1);
    TEXT_SetText(hItem, "Green Ch gain");

    hItem = WM_GetDialogItem(pMsg->hWin, TXT_BCH_GAIN);
    TEXT_SetFont(hItem, GUI_FONT_13_1);
    TEXT_SetText(hItem, "Blue Ch gain");
    //
    // Initialization of 'Spinboxes'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, SBOX_SCALE);
    SPINBOX_SetFont(hItem, GUI_FONT_13B_1);
    SPINBOX_SetTextColor(hItem, EDIT_CI_ENABLED, GUI_BLACK);
    SPINBOX_SetRange(hItem, 1, 10);
    SPINBOX_SetValue(hItem, 1);
    //
    // Initialization of 'Sliders'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, SLDR_EXPOSURE);
	SLIDER_SetRange(hItem, 0, 673);
	SLIDER_SetValue(hItem, 256);

    hItem = WM_GetDialogItem(pMsg->hWin, SLDR_GAIN);
	SLIDER_SetRange(hItem, 0, 7);
	SLIDER_SetValue(hItem, 4);

    hItem = WM_GetDialogItem(pMsg->hWin, SLDR_BRIGHTNESS);
	SLIDER_SetRange(hItem, 0, 255);
	SLIDER_SetValue(hItem, 127);

    hItem = WM_GetDialogItem(pMsg->hWin, SLDR_CONTRAST);
	SLIDER_SetRange(hItem, 0, 100);
	SLIDER_SetValue(hItem, 50);

    hItem = WM_GetDialogItem(pMsg->hWin, SLDR_SATURATION);
	SLIDER_SetRange(hItem, 0, 100);
	SLIDER_SetValue(hItem, 80);

    hItem = WM_GetDialogItem(pMsg->hWin, SLDR_GAIN_CEIL);
	SLIDER_SetRange(hItem, 0, 6);
	SLIDER_SetValue(hItem, 4);

    hItem = WM_GetDialogItem(pMsg->hWin, SLDR_RCH_GAIN);
	SLIDER_SetRange(hItem, 0, 255);
	SLIDER_SetValue(hItem, 77);

    hItem = WM_GetDialogItem(pMsg->hWin, SLDR_GCH_GAIN);
	SLIDER_SetRange(hItem, 0, 255);
	SLIDER_SetValue(hItem, 103);

    hItem = WM_GetDialogItem(pMsg->hWin, SLDR_BCH_GAIN);
	SLIDER_SetRange(hItem, 0, 255);
	SLIDER_SetValue(hItem, 153);
    //
    // Initialization of 'Buttons'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, BTN_CANCEL);
    BUTTON_SetText(hItem, "Cancel");
    BUTTON_SetFont(hItem, GUI_FONT_16B_1);

    hItem = WM_GetDialogItem(pMsg->hWin, BTN_OK);
    BUTTON_SetText(hItem, "Ok");
    BUTTON_SetFont(hItem, GUI_FONT_16B_1);
    //
    // Initialization of 'Check boxes'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, CBOX_EXPOSURE);
    CHECKBOX_SetText(hItem, "AEC enable");
    CHECKBOX_SetState(hItem, 1);

    hItem = WM_GetDialogItem(pMsg->hWin, CBOX_GAIN);
    CHECKBOX_SetText(hItem, "AGC enable");
    CHECKBOX_SetState(hItem, 1);

    hItem = WM_GetDialogItem(pMsg->hWin, CBOX_AWB);
    CHECKBOX_SetText(hItem, "AWB enable");
    CHECKBOX_SetState(hItem, 1);

    hItem = WM_GetDialogItem(pMsg->hWin, CBOX_HFLIP);
    CHECKBOX_SetText(hItem, "Flip horizontal");
    CHECKBOX_SetState(hItem, 0);

    hItem = WM_GetDialogItem(pMsg->hWin, CBOX_VFLIP);
    CHECKBOX_SetText(hItem, "Flip vertical");
    CHECKBOX_SetState(hItem, 0);

    hItem = WM_GetDialogItem(pMsg->hWin, CBOX_CBAR);
    CHECKBOX_SetText(hItem, "Color bar");
    CHECKBOX_SetState(hItem, 0);

    hItem = WM_GetDialogItem(pMsg->hWin, CBOX_NIGHT_MODE);
    CHECKBOX_SetText(hItem, "Night mode");
    CHECKBOX_SetState(hItem, 0);

    hItem = WM_GetDialogItem(pMsg->hWin, CBOX_HIST_AEC);
    CHECKBOX_SetText(hItem, "Hist. AEC");
    CHECKBOX_SetState(hItem, 0);

    hItem = WM_GetDialogItem(pMsg->hWin, CBOX_DENOISE);
    CHECKBOX_SetText(hItem, "Denoise");
    CHECKBOX_SetState(hItem, 0);
    //
    // Initialization of 'Dropdown'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, DDWN_RESOLUTION);
    DROPDOWN_AddString(hItem, "VGA");
    DROPDOWN_AddString(hItem, "QVGA");
    DROPDOWN_AddString(hItem, "QQVGA");
    DROPDOWN_AddString(hItem, "QQQVGA");
    DROPDOWN_AddString(hItem, "CIF");
    DROPDOWN_AddString(hItem, "QCIF");
    DROPDOWN_AddString(hItem, "QQCIF");
    DROPDOWN_SetSel(hItem, 1);
    DROPDOWN_SetFont(hItem, GUI_FONT_16_1);

    hItem = WM_GetDialogItem(pMsg->hWin, DDWN_FORMAT);
    DROPDOWN_AddString(hItem, "YUV422");
    DROPDOWN_AddString(hItem, "RGB565");
    DROPDOWN_SetSel(hItem, 0);
    DROPDOWN_SetFont(hItem, GUI_FONT_16_1);

    hItem = WM_GetDialogItem(pMsg->hWin, DDWN_EFFECT);
    DROPDOWN_AddString(hItem, "Normal");
    DROPDOWN_AddString(hItem, "Antique");
    DROPDOWN_AddString(hItem, "B&W");
    DROPDOWN_AddString(hItem, "Negative");
    DROPDOWN_AddString(hItem, "B&W Negative");
    DROPDOWN_AddString(hItem, "Bluish");
    DROPDOWN_AddString(hItem, "Greenish");
    DROPDOWN_AddString(hItem, "Reddish");
    DROPDOWN_SetSel(hItem, 0);
    DROPDOWN_SetFont(hItem, GUI_FONT_16_1);
    //
    // Initialization of 'Labels'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, LBL_EXPOSURE);
    TEXT_SetFont(hItem, GUI_FONT_13_1);
    TEXT_SetText(hItem, "12.5");	// for slider val 256

    hItem = WM_GetDialogItem(pMsg->hWin, LBL_GAIN);
    TEXT_SetFont(hItem, GUI_FONT_13_1);
    TEXT_SetText(hItem, "16x");

    hItem = WM_GetDialogItem(pMsg->hWin, LBL_BRIGHTNESS);
    TEXT_SetFont(hItem, GUI_FONT_13_1);
    TEXT_SetText(hItem, "127");

    hItem = WM_GetDialogItem(pMsg->hWin, LBL_CONTRAST);
    TEXT_SetFont(hItem, GUI_FONT_13_1);
    TEXT_SetText(hItem, "50");

    hItem = WM_GetDialogItem(pMsg->hWin, LBL_SATURATION);
    TEXT_SetFont(hItem, GUI_FONT_13_1);
    TEXT_SetText(hItem, "80");

    hItem = WM_GetDialogItem(pMsg->hWin, LBL_GAIN_CEIL);
    TEXT_SetFont(hItem, GUI_FONT_13_1);
    TEXT_SetText(hItem, "32x");

    hItem = WM_GetDialogItem(pMsg->hWin, LBL_RCH_GAIN);
    TEXT_SetFont(hItem, GUI_FONT_13_1);
    TEXT_SetText(hItem, "77");

    hItem = WM_GetDialogItem(pMsg->hWin, LBL_GCH_GAIN);
    TEXT_SetFont(hItem, GUI_FONT_13_1);
    TEXT_SetText(hItem, "103");

    hItem = WM_GetDialogItem(pMsg->hWin, LBL_BCH_GAIN);
    TEXT_SetFont(hItem, GUI_FONT_13_1);
    TEXT_SetText(hItem, "153");
    // USER START (Optionally insert additional code for further widget initialization)

    // USER END
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case SLDR_EXPOSURE: // Notifications sent by 'SLDR_EXPOSURE'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
    	  hItem = WM_GetDialogItem(pMsg->hWin, SLDR_EXPOSURE);
    	  int val=SLIDER_GetValue(hItem);
    	  uint16_t tn=0; uint8_t tf=0;
    	  calculateExposureTimeMS(val,&tn,&tf);
    	  sprintf((char *)str, "%d.%d", tn, tf);
    	  hItem = WM_GetDialogItem(pMsg->hWin, LBL_EXPOSURE);
    	  TEXT_SetText(hItem, str);
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case SLDR_GAIN: // Notifications sent by 'SLDR_GAIN'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
    	  hItem = WM_GetDialogItem(pMsg->hWin, SLDR_GAIN);
    	  int val=SLIDER_GetValue(hItem);
    	  sprintf((char *)str, "%dx", (1<<val));
    	  hItem = WM_GetDialogItem(pMsg->hWin, LBL_GAIN);
    	  TEXT_SetText(hItem, str);
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
	case SLDR_BRIGHTNESS: // Notifications sent by 'SLDR_BRIGHTNESS'
		switch(NCode) {
		case WM_NOTIFICATION_CLICKED:
		  // USER START (Optionally insert code for reacting on notification message)
		  // USER END
		  break;
		case WM_NOTIFICATION_RELEASED:
		  // USER START (Optionally insert code for reacting on notification message)
		  // USER END
		  break;
		case WM_NOTIFICATION_VALUE_CHANGED:
		  // USER START (Optionally insert code for reacting on notification message)
		  hItem = WM_GetDialogItem(pMsg->hWin, SLDR_BRIGHTNESS);
		  int val=SLIDER_GetValue(hItem);
		  sprintf((char *)str, "%d", val);
		  hItem = WM_GetDialogItem(pMsg->hWin, LBL_BRIGHTNESS);
		  TEXT_SetText(hItem, str);
		  // USER END
		  break;
		// USER START (Optionally insert additional code for further notification handling)
		// USER END
		}
		break;
	case SLDR_CONTRAST: // Notifications sent by 'SLDR_CONTRAST'
		switch(NCode) {
		case WM_NOTIFICATION_CLICKED:
		// USER START (Optionally insert code for reacting on notification message)
		// USER END
		break;
		case WM_NOTIFICATION_RELEASED:
		// USER START (Optionally insert code for reacting on notification message)
		// USER END
		break;
		case WM_NOTIFICATION_VALUE_CHANGED:
		// USER START (Optionally insert code for reacting on notification message)
		  hItem = WM_GetDialogItem(pMsg->hWin, SLDR_CONTRAST);
		  int val=SLIDER_GetValue(hItem);
		  sprintf((char *)str, "%d", val);
		  hItem = WM_GetDialogItem(pMsg->hWin, LBL_CONTRAST);
		  TEXT_SetText(hItem, str);
		// USER END
		break;
		// USER START (Optionally insert additional code for further notification handling)
		// USER END
		}
		break;
	case SLDR_SATURATION: // Notifications sent by 'SLDR_SATURATION'
		switch(NCode) {
		case WM_NOTIFICATION_CLICKED:
		// USER START (Optionally insert code for reacting on notification message)
		// USER END
		break;
		case WM_NOTIFICATION_RELEASED:
		// USER START (Optionally insert code for reacting on notification message)
		// USER END
		break;
		case WM_NOTIFICATION_VALUE_CHANGED:
		// USER START (Optionally insert code for reacting on notification message)
		  hItem = WM_GetDialogItem(pMsg->hWin, SLDR_SATURATION);
		  int val=SLIDER_GetValue(hItem);
		  sprintf((char *)str, "%d", val);
		  hItem = WM_GetDialogItem(pMsg->hWin, LBL_SATURATION);
		  TEXT_SetText(hItem, str);
		// USER END
		break;
		// USER START (Optionally insert additional code for further notification handling)
		// USER END
		}
		break;
	case SLDR_GAIN_CEIL: // Notifications sent by 'SLDR_GAIN_CEIL'
		switch(NCode) {
		case WM_NOTIFICATION_CLICKED:
		  // USER START (Optionally insert code for reacting on notification message)
		  // USER END
		  break;
		case WM_NOTIFICATION_RELEASED:
		  // USER START (Optionally insert code for reacting on notification message)
		  // USER END
		  break;
		case WM_NOTIFICATION_VALUE_CHANGED:
		  // USER START (Optionally insert code for reacting on notification message)
		  hItem = WM_GetDialogItem(pMsg->hWin, SLDR_GAIN_CEIL);
		  int val=SLIDER_GetValue(hItem);
		  sprintf((char *)str, "%dx", (1<<(val+1)));
		  hItem = WM_GetDialogItem(pMsg->hWin, LBL_GAIN_CEIL);
		  TEXT_SetText(hItem, str);
		  // USER END
		  break;
		// USER START (Optionally insert additional code for further notification handling)
		// USER END
		}
		break;
	case SLDR_RCH_GAIN: // Notifications sent by 'SLDR_RCH_GAIN'
		switch(NCode) {
		case WM_NOTIFICATION_CLICKED:
		// USER START (Optionally insert code for reacting on notification message)
		// USER END
		break;
		case WM_NOTIFICATION_RELEASED:
		// USER START (Optionally insert code for reacting on notification message)
		// USER END
		break;
		case WM_NOTIFICATION_VALUE_CHANGED:
		// USER START (Optionally insert code for reacting on notification message)
		  hItem = WM_GetDialogItem(pMsg->hWin, SLDR_RCH_GAIN);
		  int val=SLIDER_GetValue(hItem);
		  sprintf((char *)str, "%d", val);
		  hItem = WM_GetDialogItem(pMsg->hWin, LBL_RCH_GAIN);
		  TEXT_SetText(hItem, str);
		// USER END
		break;
		// USER START (Optionally insert additional code for further notification handling)
		// USER END
		}
		break;
	case SLDR_GCH_GAIN: // Notifications sent by 'SLDR_GCH_GAIN'
		switch(NCode) {
		case WM_NOTIFICATION_CLICKED:
		// USER START (Optionally insert code for reacting on notification message)
		// USER END
		break;
		case WM_NOTIFICATION_RELEASED:
		// USER START (Optionally insert code for reacting on notification message)
		// USER END
		break;
		case WM_NOTIFICATION_VALUE_CHANGED:
		// USER START (Optionally insert code for reacting on notification message)
		  hItem = WM_GetDialogItem(pMsg->hWin, SLDR_GCH_GAIN);
		  int val=SLIDER_GetValue(hItem);
		  sprintf((char *)str, "%d", val);
		  hItem = WM_GetDialogItem(pMsg->hWin, LBL_GCH_GAIN);
		  TEXT_SetText(hItem, str);
		// USER END
		break;
		// USER START (Optionally insert additional code for further notification handling)
		// USER END
		}
		break;
	case SLDR_BCH_GAIN: // Notifications sent by 'SLDR_BCH_GAIN'
		switch(NCode) {
		case WM_NOTIFICATION_CLICKED:
		// USER START (Optionally insert code for reacting on notification message)
		// USER END
		break;
		case WM_NOTIFICATION_RELEASED:
		// USER START (Optionally insert code for reacting on notification message)
		// USER END
		break;
		case WM_NOTIFICATION_VALUE_CHANGED:
		// USER START (Optionally insert code for reacting on notification message)
		  hItem = WM_GetDialogItem(pMsg->hWin, SLDR_BCH_GAIN);
		  int val=SLIDER_GetValue(hItem);
		  sprintf((char *)str, "%d", val);
		  hItem = WM_GetDialogItem(pMsg->hWin, LBL_BCH_GAIN);
		  TEXT_SetText(hItem, str);
		// USER END
		break;
		// USER START (Optionally insert additional code for further notification handling)
		// USER END
		}
		break;
    }
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
*       CreateSettings
*/

WM_HWIN CreateSettings(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return hWin;
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/
