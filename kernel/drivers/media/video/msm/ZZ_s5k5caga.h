
#undef CAMERADBG

#ifdef CONFIG_S5K5CAGA_DEBUG
#define CAMERADBG(fmt, args...) printk(KERN_ERR fmt, ##args)
#else
#define CAMERADBG(fmt, args...) do { } while (0)
#endif

/*
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Effect_0919

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
#if 0
static void EFFECT_AQUA(void)
{
//<CAMTUNING_EFFECT_AQUA>
s5k5caga_i2c_write(0xFCFC, 0xD000);
s5k5caga_i2c_write(0x0028, 0x7000);
s5k5caga_i2c_write(0x002A, 0x021E);
s5k5caga_i2c_write(0x0F12, 0x0005);

CAMERADBG( " ======EFFECT_AQUA is OK====== \n" );
}
#endif


static  void EFFECT_Black_and_White(void)
{
//<CAMTUNING_EFFECT_MONO>
s5k5caga_i2c_write(0xFCFC, 0xD000);
s5k5caga_i2c_write(0x0028, 0x7000);
s5k5caga_i2c_write(0x002A, 0x021E);
s5k5caga_i2c_write(0x0F12, 0x0001);

CAMERADBG( " ======EFFECT_Black_and_White is OK====== \n" );
}

static void EFFECT_Emboss_mono(void)
{
//<CAMTUNING_EFFECT_SKETCH>
s5k5caga_i2c_write(0xFCFC, 0xD000);
s5k5caga_i2c_write(0x0028, 0x7000);

s5k5caga_i2c_write(0x002A, 0x3286);
s5k5caga_i2c_write(0x0F12, 0x0000); //Pre/Post gamma on(\u76d4\u6c57)

s5k5caga_i2c_write(0x002A, 0x021E);
s5k5caga_i2c_write(0x0F12, 0x0008); //Sketch mode

CAMERADBG( " ======EFFECT_Emboss_mono is OK====== \n" );
}

static void EFFECT_Negative(void)
{
//<CAMTUNING_EFFECT_NEGATIVE>
s5k5caga_i2c_write(0xFCFC, 0xD000);
s5k5caga_i2c_write(0x0028, 0x7000);
s5k5caga_i2c_write(0x002A, 0x021E);
s5k5caga_i2c_write(0x0F12, 0x0003);

CAMERADBG( " ======EFFECT_Negative is OK====== \n" );
}


static void EFFECT_Normal(void)
{
//<CAMTUNING_EFFECT_OFF>
s5k5caga_i2c_write(0xFCFC, 0xD000);
s5k5caga_i2c_write(0x0028, 0x7000);

s5k5caga_i2c_write(0x002A, 0x3286);
s5k5caga_i2c_write(0x0F12, 0x0001); //Pre/Post gamma on(\u76d4\u6c57)

s5k5caga_i2c_write(0x002A, 0x021E);
s5k5caga_i2c_write(0x0F12, 0x0000); //Normal mode

CAMERADBG( " ======EFFECT_Normal is OK====== \n" );
}


static void EFFECT_Sepia(void)
{
//<CAMTUNING_EFFECT_SEPIA>
s5k5caga_i2c_write(0xFCFC, 0xD000);
s5k5caga_i2c_write(0x0028, 0x7000);
s5k5caga_i2c_write(0x002A, 0x021E);
s5k5caga_i2c_write(0x0F12, 0x0004);

CAMERADBG( " ======EFFECT_Sepia is OK====== \n" );
}



static void EFFECT_Sketch(void)
{
//<CAMTUNING_EFFECT_SKETCH>
s5k5caga_i2c_write(0xFCFC, 0xD000);
s5k5caga_i2c_write(0x0028, 0x7000);

s5k5caga_i2c_write(0x002A, 0x3286);
s5k5caga_i2c_write(0x0F12, 0x0000); //Pre/Post gamma on(\u76d4\u6c57)

s5k5caga_i2c_write(0x002A, 0x021E);
s5k5caga_i2c_write(0x0F12, 0x0006); //Sketch mode

CAMERADBG( " ======EFFECT_Sketch is OK====== \n" );
}

/*
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

AWB APPLICATION_0920

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

static void CAMTUNING_WHITE_BALANCE_AUTO(void)
{
s5k5caga_i2c_write(0xfcfc, 0xd000); 
s5k5caga_i2c_write(0x0028, 0x7000); 
s5k5caga_i2c_write(0x002a, 0x246E); 	 
s5k5caga_i2c_write(0x0f12, 0x0001);
CAMERADBG( " ======CAMTUNING_WHITE_BALANCE_AUTO is OK====== \n" );
}

static void CAMTUNING_WHITE_BALANCE_DAYLIGHT(void)
{
s5k5caga_i2c_write(0xfcfc, 0xd000); 
s5k5caga_i2c_write(0x0028, 0x7000); 
s5k5caga_i2c_write(0x002a, 0x246E); 	 
s5k5caga_i2c_write(0x0f12, 0x0000);
s5k5caga_i2c_write(0x002a, 0x04A0); 	 
s5k5caga_i2c_write(0x0f12, 0x05E0);
s5k5caga_i2c_write(0x0f12, 0x0001);
s5k5caga_i2c_write(0x0f12, 0x0400);
s5k5caga_i2c_write(0x0f12, 0x0001);
s5k5caga_i2c_write(0x0f12, 0x0530);
s5k5caga_i2c_write(0x0f12, 0x0001);
CAMERADBG( " ======CAMTUNING_WHITE_BALANCE_DAYLIGHT is OK====== \n" );
}

static void CAMTUNING_WHITE_BALANCE_CLOUDY(void)
{
s5k5caga_i2c_write(0xfcfc, 0xd000); 
s5k5caga_i2c_write(0x0028, 0x7000); 
s5k5caga_i2c_write(0x002a, 0x246E); 	 
s5k5caga_i2c_write(0x0f12, 0x0000);
s5k5caga_i2c_write(0x002a, 0x04A0); 	 
s5k5caga_i2c_write(0x0f12, 0x0740); //0710 
s5k5caga_i2c_write(0x0f12, 0x0001); //0001 
s5k5caga_i2c_write(0x0f12, 0x0400); //0400 
s5k5caga_i2c_write(0x0f12, 0x0001); //0001 
s5k5caga_i2c_write(0x0f12, 0x0460); //0485 
s5k5caga_i2c_write(0x0f12, 0x0001); //0001 
CAMERADBG( " ======CAMTUNING_WHITE_BALANCE_CLOUDY is OK====== \n" );
}

static void CAMTUNING_WHITE_BALANCE_FLUORESCENT(void)
{
s5k5caga_i2c_write(0xfcfc, 0xd000); 
s5k5caga_i2c_write(0x0028, 0x7000); 
s5k5caga_i2c_write(0x002a, 0x246E); 	 
s5k5caga_i2c_write(0x0f12, 0x0000);
s5k5caga_i2c_write(0x002a, 0x04A0); 	 
s5k5caga_i2c_write(0x0f12, 0x0575); 
s5k5caga_i2c_write(0x0f12, 0x0001);
s5k5caga_i2c_write(0x0f12, 0x0400); 
s5k5caga_i2c_write(0x0f12, 0x0001);
s5k5caga_i2c_write(0x0f12, 0x0800); 
s5k5caga_i2c_write(0x0f12, 0x0001);  
CAMERADBG( " ======CAMTUNING_WHITE_BALANCE_FLUORESCENT is OK====== \n" );
}

static void CAMTUNING_WHITE_BALANCE_INCANDESCENT(void)
{
s5k5caga_i2c_write(0xfcfc, 0xd000); 
s5k5caga_i2c_write(0x0028, 0x7000); 
s5k5caga_i2c_write(0x002a, 0x246E); 	 
s5k5caga_i2c_write(0x0f12, 0x0000);
s5k5caga_i2c_write(0x002a, 0x04A0); 	 
s5k5caga_i2c_write(0x0f12, 0x0400);  //03D0
s5k5caga_i2c_write(0x0f12, 0x0001);  //0001
s5k5caga_i2c_write(0x0f12, 0x0400);  //0400
s5k5caga_i2c_write(0x0f12, 0x0001);  //0001
s5k5caga_i2c_write(0x0f12, 0x0940);  //0920
s5k5caga_i2c_write(0x0f12, 0x0001);  //0001
CAMERADBG( " ======CAMTUNING_WHITE_BALANCE_INCANDESCENT is OK====== \n" );
}


/*
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

EV APPLICATION_0920

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

static void EV_pos2(void)
{
s5k5caga_i2c_write(0x0028, 0x7000);
s5k5caga_i2c_write(0x002A, 0x020C);
s5k5caga_i2c_write(0x0F12, 0x003F);
CAMERADBG( " ======EV +2 is OK====== \n" );
}
static void EV_pos1(void)
{
s5k5caga_i2c_write(0x0028, 0x7000);
s5k5caga_i2c_write(0x002A, 0x020C);
s5k5caga_i2c_write(0x0F12, 0x001F);
CAMERADBG( " ======EV +1 is OK====== \n" );
}
static void EV_pos0(void)//default
{
s5k5caga_i2c_write(0x0028, 0x7000);
s5k5caga_i2c_write(0x002A, 0x020C);
s5k5caga_i2c_write(0x0F12, 0x0000);
CAMERADBG( " ======EV +0 is OK====== \n" );
}
static void EV_neg1(void)
{
s5k5caga_i2c_write(0x0028, 0x7000);
s5k5caga_i2c_write(0x002A, 0x020C);
s5k5caga_i2c_write(0x0F12, 0xFFE0);
CAMERADBG( " ======EV -1 is OK====== \n" );
}
static void EV_neg2(void)
{
s5k5caga_i2c_write(0x0028, 0x7000);
s5k5caga_i2c_write(0x002A, 0x020C);
s5k5caga_i2c_write(0x0F12, 0xFFC0);
CAMERADBG( " ======EV -2 is OK====== \n" );
}
/*
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

anti banding

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

static void Antibanding_auto(void)//default
{
//Auto-XCLK24MHz
/*
s5k5caga_i2c_write(0x0028,0x7000);
s5k5caga_i2c_write(0x002A,0x04D2);
s5k5caga_i2c_write(0x0F12,0x067F);
s5k5caga_i2c_write(0x002A,0x04BA);
s5k5caga_i2c_write(0x0F12,0x0000);
s5k5caga_i2c_write(0x0F12,0x0001);*/
//Auto banding 
s5k5caga_i2c_write(0x0028, 0x7000); 	// Set page 						
s5k5caga_i2c_write(0x002A, 0x0C18);
s5k5caga_i2c_write(0x0F12, 0x0001); // 0001: 60Hz start auto / 0000: 50Hz start auto //AFC_Default60Hz 70000C18	
s5k5caga_i2c_write(0x002A, 0x04D2); 	// Set address						
s5k5caga_i2c_write(0x0F12, 0x067F); 	// #REG_TC_DBG_AutoAlgEnBits, AA_FLICKER ON
CAMERADBG( " ======Antibanding_auto is OK====== \n" );
}
static void Antibanding_50Hz(void)
{/*
s5k5caga_i2c_write(0x0028,0x7000);
s5k5caga_i2c_write(0x002A,0x04D2);
s5k5caga_i2c_write(0x0F12,0x065F);
s5k5caga_i2c_write(0x002A,0x04BA);
s5k5caga_i2c_write(0x0F12,0x0001);
s5k5caga_i2c_write(0x002A,0x04BC);
s5k5caga_i2c_write(0x0F12,0x0001);*/
//50HZ	
s5k5caga_i2c_write(0x0028, 0x7000); 	// Set page 						
s5k5caga_i2c_write(0x002A, 0x04D2); 	// Set address						
s5k5caga_i2c_write(0x0F12, 0x065F); 	// #REG_TC_DBG_AutoAlgEnBits, AA_FLICKER OFF			
s5k5caga_i2c_write(0x002a, 0x04BA);	//REG_SF_USER_FlickerQuant, 0: no AFC, 1: 50Hz, 2: 60Hz
s5k5caga_i2c_write(0x0F12, 0x0001);		//50Hz
s5k5caga_i2c_write(0x0F12, 0x0001);		//Sync F/W
CAMERADBG( " ======Antibanding_50Hz is OK====== \n" );
}
static void Antibanding_60Hz(void)
{/*
s5k5caga_i2c_write(0x0028,0x7000);
s5k5caga_i2c_write(0x002A,0x04D2);
s5k5caga_i2c_write(0x0F12,0x065F);
s5k5caga_i2c_write(0x002A,0x04BA);
s5k5caga_i2c_write(0x0F12,0x0002);
s5k5caga_i2c_write(0x002A,0x04BC);
s5k5caga_i2c_write(0x0F12,0x0001);*/
//60HZ			
s5k5caga_i2c_write(0x0028, 0x7000); 	// Set page 						
s5k5caga_i2c_write(0x002A, 0x04D2); 	// Set address						
s5k5caga_i2c_write(0x0F12, 0x065F); 	// #REG_TC_DBG_AutoAlgEnBits, AA_FLICKER OFF		
s5k5caga_i2c_write(0x002a, 0x04BA);	//REG_SF_USER_FlickerQuant, 0: no AFC, 1: 50Hz, 2: 60Hz		
s5k5caga_i2c_write(0x0F12, 0x0002);		// 60Hz
s5k5caga_i2c_write(0x0F12, 0x0001);		//Sync F/W
CAMERADBG( " ======Antibanding_60Hz is OK====== \n" );
}
/*
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

bestshot

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

static void Bestshot_Night(void)
{
      //night 
s5k5caga_i2c_write(0x0028, 0x7000);                                                                       
s5k5caga_i2c_write(0x002A, 0x0288);                                                                       
s5k5caga_i2c_write(0x0F12, 0x07D0);    
s5k5caga_i2c_write(0x0F12, 0x0535);    
s5k5caga_i2c_write(0x002A, 0x023c);                                         
s5k5caga_i2c_write(0x0F12, 0x0000); 
s5k5caga_i2c_write(0x002A, 0x0240);                                         
s5k5caga_i2c_write(0x0F12, 0x0001);                                                                       
s5k5caga_i2c_write(0x002A, 0x0230);                                              
s5k5caga_i2c_write(0x0F12, 0x0001);                                                                       
s5k5caga_i2c_write(0x002A, 0x023E);                                           
s5k5caga_i2c_write(0x0F12, 0x0001);
s5k5caga_i2c_write(0x002A, 0x0220);                                           
s5k5caga_i2c_write(0x0F12, 0x0001);
s5k5caga_i2c_write(0x0F12, 0x0001);  
}

static void Bestshot_Normal(void)
{
  //normal fix 15fps
s5k5caga_i2c_write(0x0028, 0x7000);   
s5k5caga_i2c_write(0x002A, 0x0288);			
s5k5caga_i2c_write(0x0F12, 0x03E8);                    
s5k5caga_i2c_write(0x0F12, 0x029A);
s5k5caga_i2c_write(0x002A, 0x023c);                                         
s5k5caga_i2c_write(0x0F12, 0x0000); 
s5k5caga_i2c_write(0x002A, 0x0240);                                         
s5k5caga_i2c_write(0x0F12, 0x0001);                                                                       
s5k5caga_i2c_write(0x002A, 0x0230);                                              
s5k5caga_i2c_write(0x0F12, 0x0001);                                                                       
s5k5caga_i2c_write(0x002A, 0x023E);                                           
s5k5caga_i2c_write(0x0F12, 0x0001);
s5k5caga_i2c_write(0x002A, 0x0220);                                           
s5k5caga_i2c_write(0x0F12, 0x0001);
s5k5caga_i2c_write(0x0F12, 0x0001); 
}
#if 0
/*
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

ZOOM_0922

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
static void zoom_1_00(void)
{
//Zoom Step 1                                                
s5k5caga_i2c_write(0xFCFC, 0xD000);                                              
s5k5caga_i2c_write(0x0028, 0x7000);                                              
s5k5caga_i2c_write(0x002A, 0x0474);                                              
s5k5caga_i2c_write(0x0F12, 0x0100);	//#REG_TC_PZOOM_uZoomTarget88   //x1.00              
s5k5caga_i2c_write(0x002A, 0x0466);                                              
s5k5caga_i2c_write(0x0F12, 0x0002);	//#REG_TC_PZOOM_ZoomPanTiltReques
CAMERADBG( " ======%s is OK====== \n",__FUNCTION__ );
}

static void zoom_1_34(void)
{
//Zoom Step 2                                                                                              
s5k5caga_i2c_write(0xFCFC, 0xD000);                                              
s5k5caga_i2c_write(0x0028, 0x7000);                                              
s5k5caga_i2c_write(0x002A, 0x0474);                                              
s5k5caga_i2c_write(0x0F12, 0x0157);	//#REG_TC_PZOOM_uZoomTarget88   //x1.34             
s5k5caga_i2c_write(0x002A, 0x0466);                                              
s5k5caga_i2c_write(0x0F12, 0x0002);	//#REG_TC_PZOOM_ZoomPanTiltReques
CAMERADBG( " ======%s is OK====== \n",__FUNCTION__ );
}
static void zoom_1_79(void)
{
//Zoom Step 3                                                
s5k5caga_i2c_write(0xFCFC, 0xD000);                                              
s5k5caga_i2c_write(0x0028, 0x7000);                                              
s5k5caga_i2c_write(0x002A, 0x0474);                                              
s5k5caga_i2c_write(0x0F12, 0x01CB);	//#REG_TC_PZOOM_uZoomTarget88   //x1.79             
s5k5caga_i2c_write(0x002A, 0x0466);                                              
s5k5caga_i2c_write(0x0F12, 0x0002);	//#REG_TC_PZOOM_ZoomPanTiltReques
CAMERADBG( " ======%s is OK====== \n",__FUNCTION__ );
}
static void zoom_2_40(void)
{
//Zoom Step 4                                                
s5k5caga_i2c_write(0xFCFC, 0xD000);                                              
s5k5caga_i2c_write(0x0028, 0x7000);                                              
s5k5caga_i2c_write(0x002A, 0x0474);                                              
s5k5caga_i2c_write(0x0F12, 0x0266);	//#REG_TC_PZOOM_uZoomTarget88   //x2.4             
s5k5caga_i2c_write(0x002A, 0x0466);                                              
s5k5caga_i2c_write(0x0F12, 0x0002);	//#REG_TC_PZOOM_ZoomPanTiltReques
CAMERADBG( " ======%s is OK====== \n",__FUNCTION__ );
}
#endif



static void Preview_config( unsigned int num )
{
s5k5caga_i2c_write(0xFCFC, 0xD000); //Reset            //
s5k5caga_i2c_write(0x0028, 0x7000); 
  
s5k5caga_i2c_write(0x002A, 0x023C);  //#REG_TC_GP_ActivePrevConfig                                       
s5k5caga_i2c_write(0x0F12, num);  // num
s5k5caga_i2c_write(0x002A, 0x0240);  //#REG_TC_GP_PrevOpenAfterChange   
s5k5caga_i2c_write(0x0F12, 0x0001);      
s5k5caga_i2c_write(0x002A, 0x0230);  //#REG_TC_GP_NewConfigSync                                       
s5k5caga_i2c_write(0x0F12, 0x0001);         
s5k5caga_i2c_write(0x002A, 0x023E);  //#REG_TC_GP_PrevConfigChanged                                        
s5k5caga_i2c_write(0x0F12, 0x0001); 
CAMERADBG( "******************************************************************%s is OK, num=%d ******************************************************************\n",__FUNCTION__,num );
}

static void Capture_config( unsigned int  size )
{
s5k5caga_i2c_write(0xFCFC,  0xD000); //Reset            //
s5k5caga_i2c_write(0x0028,   0x7000);
s5k5caga_i2c_write(0x002a,   0x0244); //#REG_TC_GP_ActiveCapConfig

if (size == 300)//(2048*1536))
{
CAMERADBG( " capture size : 3M  \n" );
s5k5caga_i2c_write(0x0f12,   0); //num 
}
else if (size == 200)//(1600*1200))
{
CAMERADBG( " capture size : 2M  \n" );
s5k5caga_i2c_write(0x0f12,   1); //num 
}
else if (size == 130)//(1280*1024))
{
CAMERADBG( " capture size : 1.3M  \n" );
s5k5caga_i2c_write(0x0f12,   2); //num 
}
//else if (size == ?)//(640*480))
//{
//CAMERADBG( " capture size : VGA  \n" );
//s5k5caga_i2c_write(0x0f12,   3); //num 
//}
else
{
printk( " capture size : ERROR camera size, change to 3M  \n" );
s5k5caga_i2c_write(0x0f12,   0); //num 
}

s5k5caga_i2c_write(0x0f12,   0x0001); //#REG_TC_GP_CapConfigChanged 
s5k5caga_i2c_write(0x002a,   0x0230);
s5k5caga_i2c_write(0x0f12,   0x0001); //#REG_TC_GP_NewConfigSync
s5k5caga_i2c_write(0x002a,   0x0224);
s5k5caga_i2c_write(0x0f12,   0x0001); //#REG_TC_GP_EnableCapture 
s5k5caga_i2c_write(0x0f12,   0x0001); //#REG_TC_GP_EnableCaptureChanged

}


/*************************static void zz_poppy_init(void)*************************/
//#include "ZZ_N03.h"
//#include "ZZ_N05.h"
//#include "ZZ_Poppy_5CA_Ini_V03_111027sarah.h"
//#include "ZZ_Poppy_5CA_Ini_V03_110922.h"
#include "ZZ_Poppy_5CA_Ini_V04_111209.h"

#undef CAMERADBG

