#include "mode.h"
#include "includes.h"


void Serial_ReSourceDeInit(void)
{
  memset(&infoHost, 0, sizeof(infoHost));
  resetInfoFile();
  SD_DeInit();
#ifdef BUZZER_SUPPORT
  Buzzer_DeConfig();
#endif
  Serial_DeConfig();
}

void Serial_ReSourceInit(void)
{
#ifdef BUZZER_SUPPORT
  Buzzer_Config();
#endif
  Serial_Config(infoSettings.baudrate);
  
#ifdef U_DISK_SUPPROT
  USBH_Init(&USB_OTG_Core, USB_OTG_FS_CORE_ID, &USB_Host, &USBH_MSC_cb, &USR_cb);
#endif
}

void infoMenuSelect(void)
{
  infoMenu.cur = 0;
  switch(infoSettings.mode)
  {
    case SERIAL_TSC:
    {
      Serial_ReSourceInit();
      scanUpdates();
      u32 startUpTime = OS_GetTime();
      heatSetUpdateTime(100);
      infoMenu.menu[infoMenu.cur] = menuMain;
      LOGO_ReadDisplay();
      while(OS_GetTime() - startUpTime < 300)  //Display 3s logo
      {                                                                                                                     
        loopProcess();	
      }
      heatSetUpdateTime(300);
      break;
    }
      
    case LCD12864:
      infoMenu.menu[infoMenu.cur] = menuST7920;      
      break;    
  }
}

#ifdef LCD_ENCODER_SUPPORT
void menuMode(void)
{  
  RADIO modeRadio = {
    {"Serial Touch Screen", "LCD12864 Simulator", "LCD2004 Simulator"},
    SIMULATOR_XSTART, SIMULATOR_YSTART,
    BYTE_HEIGHT*2, 2,
    0
  };
  int16_t nowEncoder = encoderPosition = 0;
  int8_t  nowMode = modeRadio.select = infoSettings.mode;
  
  GUI_Clear(BLACK);
  RADIO_Create(&modeRadio);
  Serial_ReSourceDeInit();
  while(LCD_ReadBtn(LCD_BUTTON_INTERVALS));      //wait for button release
  
  while(infoMenu.menu[infoMenu.cur] == menuMode)
  {
    if(LCD_ReadBtn(LCD_BUTTON_INTERVALS))
    {
      break;
    }
    if(encoderPosition)
    {
      nowMode = limitValue(0, nowMode + encoderPosition, modeRadio.num - 1);
      RADIO_Select(&modeRadio, nowMode);
      encoderPosition = 0;    
    }
    
    LCD_LoopEncoder();
  }
  if(infoSettings.mode != nowMode)
  {
    infoSettings.mode = nowMode;
    storePara();
  }
  infoMenuSelect();
}
#endif
