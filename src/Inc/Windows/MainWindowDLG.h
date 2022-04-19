/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

#include "DIALOG.h"
#include <stdint.h>

#define WIN_MAIN	   	(GUI_ID_USER + 0x00)
#define TXT_TITLE     	(GUI_ID_USER + 0x01)

#define IMG_SENSOR	  	(GUI_ID_USER + 0x02)
#define IMG_STM32	  	(GUI_ID_USER + 0x03)
#define IMG_SEGGER	  	(GUI_ID_USER + 0x04)
#define IMG_MIKROE	  	(GUI_ID_USER + 0x05)

#define BTN_STREAM   	(GUI_ID_USER + 0x06)
#define BTN_SETTINGS	(GUI_ID_USER + 0x07)
#define BTN_CAPTURE		(GUI_ID_USER + 0x08)
#define BTN_GALLERY		(GUI_ID_USER + 0x09)

#define TXT_STREAM		(GUI_ID_USER + 0x0A)
#define TXT_SETTINGS	(GUI_ID_USER + 0x0B)
#define TXT_CAPTURE		(GUI_ID_USER + 0x0C)
#define TXT_GALLERY		(GUI_ID_USER + 0x0D)
#define TXT_JPG			(GUI_ID_USER + 0x0E)

// USER START (Optionally insert additional static data)
WM_HWIN CreateMainWindow(void);
// USER END

