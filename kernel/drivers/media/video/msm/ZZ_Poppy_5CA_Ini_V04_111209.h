
//REV01 for Poppy£ºMclk 12M, Pclk:91Mhz ,preview size VGA YUV 422, capture mode YUV data format,qualified on demo,noise performance improved.
//REV02 for Poppy£ºChange preview config0 preview size as 428*320, 
//                 Add Video preview config1:15fps,preview config2:30fps;
//                 add capture config 1:2M resolution,capture config 2:VGA resolution,
//REV03 for Poppy£ºChange capture config 2 as 1.3M resolution,and add capture config 3 as VGA
//REV04 for Poppy: Change  data output order to YUYV(7000036C=0042);
//                 Change camera preview config as VGA requested by drive team;
//                 Del video preview config2 by UE team update.
//                 Modify some comments

//void Poppy_5CA_Ini_V04_111209(void)
static void zz_poppy_init(void)
{

// ARM GO
// Direct mode 
s5k5caga_i2c_write(0xFCFC, 0xD000); //Reset            //
s5k5caga_i2c_write(0x0010, 0x0001); //Clear host interrupt so main will wait //
s5k5caga_i2c_write(0x1030, 0x0000); //ARM go           //
s5k5caga_i2c_write(0x0014, 0x0001); //Wait100mSec          //

//Delay 100ms 
mdelay(10);   //ffff 0064  //Delay 100ms 

// Start T&P part
// DO NOT DELETE T&P SECTION COMMENTS! They are required to debug T&P related issues.

s5k5caga_i2c_write(0x0028, 0x7000);
s5k5caga_i2c_write(0x002A, 0x2CF8);
s5k5caga_i2c_write(0x0F12, 0xB510);
s5k5caga_i2c_write(0x0F12, 0x4827);
s5k5caga_i2c_write(0x0F12, 0x21C0);
s5k5caga_i2c_write(0x0F12, 0x8041);
s5k5caga_i2c_write(0x0F12, 0x4825);
s5k5caga_i2c_write(0x0F12, 0x4A26);
s5k5caga_i2c_write(0x0F12, 0x3020);
s5k5caga_i2c_write(0x0F12, 0x8382);
s5k5caga_i2c_write(0x0F12, 0x1D12);
s5k5caga_i2c_write(0x0F12, 0x83C2);
s5k5caga_i2c_write(0x0F12, 0x4822);
s5k5caga_i2c_write(0x0F12, 0x3040);
s5k5caga_i2c_write(0x0F12, 0x8041);
s5k5caga_i2c_write(0x0F12, 0x4821);
s5k5caga_i2c_write(0x0F12, 0x4922);
s5k5caga_i2c_write(0x0F12, 0x3060);
s5k5caga_i2c_write(0x0F12, 0x8381);
s5k5caga_i2c_write(0x0F12, 0x1D09);
s5k5caga_i2c_write(0x0F12, 0x83C1);
s5k5caga_i2c_write(0x0F12, 0x4821);
s5k5caga_i2c_write(0x0F12, 0x491D);
s5k5caga_i2c_write(0x0F12, 0x8802);
s5k5caga_i2c_write(0x0F12, 0x3980);
s5k5caga_i2c_write(0x0F12, 0x804A);
s5k5caga_i2c_write(0x0F12, 0x8842);
s5k5caga_i2c_write(0x0F12, 0x808A);
s5k5caga_i2c_write(0x0F12, 0x8882);
s5k5caga_i2c_write(0x0F12, 0x80CA);
s5k5caga_i2c_write(0x0F12, 0x88C2);
s5k5caga_i2c_write(0x0F12, 0x810A);
s5k5caga_i2c_write(0x0F12, 0x8902);
s5k5caga_i2c_write(0x0F12, 0x491C);
s5k5caga_i2c_write(0x0F12, 0x80CA);
s5k5caga_i2c_write(0x0F12, 0x8942);
s5k5caga_i2c_write(0x0F12, 0x814A);
s5k5caga_i2c_write(0x0F12, 0x8982);
s5k5caga_i2c_write(0x0F12, 0x830A);
s5k5caga_i2c_write(0x0F12, 0x89C2);
s5k5caga_i2c_write(0x0F12, 0x834A);
s5k5caga_i2c_write(0x0F12, 0x8A00);
s5k5caga_i2c_write(0x0F12, 0x4918);
s5k5caga_i2c_write(0x0F12, 0x8188);
s5k5caga_i2c_write(0x0F12, 0x4918);
s5k5caga_i2c_write(0x0F12, 0x4819);
s5k5caga_i2c_write(0x0F12, 0xF000);
s5k5caga_i2c_write(0x0F12, 0xFA0E);
s5k5caga_i2c_write(0x0F12, 0x4918);
s5k5caga_i2c_write(0x0F12, 0x4819);
s5k5caga_i2c_write(0x0F12, 0x6341);
s5k5caga_i2c_write(0x0F12, 0x4919);
s5k5caga_i2c_write(0x0F12, 0x4819);
s5k5caga_i2c_write(0x0F12, 0xF000);
s5k5caga_i2c_write(0x0F12, 0xFA07);
s5k5caga_i2c_write(0x0F12, 0x4816);
s5k5caga_i2c_write(0x0F12, 0x4918);
s5k5caga_i2c_write(0x0F12, 0x3840);
s5k5caga_i2c_write(0x0F12, 0x62C1);
s5k5caga_i2c_write(0x0F12, 0x4918);
s5k5caga_i2c_write(0x0F12, 0x3880);
s5k5caga_i2c_write(0x0F12, 0x63C1);
s5k5caga_i2c_write(0x0F12, 0x4917);
s5k5caga_i2c_write(0x0F12, 0x6301);
s5k5caga_i2c_write(0x0F12, 0x4917);
s5k5caga_i2c_write(0x0F12, 0x3040);
s5k5caga_i2c_write(0x0F12, 0x6181);
s5k5caga_i2c_write(0x0F12, 0x4917);
s5k5caga_i2c_write(0x0F12, 0x4817);
s5k5caga_i2c_write(0x0F12, 0xF000);
s5k5caga_i2c_write(0x0F12, 0xF9F7);
s5k5caga_i2c_write(0x0F12, 0x4917);
s5k5caga_i2c_write(0x0F12, 0x4817);
s5k5caga_i2c_write(0x0F12, 0xF000);
s5k5caga_i2c_write(0x0F12, 0xF9F3);
s5k5caga_i2c_write(0x0F12, 0x4917);
s5k5caga_i2c_write(0x0F12, 0x4817);
s5k5caga_i2c_write(0x0F12, 0xF000);
s5k5caga_i2c_write(0x0F12, 0xF9EF);
s5k5caga_i2c_write(0x0F12, 0xBC10);
s5k5caga_i2c_write(0x0F12, 0xBC08);
s5k5caga_i2c_write(0x0F12, 0x4718);
s5k5caga_i2c_write(0x0F12, 0x1100);
s5k5caga_i2c_write(0x0F12, 0xD000);
s5k5caga_i2c_write(0x0F12, 0x267C);
s5k5caga_i2c_write(0x0F12, 0x0000);
s5k5caga_i2c_write(0x0F12, 0x2CE8);
s5k5caga_i2c_write(0x0F12, 0x0000);
s5k5caga_i2c_write(0x0F12, 0x3274);
s5k5caga_i2c_write(0x0F12, 0x7000);
s5k5caga_i2c_write(0x0F12, 0xF400);
s5k5caga_i2c_write(0x0F12, 0xD000);
s5k5caga_i2c_write(0x0F12, 0xF520);
s5k5caga_i2c_write(0x0F12, 0xD000);
s5k5caga_i2c_write(0x0F12, 0x2DF1);
s5k5caga_i2c_write(0x0F12, 0x7000);
s5k5caga_i2c_write(0x0F12, 0x89A9);
s5k5caga_i2c_write(0x0F12, 0x0000);
s5k5caga_i2c_write(0x0F12, 0x2E43);
s5k5caga_i2c_write(0x0F12, 0x7000);
s5k5caga_i2c_write(0x0F12, 0x0140);
s5k5caga_i2c_write(0x0F12, 0x7000);
s5k5caga_i2c_write(0x0F12, 0x2E7D);
s5k5caga_i2c_write(0x0F12, 0x7000);
s5k5caga_i2c_write(0x0F12, 0xB4F7);
s5k5caga_i2c_write(0x0F12, 0x0000);
s5k5caga_i2c_write(0x0F12, 0x2F07);
s5k5caga_i2c_write(0x0F12, 0x7000);
s5k5caga_i2c_write(0x0F12, 0x2F2B);
s5k5caga_i2c_write(0x0F12, 0x7000);
s5k5caga_i2c_write(0x0F12, 0x2FD1);
s5k5caga_i2c_write(0x0F12, 0x7000);
s5k5caga_i2c_write(0x0F12, 0x2FE5);
s5k5caga_i2c_write(0x0F12, 0x7000);
s5k5caga_i2c_write(0x0F12, 0x2FB9);
s5k5caga_i2c_write(0x0F12, 0x7000);
s5k5caga_i2c_write(0x0F12, 0x013D);
s5k5caga_i2c_write(0x0F12, 0x0001);
s5k5caga_i2c_write(0x0F12, 0x306B);
s5k5caga_i2c_write(0x0F12, 0x7000);
s5k5caga_i2c_write(0x0F12, 0x5823);
s5k5caga_i2c_write(0x0F12, 0x0000);
s5k5caga_i2c_write(0x0F12, 0x30B9);
s5k5caga_i2c_write(0x0F12, 0x7000);
s5k5caga_i2c_write(0x0F12, 0xD789);
s5k5caga_i2c_write(0x0F12, 0x0000);
s5k5caga_i2c_write(0x0F12, 0xB570);
s5k5caga_i2c_write(0x0F12, 0x6804);
s5k5caga_i2c_write(0x0F12, 0x6845);
s5k5caga_i2c_write(0x0F12, 0x6881);
s5k5caga_i2c_write(0x0F12, 0x6840);
s5k5caga_i2c_write(0x0F12, 0x2900);
s5k5caga_i2c_write(0x0F12, 0x6880);
s5k5caga_i2c_write(0x0F12, 0xD007);
s5k5caga_i2c_write(0x0F12, 0x49C3);
s5k5caga_i2c_write(0x0F12, 0x8949);
s5k5caga_i2c_write(0x0F12, 0x084A);
s5k5caga_i2c_write(0x0F12, 0x1880);
s5k5caga_i2c_write(0x0F12, 0xF000);
s5k5caga_i2c_write(0x0F12, 0xF9BA);
s5k5caga_i2c_write(0x0F12, 0x80A0);
s5k5caga_i2c_write(0x0F12, 0xE000);
s5k5caga_i2c_write(0x0F12, 0x80A0);
s5k5caga_i2c_write(0x0F12, 0x88A0);
s5k5caga_i2c_write(0x0F12, 0x2800);
s5k5caga_i2c_write(0x0F12, 0xD010);
s5k5caga_i2c_write(0x0F12, 0x68A9);
s5k5caga_i2c_write(0x0F12, 0x6828);
s5k5caga_i2c_write(0x0F12, 0x084A);
s5k5caga_i2c_write(0x0F12, 0x1880);
s5k5caga_i2c_write(0x0F12, 0xF000);
s5k5caga_i2c_write(0x0F12, 0xF9AE);
s5k5caga_i2c_write(0x0F12, 0x8020);
s5k5caga_i2c_write(0x0F12, 0x1D2D);
s5k5caga_i2c_write(0x0F12, 0xCD03);
s5k5caga_i2c_write(0x0F12, 0x084A);
s5k5caga_i2c_write(0x0F12, 0x1880);
s5k5caga_i2c_write(0x0F12, 0xF000);
s5k5caga_i2c_write(0x0F12, 0xF9A7);
s5k5caga_i2c_write(0x0F12, 0x8060);
s5k5caga_i2c_write(0x0F12, 0xBC70);
s5k5caga_i2c_write(0x0F12, 0xBC08);
s5k5caga_i2c_write(0x0F12, 0x4718);
s5k5caga_i2c_write(0x0F12, 0x2000);
s5k5caga_i2c_write(0x0F12, 0x8060);
s5k5caga_i2c_write(0x0F12, 0x8020);
s5k5caga_i2c_write(0x0F12, 0xE7F8);
s5k5caga_i2c_write(0x0F12, 0xB510);
s5k5caga_i2c_write(0x0F12, 0xF000);
s5k5caga_i2c_write(0x0F12, 0xF9A2);
s5k5caga_i2c_write(0x0F12, 0x48B2);
s5k5caga_i2c_write(0x0F12, 0x8A40);
s5k5caga_i2c_write(0x0F12, 0x2800);
s5k5caga_i2c_write(0x0F12, 0xD00C);
s5k5caga_i2c_write(0x0F12, 0x48B1);
s5k5caga_i2c_write(0x0F12, 0x49B2);
s5k5caga_i2c_write(0x0F12, 0x8800);
s5k5caga_i2c_write(0x0F12, 0x4AB2);
s5k5caga_i2c_write(0x0F12, 0x2805);
s5k5caga_i2c_write(0x0F12, 0xD003);
s5k5caga_i2c_write(0x0F12, 0x4BB1);
s5k5caga_i2c_write(0x0F12, 0x795B);
s5k5caga_i2c_write(0x0F12, 0x2B00);
s5k5caga_i2c_write(0x0F12, 0xD005);
s5k5caga_i2c_write(0x0F12, 0x2001);
s5k5caga_i2c_write(0x0F12, 0x8008);
s5k5caga_i2c_write(0x0F12, 0x8010);
s5k5caga_i2c_write(0x0F12, 0xBC10);
s5k5caga_i2c_write(0x0F12, 0xBC08);
s5k5caga_i2c_write(0x0F12, 0x4718);
s5k5caga_i2c_write(0x0F12, 0x2800);
s5k5caga_i2c_write(0x0F12, 0xD1FA);
s5k5caga_i2c_write(0x0F12, 0x2000);
s5k5caga_i2c_write(0x0F12, 0x8008);
s5k5caga_i2c_write(0x0F12, 0x8010);
s5k5caga_i2c_write(0x0F12, 0xE7F6);
s5k5caga_i2c_write(0x0F12, 0xB5F8);
s5k5caga_i2c_write(0x0F12, 0x2407);
s5k5caga_i2c_write(0x0F12, 0x2C06);
s5k5caga_i2c_write(0x0F12, 0xD035);
s5k5caga_i2c_write(0x0F12, 0x2C07);
s5k5caga_i2c_write(0x0F12, 0xD033);
s5k5caga_i2c_write(0x0F12, 0x48A3);
s5k5caga_i2c_write(0x0F12, 0x8BC1);
s5k5caga_i2c_write(0x0F12, 0x2900);
s5k5caga_i2c_write(0x0F12, 0xD02A);
s5k5caga_i2c_write(0x0F12, 0x00A2);
s5k5caga_i2c_write(0x0F12, 0x1815);
s5k5caga_i2c_write(0x0F12, 0x4AA4);
s5k5caga_i2c_write(0x0F12, 0x6DEE);
s5k5caga_i2c_write(0x0F12, 0x8A92);
s5k5caga_i2c_write(0x0F12, 0x4296);
s5k5caga_i2c_write(0x0F12, 0xD923);
s5k5caga_i2c_write(0x0F12, 0x0028);
s5k5caga_i2c_write(0x0F12, 0x3080);
s5k5caga_i2c_write(0x0F12, 0x0007);
s5k5caga_i2c_write(0x0F12, 0x69C0);
s5k5caga_i2c_write(0x0F12, 0xF000);
s5k5caga_i2c_write(0x0F12, 0xF96B);
s5k5caga_i2c_write(0x0F12, 0x1C71);
s5k5caga_i2c_write(0x0F12, 0x0280);
s5k5caga_i2c_write(0x0F12, 0xF000);
s5k5caga_i2c_write(0x0F12, 0xF967);
s5k5caga_i2c_write(0x0F12, 0x0006);
s5k5caga_i2c_write(0x0F12, 0x4898);
s5k5caga_i2c_write(0x0F12, 0x0061);
s5k5caga_i2c_write(0x0F12, 0x1808);
s5k5caga_i2c_write(0x0F12, 0x8D80);
s5k5caga_i2c_write(0x0F12, 0x0A01);
s5k5caga_i2c_write(0x0F12, 0x0600);
s5k5caga_i2c_write(0x0F12, 0x0E00);
s5k5caga_i2c_write(0x0F12, 0x1A08);
s5k5caga_i2c_write(0x0F12, 0xF000);
s5k5caga_i2c_write(0x0F12, 0xF96A);
s5k5caga_i2c_write(0x0F12, 0x0002);
s5k5caga_i2c_write(0x0F12, 0x6DE9);
s5k5caga_i2c_write(0x0F12, 0x6FE8);
s5k5caga_i2c_write(0x0F12, 0x1A08);
s5k5caga_i2c_write(0x0F12, 0x4351);
s5k5caga_i2c_write(0x0F12, 0x0300);
s5k5caga_i2c_write(0x0F12, 0x1C49);
s5k5caga_i2c_write(0x0F12, 0xF000);
s5k5caga_i2c_write(0x0F12, 0xF953);
s5k5caga_i2c_write(0x0F12, 0x0401);
s5k5caga_i2c_write(0x0F12, 0x0430);
s5k5caga_i2c_write(0x0F12, 0x0C00);
s5k5caga_i2c_write(0x0F12, 0x4301);
s5k5caga_i2c_write(0x0F12, 0x61F9);
s5k5caga_i2c_write(0x0F12, 0xE004);
s5k5caga_i2c_write(0x0F12, 0x00A2);
s5k5caga_i2c_write(0x0F12, 0x4990);
s5k5caga_i2c_write(0x0F12, 0x1810);
s5k5caga_i2c_write(0x0F12, 0x3080);
s5k5caga_i2c_write(0x0F12, 0x61C1);
s5k5caga_i2c_write(0x0F12, 0x1E64);
s5k5caga_i2c_write(0x0F12, 0xD2C5);
s5k5caga_i2c_write(0x0F12, 0x2006);
s5k5caga_i2c_write(0x0F12, 0xF000);
s5k5caga_i2c_write(0x0F12, 0xF959);
s5k5caga_i2c_write(0x0F12, 0x2007);
s5k5caga_i2c_write(0x0F12, 0xF000);
s5k5caga_i2c_write(0x0F12, 0xF956);
s5k5caga_i2c_write(0x0F12, 0xBCF8);
s5k5caga_i2c_write(0x0F12, 0xBC08);
s5k5caga_i2c_write(0x0F12, 0x4718);
s5k5caga_i2c_write(0x0F12, 0xB510);
s5k5caga_i2c_write(0x0F12, 0xF000);
s5k5caga_i2c_write(0x0F12, 0xF958);
s5k5caga_i2c_write(0x0F12, 0x2800);
s5k5caga_i2c_write(0x0F12, 0xD00A);
s5k5caga_i2c_write(0x0F12, 0x4881);
s5k5caga_i2c_write(0x0F12, 0x8B81);
s5k5caga_i2c_write(0x0F12, 0x0089);
s5k5caga_i2c_write(0x0F12, 0x1808);
s5k5caga_i2c_write(0x0F12, 0x6DC1);
s5k5caga_i2c_write(0x0F12, 0x4883);
s5k5caga_i2c_write(0x0F12, 0x8A80);
s5k5caga_i2c_write(0x0F12, 0x4281);
s5k5caga_i2c_write(0x0F12, 0xD901);
s5k5caga_i2c_write(0x0F12, 0x2001);
s5k5caga_i2c_write(0x0F12, 0xE7A1);
s5k5caga_i2c_write(0x0F12, 0x2000);
s5k5caga_i2c_write(0x0F12, 0xE79F);
s5k5caga_i2c_write(0x0F12, 0xB5F8);
s5k5caga_i2c_write(0x0F12, 0x0004);
s5k5caga_i2c_write(0x0F12, 0x4F80);
s5k5caga_i2c_write(0x0F12, 0x227D);
s5k5caga_i2c_write(0x0F12, 0x8938);
s5k5caga_i2c_write(0x0F12, 0x0152);
s5k5caga_i2c_write(0x0F12, 0x4342);
s5k5caga_i2c_write(0x0F12, 0x487E);
s5k5caga_i2c_write(0x0F12, 0x9000);
s5k5caga_i2c_write(0x0F12, 0x8A01);
s5k5caga_i2c_write(0x0F12, 0x0848);
s5k5caga_i2c_write(0x0F12, 0x1810);
s5k5caga_i2c_write(0x0F12, 0xF000);
s5k5caga_i2c_write(0x0F12, 0xF91D);
s5k5caga_i2c_write(0x0F12, 0x210F);
s5k5caga_i2c_write(0x0F12, 0xF000);
s5k5caga_i2c_write(0x0F12, 0xF940);
s5k5caga_i2c_write(0x0F12, 0x497A);
s5k5caga_i2c_write(0x0F12, 0x8C49);
s5k5caga_i2c_write(0x0F12, 0x090E);
s5k5caga_i2c_write(0x0F12, 0x0136);
s5k5caga_i2c_write(0x0F12, 0x4306);
s5k5caga_i2c_write(0x0F12, 0x4979);
s5k5caga_i2c_write(0x0F12, 0x2C00);
s5k5caga_i2c_write(0x0F12, 0xD003);
s5k5caga_i2c_write(0x0F12, 0x2001);
s5k5caga_i2c_write(0x0F12, 0x0240);
s5k5caga_i2c_write(0x0F12, 0x4330);
s5k5caga_i2c_write(0x0F12, 0x8108);
s5k5caga_i2c_write(0x0F12, 0x4876);
s5k5caga_i2c_write(0x0F12, 0x2C00);
s5k5caga_i2c_write(0x0F12, 0x8D00);
s5k5caga_i2c_write(0x0F12, 0xD001);
s5k5caga_i2c_write(0x0F12, 0x2501);
s5k5caga_i2c_write(0x0F12, 0xE000);
s5k5caga_i2c_write(0x0F12, 0x2500);
s5k5caga_i2c_write(0x0F12, 0x4972);
s5k5caga_i2c_write(0x0F12, 0x4328);
s5k5caga_i2c_write(0x0F12, 0x8008);
s5k5caga_i2c_write(0x0F12, 0x207D);
s5k5caga_i2c_write(0x0F12, 0x00C0);
s5k5caga_i2c_write(0x0F12, 0xF000);
s5k5caga_i2c_write(0x0F12, 0xF92E);
s5k5caga_i2c_write(0x0F12, 0x2C00);
s5k5caga_i2c_write(0x0F12, 0x496E);
s5k5caga_i2c_write(0x0F12, 0x0328);
s5k5caga_i2c_write(0x0F12, 0x4330);
s5k5caga_i2c_write(0x0F12, 0x8108);
s5k5caga_i2c_write(0x0F12, 0x88F8);
s5k5caga_i2c_write(0x0F12, 0x2C00);
s5k5caga_i2c_write(0x0F12, 0x01AA);
s5k5caga_i2c_write(0x0F12, 0x4310);
s5k5caga_i2c_write(0x0F12, 0x8088);
s5k5caga_i2c_write(0x0F12, 0x9800);
s5k5caga_i2c_write(0x0F12, 0x8A01);
s5k5caga_i2c_write(0x0F12, 0x486A);
s5k5caga_i2c_write(0x0F12, 0xF000);
s5k5caga_i2c_write(0x0F12, 0xF8F1);
s5k5caga_i2c_write(0x0F12, 0x496A);
s5k5caga_i2c_write(0x0F12, 0x8809);
s5k5caga_i2c_write(0x0F12, 0x4348);
s5k5caga_i2c_write(0x0F12, 0x0400);
s5k5caga_i2c_write(0x0F12, 0x0C00);
s5k5caga_i2c_write(0x0F12, 0xF000);
s5k5caga_i2c_write(0x0F12, 0xF918);
s5k5caga_i2c_write(0x0F12, 0x0020);
s5k5caga_i2c_write(0x0F12, 0xF000);
s5k5caga_i2c_write(0x0F12, 0xF91D);
s5k5caga_i2c_write(0x0F12, 0x4866);
s5k5caga_i2c_write(0x0F12, 0x7004);
s5k5caga_i2c_write(0x0F12, 0xE7A3);
s5k5caga_i2c_write(0x0F12, 0xB510);
s5k5caga_i2c_write(0x0F12, 0x0004);
s5k5caga_i2c_write(0x0F12, 0xF000);
s5k5caga_i2c_write(0x0F12, 0xF91E);
s5k5caga_i2c_write(0x0F12, 0x6020);
s5k5caga_i2c_write(0x0F12, 0x4963);
s5k5caga_i2c_write(0x0F12, 0x8B49);
s5k5caga_i2c_write(0x0F12, 0x0789);
s5k5caga_i2c_write(0x0F12, 0xD001);
s5k5caga_i2c_write(0x0F12, 0x0040);
s5k5caga_i2c_write(0x0F12, 0x6020);
s5k5caga_i2c_write(0x0F12, 0xE74C);
s5k5caga_i2c_write(0x0F12, 0xB510);
s5k5caga_i2c_write(0x0F12, 0xF000);
s5k5caga_i2c_write(0x0F12, 0xF91B);
s5k5caga_i2c_write(0x0F12, 0x485F);
s5k5caga_i2c_write(0x0F12, 0x8880);
s5k5caga_i2c_write(0x0F12, 0x0601);
s5k5caga_i2c_write(0x0F12, 0x4854);
s5k5caga_i2c_write(0x0F12, 0x1609);
s5k5caga_i2c_write(0x0F12, 0x8141);
s5k5caga_i2c_write(0x0F12, 0xE742);
s5k5caga_i2c_write(0x0F12, 0xB5F8);
s5k5caga_i2c_write(0x0F12, 0x000F);
s5k5caga_i2c_write(0x0F12, 0x4C55);
s5k5caga_i2c_write(0x0F12, 0x3420);
s5k5caga_i2c_write(0x0F12, 0x2500);
s5k5caga_i2c_write(0x0F12, 0x5765);
s5k5caga_i2c_write(0x0F12, 0x0039);
s5k5caga_i2c_write(0x0F12, 0xF000);
s5k5caga_i2c_write(0x0F12, 0xF913);
s5k5caga_i2c_write(0x0F12, 0x9000);
s5k5caga_i2c_write(0x0F12, 0x2600);
s5k5caga_i2c_write(0x0F12, 0x57A6);
s5k5caga_i2c_write(0x0F12, 0x4C4C);
s5k5caga_i2c_write(0x0F12, 0x42AE);
s5k5caga_i2c_write(0x0F12, 0xD01B);
s5k5caga_i2c_write(0x0F12, 0x4D54);
s5k5caga_i2c_write(0x0F12, 0x8AE8);
s5k5caga_i2c_write(0x0F12, 0x2800);
s5k5caga_i2c_write(0x0F12, 0xD013);
s5k5caga_i2c_write(0x0F12, 0x484D);
s5k5caga_i2c_write(0x0F12, 0x8A01);
s5k5caga_i2c_write(0x0F12, 0x8B80);
s5k5caga_i2c_write(0x0F12, 0x4378);
s5k5caga_i2c_write(0x0F12, 0xF000);
s5k5caga_i2c_write(0x0F12, 0xF8B5);
s5k5caga_i2c_write(0x0F12, 0x89A9);
s5k5caga_i2c_write(0x0F12, 0x1A41);
s5k5caga_i2c_write(0x0F12, 0x484E);
s5k5caga_i2c_write(0x0F12, 0x3820);
s5k5caga_i2c_write(0x0F12, 0x8AC0);
s5k5caga_i2c_write(0x0F12, 0x4348);
s5k5caga_i2c_write(0x0F12, 0x17C1);
s5k5caga_i2c_write(0x0F12, 0x0D89);
s5k5caga_i2c_write(0x0F12, 0x1808);
s5k5caga_i2c_write(0x0F12, 0x1280);
s5k5caga_i2c_write(0x0F12, 0x8961);
s5k5caga_i2c_write(0x0F12, 0x1A08);
s5k5caga_i2c_write(0x0F12, 0x8160);
s5k5caga_i2c_write(0x0F12, 0xE003);
s5k5caga_i2c_write(0x0F12, 0x88A8);
s5k5caga_i2c_write(0x0F12, 0x0600);
s5k5caga_i2c_write(0x0F12, 0x1600);
s5k5caga_i2c_write(0x0F12, 0x8160);
s5k5caga_i2c_write(0x0F12, 0x200A);
s5k5caga_i2c_write(0x0F12, 0x5E20);
s5k5caga_i2c_write(0x0F12, 0x42B0);
s5k5caga_i2c_write(0x0F12, 0xD011);
s5k5caga_i2c_write(0x0F12, 0xF000);
s5k5caga_i2c_write(0x0F12, 0xF8AB);
s5k5caga_i2c_write(0x0F12, 0x1D40);
s5k5caga_i2c_write(0x0F12, 0x00C3);
s5k5caga_i2c_write(0x0F12, 0x1A18);
s5k5caga_i2c_write(0x0F12, 0x214B);
s5k5caga_i2c_write(0x0F12, 0xF000);
s5k5caga_i2c_write(0x0F12, 0xF897);
s5k5caga_i2c_write(0x0F12, 0x211F);
s5k5caga_i2c_write(0x0F12, 0xF000);
s5k5caga_i2c_write(0x0F12, 0xF8BA);
s5k5caga_i2c_write(0x0F12, 0x210A);
s5k5caga_i2c_write(0x0F12, 0x5E61);
s5k5caga_i2c_write(0x0F12, 0x0FC9);
s5k5caga_i2c_write(0x0F12, 0x0149);
s5k5caga_i2c_write(0x0F12, 0x4301);
s5k5caga_i2c_write(0x0F12, 0x483D);
s5k5caga_i2c_write(0x0F12, 0x81C1);
s5k5caga_i2c_write(0x0F12, 0x9800);
s5k5caga_i2c_write(0x0F12, 0xE74A);
s5k5caga_i2c_write(0x0F12, 0xB5F1);
s5k5caga_i2c_write(0x0F12, 0xB082);
s5k5caga_i2c_write(0x0F12, 0x2500);
s5k5caga_i2c_write(0x0F12, 0x483A);
s5k5caga_i2c_write(0x0F12, 0x9001);
s5k5caga_i2c_write(0x0F12, 0x2400);
s5k5caga_i2c_write(0x0F12, 0x2028);
s5k5caga_i2c_write(0x0F12, 0x4368);
s5k5caga_i2c_write(0x0F12, 0x4A39);
s5k5caga_i2c_write(0x0F12, 0x4925);
s5k5caga_i2c_write(0x0F12, 0x1887);
s5k5caga_i2c_write(0x0F12, 0x1840);
s5k5caga_i2c_write(0x0F12, 0x9000);
s5k5caga_i2c_write(0x0F12, 0x9800);
s5k5caga_i2c_write(0x0F12, 0x0066);
s5k5caga_i2c_write(0x0F12, 0x9A01);
s5k5caga_i2c_write(0x0F12, 0x1980);
s5k5caga_i2c_write(0x0F12, 0x218C);
s5k5caga_i2c_write(0x0F12, 0x5A09);
s5k5caga_i2c_write(0x0F12, 0x8A80);
s5k5caga_i2c_write(0x0F12, 0x8812);
s5k5caga_i2c_write(0x0F12, 0xF000);
s5k5caga_i2c_write(0x0F12, 0xF8CA);
s5k5caga_i2c_write(0x0F12, 0x53B8);
s5k5caga_i2c_write(0x0F12, 0x1C64);
s5k5caga_i2c_write(0x0F12, 0x2C14);
s5k5caga_i2c_write(0x0F12, 0xDBF1);
s5k5caga_i2c_write(0x0F12, 0x1C6D);
s5k5caga_i2c_write(0x0F12, 0x2D03);
s5k5caga_i2c_write(0x0F12, 0xDBE6);
s5k5caga_i2c_write(0x0F12, 0x9802);
s5k5caga_i2c_write(0x0F12, 0x6800);
s5k5caga_i2c_write(0x0F12, 0x0600);
s5k5caga_i2c_write(0x0F12, 0x0E00);
s5k5caga_i2c_write(0x0F12, 0xF000);
s5k5caga_i2c_write(0x0F12, 0xF8C5);
s5k5caga_i2c_write(0x0F12, 0xBCFE);
s5k5caga_i2c_write(0x0F12, 0xBC08);
s5k5caga_i2c_write(0x0F12, 0x4718);
s5k5caga_i2c_write(0x0F12, 0xB570);
s5k5caga_i2c_write(0x0F12, 0x6805);
s5k5caga_i2c_write(0x0F12, 0x2404);
s5k5caga_i2c_write(0x0F12, 0xF000);
s5k5caga_i2c_write(0x0F12, 0xF8C5);
s5k5caga_i2c_write(0x0F12, 0x2800);
s5k5caga_i2c_write(0x0F12, 0xD103);
s5k5caga_i2c_write(0x0F12, 0xF000);
s5k5caga_i2c_write(0x0F12, 0xF8C9);
s5k5caga_i2c_write(0x0F12, 0x2800);
s5k5caga_i2c_write(0x0F12, 0xD000);
s5k5caga_i2c_write(0x0F12, 0x2400);
s5k5caga_i2c_write(0x0F12, 0x3540);
s5k5caga_i2c_write(0x0F12, 0x88E8);
s5k5caga_i2c_write(0x0F12, 0x0500);
s5k5caga_i2c_write(0x0F12, 0xD403);
s5k5caga_i2c_write(0x0F12, 0x4822);
s5k5caga_i2c_write(0x0F12, 0x89C0);
s5k5caga_i2c_write(0x0F12, 0x2800);
s5k5caga_i2c_write(0x0F12, 0xD002);
s5k5caga_i2c_write(0x0F12, 0x2008);
s5k5caga_i2c_write(0x0F12, 0x4304);
s5k5caga_i2c_write(0x0F12, 0xE001);
s5k5caga_i2c_write(0x0F12, 0x2010);
s5k5caga_i2c_write(0x0F12, 0x4304);
s5k5caga_i2c_write(0x0F12, 0x481F);
s5k5caga_i2c_write(0x0F12, 0x8B80);
s5k5caga_i2c_write(0x0F12, 0x0700);
s5k5caga_i2c_write(0x0F12, 0x0F81);
s5k5caga_i2c_write(0x0F12, 0x2001);
s5k5caga_i2c_write(0x0F12, 0x2900);
s5k5caga_i2c_write(0x0F12, 0xD000);
s5k5caga_i2c_write(0x0F12, 0x4304);
s5k5caga_i2c_write(0x0F12, 0x491C);
s5k5caga_i2c_write(0x0F12, 0x8B0A);
s5k5caga_i2c_write(0x0F12, 0x42A2);
s5k5caga_i2c_write(0x0F12, 0xD004);
s5k5caga_i2c_write(0x0F12, 0x0762);
s5k5caga_i2c_write(0x0F12, 0xD502);
s5k5caga_i2c_write(0x0F12, 0x4A19);
s5k5caga_i2c_write(0x0F12, 0x3220);
s5k5caga_i2c_write(0x0F12, 0x8110);
s5k5caga_i2c_write(0x0F12, 0x830C);
s5k5caga_i2c_write(0x0F12, 0xE691);
s5k5caga_i2c_write(0x0F12, 0x0C3C);
s5k5caga_i2c_write(0x0F12, 0x7000);
s5k5caga_i2c_write(0x0F12, 0x3274);
s5k5caga_i2c_write(0x0F12, 0x7000);
s5k5caga_i2c_write(0x0F12, 0x26E8);
s5k5caga_i2c_write(0x0F12, 0x7000);
s5k5caga_i2c_write(0x0F12, 0x6100);
s5k5caga_i2c_write(0x0F12, 0xD000);
s5k5caga_i2c_write(0x0F12, 0x6500);
s5k5caga_i2c_write(0x0F12, 0xD000);
s5k5caga_i2c_write(0x0F12, 0x1A7C);
s5k5caga_i2c_write(0x0F12, 0x7000);
s5k5caga_i2c_write(0x0F12, 0x1120);
s5k5caga_i2c_write(0x0F12, 0x7000);
s5k5caga_i2c_write(0x0F12, 0xFFFF);
s5k5caga_i2c_write(0x0F12, 0x0000);
s5k5caga_i2c_write(0x0F12, 0x3374);
s5k5caga_i2c_write(0x0F12, 0x7000);
s5k5caga_i2c_write(0x0F12, 0x1D6C);
s5k5caga_i2c_write(0x0F12, 0x7000);
s5k5caga_i2c_write(0x0F12, 0x167C);
s5k5caga_i2c_write(0x0F12, 0x7000);
s5k5caga_i2c_write(0x0F12, 0xF400);
s5k5caga_i2c_write(0x0F12, 0xD000);
s5k5caga_i2c_write(0x0F12, 0x2C2C);
s5k5caga_i2c_write(0x0F12, 0x7000);
s5k5caga_i2c_write(0x0F12, 0x40A0);
s5k5caga_i2c_write(0x0F12, 0x00DD);
s5k5caga_i2c_write(0x0F12, 0xF520);
s5k5caga_i2c_write(0x0F12, 0xD000);
s5k5caga_i2c_write(0x0F12, 0x2C29);
s5k5caga_i2c_write(0x0F12, 0x7000);
s5k5caga_i2c_write(0x0F12, 0x1A54);
s5k5caga_i2c_write(0x0F12, 0x7000);
s5k5caga_i2c_write(0x0F12, 0x1564);
s5k5caga_i2c_write(0x0F12, 0x7000);
s5k5caga_i2c_write(0x0F12, 0xF2A0);
s5k5caga_i2c_write(0x0F12, 0xD000);
s5k5caga_i2c_write(0x0F12, 0x2440);
s5k5caga_i2c_write(0x0F12, 0x7000);
s5k5caga_i2c_write(0x0F12, 0x05A0);
s5k5caga_i2c_write(0x0F12, 0x7000);
s5k5caga_i2c_write(0x0F12, 0x2894);
s5k5caga_i2c_write(0x0F12, 0x7000);
s5k5caga_i2c_write(0x0F12, 0x1224);
s5k5caga_i2c_write(0x0F12, 0x7000);
s5k5caga_i2c_write(0x0F12, 0xB000);
s5k5caga_i2c_write(0x0F12, 0xD000);
s5k5caga_i2c_write(0x0F12, 0x4778);
s5k5caga_i2c_write(0x0F12, 0x46C0);
s5k5caga_i2c_write(0x0F12, 0xC000);
s5k5caga_i2c_write(0x0F12, 0xE59F);
s5k5caga_i2c_write(0x0F12, 0xFF1C);
s5k5caga_i2c_write(0x0F12, 0xE12F);
s5k5caga_i2c_write(0x0F12, 0x1A3F);
s5k5caga_i2c_write(0x0F12, 0x0001);
s5k5caga_i2c_write(0x0F12, 0x4778);
s5k5caga_i2c_write(0x0F12, 0x46C0);
s5k5caga_i2c_write(0x0F12, 0xF004);
s5k5caga_i2c_write(0x0F12, 0xE51F);
s5k5caga_i2c_write(0x0F12, 0x1F48);
s5k5caga_i2c_write(0x0F12, 0x0001);
s5k5caga_i2c_write(0x0F12, 0x4778);
s5k5caga_i2c_write(0x0F12, 0x46C0);
s5k5caga_i2c_write(0x0F12, 0xC000);
s5k5caga_i2c_write(0x0F12, 0xE59F);
s5k5caga_i2c_write(0x0F12, 0xFF1C);
s5k5caga_i2c_write(0x0F12, 0xE12F);
s5k5caga_i2c_write(0x0F12, 0x24BD);
s5k5caga_i2c_write(0x0F12, 0x0000);
s5k5caga_i2c_write(0x0F12, 0x4778);
s5k5caga_i2c_write(0x0F12, 0x46C0);
s5k5caga_i2c_write(0x0F12, 0xC000);
s5k5caga_i2c_write(0x0F12, 0xE59F);
s5k5caga_i2c_write(0x0F12, 0xFF1C);
s5k5caga_i2c_write(0x0F12, 0xE12F);
s5k5caga_i2c_write(0x0F12, 0x36DD);
s5k5caga_i2c_write(0x0F12, 0x0000);
s5k5caga_i2c_write(0x0F12, 0x4778);
s5k5caga_i2c_write(0x0F12, 0x46C0);
s5k5caga_i2c_write(0x0F12, 0xC000);
s5k5caga_i2c_write(0x0F12, 0xE59F);
s5k5caga_i2c_write(0x0F12, 0xFF1C);
s5k5caga_i2c_write(0x0F12, 0xE12F);
s5k5caga_i2c_write(0x0F12, 0xB4CF);
s5k5caga_i2c_write(0x0F12, 0x0000);
s5k5caga_i2c_write(0x0F12, 0x4778);
s5k5caga_i2c_write(0x0F12, 0x46C0);
s5k5caga_i2c_write(0x0F12, 0xC000);
s5k5caga_i2c_write(0x0F12, 0xE59F);
s5k5caga_i2c_write(0x0F12, 0xFF1C);
s5k5caga_i2c_write(0x0F12, 0xE12F);
s5k5caga_i2c_write(0x0F12, 0xB5D7);
s5k5caga_i2c_write(0x0F12, 0x0000);
s5k5caga_i2c_write(0x0F12, 0x4778);
s5k5caga_i2c_write(0x0F12, 0x46C0);
s5k5caga_i2c_write(0x0F12, 0xC000);
s5k5caga_i2c_write(0x0F12, 0xE59F);
s5k5caga_i2c_write(0x0F12, 0xFF1C);
s5k5caga_i2c_write(0x0F12, 0xE12F);
s5k5caga_i2c_write(0x0F12, 0x36ED);
s5k5caga_i2c_write(0x0F12, 0x0000);
s5k5caga_i2c_write(0x0F12, 0x4778);
s5k5caga_i2c_write(0x0F12, 0x46C0);
s5k5caga_i2c_write(0x0F12, 0xC000);
s5k5caga_i2c_write(0x0F12, 0xE59F);
s5k5caga_i2c_write(0x0F12, 0xFF1C);
s5k5caga_i2c_write(0x0F12, 0xE12F);
s5k5caga_i2c_write(0x0F12, 0xF53F);
s5k5caga_i2c_write(0x0F12, 0x0000);
s5k5caga_i2c_write(0x0F12, 0x4778);
s5k5caga_i2c_write(0x0F12, 0x46C0);
s5k5caga_i2c_write(0x0F12, 0xC000);
s5k5caga_i2c_write(0x0F12, 0xE59F);
s5k5caga_i2c_write(0x0F12, 0xFF1C);
s5k5caga_i2c_write(0x0F12, 0xE12F);
s5k5caga_i2c_write(0x0F12, 0xF5D9);
s5k5caga_i2c_write(0x0F12, 0x0000);
s5k5caga_i2c_write(0x0F12, 0x4778);
s5k5caga_i2c_write(0x0F12, 0x46C0);
s5k5caga_i2c_write(0x0F12, 0xC000);
s5k5caga_i2c_write(0x0F12, 0xE59F);
s5k5caga_i2c_write(0x0F12, 0xFF1C);
s5k5caga_i2c_write(0x0F12, 0xE12F);
s5k5caga_i2c_write(0x0F12, 0x013D);
s5k5caga_i2c_write(0x0F12, 0x0001);
s5k5caga_i2c_write(0x0F12, 0x4778);
s5k5caga_i2c_write(0x0F12, 0x46C0);
s5k5caga_i2c_write(0x0F12, 0xC000);
s5k5caga_i2c_write(0x0F12, 0xE59F);
s5k5caga_i2c_write(0x0F12, 0xFF1C);
s5k5caga_i2c_write(0x0F12, 0xE12F);
s5k5caga_i2c_write(0x0F12, 0xF5C9);
s5k5caga_i2c_write(0x0F12, 0x0000);
s5k5caga_i2c_write(0x0F12, 0x4778);
s5k5caga_i2c_write(0x0F12, 0x46C0);
s5k5caga_i2c_write(0x0F12, 0xC000);
s5k5caga_i2c_write(0x0F12, 0xE59F);
s5k5caga_i2c_write(0x0F12, 0xFF1C);
s5k5caga_i2c_write(0x0F12, 0xE12F);
s5k5caga_i2c_write(0x0F12, 0xFAA9);
s5k5caga_i2c_write(0x0F12, 0x0000);
s5k5caga_i2c_write(0x0F12, 0x4778);
s5k5caga_i2c_write(0x0F12, 0x46C0);
s5k5caga_i2c_write(0x0F12, 0xC000);
s5k5caga_i2c_write(0x0F12, 0xE59F);
s5k5caga_i2c_write(0x0F12, 0xFF1C);
s5k5caga_i2c_write(0x0F12, 0xE12F);
s5k5caga_i2c_write(0x0F12, 0x3723);
s5k5caga_i2c_write(0x0F12, 0x0000);
s5k5caga_i2c_write(0x0F12, 0x4778);
s5k5caga_i2c_write(0x0F12, 0x46C0);
s5k5caga_i2c_write(0x0F12, 0xC000);
s5k5caga_i2c_write(0x0F12, 0xE59F);
s5k5caga_i2c_write(0x0F12, 0xFF1C);
s5k5caga_i2c_write(0x0F12, 0xE12F);
s5k5caga_i2c_write(0x0F12, 0x5823);
s5k5caga_i2c_write(0x0F12, 0x0000);
s5k5caga_i2c_write(0x0F12, 0x4778);
s5k5caga_i2c_write(0x0F12, 0x46C0);
s5k5caga_i2c_write(0x0F12, 0xC000);
s5k5caga_i2c_write(0x0F12, 0xE59F);
s5k5caga_i2c_write(0x0F12, 0xFF1C);
s5k5caga_i2c_write(0x0F12, 0xE12F);
s5k5caga_i2c_write(0x0F12, 0xD771);
s5k5caga_i2c_write(0x0F12, 0x0000);
s5k5caga_i2c_write(0x0F12, 0x4778);
s5k5caga_i2c_write(0x0F12, 0x46C0);
s5k5caga_i2c_write(0x0F12, 0xC000);
s5k5caga_i2c_write(0x0F12, 0xE59F);
s5k5caga_i2c_write(0x0F12, 0xFF1C);
s5k5caga_i2c_write(0x0F12, 0xE12F);
s5k5caga_i2c_write(0x0F12, 0xD75B);
s5k5caga_i2c_write(0x0F12, 0x0000);
s5k5caga_i2c_write(0x0F12, 0x8117);
s5k5caga_i2c_write(0x0F12, 0x0000);

// End T&P part

//========================================================        
// CIs/APs/An setting    - 400LsB  sYsCLK 32MHz           
//========================================================        
// This regis are for FACTORY ONLY. If you change it without prior notification,
// YOU are REsIBLE for the FAILURE that will happen in the future.      
//========================================================        


s5k5caga_i2c_write(0x002A, 0x157A);
s5k5caga_i2c_write(0x0F12, 0x0001);
s5k5caga_i2c_write(0x002A, 0x1578);
s5k5caga_i2c_write(0x0F12, 0x0001);
s5k5caga_i2c_write(0x002A, 0x1576);
s5k5caga_i2c_write(0x0F12, 0x0020);
s5k5caga_i2c_write(0x002A, 0x1574);
s5k5caga_i2c_write(0x0F12, 0x0006);
s5k5caga_i2c_write(0x002A, 0x156E);
s5k5caga_i2c_write(0x0F12, 0x0001); // slope calibration tolerance in units of 1/256 
s5k5caga_i2c_write(0x002A, 0x1568);
s5k5caga_i2c_write(0x0F12, 0x00FC);

//ADC control 
s5k5caga_i2c_write(0x002A, 0x155A);
s5k5caga_i2c_write(0x0F12, 0x01CC); //ADC sAT of 450mV for 10bit default in EVT1          
s5k5caga_i2c_write(0x002A, 0x157E);                        
s5k5caga_i2c_write(0x0F12, 0x0C80); // 3200 Max. Reset ramp DCLK counts (default 2048 0x800)     
s5k5caga_i2c_write(0x0F12, 0x0578); // 1400 Max. Reset ramp DCLK counts for x3.5         
s5k5caga_i2c_write(0x002A, 0x157C);                        
s5k5caga_i2c_write(0x0F12, 0x0190); // 400 Reset ramp for x1 in DCLK counts          
s5k5caga_i2c_write(0x002A, 0x1570);                        
s5k5caga_i2c_write(0x0F12, 0x00A0); // 160 LsB                     
s5k5caga_i2c_write(0x0F12, 0x0010); // reset threshold                 
s5k5caga_i2c_write(0x002A, 0x12C4);                        
s5k5caga_i2c_write(0x0F12, 0x006A); // 106 additional timing columns.            
s5k5caga_i2c_write(0x002A, 0x12C8);                        
s5k5caga_i2c_write(0x0F12, 0x08AC); // 2220 ADC columns in normal mode including Hold & Latch    
s5k5caga_i2c_write(0x0F12, 0x0050); // 80 addition of ADC columns in Y-ave mode (default 244 0x74)

s5k5caga_i2c_write(0x002A, 0x1696); // based on APs guidelines        
s5k5caga_i2c_write(0x0F12, 0x0000); // based on APs guidelines        
s5k5caga_i2c_write(0x0F12, 0x0000); // default. 1492 used for ADC dark characteristics
s5k5caga_i2c_write(0x0F12, 0x00C6); // default. 1492 used for ADC dark characteristics
s5k5caga_i2c_write(0x0F12, 0x00C6);                                   

s5k5caga_i2c_write(0x002A, 0x1690); // when set double sampling is activated - requires different set of pointers                 
s5k5caga_i2c_write(0x0F12, 0x0001);                   

s5k5caga_i2c_write(0x002A, 0x12B0); // comp and pixel bias control 0xF40E - default for EVT1                        
s5k5caga_i2c_write(0x0F12, 0x0055); // comp and pixel bias control 0xF40E for binning mode                        
s5k5caga_i2c_write(0x0F12, 0x005A);                   

s5k5caga_i2c_write(0x002A, 0x337A); // [7] - is used for rest-only mode (EVT0 value is 0xD and HW 0x6)                    
s5k5caga_i2c_write(0x0F12, 0x0006);
s5k5caga_i2c_write(0x0F12, 0x0068);
s5k5caga_i2c_write(0x002A, 0x169E);
s5k5caga_i2c_write(0x0F12, 0x0007);
s5k5caga_i2c_write(0x002A, 0x0BF6);
s5k5caga_i2c_write(0x0F12, 0x0000);


s5k5caga_i2c_write(0x002A, 0x327C);
s5k5caga_i2c_write(0x0F12, 0x1000);
s5k5caga_i2c_write(0x0F12, 0x6998);
s5k5caga_i2c_write(0x0F12, 0x0078);
s5k5caga_i2c_write(0x0F12, 0x04FE);
s5k5caga_i2c_write(0x0F12, 0x8800);

s5k5caga_i2c_write(0x002A, 0x3274);
s5k5caga_i2c_write(0x0F12, 0x0155); //set IO driving current 2mA for Gs500
s5k5caga_i2c_write(0x0F12, 0x0155); //set IO driving current      
s5k5caga_i2c_write(0x0F12, 0x1555); //set IO driving current      
s5k5caga_i2c_write(0x0F12, 0x05D5);//0555 //set IO driving current      

s5k5caga_i2c_write(0x0028, 0x7000);
s5k5caga_i2c_write(0x002A, 0x0572);
s5k5caga_i2c_write(0x0F12, 0x0007); //#skl_usConfigStbySettings // Enable T&P code after HW stby + skip ZI part on HW wakeup.

s5k5caga_i2c_write(0x0028, 0x7000); 
s5k5caga_i2c_write(0x002A, 0x12D2);  
s5k5caga_i2c_write(0x0F12, 0x0003); //senHal_pContSenModesRegsArray[0][0]2 700012D2   
s5k5caga_i2c_write(0x0F12, 0x0003); //senHal_pContSenModesRegsArray[0][1]2 700012D4  
s5k5caga_i2c_write(0x0F12, 0x0003); //senHal_pContSenModesRegsArray[0][2]2 700012D6  
s5k5caga_i2c_write(0x0F12, 0x0003); //senHal_pContSenModesRegsArray[0][3]2 700012D8  
s5k5caga_i2c_write(0x0F12, 0x0884); //senHal_pContSenModesRegsArray[1][0]2 700012DA  
s5k5caga_i2c_write(0x0F12, 0x08CF); //senHal_pContSenModesRegsArray[1][1]2 700012DC  
s5k5caga_i2c_write(0x0F12, 0x0500); //senHal_pContSenModesRegsArray[1][2]2 700012DE  
s5k5caga_i2c_write(0x0F12, 0x054B); //senHal_pContSenModesRegsArray[1][3]2 700012E0  
s5k5caga_i2c_write(0x0F12, 0x0001); //senHal_pContSenModesRegsArray[2][0]2 700012E2  
s5k5caga_i2c_write(0x0F12, 0x0001); //senHal_pContSenModesRegsArray[2][1]2 700012E4  
s5k5caga_i2c_write(0x0F12, 0x0001); //senHal_pContSenModesRegsArray[2][2]2 700012E6  
s5k5caga_i2c_write(0x0F12, 0x0001); //senHal_pContSenModesRegsArray[2][3]2 700012E8  
s5k5caga_i2c_write(0x0F12, 0x0885); //senHal_pContSenModesRegsArray[3][0]2 700012EA  
s5k5caga_i2c_write(0x0F12, 0x0467); //senHal_pContSenModesRegsArray[3][1]2 700012EC  
s5k5caga_i2c_write(0x0F12, 0x0501); //senHal_pContSenModesRegsArray[3][2]2 700012EE  
s5k5caga_i2c_write(0x0F12, 0x02A5); //senHal_pContSenModesRegsArray[3][3]2 700012F0  
s5k5caga_i2c_write(0x0F12, 0x0001); //senHal_pContSenModesRegsArray[4][0]2 700012F2  
s5k5caga_i2c_write(0x0F12, 0x046A); //senHal_pContSenModesRegsArray[4][1]2 700012F4  
s5k5caga_i2c_write(0x0F12, 0x0001); //senHal_pContSenModesRegsArray[4][2]2 700012F6  
s5k5caga_i2c_write(0x0F12, 0x02A8); //senHal_pContSenModesRegsArray[4][3]2 700012F8  
s5k5caga_i2c_write(0x0F12, 0x0885); //senHal_pContSenModesRegsArray[5][0]2 700012FA  
s5k5caga_i2c_write(0x0F12, 0x08D0); //senHal_pContSenModesRegsArray[5][1]2 700012FC  
s5k5caga_i2c_write(0x0F12, 0x0501); //senHal_pContSenModesRegsArray[5][2]2 700012FE  
s5k5caga_i2c_write(0x0F12, 0x054C); //senHal_pContSenModesRegsArray[5][3]2 70001300  
s5k5caga_i2c_write(0x0F12, 0x0006); //senHal_pContSenModesRegsArray[6][0]2 70001302  
s5k5caga_i2c_write(0x0F12, 0x0020); //senHal_pContSenModesRegsArray[6][1]2 70001304  
s5k5caga_i2c_write(0x0F12, 0x0006); //senHal_pContSenModesRegsArray[6][2]2 70001306  
s5k5caga_i2c_write(0x0F12, 0x0020); //senHal_pContSenModesRegsArray[6][3]2 70001308  
s5k5caga_i2c_write(0x0F12, 0x0881); //senHal_pContSenModesRegsArray[7][0]2 7000130A  
s5k5caga_i2c_write(0x0F12, 0x0463); //senHal_pContSenModesRegsArray[7][1]2 7000130C  
s5k5caga_i2c_write(0x0F12, 0x04FD); //senHal_pContSenModesRegsArray[7][2]2 7000130E  
s5k5caga_i2c_write(0x0F12, 0x02A1); //senHal_pContSenModesRegsArray[7][3]2 70001310  
s5k5caga_i2c_write(0x0F12, 0x0006); //senHal_pContSenModesRegsArray[8][0]2 70001312  
s5k5caga_i2c_write(0x0F12, 0x0489); //senHal_pContSenModesRegsArray[8][1]2 70001314  
s5k5caga_i2c_write(0x0F12, 0x0006); //senHal_pContSenModesRegsArray[8][2]2 70001316  
s5k5caga_i2c_write(0x0F12, 0x02C7); //senHal_pContSenModesRegsArray[8][3]2 70001318  
s5k5caga_i2c_write(0x0F12, 0x0881); //senHal_pContSenModesRegsArray[9][0]2 7000131A  
s5k5caga_i2c_write(0x0F12, 0x08CC); //senHal_pContSenModesRegsArray[9][1]2 7000131C  
s5k5caga_i2c_write(0x0F12, 0x04FD); //senHal_pContSenModesRegsArray[9][2]2 7000131E  
s5k5caga_i2c_write(0x0F12, 0x0548); //senHal_pContSenModesRegsArray[9][3]2 70001320  
s5k5caga_i2c_write(0x0F12, 0x03A2); //senHal_pContSenModesRegsArray[10][0] 2 70001322
s5k5caga_i2c_write(0x0F12, 0x01D3); //senHal_pContSenModesRegsArray[10][1] 2 70001324
s5k5caga_i2c_write(0x0F12, 0x01E0); //senHal_pContSenModesRegsArray[10][2] 2 70001326
s5k5caga_i2c_write(0x0F12, 0x00F2); //senHal_pContSenModesRegsArray[10][3] 2 70001328
s5k5caga_i2c_write(0x0F12, 0x03F2); //senHal_pContSenModesRegsArray[11][0] 2 7000132A
s5k5caga_i2c_write(0x0F12, 0x0223); //senHal_pContSenModesRegsArray[11][1] 2 7000132C
s5k5caga_i2c_write(0x0F12, 0x0230); //senHal_pContSenModesRegsArray[11][2] 2 7000132E
s5k5caga_i2c_write(0x0F12, 0x0142); //senHal_pContSenModesRegsArray[11][3] 2 70001330
s5k5caga_i2c_write(0x0F12, 0x03A2); //senHal_pContSenModesRegsArray[12][0] 2 70001332
s5k5caga_i2c_write(0x0F12, 0x063C); //senHal_pContSenModesRegsArray[12][1] 2 70001334
s5k5caga_i2c_write(0x0F12, 0x01E0); //senHal_pContSenModesRegsArray[12][2] 2 70001336
s5k5caga_i2c_write(0x0F12, 0x0399); //senHal_pContSenModesRegsArray[12][3] 2 70001338
s5k5caga_i2c_write(0x0F12, 0x03F2); //senHal_pContSenModesRegsArray[13][0] 2 7000133A
s5k5caga_i2c_write(0x0F12, 0x068C); //senHal_pContSenModesRegsArray[13][1] 2 7000133C
s5k5caga_i2c_write(0x0F12, 0x0230); //senHal_pContSenModesRegsArray[13][2] 2 7000133E
s5k5caga_i2c_write(0x0F12, 0x03E9); //senHal_pContSenModesRegsArray[13][3] 2 70001340
s5k5caga_i2c_write(0x0F12, 0x0002); //senHal_pContSenModesRegsArray[14][0] 2 70001342
s5k5caga_i2c_write(0x0F12, 0x0002); //senHal_pContSenModesRegsArray[14][1] 2 70001344
s5k5caga_i2c_write(0x0F12, 0x0002); //senHal_pContSenModesRegsArray[14][2] 2 70001346
s5k5caga_i2c_write(0x0F12, 0x0002); //senHal_pContSenModesRegsArray[14][3] 2 70001348
s5k5caga_i2c_write(0x0F12, 0x003C); //senHal_pContSenModesRegsArray[15][0] 2 7000134A
s5k5caga_i2c_write(0x0F12, 0x003C); //senHal_pContSenModesRegsArray[15][1] 2 7000134C
s5k5caga_i2c_write(0x0F12, 0x003C); //senHal_pContSenModesRegsArray[15][2] 2 7000134E
s5k5caga_i2c_write(0x0F12, 0x003C); //senHal_pContSenModesRegsArray[15][3] 2 70001350
s5k5caga_i2c_write(0x0F12, 0x01D3); //senHal_pContSenModesRegsArray[16][0] 2 70001352
s5k5caga_i2c_write(0x0F12, 0x01D3); //senHal_pContSenModesRegsArray[16][1] 2 70001354
s5k5caga_i2c_write(0x0F12, 0x00F2); //senHal_pContSenModesRegsArray[16][2] 2 70001356
s5k5caga_i2c_write(0x0F12, 0x00F2); //senHal_pContSenModesRegsArray[16][3] 2 70001358
s5k5caga_i2c_write(0x0F12, 0x020B); //senHal_pContSenModesRegsArray[17][0] 2 7000135A
s5k5caga_i2c_write(0x0F12, 0x024A); //senHal_pContSenModesRegsArray[17][1] 2 7000135C
s5k5caga_i2c_write(0x0F12, 0x012A); //senHal_pContSenModesRegsArray[17][2] 2 7000135E
s5k5caga_i2c_write(0x0F12, 0x0169); //senHal_pContSenModesRegsArray[17][3] 2 70001360
s5k5caga_i2c_write(0x0F12, 0x0002); //senHal_pContSenModesRegsArray[18][0] 2 70001362
s5k5caga_i2c_write(0x0F12, 0x046B); //senHal_pContSenModesRegsArray[18][1] 2 70001364
s5k5caga_i2c_write(0x0F12, 0x0002); //senHal_pContSenModesRegsArray[18][2] 2 70001366
s5k5caga_i2c_write(0x0F12, 0x02A9); //senHal_pContSenModesRegsArray[18][3] 2 70001368
s5k5caga_i2c_write(0x0F12, 0x0419); //senHal_pContSenModesRegsArray[19][0] 2 7000136A
s5k5caga_i2c_write(0x0F12, 0x04A5); //senHal_pContSenModesRegsArray[19][1] 2 7000136C
s5k5caga_i2c_write(0x0F12, 0x0257); //senHal_pContSenModesRegsArray[19][2] 2 7000136E
s5k5caga_i2c_write(0x0F12, 0x02E3); //senHal_pContSenModesRegsArray[19][3] 2 70001370
s5k5caga_i2c_write(0x0F12, 0x0630); //senHal_pContSenModesRegsArray[20][0] 2 70001372
s5k5caga_i2c_write(0x0F12, 0x063C); //senHal_pContSenModesRegsArray[20][1] 2 70001374
s5k5caga_i2c_write(0x0F12, 0x038D); //senHal_pContSenModesRegsArray[20][2] 2 70001376
s5k5caga_i2c_write(0x0F12, 0x0399); //senHal_pContSenModesRegsArray[20][3] 2 70001378
s5k5caga_i2c_write(0x0F12, 0x0668); //senHal_pContSenModesRegsArray[21][0] 2 7000137A
s5k5caga_i2c_write(0x0F12, 0x06B3); //senHal_pContSenModesRegsArray[21][1] 2 7000137C
s5k5caga_i2c_write(0x0F12, 0x03C5); //senHal_pContSenModesRegsArray[21][2] 2 7000137E
s5k5caga_i2c_write(0x0F12, 0x0410); //senHal_pContSenModesRegsArray[21][3] 2 70001380
s5k5caga_i2c_write(0x0F12, 0x0001); //senHal_pContSenModesRegsArray[22][0] 2 70001382
s5k5caga_i2c_write(0x0F12, 0x0001); //senHal_pContSenModesRegsArray[22][1] 2 70001384
s5k5caga_i2c_write(0x0F12, 0x0001); //senHal_pContSenModesRegsArray[22][2] 2 70001386
s5k5caga_i2c_write(0x0F12, 0x0001); //senHal_pContSenModesRegsArray[22][3] 2 70001388
s5k5caga_i2c_write(0x0F12, 0x03A2); //senHal_pContSenModesRegsArray[23][0] 2 7000138A
s5k5caga_i2c_write(0x0F12, 0x01D3); //senHal_pContSenModesRegsArray[23][1] 2 7000138C
s5k5caga_i2c_write(0x0F12, 0x01E0); //senHal_pContSenModesRegsArray[23][2] 2 7000138E
s5k5caga_i2c_write(0x0F12, 0x00F2); //senHal_pContSenModesRegsArray[23][3] 2 70001390
s5k5caga_i2c_write(0x0F12, 0x0000); //senHal_pContSenModesRegsArray[24][0] 2 70001392
s5k5caga_i2c_write(0x0F12, 0x0461); //senHal_pContSenModesRegsArray[24][1] 2 70001394
s5k5caga_i2c_write(0x0F12, 0x0000); //senHal_pContSenModesRegsArray[24][2] 2 70001396
s5k5caga_i2c_write(0x0F12, 0x029F); //senHal_pContSenModesRegsArray[24][3] 2 70001398
s5k5caga_i2c_write(0x0F12, 0x0000); //senHal_pContSenModesRegsArray[25][0] 2 7000139A
s5k5caga_i2c_write(0x0F12, 0x063C); //senHal_pContSenModesRegsArray[25][1] 2 7000139C
s5k5caga_i2c_write(0x0F12, 0x0000); //senHal_pContSenModesRegsArray[25][2] 2 7000139E
s5k5caga_i2c_write(0x0F12, 0x0399); //senHal_pContSenModesRegsArray[25][3] 2 700013A0
s5k5caga_i2c_write(0x0F12, 0x003D); //senHal_pContSenModesRegsArray[26][0] 2 700013A2
s5k5caga_i2c_write(0x0F12, 0x003D); //senHal_pContSenModesRegsArray[26][1] 2 700013A4
s5k5caga_i2c_write(0x0F12, 0x003D); //senHal_pContSenModesRegsArray[26][2] 2 700013A6
s5k5caga_i2c_write(0x0F12, 0x003D); //senHal_pContSenModesRegsArray[26][3] 2 700013A8
s5k5caga_i2c_write(0x0F12, 0x01D0); //senHal_pContSenModesRegsArray[27][0] 2 700013AA
s5k5caga_i2c_write(0x0F12, 0x01D0); //senHal_pContSenModesRegsArray[27][1] 2 700013AC
s5k5caga_i2c_write(0x0F12, 0x00EF); //senHal_pContSenModesRegsArray[27][2] 2 700013AE
s5k5caga_i2c_write(0x0F12, 0x00EF); //senHal_pContSenModesRegsArray[27][3] 2 700013B0
s5k5caga_i2c_write(0x0F12, 0x020C); //senHal_pContSenModesRegsArray[28][0] 2 700013B2
s5k5caga_i2c_write(0x0F12, 0x024B); //senHal_pContSenModesRegsArray[28][1] 2 700013B4
s5k5caga_i2c_write(0x0F12, 0x012B); //senHal_pContSenModesRegsArray[28][2] 2 700013B6
s5k5caga_i2c_write(0x0F12, 0x016A); //senHal_pContSenModesRegsArray[28][3] 2 700013B8
s5k5caga_i2c_write(0x0F12, 0x039F); //senHal_pContSenModesRegsArray[29][0] 2 700013BA
s5k5caga_i2c_write(0x0F12, 0x045E); //senHal_pContSenModesRegsArray[29][1] 2 700013BC
s5k5caga_i2c_write(0x0F12, 0x01DD); //senHal_pContSenModesRegsArray[29][2] 2 700013BE
s5k5caga_i2c_write(0x0F12, 0x029C); //senHal_pContSenModesRegsArray[29][3] 2 700013C0
s5k5caga_i2c_write(0x0F12, 0x041A); //senHal_pContSenModesRegsArray[30][0] 2 700013C2
s5k5caga_i2c_write(0x0F12, 0x04A6); //senHal_pContSenModesRegsArray[30][1] 2 700013C4
s5k5caga_i2c_write(0x0F12, 0x0258); //senHal_pContSenModesRegsArray[30][2] 2 700013C6
s5k5caga_i2c_write(0x0F12, 0x02E4); //senHal_pContSenModesRegsArray[30][3] 2 700013C8
s5k5caga_i2c_write(0x0F12, 0x062D); //senHal_pContSenModesRegsArray[31][0] 2 700013CA
s5k5caga_i2c_write(0x0F12, 0x0639); //senHal_pContSenModesRegsArray[31][1] 2 700013CC
s5k5caga_i2c_write(0x0F12, 0x038A); //senHal_pContSenModesRegsArray[31][2] 2 700013CE
s5k5caga_i2c_write(0x0F12, 0x0396); //senHal_pContSenModesRegsArray[31][3] 2 700013D0
s5k5caga_i2c_write(0x0F12, 0x0669); //senHal_pContSenModesRegsArray[32][0] 2 700013D2
s5k5caga_i2c_write(0x0F12, 0x06B4); //senHal_pContSenModesRegsArray[32][1] 2 700013D4
s5k5caga_i2c_write(0x0F12, 0x03C6); //senHal_pContSenModesRegsArray[32][2] 2 700013D6
s5k5caga_i2c_write(0x0F12, 0x0411); //senHal_pContSenModesRegsArray[32][3] 2 700013D8
s5k5caga_i2c_write(0x0F12, 0x087C); //senHal_pContSenModesRegsArray[33][0] 2 700013DA
s5k5caga_i2c_write(0x0F12, 0x08C7); //senHal_pContSenModesRegsArray[33][1] 2 700013DC
s5k5caga_i2c_write(0x0F12, 0x04F8); //senHal_pContSenModesRegsArray[33][2] 2 700013DE
s5k5caga_i2c_write(0x0F12, 0x0543); //senHal_pContSenModesRegsArray[33][3] 2 700013E0
s5k5caga_i2c_write(0x0F12, 0x0040); //senHal_pContSenModesRegsArray[34][0] 2 700013E2
s5k5caga_i2c_write(0x0F12, 0x0040); //senHal_pContSenModesRegsArray[34][1] 2 700013E4
s5k5caga_i2c_write(0x0F12, 0x0040); //senHal_pContSenModesRegsArray[34][2] 2 700013E6
s5k5caga_i2c_write(0x0F12, 0x0040); //senHal_pContSenModesRegsArray[34][3] 2 700013E8
s5k5caga_i2c_write(0x0F12, 0x01D0); //senHal_pContSenModesRegsArray[35][0] 2 700013EA
s5k5caga_i2c_write(0x0F12, 0x01D0); //senHal_pContSenModesRegsArray[35][1] 2 700013EC
s5k5caga_i2c_write(0x0F12, 0x00EF); //senHal_pContSenModesRegsArray[35][2] 2 700013EE
s5k5caga_i2c_write(0x0F12, 0x00EF); //senHal_pContSenModesRegsArray[35][3] 2 700013F0
s5k5caga_i2c_write(0x0F12, 0x020F); //senHal_pContSenModesRegsArray[36][0] 2 700013F2
s5k5caga_i2c_write(0x0F12, 0x024E); //senHal_pContSenModesRegsArray[36][1] 2 700013F4
s5k5caga_i2c_write(0x0F12, 0x012E); //senHal_pContSenModesRegsArray[36][2] 2 700013F6
s5k5caga_i2c_write(0x0F12, 0x016D); //senHal_pContSenModesRegsArray[36][3] 2 700013F8
s5k5caga_i2c_write(0x0F12, 0x039F); //senHal_pContSenModesRegsArray[37][0] 2 700013FA
s5k5caga_i2c_write(0x0F12, 0x045E); //senHal_pContSenModesRegsArray[37][1] 2 700013FC
s5k5caga_i2c_write(0x0F12, 0x01DD); //senHal_pContSenModesRegsArray[37][2] 2 700013FE
s5k5caga_i2c_write(0x0F12, 0x029C); //senHal_pContSenModesRegsArray[37][3] 2 70001400
s5k5caga_i2c_write(0x0F12, 0x041D); //senHal_pContSenModesRegsArray[38][0] 2 70001402
s5k5caga_i2c_write(0x0F12, 0x04A9); //senHal_pContSenModesRegsArray[38][1] 2 70001404
s5k5caga_i2c_write(0x0F12, 0x025B); //senHal_pContSenModesRegsArray[38][2] 2 70001406
s5k5caga_i2c_write(0x0F12, 0x02E7); //senHal_pContSenModesRegsArray[38][3] 2 70001408
s5k5caga_i2c_write(0x0F12, 0x062D); //senHal_pContSenModesRegsArray[39][0] 2 7000140A
s5k5caga_i2c_write(0x0F12, 0x0639); //senHal_pContSenModesRegsArray[39][1] 2 7000140C
s5k5caga_i2c_write(0x0F12, 0x038A); //senHal_pContSenModesRegsArray[39][2] 2 7000140E
s5k5caga_i2c_write(0x0F12, 0x0396); //senHal_pContSenModesRegsArray[39][3] 2 70001410
s5k5caga_i2c_write(0x0F12, 0x066C); //senHal_pContSenModesRegsArray[40][0] 2 70001412
s5k5caga_i2c_write(0x0F12, 0x06B7); //senHal_pContSenModesRegsArray[40][1] 2 70001414
s5k5caga_i2c_write(0x0F12, 0x03C9); //senHal_pContSenModesRegsArray[40][2] 2 70001416
s5k5caga_i2c_write(0x0F12, 0x0414); //senHal_pContSenModesRegsArray[40][3] 2 70001418
s5k5caga_i2c_write(0x0F12, 0x087C); //senHal_pContSenModesRegsArray[41][0] 2 7000141A
s5k5caga_i2c_write(0x0F12, 0x08C7); //senHal_pContSenModesRegsArray[41][1] 2 7000141C
s5k5caga_i2c_write(0x0F12, 0x04F8); //senHal_pContSenModesRegsArray[41][2] 2 7000141E
s5k5caga_i2c_write(0x0F12, 0x0543); //senHal_pContSenModesRegsArray[41][3] 2 70001420
s5k5caga_i2c_write(0x0F12, 0x0040); //senHal_pContSenModesRegsArray[42][0] 2 70001422
s5k5caga_i2c_write(0x0F12, 0x0040); //senHal_pContSenModesRegsArray[42][1] 2 70001424
s5k5caga_i2c_write(0x0F12, 0x0040); //senHal_pContSenModesRegsArray[42][2] 2 70001426
s5k5caga_i2c_write(0x0F12, 0x0040); //senHal_pContSenModesRegsArray[42][3] 2 70001428
s5k5caga_i2c_write(0x0F12, 0x01D0); //senHal_pContSenModesRegsArray[43][0] 2 7000142A
s5k5caga_i2c_write(0x0F12, 0x01D0); //senHal_pContSenModesRegsArray[43][1] 2 7000142C
s5k5caga_i2c_write(0x0F12, 0x00EF); //senHal_pContSenModesRegsArray[43][2] 2 7000142E
s5k5caga_i2c_write(0x0F12, 0x00EF); //senHal_pContSenModesRegsArray[43][3] 2 70001430
s5k5caga_i2c_write(0x0F12, 0x020F); //senHal_pContSenModesRegsArray[44][0] 2 70001432
s5k5caga_i2c_write(0x0F12, 0x024E); //senHal_pContSenModesRegsArray[44][1] 2 70001434
s5k5caga_i2c_write(0x0F12, 0x012E); //senHal_pContSenModesRegsArray[44][2] 2 70001436
s5k5caga_i2c_write(0x0F12, 0x016D); //senHal_pContSenModesRegsArray[44][3] 2 70001438
s5k5caga_i2c_write(0x0F12, 0x039F); //senHal_pContSenModesRegsArray[45][0] 2 7000143A
s5k5caga_i2c_write(0x0F12, 0x045E); //senHal_pContSenModesRegsArray[45][1] 2 7000143C
s5k5caga_i2c_write(0x0F12, 0x01DD); //senHal_pContSenModesRegsArray[45][2] 2 7000143E
s5k5caga_i2c_write(0x0F12, 0x029C); //senHal_pContSenModesRegsArray[45][3] 2 70001440
s5k5caga_i2c_write(0x0F12, 0x041D); //senHal_pContSenModesRegsArray[46][0] 2 70001442
s5k5caga_i2c_write(0x0F12, 0x04A9); //senHal_pContSenModesRegsArray[46][1] 2 70001444
s5k5caga_i2c_write(0x0F12, 0x025B); //senHal_pContSenModesRegsArray[46][2] 2 70001446
s5k5caga_i2c_write(0x0F12, 0x02E7); //senHal_pContSenModesRegsArray[46][3] 2 70001448
s5k5caga_i2c_write(0x0F12, 0x062D); //senHal_pContSenModesRegsArray[47][0] 2 7000144A
s5k5caga_i2c_write(0x0F12, 0x0639); //senHal_pContSenModesRegsArray[47][1] 2 7000144C
s5k5caga_i2c_write(0x0F12, 0x038A); //senHal_pContSenModesRegsArray[47][2] 2 7000144E
s5k5caga_i2c_write(0x0F12, 0x0396); //senHal_pContSenModesRegsArray[47][3] 2 70001450
s5k5caga_i2c_write(0x0F12, 0x066C); //senHal_pContSenModesRegsArray[48][0] 2 70001452
s5k5caga_i2c_write(0x0F12, 0x06B7); //senHal_pContSenModesRegsArray[48][1] 2 70001454
s5k5caga_i2c_write(0x0F12, 0x03C9); //senHal_pContSenModesRegsArray[48][2] 2 70001456
s5k5caga_i2c_write(0x0F12, 0x0414); //senHal_pContSenModesRegsArray[48][3] 2 70001458
s5k5caga_i2c_write(0x0F12, 0x087C); //senHal_pContSenModesRegsArray[49][0] 2 7000145A
s5k5caga_i2c_write(0x0F12, 0x08C7); //senHal_pContSenModesRegsArray[49][1] 2 7000145C
s5k5caga_i2c_write(0x0F12, 0x04F8); //senHal_pContSenModesRegsArray[49][2] 2 7000145E
s5k5caga_i2c_write(0x0F12, 0x0543); //senHal_pContSenModesRegsArray[49][3] 2 70001460
s5k5caga_i2c_write(0x0F12, 0x003D); //senHal_pContSenModesRegsArray[50][0] 2 70001462
s5k5caga_i2c_write(0x0F12, 0x003D); //senHal_pContSenModesRegsArray[50][1] 2 70001464
s5k5caga_i2c_write(0x0F12, 0x003D); //senHal_pContSenModesRegsArray[50][2] 2 70001466
s5k5caga_i2c_write(0x0F12, 0x003D); //senHal_pContSenModesRegsArray[50][3] 2 70001468
s5k5caga_i2c_write(0x0F12, 0x01D2); //senHal_pContSenModesRegsArray[51][0] 2 7000146A
s5k5caga_i2c_write(0x0F12, 0x01D2); //senHal_pContSenModesRegsArray[51][1] 2 7000146C
s5k5caga_i2c_write(0x0F12, 0x00F1); //senHal_pContSenModesRegsArray[51][2] 2 7000146E
s5k5caga_i2c_write(0x0F12, 0x00F1); //senHal_pContSenModesRegsArray[51][3] 2 70001470
s5k5caga_i2c_write(0x0F12, 0x020C); //senHal_pContSenModesRegsArray[52][0] 2 70001472
s5k5caga_i2c_write(0x0F12, 0x024B); //senHal_pContSenModesRegsArray[52][1] 2 70001474
s5k5caga_i2c_write(0x0F12, 0x012B); //senHal_pContSenModesRegsArray[52][2] 2 70001476
s5k5caga_i2c_write(0x0F12, 0x016A); //senHal_pContSenModesRegsArray[52][3] 2 70001478
s5k5caga_i2c_write(0x0F12, 0x03A1); //senHal_pContSenModesRegsArray[53][0] 2 7000147A
s5k5caga_i2c_write(0x0F12, 0x0460); //senHal_pContSenModesRegsArray[53][1] 2 7000147C
s5k5caga_i2c_write(0x0F12, 0x01DF); //senHal_pContSenModesRegsArray[53][2] 2 7000147E
s5k5caga_i2c_write(0x0F12, 0x029E); //senHal_pContSenModesRegsArray[53][3] 2 70001480
s5k5caga_i2c_write(0x0F12, 0x041A); //senHal_pContSenModesRegsArray[54][0] 2 70001482
s5k5caga_i2c_write(0x0F12, 0x04A6); //senHal_pContSenModesRegsArray[54][1] 2 70001484
s5k5caga_i2c_write(0x0F12, 0x0258); //senHal_pContSenModesRegsArray[54][2] 2 70001486
s5k5caga_i2c_write(0x0F12, 0x02E4); //senHal_pContSenModesRegsArray[54][3] 2 70001488
s5k5caga_i2c_write(0x0F12, 0x062F); //senHal_pContSenModesRegsArray[55][0] 2 7000148A
s5k5caga_i2c_write(0x0F12, 0x063B); //senHal_pContSenModesRegsArray[55][1] 2 7000148C
s5k5caga_i2c_write(0x0F12, 0x038C); //senHal_pContSenModesRegsArray[55][2] 2 7000148E
s5k5caga_i2c_write(0x0F12, 0x0398); //senHal_pContSenModesRegsArray[55][3] 2 70001490
s5k5caga_i2c_write(0x0F12, 0x0669); //senHal_pContSenModesRegsArray[56][0] 2 70001492
s5k5caga_i2c_write(0x0F12, 0x06B4); //senHal_pContSenModesRegsArray[56][1] 2 70001494
s5k5caga_i2c_write(0x0F12, 0x03C6); //senHal_pContSenModesRegsArray[56][2] 2 70001496
s5k5caga_i2c_write(0x0F12, 0x0411); //senHal_pContSenModesRegsArray[56][3] 2 70001498
s5k5caga_i2c_write(0x0F12, 0x087E); //senHal_pContSenModesRegsArray[57][0] 2 7000149A
s5k5caga_i2c_write(0x0F12, 0x08C9); //senHal_pContSenModesRegsArray[57][1] 2 7000149C
s5k5caga_i2c_write(0x0F12, 0x04FA); //senHal_pContSenModesRegsArray[57][2] 2 7000149E
s5k5caga_i2c_write(0x0F12, 0x0545); //senHal_pContSenModesRegsArray[57][3] 2 700014A0
s5k5caga_i2c_write(0x0F12, 0x03A2); //senHal_pContSenModesRegsArray[58][0] 2 700014A2
s5k5caga_i2c_write(0x0F12, 0x01D3); //senHal_pContSenModesRegsArray[58][1] 2 700014A4
s5k5caga_i2c_write(0x0F12, 0x01E0); //senHal_pContSenModesRegsArray[58][2] 2 700014A6
s5k5caga_i2c_write(0x0F12, 0x00F2); //senHal_pContSenModesRegsArray[58][3] 2 700014A8
s5k5caga_i2c_write(0x0F12, 0x03AF); //senHal_pContSenModesRegsArray[59][0] 2 700014AA
s5k5caga_i2c_write(0x0F12, 0x01E0); //senHal_pContSenModesRegsArray[59][1] 2 700014AC
s5k5caga_i2c_write(0x0F12, 0x01ED); //senHal_pContSenModesRegsArray[59][2] 2 700014AE
s5k5caga_i2c_write(0x0F12, 0x00FF); //senHal_pContSenModesRegsArray[59][3] 2 700014B0
s5k5caga_i2c_write(0x0F12, 0x0000); //senHal_pContSenModesRegsArray[60][0] 2 700014B2
s5k5caga_i2c_write(0x0F12, 0x0461); //senHal_pContSenModesRegsArray[60][1] 2 700014B4
s5k5caga_i2c_write(0x0F12, 0x0000); //senHal_pContSenModesRegsArray[60][2] 2 700014B6
s5k5caga_i2c_write(0x0F12, 0x029F); //senHal_pContSenModesRegsArray[60][3] 2 700014B8
s5k5caga_i2c_write(0x0F12, 0x0000); //senHal_pContSenModesRegsArray[61][0] 2 700014BA
s5k5caga_i2c_write(0x0F12, 0x046E); //senHal_pContSenModesRegsArray[61][1] 2 700014BC
s5k5caga_i2c_write(0x0F12, 0x0000); //senHal_pContSenModesRegsArray[61][2] 2 700014BE
s5k5caga_i2c_write(0x0F12, 0x02AC); //senHal_pContSenModesRegsArray[61][3] 2 700014C0
s5k5caga_i2c_write(0x0F12, 0x0000); //senHal_pContSenModesRegsArray[62][0] 2 700014C2
s5k5caga_i2c_write(0x0F12, 0x063C); //senHal_pContSenModesRegsArray[62][1] 2 700014C4
s5k5caga_i2c_write(0x0F12, 0x0000); //senHal_pContSenModesRegsArray[62][2] 2 700014C6
s5k5caga_i2c_write(0x0F12, 0x0399); //senHal_pContSenModesRegsArray[62][3] 2 700014C8
s5k5caga_i2c_write(0x0F12, 0x0000); //senHal_pContSenModesRegsArray[63][0] 2 700014CA
s5k5caga_i2c_write(0x0F12, 0x0649); //senHal_pContSenModesRegsArray[63][1] 2 700014CC
s5k5caga_i2c_write(0x0F12, 0x0000); //senHal_pContSenModesRegsArray[63][2] 2 700014CE
s5k5caga_i2c_write(0x0F12, 0x03A6); //senHal_pContSenModesRegsArray[63][3] 2 700014D0
s5k5caga_i2c_write(0x0F12, 0x0000); //senHal_pContSenModesRegsArray[64][0] 2 700014D2
s5k5caga_i2c_write(0x0F12, 0x0000); //senHal_pContSenModesRegsArray[64][1] 2 700014D4
s5k5caga_i2c_write(0x0F12, 0x0000); //senHal_pContSenModesRegsArray[64][2] 2 700014D6
s5k5caga_i2c_write(0x0F12, 0x0000); //senHal_pContSenModesRegsArray[64][3] 2 700014D8
s5k5caga_i2c_write(0x0F12, 0x0000); //senHal_pContSenModesRegsArray[65][0] 2 700014DA
s5k5caga_i2c_write(0x0F12, 0x0000); //senHal_pContSenModesRegsArray[65][1] 2 700014DC
s5k5caga_i2c_write(0x0F12, 0x0000); //senHal_pContSenModesRegsArray[65][2] 2 700014DE
s5k5caga_i2c_write(0x0F12, 0x0000); //senHal_pContSenModesRegsArray[65][3] 2 700014E0
s5k5caga_i2c_write(0x0F12, 0x03AA); //senHal_pContSenModesRegsArray[66][0] 2 700014E2
s5k5caga_i2c_write(0x0F12, 0x01DB); //senHal_pContSenModesRegsArray[66][1] 2 700014E4
s5k5caga_i2c_write(0x0F12, 0x01E8); //senHal_pContSenModesRegsArray[66][2] 2 700014E6
s5k5caga_i2c_write(0x0F12, 0x00FA); //senHal_pContSenModesRegsArray[66][3] 2 700014E8
s5k5caga_i2c_write(0x0F12, 0x03B7); //senHal_pContSenModesRegsArray[67][0] 2 700014EA
s5k5caga_i2c_write(0x0F12, 0x01E8); //senHal_pContSenModesRegsArray[67][1] 2 700014EC
s5k5caga_i2c_write(0x0F12, 0x01F5); //senHal_pContSenModesRegsArray[67][2] 2 700014EE
s5k5caga_i2c_write(0x0F12, 0x0107); //senHal_pContSenModesRegsArray[67][3] 2 700014F0
s5k5caga_i2c_write(0x0F12, 0x0000); //senHal_pContSenModesRegsArray[68][0] 2 700014F2
s5k5caga_i2c_write(0x0F12, 0x0469); //senHal_pContSenModesRegsArray[68][1] 2 700014F4
s5k5caga_i2c_write(0x0F12, 0x0000); //senHal_pContSenModesRegsArray[68][2] 2 700014F6
s5k5caga_i2c_write(0x0F12, 0x02A7); //senHal_pContSenModesRegsArray[68][3] 2 700014F8
s5k5caga_i2c_write(0x0F12, 0x0000); //senHal_pContSenModesRegsArray[69][0] 2 700014FA
s5k5caga_i2c_write(0x0F12, 0x0476); //senHal_pContSenModesRegsArray[69][1] 2 700014FC
s5k5caga_i2c_write(0x0F12, 0x0000); //senHal_pContSenModesRegsArray[69][2] 2 700014FE
s5k5caga_i2c_write(0x0F12, 0x02B4); //senHal_pContSenModesRegsArray[69][3] 2 70001500
s5k5caga_i2c_write(0x0F12, 0x0000); //senHal_pContSenModesRegsArray[70][0] 2 70001502
s5k5caga_i2c_write(0x0F12, 0x0644); //senHal_pContSenModesRegsArray[70][1] 2 70001504
s5k5caga_i2c_write(0x0F12, 0x0000); //senHal_pContSenModesRegsArray[70][2] 2 70001506
s5k5caga_i2c_write(0x0F12, 0x03A1); //senHal_pContSenModesRegsArray[70][3] 2 70001508
s5k5caga_i2c_write(0x0F12, 0x0000); //senHal_pContSenModesRegsArray[71][0] 2 7000150A
s5k5caga_i2c_write(0x0F12, 0x0651); //senHal_pContSenModesRegsArray[71][1] 2 7000150C
s5k5caga_i2c_write(0x0F12, 0x0000); //senHal_pContSenModesRegsArray[71][2] 2 7000150E
s5k5caga_i2c_write(0x0F12, 0x03AE); //senHal_pContSenModesRegsArray[71][3] 2 70001510
s5k5caga_i2c_write(0x0F12, 0x0000); //senHal_pContSenModesRegsArray[72][0] 2 70001512
s5k5caga_i2c_write(0x0F12, 0x0000); //senHal_pContSenModesRegsArray[72][1] 2 70001514
s5k5caga_i2c_write(0x0F12, 0x0000); //senHal_pContSenModesRegsArray[72][2] 2 70001516
s5k5caga_i2c_write(0x0F12, 0x0000); //senHal_pContSenModesRegsArray[72][3] 2 70001518
s5k5caga_i2c_write(0x0F12, 0x0000); //senHal_pContSenModesRegsArray[73][0] 2 7000151A
s5k5caga_i2c_write(0x0F12, 0x0000); //senHal_pContSenModesRegsArray[73][1] 2 7000151C
s5k5caga_i2c_write(0x0F12, 0x0000); //senHal_pContSenModesRegsArray[73][2] 2 7000151E
s5k5caga_i2c_write(0x0F12, 0x0000); //senHal_pContSenModesRegsArray[73][3] 2 70001520
s5k5caga_i2c_write(0x0F12, 0x0001); //senHal_pContSenModesRegsArray[74][0] 2 70001522
s5k5caga_i2c_write(0x0F12, 0x0001); //senHal_pContSenModesRegsArray[74][1] 2 70001524
s5k5caga_i2c_write(0x0F12, 0x0001); //senHal_pContSenModesRegsArray[74][2] 2 70001526
s5k5caga_i2c_write(0x0F12, 0x0001); //senHal_pContSenModesRegsArray[74][3] 2 70001528
s5k5caga_i2c_write(0x0F12, 0x000F); //senHal_pContSenModesRegsArray[75][0] 2 7000152A
s5k5caga_i2c_write(0x0F12, 0x000F); //senHal_pContSenModesRegsArray[75][1] 2 7000152C
s5k5caga_i2c_write(0x0F12, 0x000F); //senHal_pContSenModesRegsArray[75][2] 2 7000152E
s5k5caga_i2c_write(0x0F12, 0x000F); //senHal_pContSenModesRegsArray[75][3] 2 70001530
s5k5caga_i2c_write(0x0F12, 0x05AD); //senHal_pContSenModesRegsArray[76][0] 2 70001532
s5k5caga_i2c_write(0x0F12, 0x03DE); //senHal_pContSenModesRegsArray[76][1] 2 70001534
s5k5caga_i2c_write(0x0F12, 0x030A); //senHal_pContSenModesRegsArray[76][2] 2 70001536
s5k5caga_i2c_write(0x0F12, 0x021C); //senHal_pContSenModesRegsArray[76][3] 2 70001538
s5k5caga_i2c_write(0x0F12, 0x062F); //senHal_pContSenModesRegsArray[77][0] 2 7000153A
s5k5caga_i2c_write(0x0F12, 0x0460); //senHal_pContSenModesRegsArray[77][1] 2 7000153C
s5k5caga_i2c_write(0x0F12, 0x038C); //senHal_pContSenModesRegsArray[77][2] 2 7000153E
s5k5caga_i2c_write(0x0F12, 0x029E); //senHal_pContSenModesRegsArray[77][3] 2 70001540
s5k5caga_i2c_write(0x0F12, 0x07FC); //senHal_pContSenModesRegsArray[78][0] 2 70001542
s5k5caga_i2c_write(0x0F12, 0x0847); //senHal_pContSenModesRegsArray[78][1] 2 70001544
s5k5caga_i2c_write(0x0F12, 0x0478); //senHal_pContSenModesRegsArray[78][2] 2 70001546
s5k5caga_i2c_write(0x0F12, 0x04C3); //senHal_pContSenModesRegsArray[78][3] 2 70001548
s5k5caga_i2c_write(0x0F12, 0x0000); //senHal_pContSenModesRegsArray[79][0] 2 7000154A
s5k5caga_i2c_write(0x0F12, 0x0000); //senHal_pContSenModesRegsArray[79][1] 2 7000154C
s5k5caga_i2c_write(0x0F12, 0x0000); //senHal_pContSenModesRegsArray[79][2] 2 7000154E
s5k5caga_i2c_write(0x0F12, 0x0000); //senHal_pContSenModesRegsArray[79][3] 2 70001550

s5k5caga_i2c_write(0x002A, 0x12B8); //disable CINTR 0           
s5k5caga_i2c_write(0x0F12, 0x1000);   
//============================================================
// ISP-FE Setting
//============================================================
s5k5caga_i2c_write(0x002A, 0x158A);
s5k5caga_i2c_write(0x0F12, 0xEAF0);
s5k5caga_i2c_write(0x002A, 0x15C6);
s5k5caga_i2c_write(0x0F12, 0x0020);
s5k5caga_i2c_write(0x0F12, 0x0060);
s5k5caga_i2c_write(0x002A, 0x15BC);
s5k5caga_i2c_write(0x0F12, 0x0200); // added by Shy.

//Analog Offset for MSM
s5k5caga_i2c_write(0x002A, 0x1608); 
s5k5caga_i2c_write(0x0F12, 0x0100); // #gisp_msm_sAnalogOffset[0] 
s5k5caga_i2c_write(0x0F12, 0x0100); // #gisp_msm_sAnalogOffset[1]
s5k5caga_i2c_write(0x0F12, 0x0100); // #gisp_msm_sAnalogOffset[2]
s5k5caga_i2c_write(0x0F12, 0x0100); // #gisp_msm_sAnalogOffset[3]

//============================================================
// ISP-FE Setting END
//============================================================
//================================================================================================
// SET JPEG & SPOOF
//================================================================================================
s5k5caga_i2c_write(0x002A, 0x0454);
s5k5caga_i2c_write(0x0F12, 0x0055);// #REG_TC_BRC_usCaptureQuality // JPEG BRC (BitRateControl) value // 85

//================================================================================================
// SET THUMBNAIL
// # Foramt : RGB565
// # Size: VGA
//================================================================================================
//0F12 0001 // thumbnail enable
//0F12 0140 // Width//320 //640
//0F12 00F0 // Height //240 //480
//0F12 0000 // Thumbnail format : 0RGB565 1 RGB888 2 Full-YUV (0-255)

//================================================================================================
// SET AE
//================================================================================================
// AE target 
s5k5caga_i2c_write(0x002A, 0x0F70);
s5k5caga_i2c_write(0x0F12, 0x003A); // #TVAR_ae_BrAve
// AE mode
s5k5caga_i2c_write(0x002A, 0x0F76);
s5k5caga_i2c_write(0x0F12, 0x0005); //Disable illumination & contrast  // #ae_StatMode
// AE weight  
s5k5caga_i2c_write(0x002A, 0x0F7E);
s5k5caga_i2c_write(0x0F12, 0x0000); //#ae_WeightTbl_16_0_
s5k5caga_i2c_write(0x0F12, 0x0000); //#ae_WeightTbl_16_1_
s5k5caga_i2c_write(0x0F12, 0x0000); //#ae_WeightTbl_16_2_
s5k5caga_i2c_write(0x0F12, 0x0000); //#ae_WeightTbl_16_3_
s5k5caga_i2c_write(0x0F12, 0x0101); //#ae_WeightTbl_16_4_
s5k5caga_i2c_write(0x0F12, 0x0101); //#ae_WeightTbl_16_5_
s5k5caga_i2c_write(0x0F12, 0x0101); //#ae_WeightTbl_16_6_
s5k5caga_i2c_write(0x0F12, 0x0101); //#ae_WeightTbl_16_7_
s5k5caga_i2c_write(0x0F12, 0x0101); //#ae_WeightTbl_16_8_
s5k5caga_i2c_write(0x0F12, 0x0201); //#ae_WeightTbl_16_9_
s5k5caga_i2c_write(0x0F12, 0x0102); //#ae_WeightTbl_16_10
s5k5caga_i2c_write(0x0F12, 0x0101); //#ae_WeightTbl_16_11
s5k5caga_i2c_write(0x0F12, 0x0101); //#ae_WeightTbl_16_12
s5k5caga_i2c_write(0x0F12, 0x0202); //#ae_WeightTbl_16_13
s5k5caga_i2c_write(0x0F12, 0x0202); //#ae_WeightTbl_16_14
s5k5caga_i2c_write(0x0F12, 0x0101); //#ae_WeightTbl_16_15
s5k5caga_i2c_write(0x0F12, 0x0101); //#ae_WeightTbl_16_16
s5k5caga_i2c_write(0x0F12, 0x0202); //#ae_WeightTbl_16_17
s5k5caga_i2c_write(0x0F12, 0x0202); //#ae_WeightTbl_16_18
s5k5caga_i2c_write(0x0F12, 0x0101); //#ae_WeightTbl_16_19
s5k5caga_i2c_write(0x0F12, 0x0201); //#ae_WeightTbl_16_20
s5k5caga_i2c_write(0x0F12, 0x0202); //#ae_WeightTbl_16_21
s5k5caga_i2c_write(0x0F12, 0x0202); //#ae_WeightTbl_16_22
s5k5caga_i2c_write(0x0F12, 0x0102); //#ae_WeightTbl_16_23
s5k5caga_i2c_write(0x0F12, 0x0201); //#ae_WeightTbl_16_24
s5k5caga_i2c_write(0x0F12, 0x0202); //#ae_WeightTbl_16_25
s5k5caga_i2c_write(0x0F12, 0x0202); //#ae_WeightTbl_16_26
s5k5caga_i2c_write(0x0F12, 0x0102); //#ae_WeightTbl_16_27
s5k5caga_i2c_write(0x0F12, 0x0101); //#ae_WeightTbl_16_28
s5k5caga_i2c_write(0x0F12, 0x0101); //#ae_WeightTbl_16_29
s5k5caga_i2c_write(0x0F12, 0x0101); //#ae_WeightTbl_16_30
s5k5caga_i2c_write(0x0F12, 0x0101); //#ae_WeightTbl_16_31

//================================================================================================
// SET FLICKER
//================================================================================================
s5k5caga_i2c_write(0x002A, 0x0C18);
s5k5caga_i2c_write(0x0F12, 0x0001); // 0001: 60Hz start auto / 0000: 50Hz start auto
s5k5caga_i2c_write(0x002A, 0x04D2);
s5k5caga_i2c_write(0x0F12, 0x067F);

//================================================================================================
// SET GAS
//================================================================================================
// GAS alpha
// R, Gr, Gb, B per light source
s5k5caga_i2c_write(0x002A, 0x06CE);
s5k5caga_i2c_write(0x0F12, 0x0130); // #TVAR_ash_GASAlpha[0] // Horizon
s5k5caga_i2c_write(0x0F12, 0x0100); // #TVAR_ash_GASAlpha[1]
s5k5caga_i2c_write(0x0F12, 0x0100); // #TVAR_ash_GASAlpha[2]
s5k5caga_i2c_write(0x0F12, 0x0100); // #TVAR_ash_GASAlpha[3]
s5k5caga_i2c_write(0x0F12, 0x0120); // #TVAR_ash_GASAlpha[4] // IncandA
s5k5caga_i2c_write(0x0F12, 0x0100); // #TVAR_ash_GASAlpha[5]
s5k5caga_i2c_write(0x0F12, 0x0100); // #TVAR_ash_GASAlpha[6]
s5k5caga_i2c_write(0x0F12, 0x0100); // #TVAR_ash_GASAlpha[7]
s5k5caga_i2c_write(0x0F12, 0x0100); // #TVAR_ash_GASAlpha[8] // WW
s5k5caga_i2c_write(0x0F12, 0x0100); // #TVAR_ash_GASAlpha[9]
s5k5caga_i2c_write(0x0F12, 0x0100); // #TVAR_ash_GASAlpha[10]
s5k5caga_i2c_write(0x0F12, 0x0100); // #TVAR_ash_GASAlpha[11]//*<BU5D05950 zhangsheng 20100422 begin*/
s5k5caga_i2c_write(0x0F12, 0x00FA); // #TVAR_ash_GASAlpha[12]// CWF
s5k5caga_i2c_write(0x0F12, 0x0100); // #TVAR_ash_GASAlpha[13]
s5k5caga_i2c_write(0x0F12, 0x0100); // #TVAR_ash_GASAlpha[14]
s5k5caga_i2c_write(0x0F12, 0x0120); // #TVAR_ash_GASAlpha[15]//*BU5D05950 zhangsheng 20100422 end>*/
s5k5caga_i2c_write(0x0F12, 0x0100); // #TVAR_ash_GASAlpha[16]// D50
s5k5caga_i2c_write(0x0F12, 0x0100); // #TVAR_ash_GASAlpha[17]
s5k5caga_i2c_write(0x0F12, 0x0100); // #TVAR_ash_GASAlpha[18]
s5k5caga_i2c_write(0x0F12, 0x0120); // #TVAR_ash_GASAlpha[19]
s5k5caga_i2c_write(0x0F12, 0x0100); // #TVAR_ash_GASAlpha[20]// D65
s5k5caga_i2c_write(0x0F12, 0x0100); // #TVAR_ash_GASAlpha[21]
s5k5caga_i2c_write(0x0F12, 0x0100); // #TVAR_ash_GASAlpha[22]
s5k5caga_i2c_write(0x0F12, 0x0100); // #TVAR_ash_GASAlpha[23]
s5k5caga_i2c_write(0x0F12, 0x0100); // #TVAR_ash_GASAlpha[24]// D75
s5k5caga_i2c_write(0x0F12, 0x0100); // #TVAR_ash_GASAlpha[25]
s5k5caga_i2c_write(0x0F12, 0x0100); // #TVAR_ash_GASAlpha[26]
s5k5caga_i2c_write(0x0F12, 0x0100); // #TVAR_ash_GASAlpha[27]
s5k5caga_i2c_write(0x0F12, 0x0100); // #TVAR_ash_GASOutdoorAlpha[0] // Outdoor
s5k5caga_i2c_write(0x0F12, 0x0100); // #TVAR_ash_GASOutdoorAlpha[1]
s5k5caga_i2c_write(0x0F12, 0x0100); // #TVAR_ash_GASOutdoorAlpha[2]
s5k5caga_i2c_write(0x0F12, 0x0100); // #TVAR_ash_GASOutdoorAlpha[3]
// GAS beta
s5k5caga_i2c_write(0x0F12, 0x0000); // #ash_GASBeta[0]// Horizon
s5k5caga_i2c_write(0x0F12, 0x0000); // #ash_GASBeta[1]
s5k5caga_i2c_write(0x0F12, 0x0000); // #ash_GASBeta[2]
s5k5caga_i2c_write(0x0F12, 0x0000); // #ash_GASBeta[3]
s5k5caga_i2c_write(0x0F12, 0x0000); // #ash_GASBeta[4]// IncandA
s5k5caga_i2c_write(0x0F12, 0x0000); // #ash_GASBeta[5]
s5k5caga_i2c_write(0x0F12, 0x0000); // #ash_GASBeta[6]
s5k5caga_i2c_write(0x0F12, 0x0000); // #ash_GASBeta[7]
s5k5caga_i2c_write(0x0F12, 0x0000); // #ash_GASBeta[8]// WW 
s5k5caga_i2c_write(0x0F12, 0x0000); // #ash_GASBeta[9]
s5k5caga_i2c_write(0x0F12, 0x0000); // #ash_GASBeta[10] 
s5k5caga_i2c_write(0x0F12, 0x0000); // #ash_GASBeta[11] 
s5k5caga_i2c_write(0x0F12, 0x0000); // #ash_GASBeta[12] // CWF
s5k5caga_i2c_write(0x0F12, 0x0000); // #ash_GASBeta[13] 
s5k5caga_i2c_write(0x0F12, 0x0000); // #ash_GASBeta[14] 
s5k5caga_i2c_write(0x0F12, 0x0000); // #ash_GASBeta[15] 
s5k5caga_i2c_write(0x0F12, 0x0000); // #ash_GASBeta[16] // D50
s5k5caga_i2c_write(0x0F12, 0x0000); // #ash_GASBeta[17] 
s5k5caga_i2c_write(0x0F12, 0x0000); // #ash_GASBeta[18] 
s5k5caga_i2c_write(0x0F12, 0x0000); // #ash_GASBeta[19] 
s5k5caga_i2c_write(0x0F12, 0x0000); // #ash_GASBeta[20] // D65
s5k5caga_i2c_write(0x0F12, 0x0000); // #ash_GASBeta[21] 
s5k5caga_i2c_write(0x0F12, 0x0000); // #ash_GASBeta[22] 
s5k5caga_i2c_write(0x0F12, 0x0000); // #ash_GASBeta[23] 
s5k5caga_i2c_write(0x0F12, 0x0000); // #ash_GASBeta[24] // D75
s5k5caga_i2c_write(0x0F12, 0x0000); // #ash_GASBeta[25] 
s5k5caga_i2c_write(0x0F12, 0x0000); // #ash_GASBeta[26] 
s5k5caga_i2c_write(0x0F12, 0x0000); // #ash_GASBeta[27] 
s5k5caga_i2c_write(0x0F12, 0x0000); // #ash_GASOutdoorBeta[0] // Outdoor
s5k5caga_i2c_write(0x0F12, 0x0000); // #ash_GASOutdoorBeta[1]
s5k5caga_i2c_write(0x0F12, 0x0000); // #ash_GASOutdoorBeta[2]
s5k5caga_i2c_write(0x0F12, 0x0000); // #ash_GASOutdoorBeta[3]
s5k5caga_i2c_write(0x002A, 0x06B4);
s5k5caga_i2c_write(0x0F12, 0x0000); // #wbt_bUseOutdoorASH ON:1 OFF:0

// Parabloic function
s5k5caga_i2c_write(0x002A, 0x075A);
s5k5caga_i2c_write(0x0F12, 0x0000); // #ash_bParabolicEstimation
s5k5caga_i2c_write(0x0F12, 0x0400); // #ash_uParabolicCenterX
s5k5caga_i2c_write(0x0F12, 0x0300); // #ash_uParabolicCenterY
s5k5caga_i2c_write(0x0F12, 0x0010); // #ash_uParabolicScalingA
s5k5caga_i2c_write(0x0F12, 0x0011); // #ash_uParabolicScalingB
s5k5caga_i2c_write(0x002A, 0x06C6);
s5k5caga_i2c_write(0x0F12, 0x0100); //ash_CGrasAlphas_0_
s5k5caga_i2c_write(0x0F12, 0x0100); //ash_CGrasAlphas_1_
s5k5caga_i2c_write(0x0F12, 0x0100); //ash_CGrasAlphas_2_
s5k5caga_i2c_write(0x0F12, 0x0100); //ash_CGrasAlphas_3_
s5k5caga_i2c_write(0x002A, 0x0E3C);
s5k5caga_i2c_write(0x0F12, 0x00C0); // #awbb_Alpha_Comp_Mode
s5k5caga_i2c_write(0x002A, 0x074E); 
s5k5caga_i2c_write(0x0F12, 0x0000); // #ash_bLumaMode //use Beta : 0001 not use Beta : 0000

// GAS LUT start address // 7000_347C 
s5k5caga_i2c_write(0x002A, 0x0754);
s5k5caga_i2c_write(0x0F12, 0x347C);
s5k5caga_i2c_write(0x0F12, 0x7000);
// GAS LUT
s5k5caga_i2c_write(0x002A, 0x347C); //*<BU5D05950 zhangsheng 20100422 begin*/ 
s5k5caga_i2c_write(0x0F12, 0x0164); // #TVAR_ash_pGAS[0]
s5k5caga_i2c_write(0x0F12, 0x016B); // #TVAR_ash_pGAS[1]
s5k5caga_i2c_write(0x0F12, 0x013F); // #TVAR_ash_pGAS[2]
s5k5caga_i2c_write(0x0F12, 0x010B); // #TVAR_ash_pGAS[3]
s5k5caga_i2c_write(0x0F12, 0x00EF); // #TVAR_ash_pGAS[4]
s5k5caga_i2c_write(0x0F12, 0x00DC); // #TVAR_ash_pGAS[5]
s5k5caga_i2c_write(0x0F12, 0x00D4); // #TVAR_ash_pGAS[6]
s5k5caga_i2c_write(0x0F12, 0x00D4); // #TVAR_ash_pGAS[7]
s5k5caga_i2c_write(0x0F12, 0x00E1); // #TVAR_ash_pGAS[8]
s5k5caga_i2c_write(0x0F12, 0x00FB); // #TVAR_ash_pGAS[9]
s5k5caga_i2c_write(0x0F12, 0x0127); // #TVAR_ash_pGAS[10]
s5k5caga_i2c_write(0x0F12, 0x014E); // #TVAR_ash_pGAS[11]
s5k5caga_i2c_write(0x0F12, 0x0146); // #TVAR_ash_pGAS[12]
s5k5caga_i2c_write(0x0F12, 0x0157); // #TVAR_ash_pGAS[13]
s5k5caga_i2c_write(0x0F12, 0x0140); // #TVAR_ash_pGAS[14]
s5k5caga_i2c_write(0x0F12, 0x0103); // #TVAR_ash_pGAS[15]
s5k5caga_i2c_write(0x0F12, 0x00D5); // #TVAR_ash_pGAS[16]
s5k5caga_i2c_write(0x0F12, 0x00B5); // #TVAR_ash_pGAS[17]
s5k5caga_i2c_write(0x0F12, 0x00A1); // #TVAR_ash_pGAS[18]
s5k5caga_i2c_write(0x0F12, 0x0097); // #TVAR_ash_pGAS[19]
s5k5caga_i2c_write(0x0F12, 0x009A); // #TVAR_ash_pGAS[20]
s5k5caga_i2c_write(0x0F12, 0x00AA); // #TVAR_ash_pGAS[21]
s5k5caga_i2c_write(0x0F12, 0x00C5); // #TVAR_ash_pGAS[22]
s5k5caga_i2c_write(0x0F12, 0x00F2); // #TVAR_ash_pGAS[23]
s5k5caga_i2c_write(0x0F12, 0x0131); // #TVAR_ash_pGAS[24]
s5k5caga_i2c_write(0x0F12, 0x0142); // #TVAR_ash_pGAS[25]
s5k5caga_i2c_write(0x0F12, 0x0132); // #TVAR_ash_pGAS[26]
s5k5caga_i2c_write(0x0F12, 0x0108); // #TVAR_ash_pGAS[27]
s5k5caga_i2c_write(0x0F12, 0x00CA); // #TVAR_ash_pGAS[28]
s5k5caga_i2c_write(0x0F12, 0x009B); // #TVAR_ash_pGAS[29]
s5k5caga_i2c_write(0x0F12, 0x0079); // #TVAR_ash_pGAS[30]
s5k5caga_i2c_write(0x0F12, 0x0061); // #TVAR_ash_pGAS[31]
s5k5caga_i2c_write(0x0F12, 0x0059); // #TVAR_ash_pGAS[32]
s5k5caga_i2c_write(0x0F12, 0x0060); // #TVAR_ash_pGAS[33]
s5k5caga_i2c_write(0x0F12, 0x0075); // #TVAR_ash_pGAS[34]
s5k5caga_i2c_write(0x0F12, 0x0094); // #TVAR_ash_pGAS[35]
s5k5caga_i2c_write(0x0F12, 0x00C2); // #TVAR_ash_pGAS[36]
s5k5caga_i2c_write(0x0F12, 0x0106); // #TVAR_ash_pGAS[37]
s5k5caga_i2c_write(0x0F12, 0x0137); // #TVAR_ash_pGAS[38]
s5k5caga_i2c_write(0x0F12, 0x0109); // #TVAR_ash_pGAS[39]
s5k5caga_i2c_write(0x0F12, 0x00DA); // #TVAR_ash_pGAS[40]
s5k5caga_i2c_write(0x0F12, 0x009E); // #TVAR_ash_pGAS[41]
s5k5caga_i2c_write(0x0F12, 0x006D); // #TVAR_ash_pGAS[42]
s5k5caga_i2c_write(0x0F12, 0x0048); // #TVAR_ash_pGAS[43]
s5k5caga_i2c_write(0x0F12, 0x0030); // #TVAR_ash_pGAS[44]
s5k5caga_i2c_write(0x0F12, 0x0026); // #TVAR_ash_pGAS[45]
s5k5caga_i2c_write(0x0F12, 0x002D); // #TVAR_ash_pGAS[46]
s5k5caga_i2c_write(0x0F12, 0x0048); // #TVAR_ash_pGAS[47]
s5k5caga_i2c_write(0x0F12, 0x0072); // #TVAR_ash_pGAS[48]
s5k5caga_i2c_write(0x0F12, 0x00A5); // #TVAR_ash_pGAS[49]
s5k5caga_i2c_write(0x0F12, 0x00E7); // #TVAR_ash_pGAS[50]
s5k5caga_i2c_write(0x0F12, 0x011F); // #TVAR_ash_pGAS[51]
s5k5caga_i2c_write(0x0F12, 0x00F0); // #TVAR_ash_pGAS[52]
s5k5caga_i2c_write(0x0F12, 0x00C0); // #TVAR_ash_pGAS[53]
s5k5caga_i2c_write(0x0F12, 0x0082); // #TVAR_ash_pGAS[54]
s5k5caga_i2c_write(0x0F12, 0x004F); // #TVAR_ash_pGAS[55]
s5k5caga_i2c_write(0x0F12, 0x0029); // #TVAR_ash_pGAS[56]
s5k5caga_i2c_write(0x0F12, 0x0012); // #TVAR_ash_pGAS[57]
s5k5caga_i2c_write(0x0F12, 0x000C); // #TVAR_ash_pGAS[58]
s5k5caga_i2c_write(0x0F12, 0x0015); // #TVAR_ash_pGAS[59]
s5k5caga_i2c_write(0x0F12, 0x002D); // #TVAR_ash_pGAS[60]
s5k5caga_i2c_write(0x0F12, 0x005B); // #TVAR_ash_pGAS[61]
s5k5caga_i2c_write(0x0F12, 0x0091); // #TVAR_ash_pGAS[62]
s5k5caga_i2c_write(0x0F12, 0x00D5); // #TVAR_ash_pGAS[63]
s5k5caga_i2c_write(0x0F12, 0x0110); // #TVAR_ash_pGAS[64]
s5k5caga_i2c_write(0x0F12, 0x00E4); // #TVAR_ash_pGAS[65]
s5k5caga_i2c_write(0x0F12, 0x00B5); // #TVAR_ash_pGAS[66]
s5k5caga_i2c_write(0x0F12, 0x0075); // #TVAR_ash_pGAS[67]
s5k5caga_i2c_write(0x0F12, 0x0042); // #TVAR_ash_pGAS[68]
s5k5caga_i2c_write(0x0F12, 0x001C); // #TVAR_ash_pGAS[69]
s5k5caga_i2c_write(0x0F12, 0x0006); // #TVAR_ash_pGAS[70]
s5k5caga_i2c_write(0x0F12, 0x0000); // #TVAR_ash_pGAS[71]
s5k5caga_i2c_write(0x0F12, 0x000A); // #TVAR_ash_pGAS[72]
s5k5caga_i2c_write(0x0F12, 0x0021); // #TVAR_ash_pGAS[73]
s5k5caga_i2c_write(0x0F12, 0x0053); // #TVAR_ash_pGAS[74]
s5k5caga_i2c_write(0x0F12, 0x008B); // #TVAR_ash_pGAS[75]
s5k5caga_i2c_write(0x0F12, 0x00D0); // #TVAR_ash_pGAS[76]
s5k5caga_i2c_write(0x0F12, 0x010D); // #TVAR_ash_pGAS[77]
s5k5caga_i2c_write(0x0F12, 0x00E7); // #TVAR_ash_pGAS[78]
s5k5caga_i2c_write(0x0F12, 0x00B8); // #TVAR_ash_pGAS[79]
s5k5caga_i2c_write(0x0F12, 0x0079); // #TVAR_ash_pGAS[80]
s5k5caga_i2c_write(0x0F12, 0x0047); // #TVAR_ash_pGAS[81]
s5k5caga_i2c_write(0x0F12, 0x0021); // #TVAR_ash_pGAS[82]
s5k5caga_i2c_write(0x0F12, 0x000C); // #TVAR_ash_pGAS[83]
s5k5caga_i2c_write(0x0F12, 0x0004); // #TVAR_ash_pGAS[84]
s5k5caga_i2c_write(0x0F12, 0x000E); // #TVAR_ash_pGAS[85]
s5k5caga_i2c_write(0x0F12, 0x002B); // #TVAR_ash_pGAS[86]
s5k5caga_i2c_write(0x0F12, 0x0059); // #TVAR_ash_pGAS[87]
s5k5caga_i2c_write(0x0F12, 0x0093); // #TVAR_ash_pGAS[88]
s5k5caga_i2c_write(0x0F12, 0x00DA); // #TVAR_ash_pGAS[89]
s5k5caga_i2c_write(0x0F12, 0x0114); // #TVAR_ash_pGAS[90]
s5k5caga_i2c_write(0x0F12, 0x00F9); // #TVAR_ash_pGAS[91]
s5k5caga_i2c_write(0x0F12, 0x00C9); // #TVAR_ash_pGAS[92]
s5k5caga_i2c_write(0x0F12, 0x008B); // #TVAR_ash_pGAS[93]
s5k5caga_i2c_write(0x0F12, 0x005C); // #TVAR_ash_pGAS[94]
s5k5caga_i2c_write(0x0F12, 0x0039); // #TVAR_ash_pGAS[95]
s5k5caga_i2c_write(0x0F12, 0x0024); // #TVAR_ash_pGAS[96]
s5k5caga_i2c_write(0x0F12, 0x001D); // #TVAR_ash_pGAS[97]
s5k5caga_i2c_write(0x0F12, 0x0028); // #TVAR_ash_pGAS[98]
s5k5caga_i2c_write(0x0F12, 0x0044); // #TVAR_ash_pGAS[99]
s5k5caga_i2c_write(0x0F12, 0x0072); // #TVAR_ash_pGAS[100]
s5k5caga_i2c_write(0x0F12, 0x00A9); // #TVAR_ash_pGAS[101]
s5k5caga_i2c_write(0x0F12, 0x00F0); // #TVAR_ash_pGAS[102]
s5k5caga_i2c_write(0x0F12, 0x0126); // #TVAR_ash_pGAS[103]
s5k5caga_i2c_write(0x0F12, 0x0115); // #TVAR_ash_pGAS[104]
s5k5caga_i2c_write(0x0F12, 0x00EE); // #TVAR_ash_pGAS[105]
s5k5caga_i2c_write(0x0F12, 0x00AD); // #TVAR_ash_pGAS[106]
s5k5caga_i2c_write(0x0F12, 0x0081); // #TVAR_ash_pGAS[107]
s5k5caga_i2c_write(0x0F12, 0x0061); // #TVAR_ash_pGAS[108]
s5k5caga_i2c_write(0x0F12, 0x004D); // #TVAR_ash_pGAS[109]
s5k5caga_i2c_write(0x0F12, 0x0049); // #TVAR_ash_pGAS[110]
s5k5caga_i2c_write(0x0F12, 0x0053); // #TVAR_ash_pGAS[111]
s5k5caga_i2c_write(0x0F12, 0x006F); // #TVAR_ash_pGAS[112]
s5k5caga_i2c_write(0x0F12, 0x0099); // #TVAR_ash_pGAS[113]
s5k5caga_i2c_write(0x0F12, 0x00CF); // #TVAR_ash_pGAS[114]
s5k5caga_i2c_write(0x0F12, 0x0118); // #TVAR_ash_pGAS[115]
s5k5caga_i2c_write(0x0F12, 0x0140); // #TVAR_ash_pGAS[116]
s5k5caga_i2c_write(0x0F12, 0x0136); // #TVAR_ash_pGAS[117]
s5k5caga_i2c_write(0x0F12, 0x0122); // #TVAR_ash_pGAS[118]
s5k5caga_i2c_write(0x0F12, 0x00E2); // #TVAR_ash_pGAS[119]
s5k5caga_i2c_write(0x0F12, 0x00B6); // #TVAR_ash_pGAS[120]
s5k5caga_i2c_write(0x0F12, 0x009A); // #TVAR_ash_pGAS[121]
s5k5caga_i2c_write(0x0F12, 0x0089); // #TVAR_ash_pGAS[122]
s5k5caga_i2c_write(0x0F12, 0x0086); // #TVAR_ash_pGAS[123]
s5k5caga_i2c_write(0x0F12, 0x0090); // #TVAR_ash_pGAS[124]
s5k5caga_i2c_write(0x0F12, 0x00AA); // #TVAR_ash_pGAS[125]
s5k5caga_i2c_write(0x0F12, 0x00D1); // #TVAR_ash_pGAS[126]
s5k5caga_i2c_write(0x0F12, 0x0108); // #TVAR_ash_pGAS[127]
s5k5caga_i2c_write(0x0F12, 0x0150); // #TVAR_ash_pGAS[128]
s5k5caga_i2c_write(0x0F12, 0x015D); // #TVAR_ash_pGAS[129]
s5k5caga_i2c_write(0x0F12, 0x0141); // #TVAR_ash_pGAS[130]
s5k5caga_i2c_write(0x0F12, 0x0150); // #TVAR_ash_pGAS[131]
s5k5caga_i2c_write(0x0F12, 0x011A); // #TVAR_ash_pGAS[132]
s5k5caga_i2c_write(0x0F12, 0x00EF); // #TVAR_ash_pGAS[133]
s5k5caga_i2c_write(0x0F12, 0x00D5); // #TVAR_ash_pGAS[134]
s5k5caga_i2c_write(0x0F12, 0x00C9); // #TVAR_ash_pGAS[135]
s5k5caga_i2c_write(0x0F12, 0x00C6); // #TVAR_ash_pGAS[136]
s5k5caga_i2c_write(0x0F12, 0x00D0); // #TVAR_ash_pGAS[137]
s5k5caga_i2c_write(0x0F12, 0x00E6); // #TVAR_ash_pGAS[138]
s5k5caga_i2c_write(0x0F12, 0x010D); // #TVAR_ash_pGAS[139]
s5k5caga_i2c_write(0x0F12, 0x0142); // #TVAR_ash_pGAS[140]
s5k5caga_i2c_write(0x0F12, 0x017A); // #TVAR_ash_pGAS[141]
s5k5caga_i2c_write(0x0F12, 0x016D); // #TVAR_ash_pGAS[142]
s5k5caga_i2c_write(0x0F12, 0x012E); // #TVAR_ash_pGAS[143]
s5k5caga_i2c_write(0x0F12, 0x012A); // #TVAR_ash_pGAS[144]
s5k5caga_i2c_write(0x0F12, 0x0100); // #TVAR_ash_pGAS[145]
s5k5caga_i2c_write(0x0F12, 0x00D3); // #TVAR_ash_pGAS[146]
s5k5caga_i2c_write(0x0F12, 0x00B7); // #TVAR_ash_pGAS[147]
s5k5caga_i2c_write(0x0F12, 0x00A5); // #TVAR_ash_pGAS[148]
s5k5caga_i2c_write(0x0F12, 0x009F); // #TVAR_ash_pGAS[149]
s5k5caga_i2c_write(0x0F12, 0x00A0); // #TVAR_ash_pGAS[150]
s5k5caga_i2c_write(0x0F12, 0x00AB); // #TVAR_ash_pGAS[151]
s5k5caga_i2c_write(0x0F12, 0x00C2); // #TVAR_ash_pGAS[152]
s5k5caga_i2c_write(0x0F12, 0x00E6); // #TVAR_ash_pGAS[153]
s5k5caga_i2c_write(0x0F12, 0x0108); // #TVAR_ash_pGAS[154]
s5k5caga_i2c_write(0x0F12, 0x010B); // #TVAR_ash_pGAS[155]
s5k5caga_i2c_write(0x0F12, 0x0125); // #TVAR_ash_pGAS[156]
s5k5caga_i2c_write(0x0F12, 0x0105); // #TVAR_ash_pGAS[157]
s5k5caga_i2c_write(0x0F12, 0x00D0); // #TVAR_ash_pGAS[158]
s5k5caga_i2c_write(0x0F12, 0x00A7); // #TVAR_ash_pGAS[159]
s5k5caga_i2c_write(0x0F12, 0x008B); // #TVAR_ash_pGAS[160]
s5k5caga_i2c_write(0x0F12, 0x0078); // #TVAR_ash_pGAS[161]
s5k5caga_i2c_write(0x0F12, 0x0071); // #TVAR_ash_pGAS[162]
s5k5caga_i2c_write(0x0F12, 0x0074); // #TVAR_ash_pGAS[163]
s5k5caga_i2c_write(0x0F12, 0x0081); // #TVAR_ash_pGAS[164]
s5k5caga_i2c_write(0x0F12, 0x0097); // #TVAR_ash_pGAS[165]
s5k5caga_i2c_write(0x0F12, 0x00BC); // #TVAR_ash_pGAS[166]
s5k5caga_i2c_write(0x0F12, 0x00F2); // #TVAR_ash_pGAS[167]
s5k5caga_i2c_write(0x0F12, 0x0107); // #TVAR_ash_pGAS[168]
s5k5caga_i2c_write(0x0F12, 0x0108); // #TVAR_ash_pGAS[169]
s5k5caga_i2c_write(0x0F12, 0x00D9); // #TVAR_ash_pGAS[170]
s5k5caga_i2c_write(0x0F12, 0x00A3); // #TVAR_ash_pGAS[171]
s5k5caga_i2c_write(0x0F12, 0x007D); // #TVAR_ash_pGAS[172]
s5k5caga_i2c_write(0x0F12, 0x005E); // #TVAR_ash_pGAS[173]
s5k5caga_i2c_write(0x0F12, 0x004A); // #TVAR_ash_pGAS[174]
s5k5caga_i2c_write(0x0F12, 0x0043); // #TVAR_ash_pGAS[175]
s5k5caga_i2c_write(0x0F12, 0x0048); // #TVAR_ash_pGAS[176]
s5k5caga_i2c_write(0x0F12, 0x005A); // #TVAR_ash_pGAS[177]
s5k5caga_i2c_write(0x0F12, 0x0074); // #TVAR_ash_pGAS[178]
s5k5caga_i2c_write(0x0F12, 0x009A); // #TVAR_ash_pGAS[179]
s5k5caga_i2c_write(0x0F12, 0x00D1); // #TVAR_ash_pGAS[180]
s5k5caga_i2c_write(0x0F12, 0x00FE); // #TVAR_ash_pGAS[181]
s5k5caga_i2c_write(0x0F12, 0x00E7); // #TVAR_ash_pGAS[182]
s5k5caga_i2c_write(0x0F12, 0x00B4); // #TVAR_ash_pGAS[183]
s5k5caga_i2c_write(0x0F12, 0x0083); // #TVAR_ash_pGAS[184]
s5k5caga_i2c_write(0x0F12, 0x0058); // #TVAR_ash_pGAS[185]
s5k5caga_i2c_write(0x0F12, 0x0037); // #TVAR_ash_pGAS[186]
s5k5caga_i2c_write(0x0F12, 0x0022); // #TVAR_ash_pGAS[187]
s5k5caga_i2c_write(0x0F12, 0x001A); // #TVAR_ash_pGAS[188]
s5k5caga_i2c_write(0x0F12, 0x0022); // #TVAR_ash_pGAS[189]
s5k5caga_i2c_write(0x0F12, 0x0039); // #TVAR_ash_pGAS[190]
s5k5caga_i2c_write(0x0F12, 0x0059); // #TVAR_ash_pGAS[191]
s5k5caga_i2c_write(0x0F12, 0x0083); // #TVAR_ash_pGAS[192]
s5k5caga_i2c_write(0x0F12, 0x00BA); // #TVAR_ash_pGAS[193]
s5k5caga_i2c_write(0x0F12, 0x00ED); // #TVAR_ash_pGAS[194]
s5k5caga_i2c_write(0x0F12, 0x00D1); // #TVAR_ash_pGAS[195]
s5k5caga_i2c_write(0x0F12, 0x00A1); // #TVAR_ash_pGAS[196]
s5k5caga_i2c_write(0x0F12, 0x006C); // #TVAR_ash_pGAS[197]
s5k5caga_i2c_write(0x0F12, 0x0041); // #TVAR_ash_pGAS[198]
s5k5caga_i2c_write(0x0F12, 0x0020); // #TVAR_ash_pGAS[199]
s5k5caga_i2c_write(0x0F12, 0x000C); // #TVAR_ash_pGAS[200]
s5k5caga_i2c_write(0x0F12, 0x0009); // #TVAR_ash_pGAS[201]
s5k5caga_i2c_write(0x0F12, 0x0012); // #TVAR_ash_pGAS[202]
s5k5caga_i2c_write(0x0F12, 0x0026); // #TVAR_ash_pGAS[203]
s5k5caga_i2c_write(0x0F12, 0x004A); // #TVAR_ash_pGAS[204]
s5k5caga_i2c_write(0x0F12, 0x0074); // #TVAR_ash_pGAS[205]
s5k5caga_i2c_write(0x0F12, 0x00A9); // #TVAR_ash_pGAS[206]
s5k5caga_i2c_write(0x0F12, 0x00E0); // #TVAR_ash_pGAS[207]
s5k5caga_i2c_write(0x0F12, 0x00C7); // #TVAR_ash_pGAS[208]
s5k5caga_i2c_write(0x0F12, 0x0097); // #TVAR_ash_pGAS[209]
s5k5caga_i2c_write(0x0F12, 0x0060); // #TVAR_ash_pGAS[210]
s5k5caga_i2c_write(0x0F12, 0x0037); // #TVAR_ash_pGAS[211]
s5k5caga_i2c_write(0x0F12, 0x0016); // #TVAR_ash_pGAS[212]
s5k5caga_i2c_write(0x0F12, 0x0003); // #TVAR_ash_pGAS[213]
s5k5caga_i2c_write(0x0F12, 0x0000); // #TVAR_ash_pGAS[214]
s5k5caga_i2c_write(0x0F12, 0x000B); // #TVAR_ash_pGAS[215]
s5k5caga_i2c_write(0x0F12, 0x001E); // #TVAR_ash_pGAS[216]
s5k5caga_i2c_write(0x0F12, 0x0045); // #TVAR_ash_pGAS[217]
s5k5caga_i2c_write(0x0F12, 0x0070); // #TVAR_ash_pGAS[218]
s5k5caga_i2c_write(0x0F12, 0x00A8); // #TVAR_ash_pGAS[219]
s5k5caga_i2c_write(0x0F12, 0x00DC); // #TVAR_ash_pGAS[220]
s5k5caga_i2c_write(0x0F12, 0x00C7); // #TVAR_ash_pGAS[221]
s5k5caga_i2c_write(0x0F12, 0x009A); // #TVAR_ash_pGAS[222]
s5k5caga_i2c_write(0x0F12, 0x0064); // #TVAR_ash_pGAS[223]
s5k5caga_i2c_write(0x0F12, 0x003B); // #TVAR_ash_pGAS[224]
s5k5caga_i2c_write(0x0F12, 0x001B); // #TVAR_ash_pGAS[225]
s5k5caga_i2c_write(0x0F12, 0x0009); // #TVAR_ash_pGAS[226]
s5k5caga_i2c_write(0x0F12, 0x0004); // #TVAR_ash_pGAS[227]
s5k5caga_i2c_write(0x0F12, 0x000E); // #TVAR_ash_pGAS[228]
s5k5caga_i2c_write(0x0F12, 0x0028); // #TVAR_ash_pGAS[229]
s5k5caga_i2c_write(0x0F12, 0x004C); // #TVAR_ash_pGAS[230]
s5k5caga_i2c_write(0x0F12, 0x0078); // #TVAR_ash_pGAS[231]
s5k5caga_i2c_write(0x0F12, 0x00B0); // #TVAR_ash_pGAS[232]
s5k5caga_i2c_write(0x0F12, 0x00E5); // #TVAR_ash_pGAS[233]
s5k5caga_i2c_write(0x0F12, 0x00D6); // #TVAR_ash_pGAS[234]
s5k5caga_i2c_write(0x0F12, 0x00A8); // #TVAR_ash_pGAS[235]
s5k5caga_i2c_write(0x0F12, 0x0073); // #TVAR_ash_pGAS[236]
s5k5caga_i2c_write(0x0F12, 0x004C); // #TVAR_ash_pGAS[237]
s5k5caga_i2c_write(0x0F12, 0x002F); // #TVAR_ash_pGAS[238]
s5k5caga_i2c_write(0x0F12, 0x001C); // #TVAR_ash_pGAS[239]
s5k5caga_i2c_write(0x0F12, 0x001A); // #TVAR_ash_pGAS[240]
s5k5caga_i2c_write(0x0F12, 0x0024); // #TVAR_ash_pGAS[241]
s5k5caga_i2c_write(0x0F12, 0x003D); // #TVAR_ash_pGAS[242]
s5k5caga_i2c_write(0x0F12, 0x0060); // #TVAR_ash_pGAS[243]
s5k5caga_i2c_write(0x0F12, 0x008B); // #TVAR_ash_pGAS[244]
s5k5caga_i2c_write(0x0F12, 0x00C4); // #TVAR_ash_pGAS[245]
s5k5caga_i2c_write(0x0F12, 0x00F5); // #TVAR_ash_pGAS[246]
s5k5caga_i2c_write(0x0F12, 0x00EF); // #TVAR_ash_pGAS[247]
s5k5caga_i2c_write(0x0F12, 0x00C6); // #TVAR_ash_pGAS[248]
s5k5caga_i2c_write(0x0F12, 0x008F); // #TVAR_ash_pGAS[249]
s5k5caga_i2c_write(0x0F12, 0x006B); // #TVAR_ash_pGAS[250]
s5k5caga_i2c_write(0x0F12, 0x004F); // #TVAR_ash_pGAS[251]
s5k5caga_i2c_write(0x0F12, 0x0040); // #TVAR_ash_pGAS[252]
s5k5caga_i2c_write(0x0F12, 0x003E); // #TVAR_ash_pGAS[253]
s5k5caga_i2c_write(0x0F12, 0x0048); // #TVAR_ash_pGAS[254]
s5k5caga_i2c_write(0x0F12, 0x005F); // #TVAR_ash_pGAS[255]
s5k5caga_i2c_write(0x0F12, 0x0080); // #TVAR_ash_pGAS[256]
s5k5caga_i2c_write(0x0F12, 0x00A9); // #TVAR_ash_pGAS[257]
s5k5caga_i2c_write(0x0F12, 0x00E5); // #TVAR_ash_pGAS[258]
s5k5caga_i2c_write(0x0F12, 0x010D); // #TVAR_ash_pGAS[259]
s5k5caga_i2c_write(0x0F12, 0x0109); // #TVAR_ash_pGAS[260]
s5k5caga_i2c_write(0x0F12, 0x00F2); // #TVAR_ash_pGAS[261]
s5k5caga_i2c_write(0x0F12, 0x00B9); // #TVAR_ash_pGAS[262]
s5k5caga_i2c_write(0x0F12, 0x0094); // #TVAR_ash_pGAS[263]
s5k5caga_i2c_write(0x0F12, 0x007F); // #TVAR_ash_pGAS[264]
s5k5caga_i2c_write(0x0F12, 0x0071); // #TVAR_ash_pGAS[265]
s5k5caga_i2c_write(0x0F12, 0x0071); // #TVAR_ash_pGAS[266]
s5k5caga_i2c_write(0x0F12, 0x007B); // #TVAR_ash_pGAS[267]
s5k5caga_i2c_write(0x0F12, 0x0090); // #TVAR_ash_pGAS[268]
s5k5caga_i2c_write(0x0F12, 0x00AF); // #TVAR_ash_pGAS[269]
s5k5caga_i2c_write(0x0F12, 0x00D8); // #TVAR_ash_pGAS[270]
s5k5caga_i2c_write(0x0F12, 0x0114); // #TVAR_ash_pGAS[271]
s5k5caga_i2c_write(0x0F12, 0x0125); // #TVAR_ash_pGAS[272]
s5k5caga_i2c_write(0x0F12, 0x0113); // #TVAR_ash_pGAS[273]
s5k5caga_i2c_write(0x0F12, 0x011A); // #TVAR_ash_pGAS[274]
s5k5caga_i2c_write(0x0F12, 0x00EA); // #TVAR_ash_pGAS[275]
s5k5caga_i2c_write(0x0F12, 0x00C3); // #TVAR_ash_pGAS[276]
s5k5caga_i2c_write(0x0F12, 0x00AF); // #TVAR_ash_pGAS[277]
s5k5caga_i2c_write(0x0F12, 0x00A4); // #TVAR_ash_pGAS[278]
s5k5caga_i2c_write(0x0F12, 0x00A6); // #TVAR_ash_pGAS[279]
s5k5caga_i2c_write(0x0F12, 0x00B1); // #TVAR_ash_pGAS[280]
s5k5caga_i2c_write(0x0F12, 0x00C5); // #TVAR_ash_pGAS[281]
s5k5caga_i2c_write(0x0F12, 0x00E4); // #TVAR_ash_pGAS[282]
s5k5caga_i2c_write(0x0F12, 0x010E); // #TVAR_ash_pGAS[283]
s5k5caga_i2c_write(0x0F12, 0x013C); // #TVAR_ash_pGAS[284]
s5k5caga_i2c_write(0x0F12, 0x0134); // #TVAR_ash_pGAS[285]
s5k5caga_i2c_write(0x0F12, 0x0112); // #TVAR_ash_pGAS[286]
s5k5caga_i2c_write(0x0F12, 0x0111); // #TVAR_ash_pGAS[287]
s5k5caga_i2c_write(0x0F12, 0x00E8); // #TVAR_ash_pGAS[288]
s5k5caga_i2c_write(0x0F12, 0x00BC); // #TVAR_ash_pGAS[289]
s5k5caga_i2c_write(0x0F12, 0x00A6); // #TVAR_ash_pGAS[290]
s5k5caga_i2c_write(0x0F12, 0x009A); // #TVAR_ash_pGAS[291]
s5k5caga_i2c_write(0x0F12, 0x0099); // #TVAR_ash_pGAS[292]
s5k5caga_i2c_write(0x0F12, 0x00A1); // #TVAR_ash_pGAS[293]
s5k5caga_i2c_write(0x0F12, 0x00B2); // #TVAR_ash_pGAS[294]
s5k5caga_i2c_write(0x0F12, 0x00CD); // #TVAR_ash_pGAS[295]
s5k5caga_i2c_write(0x0F12, 0x00F7); // #TVAR_ash_pGAS[296]
s5k5caga_i2c_write(0x0F12, 0x011B); // #TVAR_ash_pGAS[297]
s5k5caga_i2c_write(0x0F12, 0x0118); // #TVAR_ash_pGAS[298]
s5k5caga_i2c_write(0x0F12, 0x010C); // #TVAR_ash_pGAS[299]
s5k5caga_i2c_write(0x0F12, 0x00EF); // #TVAR_ash_pGAS[300]
s5k5caga_i2c_write(0x0F12, 0x00BE); // #TVAR_ash_pGAS[301]
s5k5caga_i2c_write(0x0F12, 0x0097); // #TVAR_ash_pGAS[302]
s5k5caga_i2c_write(0x0F12, 0x007F); // #TVAR_ash_pGAS[303]
s5k5caga_i2c_write(0x0F12, 0x0070); // #TVAR_ash_pGAS[304]
s5k5caga_i2c_write(0x0F12, 0x006E); // #TVAR_ash_pGAS[305]
s5k5caga_i2c_write(0x0F12, 0x0076); // #TVAR_ash_pGAS[306]
s5k5caga_i2c_write(0x0F12, 0x008A); // #TVAR_ash_pGAS[307]
s5k5caga_i2c_write(0x0F12, 0x00A3); // #TVAR_ash_pGAS[308]
s5k5caga_i2c_write(0x0F12, 0x00CE); // #TVAR_ash_pGAS[309]
s5k5caga_i2c_write(0x0F12, 0x0106); // #TVAR_ash_pGAS[310]
s5k5caga_i2c_write(0x0F12, 0x0118); // #TVAR_ash_pGAS[311]
s5k5caga_i2c_write(0x0F12, 0x00F5); // #TVAR_ash_pGAS[312]
s5k5caga_i2c_write(0x0F12, 0x00C7); // #TVAR_ash_pGAS[313]
s5k5caga_i2c_write(0x0F12, 0x0096); // #TVAR_ash_pGAS[314]
s5k5caga_i2c_write(0x0F12, 0x0072); // #TVAR_ash_pGAS[315]
s5k5caga_i2c_write(0x0F12, 0x0057); // #TVAR_ash_pGAS[316]
s5k5caga_i2c_write(0x0F12, 0x0045); // #TVAR_ash_pGAS[317]
s5k5caga_i2c_write(0x0F12, 0x0042); // #TVAR_ash_pGAS[318]
s5k5caga_i2c_write(0x0F12, 0x004A); // #TVAR_ash_pGAS[319]
s5k5caga_i2c_write(0x0F12, 0x0062); // #TVAR_ash_pGAS[320]
s5k5caga_i2c_write(0x0F12, 0x007F); // #TVAR_ash_pGAS[321]
s5k5caga_i2c_write(0x0F12, 0x00A9); // #TVAR_ash_pGAS[322]
s5k5caga_i2c_write(0x0F12, 0x00E1); // #TVAR_ash_pGAS[323]
s5k5caga_i2c_write(0x0F12, 0x0110); // #TVAR_ash_pGAS[324]
s5k5caga_i2c_write(0x0F12, 0x00D7); // #TVAR_ash_pGAS[325]
s5k5caga_i2c_write(0x0F12, 0x00AA); // #TVAR_ash_pGAS[326]
s5k5caga_i2c_write(0x0F12, 0x0079); // #TVAR_ash_pGAS[327]
s5k5caga_i2c_write(0x0F12, 0x0052); // #TVAR_ash_pGAS[328]
s5k5caga_i2c_write(0x0F12, 0x0034); // #TVAR_ash_pGAS[329]
s5k5caga_i2c_write(0x0F12, 0x0020); // #TVAR_ash_pGAS[330]
s5k5caga_i2c_write(0x0F12, 0x0019); // #TVAR_ash_pGAS[331]
s5k5caga_i2c_write(0x0F12, 0x0024); // #TVAR_ash_pGAS[332]
s5k5caga_i2c_write(0x0F12, 0x003F); // #TVAR_ash_pGAS[333]
s5k5caga_i2c_write(0x0F12, 0x0062); // #TVAR_ash_pGAS[334]
s5k5caga_i2c_write(0x0F12, 0x008F); // #TVAR_ash_pGAS[335]
s5k5caga_i2c_write(0x0F12, 0x00C7); // #TVAR_ash_pGAS[336]
s5k5caga_i2c_write(0x0F12, 0x00FA); // #TVAR_ash_pGAS[337]
s5k5caga_i2c_write(0x0F12, 0x00C7); // #TVAR_ash_pGAS[338]
s5k5caga_i2c_write(0x0F12, 0x009A); // #TVAR_ash_pGAS[339]
s5k5caga_i2c_write(0x0F12, 0x0067); // #TVAR_ash_pGAS[340]
s5k5caga_i2c_write(0x0F12, 0x003F); // #TVAR_ash_pGAS[341]
s5k5caga_i2c_write(0x0F12, 0x001F); // #TVAR_ash_pGAS[342]
s5k5caga_i2c_write(0x0F12, 0x000C); // #TVAR_ash_pGAS[343]
s5k5caga_i2c_write(0x0F12, 0x0009); // #TVAR_ash_pGAS[344]
s5k5caga_i2c_write(0x0F12, 0x0013); // #TVAR_ash_pGAS[345]
s5k5caga_i2c_write(0x0F12, 0x002A); // #TVAR_ash_pGAS[346]
s5k5caga_i2c_write(0x0F12, 0x004F); // #TVAR_ash_pGAS[347]
s5k5caga_i2c_write(0x0F12, 0x007C); // #TVAR_ash_pGAS[348]
s5k5caga_i2c_write(0x0F12, 0x00B2); // #TVAR_ash_pGAS[349]
s5k5caga_i2c_write(0x0F12, 0x00E8); // #TVAR_ash_pGAS[350]
s5k5caga_i2c_write(0x0F12, 0x00C3); // #TVAR_ash_pGAS[351]
s5k5caga_i2c_write(0x0F12, 0x0095); // #TVAR_ash_pGAS[352]
s5k5caga_i2c_write(0x0F12, 0x0061); // #TVAR_ash_pGAS[353]
s5k5caga_i2c_write(0x0F12, 0x0039); // #TVAR_ash_pGAS[354]
s5k5caga_i2c_write(0x0F12, 0x0018); // #TVAR_ash_pGAS[355]
s5k5caga_i2c_write(0x0F12, 0x0004); // #TVAR_ash_pGAS[356]
s5k5caga_i2c_write(0x0F12, 0x0000); // #TVAR_ash_pGAS[357]
s5k5caga_i2c_write(0x0F12, 0x000B); // #TVAR_ash_pGAS[358]
s5k5caga_i2c_write(0x0F12, 0x0020); // #TVAR_ash_pGAS[359]
s5k5caga_i2c_write(0x0F12, 0x0047); // #TVAR_ash_pGAS[360]
s5k5caga_i2c_write(0x0F12, 0x0074); // #TVAR_ash_pGAS[361]
s5k5caga_i2c_write(0x0F12, 0x00A9); // #TVAR_ash_pGAS[362]
s5k5caga_i2c_write(0x0F12, 0x00DF); // #TVAR_ash_pGAS[363]
s5k5caga_i2c_write(0x0F12, 0x00C9); // #TVAR_ash_pGAS[364]
s5k5caga_i2c_write(0x0F12, 0x009D); // #TVAR_ash_pGAS[365]
s5k5caga_i2c_write(0x0F12, 0x0069); // #TVAR_ash_pGAS[366]
s5k5caga_i2c_write(0x0F12, 0x0041); // #TVAR_ash_pGAS[367]
s5k5caga_i2c_write(0x0F12, 0x001F); // #TVAR_ash_pGAS[368]
s5k5caga_i2c_write(0x0F12, 0x000B); // #TVAR_ash_pGAS[369]
s5k5caga_i2c_write(0x0F12, 0x0005); // #TVAR_ash_pGAS[370]
s5k5caga_i2c_write(0x0F12, 0x000E); // #TVAR_ash_pGAS[371]
s5k5caga_i2c_write(0x0F12, 0x0026); // #TVAR_ash_pGAS[372]
s5k5caga_i2c_write(0x0F12, 0x004B); // #TVAR_ash_pGAS[373]
s5k5caga_i2c_write(0x0F12, 0x0076); // #TVAR_ash_pGAS[374]
s5k5caga_i2c_write(0x0F12, 0x00AE); // #TVAR_ash_pGAS[375]
s5k5caga_i2c_write(0x0F12, 0x00E2); // #TVAR_ash_pGAS[376]
s5k5caga_i2c_write(0x0F12, 0x00DD); // #TVAR_ash_pGAS[377]
s5k5caga_i2c_write(0x0F12, 0x00B2); // #TVAR_ash_pGAS[378]
s5k5caga_i2c_write(0x0F12, 0x007D); // #TVAR_ash_pGAS[379]
s5k5caga_i2c_write(0x0F12, 0x0056); // #TVAR_ash_pGAS[380]
s5k5caga_i2c_write(0x0F12, 0x0035); // #TVAR_ash_pGAS[381]
s5k5caga_i2c_write(0x0F12, 0x0020); // #TVAR_ash_pGAS[382]
s5k5caga_i2c_write(0x0F12, 0x001A); // #TVAR_ash_pGAS[383]
s5k5caga_i2c_write(0x0F12, 0x0023); // #TVAR_ash_pGAS[384]
s5k5caga_i2c_write(0x0F12, 0x0039); // #TVAR_ash_pGAS[385]
s5k5caga_i2c_write(0x0F12, 0x005A); // #TVAR_ash_pGAS[386]
s5k5caga_i2c_write(0x0F12, 0x0084); // #TVAR_ash_pGAS[387]
s5k5caga_i2c_write(0x0F12, 0x00B9); // #TVAR_ash_pGAS[388]
s5k5caga_i2c_write(0x0F12, 0x00ED); // #TVAR_ash_pGAS[389]
s5k5caga_i2c_write(0x0F12, 0x00FB); // #TVAR_ash_pGAS[390]
s5k5caga_i2c_write(0x0F12, 0x00D3); // #TVAR_ash_pGAS[391]
s5k5caga_i2c_write(0x0F12, 0x009C); // #TVAR_ash_pGAS[392]
s5k5caga_i2c_write(0x0F12, 0x0078); // #TVAR_ash_pGAS[393]
s5k5caga_i2c_write(0x0F12, 0x005B); // #TVAR_ash_pGAS[394]
s5k5caga_i2c_write(0x0F12, 0x0046); // #TVAR_ash_pGAS[395]
s5k5caga_i2c_write(0x0F12, 0x003F); // #TVAR_ash_pGAS[396]
s5k5caga_i2c_write(0x0F12, 0x0046); // #TVAR_ash_pGAS[397]
s5k5caga_i2c_write(0x0F12, 0x005A); // #TVAR_ash_pGAS[398]
s5k5caga_i2c_write(0x0F12, 0x0077); // #TVAR_ash_pGAS[399]
s5k5caga_i2c_write(0x0F12, 0x009E); // #TVAR_ash_pGAS[400]
s5k5caga_i2c_write(0x0F12, 0x00D6); // #TVAR_ash_pGAS[401]
s5k5caga_i2c_write(0x0F12, 0x0103); // #TVAR_ash_pGAS[402]
s5k5caga_i2c_write(0x0F12, 0x0119); // #TVAR_ash_pGAS[403]
s5k5caga_i2c_write(0x0F12, 0x0102); // #TVAR_ash_pGAS[404]
s5k5caga_i2c_write(0x0F12, 0x00CA); // #TVAR_ash_pGAS[405]
s5k5caga_i2c_write(0x0F12, 0x00A4); // #TVAR_ash_pGAS[406]
s5k5caga_i2c_write(0x0F12, 0x008B); // #TVAR_ash_pGAS[407]
s5k5caga_i2c_write(0x0F12, 0x0078); // #TVAR_ash_pGAS[408]
s5k5caga_i2c_write(0x0F12, 0x0072); // #TVAR_ash_pGAS[409]
s5k5caga_i2c_write(0x0F12, 0x0076); // #TVAR_ash_pGAS[410]
s5k5caga_i2c_write(0x0F12, 0x0088); // #TVAR_ash_pGAS[411]
s5k5caga_i2c_write(0x0F12, 0x00A2); // #TVAR_ash_pGAS[412]
s5k5caga_i2c_write(0x0F12, 0x00C9); // #TVAR_ash_pGAS[413]
s5k5caga_i2c_write(0x0F12, 0x0100); // #TVAR_ash_pGAS[414]
s5k5caga_i2c_write(0x0F12, 0x0117); // #TVAR_ash_pGAS[415]
s5k5caga_i2c_write(0x0F12, 0x0127); // #TVAR_ash_pGAS[416]
s5k5caga_i2c_write(0x0F12, 0x012D); // #TVAR_ash_pGAS[417]
s5k5caga_i2c_write(0x0F12, 0x00FE); // #TVAR_ash_pGAS[418]
s5k5caga_i2c_write(0x0F12, 0x00D8); // #TVAR_ash_pGAS[419]
s5k5caga_i2c_write(0x0F12, 0x00BE); // #TVAR_ash_pGAS[420]
s5k5caga_i2c_write(0x0F12, 0x00B0); // #TVAR_ash_pGAS[421]
s5k5caga_i2c_write(0x0F12, 0x00A8); // #TVAR_ash_pGAS[422]
s5k5caga_i2c_write(0x0F12, 0x00AF); // #TVAR_ash_pGAS[423]
s5k5caga_i2c_write(0x0F12, 0x00BB); // #TVAR_ash_pGAS[424]
s5k5caga_i2c_write(0x0F12, 0x00D5); // #TVAR_ash_pGAS[425]
s5k5caga_i2c_write(0x0F12, 0x00FC); // #TVAR_ash_pGAS[426]
s5k5caga_i2c_write(0x0F12, 0x0126); // #TVAR_ash_pGAS[427]
s5k5caga_i2c_write(0x0F12, 0x0125); // #TVAR_ash_pGAS[428]
s5k5caga_i2c_write(0x0F12, 0x00DF); // #TVAR_ash_pGAS[429]
s5k5caga_i2c_write(0x0F12, 0x00EC); // #TVAR_ash_pGAS[430]
s5k5caga_i2c_write(0x0F12, 0x00C9); // #TVAR_ash_pGAS[431]
s5k5caga_i2c_write(0x0F12, 0x00A5); // #TVAR_ash_pGAS[432]
s5k5caga_i2c_write(0x0F12, 0x0094); // #TVAR_ash_pGAS[433]
s5k5caga_i2c_write(0x0F12, 0x008B); // #TVAR_ash_pGAS[434]
s5k5caga_i2c_write(0x0F12, 0x008C); // #TVAR_ash_pGAS[435]
s5k5caga_i2c_write(0x0F12, 0x0093); // #TVAR_ash_pGAS[436]
s5k5caga_i2c_write(0x0F12, 0x00A1); // #TVAR_ash_pGAS[437]
s5k5caga_i2c_write(0x0F12, 0x00BA); // #TVAR_ash_pGAS[438]
s5k5caga_i2c_write(0x0F12, 0x00DF); // #TVAR_ash_pGAS[439]
s5k5caga_i2c_write(0x0F12, 0x0101); // #TVAR_ash_pGAS[440]
s5k5caga_i2c_write(0x0F12, 0x00F6); // #TVAR_ash_pGAS[441]
s5k5caga_i2c_write(0x0F12, 0x00DA); // #TVAR_ash_pGAS[442]
s5k5caga_i2c_write(0x0F12, 0x00CE); // #TVAR_ash_pGAS[443]
s5k5caga_i2c_write(0x0F12, 0x00A4); // #TVAR_ash_pGAS[444]
s5k5caga_i2c_write(0x0F12, 0x0083); // #TVAR_ash_pGAS[445]
s5k5caga_i2c_write(0x0F12, 0x0072); // #TVAR_ash_pGAS[446]
s5k5caga_i2c_write(0x0F12, 0x0068); // #TVAR_ash_pGAS[447]
s5k5caga_i2c_write(0x0F12, 0x0067); // #TVAR_ash_pGAS[448]
s5k5caga_i2c_write(0x0F12, 0x006F); // #TVAR_ash_pGAS[449]
s5k5caga_i2c_write(0x0F12, 0x007E); // #TVAR_ash_pGAS[450]
s5k5caga_i2c_write(0x0F12, 0x0094); // #TVAR_ash_pGAS[451]
s5k5caga_i2c_write(0x0F12, 0x00B8); // #TVAR_ash_pGAS[452]
s5k5caga_i2c_write(0x0F12, 0x00EB); // #TVAR_ash_pGAS[453]
s5k5caga_i2c_write(0x0F12, 0x00F0); // #TVAR_ash_pGAS[454]
s5k5caga_i2c_write(0x0F12, 0x00C4); // #TVAR_ash_pGAS[455]
s5k5caga_i2c_write(0x0F12, 0x00A7); // #TVAR_ash_pGAS[456]
s5k5caga_i2c_write(0x0F12, 0x007D); // #TVAR_ash_pGAS[457]
s5k5caga_i2c_write(0x0F12, 0x005F); // #TVAR_ash_pGAS[458]
s5k5caga_i2c_write(0x0F12, 0x004C); // #TVAR_ash_pGAS[459]
s5k5caga_i2c_write(0x0F12, 0x0040); // #TVAR_ash_pGAS[460]
s5k5caga_i2c_write(0x0F12, 0x003F); // #TVAR_ash_pGAS[461]
s5k5caga_i2c_write(0x0F12, 0x0048); // #TVAR_ash_pGAS[462]
s5k5caga_i2c_write(0x0F12, 0x005A); // #TVAR_ash_pGAS[463]
s5k5caga_i2c_write(0x0F12, 0x0070); // #TVAR_ash_pGAS[464]
s5k5caga_i2c_write(0x0F12, 0x0092); // #TVAR_ash_pGAS[465]
s5k5caga_i2c_write(0x0F12, 0x00C6); // #TVAR_ash_pGAS[466]
s5k5caga_i2c_write(0x0F12, 0x00E2); // #TVAR_ash_pGAS[467]
s5k5caga_i2c_write(0x0F12, 0x00A5); // #TVAR_ash_pGAS[468]
s5k5caga_i2c_write(0x0F12, 0x0086); // #TVAR_ash_pGAS[469]
s5k5caga_i2c_write(0x0F12, 0x0060); // #TVAR_ash_pGAS[470]
s5k5caga_i2c_write(0x0F12, 0x0041); // #TVAR_ash_pGAS[471]
s5k5caga_i2c_write(0x0F12, 0x002A); // #TVAR_ash_pGAS[472]
s5k5caga_i2c_write(0x0F12, 0x001E); // #TVAR_ash_pGAS[473]
s5k5caga_i2c_write(0x0F12, 0x001A); // #TVAR_ash_pGAS[474]
s5k5caga_i2c_write(0x0F12, 0x0024); // #TVAR_ash_pGAS[475]
s5k5caga_i2c_write(0x0F12, 0x0038); // #TVAR_ash_pGAS[476]
s5k5caga_i2c_write(0x0F12, 0x0054); // #TVAR_ash_pGAS[477]
s5k5caga_i2c_write(0x0F12, 0x0078); // #TVAR_ash_pGAS[478]
s5k5caga_i2c_write(0x0F12, 0x00A8); // #TVAR_ash_pGAS[479]
s5k5caga_i2c_write(0x0F12, 0x00CB); // #TVAR_ash_pGAS[480]
s5k5caga_i2c_write(0x0F12, 0x0094); // #TVAR_ash_pGAS[481]
s5k5caga_i2c_write(0x0F12, 0x0078); // #TVAR_ash_pGAS[482]
s5k5caga_i2c_write(0x0F12, 0x004F); // #TVAR_ash_pGAS[483]
s5k5caga_i2c_write(0x0F12, 0x002E); // #TVAR_ash_pGAS[484]
s5k5caga_i2c_write(0x0F12, 0x0016); // #TVAR_ash_pGAS[485]
s5k5caga_i2c_write(0x0F12, 0x000A); // #TVAR_ash_pGAS[486]
s5k5caga_i2c_write(0x0F12, 0x000A); // #TVAR_ash_pGAS[487]
s5k5caga_i2c_write(0x0F12, 0x0012); // #TVAR_ash_pGAS[488]
s5k5caga_i2c_write(0x0F12, 0x0024); // #TVAR_ash_pGAS[489]
s5k5caga_i2c_write(0x0F12, 0x0042); // #TVAR_ash_pGAS[490]
s5k5caga_i2c_write(0x0F12, 0x0066); // #TVAR_ash_pGAS[491]
s5k5caga_i2c_write(0x0F12, 0x0094); // #TVAR_ash_pGAS[492]
s5k5caga_i2c_write(0x0F12, 0x00B9); // #TVAR_ash_pGAS[493]
s5k5caga_i2c_write(0x0F12, 0x008F); // #TVAR_ash_pGAS[494]
s5k5caga_i2c_write(0x0F12, 0x0071); // #TVAR_ash_pGAS[495]
s5k5caga_i2c_write(0x0F12, 0x0047); // #TVAR_ash_pGAS[496]
s5k5caga_i2c_write(0x0F12, 0x0026); // #TVAR_ash_pGAS[497]
s5k5caga_i2c_write(0x0F12, 0x000E); // #TVAR_ash_pGAS[498]
s5k5caga_i2c_write(0x0F12, 0x0002); // #TVAR_ash_pGAS[499]
s5k5caga_i2c_write(0x0F12, 0x0000); // #TVAR_ash_pGAS[500]
s5k5caga_i2c_write(0x0F12, 0x0009); // #TVAR_ash_pGAS[501]
s5k5caga_i2c_write(0x0F12, 0x0018); // #TVAR_ash_pGAS[502]
s5k5caga_i2c_write(0x0F12, 0x0039); // #TVAR_ash_pGAS[503]
s5k5caga_i2c_write(0x0F12, 0x005C); // #TVAR_ash_pGAS[504]
s5k5caga_i2c_write(0x0F12, 0x008A); // #TVAR_ash_pGAS[505]
s5k5caga_i2c_write(0x0F12, 0x00B0); // #TVAR_ash_pGAS[506]
s5k5caga_i2c_write(0x0F12, 0x0091); // #TVAR_ash_pGAS[507]
s5k5caga_i2c_write(0x0F12, 0x0075); // #TVAR_ash_pGAS[508]
s5k5caga_i2c_write(0x0F12, 0x004B); // #TVAR_ash_pGAS[509]
s5k5caga_i2c_write(0x0F12, 0x002C); // #TVAR_ash_pGAS[510]
s5k5caga_i2c_write(0x0F12, 0x0013); // #TVAR_ash_pGAS[511]
s5k5caga_i2c_write(0x0F12, 0x0006); // #TVAR_ash_pGAS[512]
s5k5caga_i2c_write(0x0F12, 0x0002); // #TVAR_ash_pGAS[513]
s5k5caga_i2c_write(0x0F12, 0x000A); // #TVAR_ash_pGAS[514]
s5k5caga_i2c_write(0x0F12, 0x001D); // #TVAR_ash_pGAS[515]
s5k5caga_i2c_write(0x0F12, 0x003A); // #TVAR_ash_pGAS[516]
s5k5caga_i2c_write(0x0F12, 0x005C); // #TVAR_ash_pGAS[517]
s5k5caga_i2c_write(0x0F12, 0x008A); // #TVAR_ash_pGAS[518]
s5k5caga_i2c_write(0x0F12, 0x00B1); // #TVAR_ash_pGAS[519]
s5k5caga_i2c_write(0x0F12, 0x00A5); // #TVAR_ash_pGAS[520]
s5k5caga_i2c_write(0x0F12, 0x0087); // #TVAR_ash_pGAS[521]
s5k5caga_i2c_write(0x0F12, 0x005D); // #TVAR_ash_pGAS[522]
s5k5caga_i2c_write(0x0F12, 0x003E); // #TVAR_ash_pGAS[523]
s5k5caga_i2c_write(0x0F12, 0x0027); // #TVAR_ash_pGAS[524]
s5k5caga_i2c_write(0x0F12, 0x0019); // #TVAR_ash_pGAS[525]
s5k5caga_i2c_write(0x0F12, 0x0016); // #TVAR_ash_pGAS[526]
s5k5caga_i2c_write(0x0F12, 0x001D); // #TVAR_ash_pGAS[527]
s5k5caga_i2c_write(0x0F12, 0x002E); // #TVAR_ash_pGAS[528]
s5k5caga_i2c_write(0x0F12, 0x0047); // #TVAR_ash_pGAS[529]
s5k5caga_i2c_write(0x0F12, 0x0068); // #TVAR_ash_pGAS[530]
s5k5caga_i2c_write(0x0F12, 0x0094); // #TVAR_ash_pGAS[531]
s5k5caga_i2c_write(0x0F12, 0x00BC); // #TVAR_ash_pGAS[532]
s5k5caga_i2c_write(0x0F12, 0x00BE); // #TVAR_ash_pGAS[533]
s5k5caga_i2c_write(0x0F12, 0x00A6); // #TVAR_ash_pGAS[534]
s5k5caga_i2c_write(0x0F12, 0x0079); // #TVAR_ash_pGAS[535]
s5k5caga_i2c_write(0x0F12, 0x005C); // #TVAR_ash_pGAS[536]
s5k5caga_i2c_write(0x0F12, 0x0047); // #TVAR_ash_pGAS[537]
s5k5caga_i2c_write(0x0F12, 0x003A); // #TVAR_ash_pGAS[538]
s5k5caga_i2c_write(0x0F12, 0x0036); // #TVAR_ash_pGAS[539]
s5k5caga_i2c_write(0x0F12, 0x003B); // #TVAR_ash_pGAS[540]
s5k5caga_i2c_write(0x0F12, 0x004B); // #TVAR_ash_pGAS[541]
s5k5caga_i2c_write(0x0F12, 0x0060); // #TVAR_ash_pGAS[542]
s5k5caga_i2c_write(0x0F12, 0x0080); // #TVAR_ash_pGAS[543]
s5k5caga_i2c_write(0x0F12, 0x00AD); // #TVAR_ash_pGAS[544]
s5k5caga_i2c_write(0x0F12, 0x00CF); // #TVAR_ash_pGAS[545]
s5k5caga_i2c_write(0x0F12, 0x00DC); // #TVAR_ash_pGAS[546]
s5k5caga_i2c_write(0x0F12, 0x00D2); // #TVAR_ash_pGAS[547]
s5k5caga_i2c_write(0x0F12, 0x00A3); // #TVAR_ash_pGAS[548]
s5k5caga_i2c_write(0x0F12, 0x0084); // #TVAR_ash_pGAS[549]
s5k5caga_i2c_write(0x0F12, 0x0073); // #TVAR_ash_pGAS[550]
s5k5caga_i2c_write(0x0F12, 0x0067); // #TVAR_ash_pGAS[551]
s5k5caga_i2c_write(0x0F12, 0x0062); // #TVAR_ash_pGAS[552]
s5k5caga_i2c_write(0x0F12, 0x0066); // #TVAR_ash_pGAS[553]
s5k5caga_i2c_write(0x0F12, 0x0073); // #TVAR_ash_pGAS[554]
s5k5caga_i2c_write(0x0F12, 0x0087); // #TVAR_ash_pGAS[555]
s5k5caga_i2c_write(0x0F12, 0x00A6); // #TVAR_ash_pGAS[556]
s5k5caga_i2c_write(0x0F12, 0x00D7); // #TVAR_ash_pGAS[557]
s5k5caga_i2c_write(0x0F12, 0x00E7); // #TVAR_ash_pGAS[558]
s5k5caga_i2c_write(0x0F12, 0x00ED); // #TVAR_ash_pGAS[559]
s5k5caga_i2c_write(0x0F12, 0x00F8); // #TVAR_ash_pGAS[560]
s5k5caga_i2c_write(0x0F12, 0x00D1); // #TVAR_ash_pGAS[561]
s5k5caga_i2c_write(0x0F12, 0x00B1); // #TVAR_ash_pGAS[562]
s5k5caga_i2c_write(0x0F12, 0x009F); // #TVAR_ash_pGAS[563]
s5k5caga_i2c_write(0x0F12, 0x0095); // #TVAR_ash_pGAS[564]
s5k5caga_i2c_write(0x0F12, 0x0091); // #TVAR_ash_pGAS[565]
s5k5caga_i2c_write(0x0F12, 0x0093); // #TVAR_ash_pGAS[566]
s5k5caga_i2c_write(0x0F12, 0x009E); // #TVAR_ash_pGAS[567]
s5k5caga_i2c_write(0x0F12, 0x00B0); // #TVAR_ash_pGAS[568]
s5k5caga_i2c_write(0x0F12, 0x00D1); // #TVAR_ash_pGAS[569]
s5k5caga_i2c_write(0x0F12, 0x00F7); // #TVAR_ash_pGAS[570]
s5k5caga_i2c_write(0x0F12, 0x00F5); // #TVAR_ash_pGAS[571]
s5k5caga_i2c_write(0x002A, 0x074E); 
s5k5caga_i2c_write(0x0F12, 0x0001); //ash_bLumaMode
s5k5caga_i2c_write(0x002A, 0x0D30);
s5k5caga_i2c_write(0x0F12, 0x02A7); // #awbb_GLocusR
s5k5caga_i2c_write(0x0F12, 0x0343); // #awbb_GLocusB
s5k5caga_i2c_write(0x002A, 0x06B8);
s5k5caga_i2c_write(0x0F12, 0x00E0);  // #TVAR_ash_AwbAshCord_0_ Horizon
s5k5caga_i2c_write(0x0F12, 0x00F4);  // #TVAR_ash_AwbAshCord_1_ Incan A
s5k5caga_i2c_write(0x0F12, 0x0114);  // #TVAR_ash_AwbAshCord_2_ TL84
s5k5caga_i2c_write(0x0F12, 0x013D);  // #TVAR_ash_AwbAshCord_3_ CWF
s5k5caga_i2c_write(0x0F12, 0x0171); // #TVAR_ash_AwbAshCord_4_ D50
s5k5caga_i2c_write(0x0F12, 0x0184);  // #TVAR_ash_AwbAshCord_5_ D65
s5k5caga_i2c_write(0x0F12, 0x01A8); // #TVAR_ash_AwbAshCord_6_ / /*BU5D05950 zhangsheng 20100422 end>*/
//================================================================================================
// SET CCM
//================================================================================================ 
// CCM start address // 7000_33A4
s5k5caga_i2c_write(0x002A, 0x0698);
s5k5caga_i2c_write(0x0F12, 0x33A4);
s5k5caga_i2c_write(0x0F12, 0x7000);
s5k5caga_i2c_write(0x002A, 0x33A4);
s5k5caga_i2c_write(0x0F12, 0x0285);//#TVAR_wbt_pBaseCcms// Horizon
s5k5caga_i2c_write(0x0F12, 0xFF1A);//#TVAR_wbt_pBaseCcms
s5k5caga_i2c_write(0x0F12, 0xFFA4);//#TVAR_wbt_pBaseCcms
s5k5caga_i2c_write(0x0F12, 0xFF78);//#TVAR_wbt_pBaseCcms
s5k5caga_i2c_write(0x0F12, 0x0277);//#TVAR_wbt_pBaseCcms
s5k5caga_i2c_write(0x0F12, 0xFF2A);//#TVAR_wbt_pBaseCcms
s5k5caga_i2c_write(0x0F12, 0xFFC0);//#TVAR_wbt_pBaseCcms
s5k5caga_i2c_write(0x0F12, 0x0009);//#TVAR_wbt_pBaseCcms
s5k5caga_i2c_write(0x0F12, 0x0374);//#TVAR_wbt_pBaseCcms
s5k5caga_i2c_write(0x0F12, 0x0192);//#TVAR_wbt_pBaseCcms
s5k5caga_i2c_write(0x0F12, 0x012D);//#TVAR_wbt_pBaseCcms
s5k5caga_i2c_write(0x0F12, 0xFE60);//#TVAR_wbt_pBaseCcms
s5k5caga_i2c_write(0x0F12, 0x017B);//#TVAR_wbt_pBaseCcms
s5k5caga_i2c_write(0x0F12, 0xFF86);//#TVAR_wbt_pBaseCcms
s5k5caga_i2c_write(0x0F12, 0x02DE);//#TVAR_wbt_pBaseCcms
s5k5caga_i2c_write(0x0F12, 0xFEB4);//#TVAR_wbt_pBaseCcms
s5k5caga_i2c_write(0x0F12, 0x0288);//#TVAR_wbt_pBaseCcms
s5k5caga_i2c_write(0x0F12, 0x00FA);//#TVAR_wbt_pBaseCcms//*<BU5D05950 zhangsheng 20100422 begin*/

s5k5caga_i2c_write(0x0F12, 0x0263);//#TVAR_wbt_pBaseCcms[18]// Inca
s5k5caga_i2c_write(0x0F12, 0xFF37);//#TVAR_wbt_pBaseCcms[19]
s5k5caga_i2c_write(0x0F12, 0xFFA9);//#TVAR_wbt_pBaseCcms[20]
s5k5caga_i2c_write(0x0F12, 0xFF78);//#TVAR_wbt_pBaseCcms[21]
s5k5caga_i2c_write(0x0F12, 0x0277);//#TVAR_wbt_pBaseCcms[22]
s5k5caga_i2c_write(0x0F12, 0xFF2A);//#TVAR_wbt_pBaseCcms[23]
s5k5caga_i2c_write(0x0F12, 0xFFD0);//#TVAR_wbt_pBaseCcms[24]
s5k5caga_i2c_write(0x0F12, 0x0016);//#TVAR_wbt_pBaseCcms[25]
s5k5caga_i2c_write(0x0F12, 0x0357);//#TVAR_wbt_pBaseCcms[26]
s5k5caga_i2c_write(0x0F12, 0x016D);//#TVAR_wbt_pBaseCcms[27]
s5k5caga_i2c_write(0x0F12, 0x0155);//#TVAR_wbt_pBaseCcms[28]
s5k5caga_i2c_write(0x0F12, 0xFE5D);//#TVAR_wbt_pBaseCcms[29]
s5k5caga_i2c_write(0x0F12, 0x017B);//#TVAR_wbt_pBaseCcms[30]//*BU5D05950 zhangsheng 20100422 end>*/
s5k5caga_i2c_write(0x0F12, 0xFF86);//#TVAR_wbt_pBaseCcms[31]
s5k5caga_i2c_write(0x0F12, 0x02DE);//#TVAR_wbt_pBaseCcms[32]
s5k5caga_i2c_write(0x0F12, 0xFEC9);//#TVAR_wbt_pBaseCcms[33]
s5k5caga_i2c_write(0x0F12, 0x02A2);//#TVAR_wbt_pBaseCcms[34]
s5k5caga_i2c_write(0x0F12, 0x00CB);//#TVAR_wbt_pBaseCcms[35]

s5k5caga_i2c_write(0x0F12, 0x0227);//#TVAR_wbt_pBaseCcms[36]// WW
s5k5caga_i2c_write(0x0F12, 0xFF70);//#TVAR_wbt_pBaseCcms[37]
s5k5caga_i2c_write(0x0F12, 0xFFAD);//#TVAR_wbt_pBaseCcms[38]
s5k5caga_i2c_write(0x0F12, 0xFEE6);//#TVAR_wbt_pBaseCcms[39]
s5k5caga_i2c_write(0x0F12, 0x01DB);//#TVAR_wbt_pBaseCcms[40]
s5k5caga_i2c_write(0x0F12, 0xFFA1);//#TVAR_wbt_pBaseCcms[41]
s5k5caga_i2c_write(0x0F12, 0xFF94);//#TVAR_wbt_pBaseCcms[42]
s5k5caga_i2c_write(0x0F12, 0xFFD5);//#TVAR_wbt_pBaseCcms[43]
s5k5caga_i2c_write(0x0F12, 0x0232);//#TVAR_wbt_pBaseCcms[44]
s5k5caga_i2c_write(0x0F12, 0x01AF);//#TVAR_wbt_pBaseCcms[45]
s5k5caga_i2c_write(0x0F12, 0x014F);//#TVAR_wbt_pBaseCcms[46]
s5k5caga_i2c_write(0x0F12, 0xFE95);//#TVAR_wbt_pBaseCcms[47]
s5k5caga_i2c_write(0x0F12, 0x01BF);//#TVAR_wbt_pBaseCcms[48]
s5k5caga_i2c_write(0x0F12, 0xFF9B);//#TVAR_wbt_pBaseCcms[49]
s5k5caga_i2c_write(0x0F12, 0x016F);//#TVAR_wbt_pBaseCcms[50]
s5k5caga_i2c_write(0x0F12, 0xFEFB);//#TVAR_wbt_pBaseCcms[51]
s5k5caga_i2c_write(0x0F12, 0x0168);//#TVAR_wbt_pBaseCcms[52]
s5k5caga_i2c_write(0x0F12, 0x0188);//#TVAR_wbt_pBaseCcms[53]//*<BU5D05950 zhangsheng 20100422 begin*/

s5k5caga_i2c_write(0x0F12, 0x01F3); //#TVAR_wbt_pBaseCcms[54]// CWF
s5k5caga_i2c_write(0x0F12, 0xFF70); //#TVAR_wbt_pBaseCcms[55]
s5k5caga_i2c_write(0x0F12, 0xFFD4); //#TVAR_wbt_pBaseCcms[56]
s5k5caga_i2c_write(0x0F12, 0xFF2C); //#TVAR_wbt_pBaseCcms[57]
s5k5caga_i2c_write(0x0F12, 0x01D6); //#TVAR_wbt_pBaseCcms[58]
s5k5caga_i2c_write(0x0F12, 0xFF54); //#TVAR_wbt_pBaseCcms[59]
s5k5caga_i2c_write(0x0F12, 0xFF82); //#TVAR_wbt_pBaseCcms[60]
s5k5caga_i2c_write(0x0F12, 0xFFD0); //#TVAR_wbt_pBaseCcms[61]
s5k5caga_i2c_write(0x0F12, 0x023B); //#TVAR_wbt_pBaseCcms[62]
s5k5caga_i2c_write(0x0F12, 0x01A7); //#TVAR_wbt_pBaseCcms[63]
s5k5caga_i2c_write(0x0F12, 0x0132); //#TVAR_wbt_pBaseCcms[64]
s5k5caga_i2c_write(0x0F12, 0xFEAE); //#TVAR_wbt_pBaseCcms[65]
s5k5caga_i2c_write(0x0F12, 0x0181); //#TVAR_wbt_pBaseCcms[66]
s5k5caga_i2c_write(0x0F12, 0xFF9E); //#TVAR_wbt_pBaseCcms[67]
s5k5caga_i2c_write(0x0F12, 0x019E); //#TVAR_wbt_pBaseCcms[68]
s5k5caga_i2c_write(0x0F12, 0xFF0C); //#TVAR_wbt_pBaseCcms[69]
s5k5caga_i2c_write(0x0F12, 0x01B4); //#TVAR_wbt_pBaseCcms[70]
s5k5caga_i2c_write(0x0F12, 0x011D); //#TVAR_wbt_pBaseCcms[71]//*BU5D05950 zhangsheng 20100422 end>*/

s5k5caga_i2c_write(0x0F12, 0x01C1);//#TVAR_wbt_pBaseCcms[72]// D50
s5k5caga_i2c_write(0x0F12, 0xFFB2);//#TVAR_wbt_pBaseCcms[73]
s5k5caga_i2c_write(0x0F12, 0xFFCB);//#TVAR_wbt_pBaseCcms[74]
s5k5caga_i2c_write(0x0F12, 0xFF41);//#TVAR_wbt_pBaseCcms[75]
s5k5caga_i2c_write(0x0F12, 0x01A4);//#TVAR_wbt_pBaseCcms[76]
s5k5caga_i2c_write(0x0F12, 0xFF7B);//#TVAR_wbt_pBaseCcms[77]
s5k5caga_i2c_write(0x0F12, 0xFFE0);//#TVAR_wbt_pBaseCcms[78]
s5k5caga_i2c_write(0x0F12, 0xFFCE);//#TVAR_wbt_pBaseCcms[79]
s5k5caga_i2c_write(0x0F12, 0x01E6);//#TVAR_wbt_pBaseCcms[80]
s5k5caga_i2c_write(0x0F12, 0x0114);//#TVAR_wbt_pBaseCcms[81]
s5k5caga_i2c_write(0x0F12, 0x0124);//#TVAR_wbt_pBaseCcms[82]
s5k5caga_i2c_write(0x0F12, 0xFF56);//#TVAR_wbt_pBaseCcms[83]
s5k5caga_i2c_write(0x0F12, 0x01AA);//#TVAR_wbt_pBaseCcms[84]
s5k5caga_i2c_write(0x0F12, 0xFF68);//#TVAR_wbt_pBaseCcms[85]
s5k5caga_i2c_write(0x0F12, 0x01B6);//#TVAR_wbt_pBaseCcms[86]
s5k5caga_i2c_write(0x0F12, 0xFF4F);//#TVAR_wbt_pBaseCcms[87]
s5k5caga_i2c_write(0x0F12, 0x0178);//#TVAR_wbt_pBaseCcms[88]
s5k5caga_i2c_write(0x0F12, 0x0121);//#TVAR_wbt_pBaseCcms[89]

s5k5caga_i2c_write(0x0F12, 0x01C1);//#TVAR_wbt_pBaseCcms[90]// D65
s5k5caga_i2c_write(0x0F12, 0xFFB2);//#TVAR_wbt_pBaseCcms[91]
s5k5caga_i2c_write(0x0F12, 0xFFCB);//#TVAR_wbt_pBaseCcms[92]
s5k5caga_i2c_write(0x0F12, 0xFF41);//#TVAR_wbt_pBaseCcms[93]
s5k5caga_i2c_write(0x0F12, 0x01A4);//#TVAR_wbt_pBaseCcms[94]
s5k5caga_i2c_write(0x0F12, 0xFF7B);//#TVAR_wbt_pBaseCcms[95]
s5k5caga_i2c_write(0x0F12, 0xFFE0);//#TVAR_wbt_pBaseCcms[96]
s5k5caga_i2c_write(0x0F12, 0xFFCE);//#TVAR_wbt_pBaseCcms[97]
s5k5caga_i2c_write(0x0F12, 0x01E6);//#TVAR_wbt_pBaseCcms[98]
s5k5caga_i2c_write(0x0F12, 0x0114);//#TVAR_wbt_pBaseCcms[99]
s5k5caga_i2c_write(0x0F12, 0x0124);//#TVAR_wbt_pBaseCcms[100]
s5k5caga_i2c_write(0x0F12, 0xFF56);//#TVAR_wbt_pBaseCcms[101]
s5k5caga_i2c_write(0x0F12, 0x01AA);//#TVAR_wbt_pBaseCcms[102]
s5k5caga_i2c_write(0x0F12, 0xFF68);//#TVAR_wbt_pBaseCcms[103]
s5k5caga_i2c_write(0x0F12, 0x01B6);//#TVAR_wbt_pBaseCcms[104]
s5k5caga_i2c_write(0x0F12, 0xFF4F);//#TVAR_wbt_pBaseCcms[105]
s5k5caga_i2c_write(0x0F12, 0x0178);//#TVAR_wbt_pBaseCcms[106]
s5k5caga_i2c_write(0x0F12, 0x0121);//#TVAR_wbt_pBaseCcms[107]

s5k5caga_i2c_write(0x002A, 0x06A0); // Outdoor CCM address // 7000_3380  
s5k5caga_i2c_write(0x0F12, 0x3380);
s5k5caga_i2c_write(0x0F12, 0x7000);
s5k5caga_i2c_write(0x002A, 0x3380); // Outdoor CCM
s5k5caga_i2c_write(0x0F12, 0x0218); //#TVAR_wbt_pOutdoorCcm[0]
s5k5caga_i2c_write(0x0F12, 0xFF43); //#TVAR_wbt_pOutdoorCcm[1]
s5k5caga_i2c_write(0x0F12, 0xFF92); //#TVAR_wbt_pOutdoorCcm[2]
s5k5caga_i2c_write(0x0F12, 0xFF1D); //#TVAR_wbt_pOutdoorCcm[3]
s5k5caga_i2c_write(0x0F12, 0x021E); //#TVAR_wbt_pOutdoorCcm[4]
s5k5caga_i2c_write(0x0F12, 0xFF0F); //#TVAR_wbt_pOutdoorCcm[5]
s5k5caga_i2c_write(0x0F12, 0xFFBE); //#TVAR_wbt_pOutdoorCcm[6]
s5k5caga_i2c_write(0x0F12, 0x0006); //#TVAR_wbt_pOutdoorCcm[7]
s5k5caga_i2c_write(0x0F12, 0x02AE); //#TVAR_wbt_pOutdoorCcm[8]
s5k5caga_i2c_write(0x0F12, 0x0170); //#TVAR_wbt_pOutdoorCcm[9]
s5k5caga_i2c_write(0x0F12, 0x010B); //#TVAR_wbt_pOutdoorCcm[10] 
s5k5caga_i2c_write(0x0F12, 0xFED4); //#TVAR_wbt_pOutdoorCcm[11] 
s5k5caga_i2c_write(0x0F12, 0x01B9); //#TVAR_wbt_pOutdoorCcm[12] 
s5k5caga_i2c_write(0x0F12, 0xFF59); //#TVAR_wbt_pOutdoorCcm[13] 
s5k5caga_i2c_write(0x0F12, 0x0241); //#TVAR_wbt_pOutdoorCcm[14] 
s5k5caga_i2c_write(0x0F12, 0xFF01); //#TVAR_wbt_pOutdoorCcm[15] 
s5k5caga_i2c_write(0x0F12, 0x01B8); //#TVAR_wbt_pOutdoorCcm[16] 
s5k5caga_i2c_write(0x0F12, 0x0152); //#TVAR_wbt_pOutdoorCcm[17] //White balance
// param_start awbb_IndoorGrZones_m_BGrid
s5k5caga_i2c_write(0x002A, 0x0C48);
s5k5caga_i2c_write(0x0F12, 0x03BD);//awbb_IndoorGrZones_m_BGrid[0]
s5k5caga_i2c_write(0x0F12, 0x03F2);//awbb_IndoorGrZones_m_BGrid[1]
s5k5caga_i2c_write(0x0F12, 0x036F);//awbb_IndoorGrZones_m_BGrid[2]
s5k5caga_i2c_write(0x0F12, 0x03F7);//awbb_IndoorGrZones_m_BGrid[3]
s5k5caga_i2c_write(0x0F12, 0x0335);//awbb_IndoorGrZones_m_BGrid[4]
s5k5caga_i2c_write(0x0F12, 0x03E0);//awbb_IndoorGrZones_m_BGrid[5]
s5k5caga_i2c_write(0x0F12, 0x0301);//awbb_IndoorGrZones_m_BGrid[6]
s5k5caga_i2c_write(0x0F12, 0x03B9);//awbb_IndoorGrZones_m_BGrid[7]
s5k5caga_i2c_write(0x0F12, 0x02D2);//awbb_IndoorGrZones_m_BGrid[8]
s5k5caga_i2c_write(0x0F12, 0x0392);//awbb_IndoorGrZones_m_BGrid[9]
s5k5caga_i2c_write(0x0F12, 0x02AE);//awbb_IndoorGrZones_m_BGrid[10]
s5k5caga_i2c_write(0x0F12, 0x0367);//awbb_IndoorGrZones_m_BGrid[11]
s5k5caga_i2c_write(0x0F12, 0x028F);//awbb_IndoorGrZones_m_BGrid[12]
s5k5caga_i2c_write(0x0F12, 0x033C);//awbb_IndoorGrZones_m_BGrid[13]
s5k5caga_i2c_write(0x0F12, 0x0275);//awbb_IndoorGrZones_m_BGrid[14]
s5k5caga_i2c_write(0x0F12, 0x0317);//awbb_IndoorGrZones_m_BGrid[15]
s5k5caga_i2c_write(0x0F12, 0x0259);//awbb_IndoorGrZones_m_BGrid[16]
s5k5caga_i2c_write(0x0F12, 0x02EF);//awbb_IndoorGrZones_m_BGrid[17]
s5k5caga_i2c_write(0x0F12, 0x0240);//awbb_IndoorGrZones_m_BGrid[18]
s5k5caga_i2c_write(0x0F12, 0x02D0);//awbb_IndoorGrZones_m_BGrid[19]
s5k5caga_i2c_write(0x0F12, 0x0229);//awbb_IndoorGrZones_m_BGrid[20]
s5k5caga_i2c_write(0x0F12, 0x02B1);//awbb_IndoorGrZones_m_BGrid[21]
s5k5caga_i2c_write(0x0F12, 0x0215);//awbb_IndoorGrZones_m_BGrid[22]
s5k5caga_i2c_write(0x0F12, 0x0294);//awbb_IndoorGrZones_m_BGrid[23]
s5k5caga_i2c_write(0x0F12, 0x0203);//awbb_IndoorGrZones_m_BGrid[24]
s5k5caga_i2c_write(0x0F12, 0x027F);//awbb_IndoorGrZones_m_BGrid[25]
s5k5caga_i2c_write(0x0F12, 0x01EF);//awbb_IndoorGrZones_m_BGrid[26]
s5k5caga_i2c_write(0x0F12, 0x0264);//awbb_IndoorGrZones_m_BGrid[27]
s5k5caga_i2c_write(0x0F12, 0x01E4);//awbb_IndoorGrZones_m_BGrid[28]
s5k5caga_i2c_write(0x0F12, 0x024C);//awbb_IndoorGrZones_m_BGrid[29]
s5k5caga_i2c_write(0x0F12, 0x01E5);//awbb_IndoorGrZones_m_BGrid[30]
s5k5caga_i2c_write(0x0F12, 0x0233);//awbb_IndoorGrZones_m_BGrid[31]
s5k5caga_i2c_write(0x0F12, 0x01EE);//awbb_IndoorGrZones_m_BGrid[32]
s5k5caga_i2c_write(0x0F12, 0x020F);//awbb_IndoorGrZones_m_BGrid[33]
s5k5caga_i2c_write(0x0F12, 0x0000);//awbb_IndoorGrZones_m_BGrid[34]
s5k5caga_i2c_write(0x0F12, 0x0000);//awbb_IndoorGrZones_m_BGrid[35]
s5k5caga_i2c_write(0x0F12, 0x0000);//awbb_IndoorGrZones_m_BGrid[36]
s5k5caga_i2c_write(0x0F12, 0x0000);//awbb_IndoorGrZones_m_BGrid[37]
s5k5caga_i2c_write(0x0F12, 0x0000);//awbb_IndoorGrZones_m_BGrid[38]
s5k5caga_i2c_write(0x0F12, 0x0000);//awbb_IndoorGrZones_m_BGrid[39]
s5k5caga_i2c_write(0x0F12, 0x0005); //awbb_IndoorGrZones_m_GridStep // param_end awbb_IndoorGrZones_m_BGrid
s5k5caga_i2c_write(0x0F12, 0x0000);
s5k5caga_i2c_write(0x002A, 0x0CA0);
s5k5caga_i2c_write(0x0F12, 0x0101);//011A //awbb_IndoorGrZones_m_Boffs
s5k5caga_i2c_write(0x0F12, 0x0000);
s5k5caga_i2c_write(0x002A, 0x0CE0); // param_start awbb_LowBrGrZones_m_BGrid
s5k5caga_i2c_write(0x0F12, 0x0399); //awbb_LowBrGrZones_m_BGrid[0]
s5k5caga_i2c_write(0x0F12, 0x0417); //awbb_LowBrGrZones_m_BGrid[1]
s5k5caga_i2c_write(0x0F12, 0x0327); //awbb_LowBrGrZones_m_BGrid[2]
s5k5caga_i2c_write(0x0F12, 0x0417); //awbb_LowBrGrZones_m_BGrid[3]
s5k5caga_i2c_write(0x0F12, 0x02BD); //awbb_LowBrGrZones_m_BGrid[4]
s5k5caga_i2c_write(0x0F12, 0x0409); //awbb_LowBrGrZones_m_BGrid[5]
s5k5caga_i2c_write(0x0F12, 0x0271); //awbb_LowBrGrZones_m_BGrid[6]
s5k5caga_i2c_write(0x0F12, 0x03BD); //awbb_LowBrGrZones_m_BGrid[7]
s5k5caga_i2c_write(0x0F12, 0x0231); //awbb_LowBrGrZones_m_BGrid[8]
s5k5caga_i2c_write(0x0F12, 0x036F); //awbb_LowBrGrZones_m_BGrid[9]
s5k5caga_i2c_write(0x0F12, 0x0203); //awbb_LowBrGrZones_m_BGrid[10]
s5k5caga_i2c_write(0x0F12, 0x0322); //awbb_LowBrGrZones_m_BGrid[11]
s5k5caga_i2c_write(0x0F12, 0x01D0); //awbb_LowBrGrZones_m_BGrid[12]
s5k5caga_i2c_write(0x0F12, 0x02DB); //awbb_LowBrGrZones_m_BGrid[13]
s5k5caga_i2c_write(0x0F12, 0x01AD); //awbb_LowBrGrZones_m_BGrid[14]
s5k5caga_i2c_write(0x0F12, 0x02A7); //awbb_LowBrGrZones_m_BGrid[15]
s5k5caga_i2c_write(0x0F12, 0x01AA); //awbb_LowBrGrZones_m_BGrid[16]
s5k5caga_i2c_write(0x0F12, 0x027D); //awbb_LowBrGrZones_m_BGrid[17]
s5k5caga_i2c_write(0x0F12, 0x01B0); //awbb_LowBrGrZones_m_BGrid[18]
s5k5caga_i2c_write(0x0F12, 0x0219); //awbb_LowBrGrZones_m_BGrid[19]
s5k5caga_i2c_write(0x0F12, 0x0000); //awbb_LowBrGrZones_m_BGrid[20]
s5k5caga_i2c_write(0x0F12, 0x0000); //awbb_LowBrGrZones_m_BGrid[21]
s5k5caga_i2c_write(0x0F12, 0x0000); //awbb_LowBrGrZones_m_BGrid[22]
s5k5caga_i2c_write(0x0F12, 0x0000); //awbb_LowBrGrZones_m_BGrid[23]
s5k5caga_i2c_write(0x0F12, 0x0006); //awbb_LowBrGrZones_m_GridStep // param_end awbb_LowBrGrZones_m_BGrid
s5k5caga_i2c_write(0x0F12, 0x0000);
s5k5caga_i2c_write(0x002A, 0x0D18);
s5k5caga_i2c_write(0x0F12, 0x00DF);//00FA //awbb_LowBrGrZones_m_Boffs
s5k5caga_i2c_write(0x0F12, 0x0000);
s5k5caga_i2c_write(0x002A, 0x0CA4); // param_start awbb_OutdoorGrZones_m_BGrid
s5k5caga_i2c_write(0x0F12, 0x0291);//awbb_OutdoorGrZones_m_BGrid[0]
s5k5caga_i2c_write(0x0F12, 0x02C4);//awbb_OutdoorGrZones_m_BGrid[1]
s5k5caga_i2c_write(0x0F12, 0x0244);//awbb_OutdoorGrZones_m_BGrid[2]
s5k5caga_i2c_write(0x0F12, 0x02B6);//awbb_OutdoorGrZones_m_BGrid[3]
s5k5caga_i2c_write(0x0F12, 0x0215);//awbb_OutdoorGrZones_m_BGrid[4]
s5k5caga_i2c_write(0x0F12, 0x028F);//awbb_OutdoorGrZones_m_BGrid[5]
s5k5caga_i2c_write(0x0F12, 0x01F2);//awbb_OutdoorGrZones_m_BGrid[6]
s5k5caga_i2c_write(0x0F12, 0x025F);//awbb_OutdoorGrZones_m_BGrid[7]
s5k5caga_i2c_write(0x0F12, 0x01E4);//awbb_OutdoorGrZones_m_BGrid[8]
s5k5caga_i2c_write(0x0F12, 0x021E);//awbb_OutdoorGrZones_m_BGrid[9]
s5k5caga_i2c_write(0x0F12, 0x0000);//awbb_OutdoorGrZones_m_BGrid[10]
s5k5caga_i2c_write(0x0F12, 0x0000);//awbb_OutdoorGrZones_m_BGrid[11]
s5k5caga_i2c_write(0x0F12, 0x0000);//awbb_OutdoorGrZones_m_BGrid[12]
s5k5caga_i2c_write(0x0F12, 0x0000);//awbb_OutdoorGrZones_m_BGrid[13]
s5k5caga_i2c_write(0x0F12, 0x0000);//awbb_OutdoorGrZones_m_BGrid[14]
s5k5caga_i2c_write(0x0F12, 0x0000);//awbb_OutdoorGrZones_m_BGrid[15]
s5k5caga_i2c_write(0x0F12, 0x0000);//awbb_OutdoorGrZones_m_BGrid[16]
s5k5caga_i2c_write(0x0F12, 0x0000);//awbb_OutdoorGrZones_m_BGrid[17]
s5k5caga_i2c_write(0x0F12, 0x0000);//awbb_OutdoorGrZones_m_BGrid[18]
s5k5caga_i2c_write(0x0F12, 0x0000);//awbb_OutdoorGrZones_m_BGrid[19]
s5k5caga_i2c_write(0x0F12, 0x0000);//awbb_OutdoorGrZones_m_BGrid[20]
s5k5caga_i2c_write(0x0F12, 0x0000);//awbb_OutdoorGrZones_m_BGrid[21]
s5k5caga_i2c_write(0x0F12, 0x0000);//awbb_OutdoorGrZones_m_BGrid[22]
s5k5caga_i2c_write(0x0F12, 0x0000);//awbb_OutdoorGrZones_m_BGrid[23]
s5k5caga_i2c_write(0x0F12, 0x0006); //awbb_OutdoorGrZones_m_GridStep // param_end awbb_OutdoorGrZones_m_BGrid
s5k5caga_i2c_write(0x0F12, 0x0000);
s5k5caga_i2c_write(0x002A, 0x0CDC);
s5k5caga_i2c_write(0x0F12, 0x01F2);//0212 //awbb_OutdoorGrZones_m_Boffs
s5k5caga_i2c_write(0x0F12, 0x0000);
s5k5caga_i2c_write(0x002A, 0x0D1C);
s5k5caga_i2c_write(0x0F12, 0x034D); //awbb_CrclLowT_R_c
s5k5caga_i2c_write(0x0F12, 0x0000);
s5k5caga_i2c_write(0x002A, 0x0D20);
s5k5caga_i2c_write(0x0F12, 0x016C); //awbb_CrclLowT_B_c
s5k5caga_i2c_write(0x0F12, 0x0000);
s5k5caga_i2c_write(0x002A, 0x0D24);
s5k5caga_i2c_write(0x0F12, 0x49D5); //awbb_CrclLowT_Rad_c
s5k5caga_i2c_write(0x0F12, 0x0000);
s5k5caga_i2c_write(0x002A, 0x0D46);
s5k5caga_i2c_write(0x0F12, 0x0470); //awbb_MvEq_RBthresh
s5k5caga_i2c_write(0x002A, 0x0D5C);
s5k5caga_i2c_write(0x0F12, 0x0534); //awbb_LowTempRB
s5k5caga_i2c_write(0x002A, 0x0D2C);
s5k5caga_i2c_write(0x0F12, 0x0131); //awbb_IntcR 
s5k5caga_i2c_write(0x0F12, 0x012F);  //012C //awbb_IntcB 

s5k5caga_i2c_write(0x002A, 0x0E36);    
s5k5caga_i2c_write(0x0F12, 0x0000);//0028  //R OFFSET 
s5k5caga_i2c_write(0x0F12, 0x0000);//FFD8  //B OFFSET 
s5k5caga_i2c_write(0x0F12, 0x0000);  //G OFFSET 

s5k5caga_i2c_write(0x002A, 0x0DD4);    
s5k5caga_i2c_write(0x0F12, 0x0000);//awbb_GridCorr_R[0] //           
s5k5caga_i2c_write(0x0F12, 0x0000);//awbb_GridCorr_R[1] //    
s5k5caga_i2c_write(0x0F12, 0x0000);//awbb_GridCorr_R[2] //    
s5k5caga_i2c_write(0x0F12, 0x0000);//awbb_GridCorr_R[3] //    
s5k5caga_i2c_write(0x0F12, 0xFFF1);//awbb_GridCorr_R[4] //    
s5k5caga_i2c_write(0x0F12, 0x001E);//awbb_GridCorr_R[5] //     
s5k5caga_i2c_write(0x0F12, 0x0000);//awbb_GridCorr_R[6] //    
s5k5caga_i2c_write(0x0F12, 0x0000);//awbb_GridCorr_R[7] //    
s5k5caga_i2c_write(0x0F12, 0x0000);//awbb_GridCorr_R[8] //    
s5k5caga_i2c_write(0x0F12, 0x0000);//awbb_GridCorr_R[9] //    
s5k5caga_i2c_write(0x0F12, 0xFFF1);//awbb_GridCorr_R[10] //   
s5k5caga_i2c_write(0x0F12, 0x001E);//awbb_GridCorr_R[11] //    
s5k5caga_i2c_write(0x0F12, 0x0000);//awbb_GridCorr_R[12] //   
s5k5caga_i2c_write(0x0F12, 0x0000);//awbb_GridCorr_R[13] //   
s5k5caga_i2c_write(0x0F12, 0x0000);//awbb_GridCorr_R[14] //   
s5k5caga_i2c_write(0x0F12, 0x0000);//awbb_GridCorr_R[15] //   
s5k5caga_i2c_write(0x0F12, 0xFFF1);//awbb_GridCorr_R[16] //   
s5k5caga_i2c_write(0x0F12, 0x001E);//awbb_GridCorr_R[17] //   

s5k5caga_i2c_write(0x0F12, 0x0000);//awbb_GridCorr_B[0] ////  
s5k5caga_i2c_write(0x0F12, 0x0000);//awbb_GridCorr_B[1] //    
s5k5caga_i2c_write(0x0F12, 0x006E);//awbb_GridCorr_B[2] //    
s5k5caga_i2c_write(0x0F12, 0x0046);//awbb_GridCorr_B[3] //    
s5k5caga_i2c_write(0x0F12, 0x0000);//awbb_GridCorr_B[4] //    
s5k5caga_i2c_write(0x0F12, 0xFFC0);//awbb_GridCorr_B[5] // 
s5k5caga_i2c_write(0x0F12, 0x0000);//awbb_GridCorr_B[6] //    
s5k5caga_i2c_write(0x0F12, 0x0000);//awbb_GridCorr_B[7] //    
s5k5caga_i2c_write(0x0F12, 0x006E);//awbb_GridCorr_B[8] //    
s5k5caga_i2c_write(0x0F12, 0x0046);//awbb_GridCorr_B[9] //    
s5k5caga_i2c_write(0x0F12, 0x0000);//awbb_GridCorr_B[10] //   
s5k5caga_i2c_write(0x0F12, 0xFFC0);//awbb_GridCorr_B[11] //
s5k5caga_i2c_write(0x0F12, 0x0000);//awbb_GridCorr_B[12] //   
s5k5caga_i2c_write(0x0F12, 0x0000);//awbb_GridCorr_B[13] //   
s5k5caga_i2c_write(0x0F12, 0x006E);//awbb_GridCorr_B[14] //   
s5k5caga_i2c_write(0x0F12, 0x0046);//awbb_GridCorr_B[15] //   
s5k5caga_i2c_write(0x0F12, 0x0000);//awbb_GridCorr_B[16] //   
s5k5caga_i2c_write(0x0F12, 0xFFC0);//awbb_GridCorr_B[17] //   

s5k5caga_i2c_write(0x0F12, 0x02F2);//awbb_GridConst_1[0] //             
s5k5caga_i2c_write(0x0F12, 0x0340);//awbb_GridConst_1[1] //             
s5k5caga_i2c_write(0x0F12, 0x0398);//awbb_GridConst_1[2] //             


s5k5caga_i2c_write(0x0F12, 0x0DF6);// awbb_GridConst_2[0] //         
s5k5caga_i2c_write(0x0F12, 0x0EAA);// awbb_GridConst_2[1] //         
s5k5caga_i2c_write(0x0F12, 0x0EB5);// awbb_GridConst_2[2] //         
s5k5caga_i2c_write(0x0F12, 0x0F33);// awbb_GridConst_2[3] //         
s5k5caga_i2c_write(0x0F12, 0x0F9D);// awbb_GridConst_2[4] //         
s5k5caga_i2c_write(0x0F12, 0x0FF2);// awbb_GridConst_2[5] //         

s5k5caga_i2c_write(0x0F12, 0x00AC); //awbb_GridCoeff_R_1           
s5k5caga_i2c_write(0x0F12, 0x00BD); //awbb_GridCoeff_B_1           
s5k5caga_i2c_write(0x0F12, 0x0049); //awbb_GridCoeff_R_2           
s5k5caga_i2c_write(0x0F12, 0x00F5); //awbb_GridCoeff_B_2           

s5k5caga_i2c_write(0x002A, 0x0E4A);           
s5k5caga_i2c_write(0x0F12, 0x0002); //awbb_GridEnable//    
//================================================================================================
// SET GRID OFFSET
//================================================================================================
// Not used
//002A 0E4A
//0F12 0000 // #awbb_GridEnable

//================================================================================================
// SET GAMMA
//================================================================================================
//Our //old //STW
s5k5caga_i2c_write(0x002A, 0x3288);
s5k5caga_i2c_write(0x0F12, 0x0000); //#SARR_usDualGammaLutRGBIndoor_0__0_ 0x70003288
s5k5caga_i2c_write(0x0F12, 0x0004); //#SARR_usDualGammaLutRGBIndoor_0__1_ 0x7000328A
s5k5caga_i2c_write(0x0F12, 0x0010); //#SARR_usDualGammaLutRGBIndoor_0__2_ 0x7000328C
s5k5caga_i2c_write(0x0F12, 0x002A); //#SARR_usDualGammaLutRGBIndoor_0__3_ 0x7000328E
s5k5caga_i2c_write(0x0F12, 0x0062); //#SARR_usDualGammaLutRGBIndoor_0__4_ 0x70003290
s5k5caga_i2c_write(0x0F12, 0x00D5); //#SARR_usDualGammaLutRGBIndoor_0__5_ 0x70003292
s5k5caga_i2c_write(0x0F12, 0x0138); //#SARR_usDualGammaLutRGBIndoor_0__6_ 0x70003294
s5k5caga_i2c_write(0x0F12, 0x0161); //#SARR_usDualGammaLutRGBIndoor_0__7_ 0x70003296
s5k5caga_i2c_write(0x0F12, 0x0186); //#SARR_usDualGammaLutRGBIndoor_0__8_ 0x70003298
s5k5caga_i2c_write(0x0F12, 0x01BC); //#SARR_usDualGammaLutRGBIndoor_0__9_ 0x7000329A
s5k5caga_i2c_write(0x0F12, 0x01E8); //#SARR_usDualGammaLutRGBIndoor_0__10_ 0x7000329C
s5k5caga_i2c_write(0x0F12, 0x020F); //#SARR_usDualGammaLutRGBIndoor_0__11_ 0x7000329E
s5k5caga_i2c_write(0x0F12, 0x0232); //#SARR_usDualGammaLutRGBIndoor_0__12_ 0x700032A0
s5k5caga_i2c_write(0x0F12, 0x0273); //#SARR_usDualGammaLutRGBIndoor_0__13_ 0x700032A2
s5k5caga_i2c_write(0x0F12, 0x02AF); //#SARR_usDualGammaLutRGBIndoor_0__14_ 0x700032A4
s5k5caga_i2c_write(0x0F12, 0x0309); //#SARR_usDualGammaLutRGBIndoor_0__15_ 0x700032A6
s5k5caga_i2c_write(0x0F12, 0x0355); //#SARR_usDualGammaLutRGBIndoor_0__16_ 0x700032A8
s5k5caga_i2c_write(0x0F12, 0x0394); //#SARR_usDualGammaLutRGBIndoor_0__17_ 0x700032AA
s5k5caga_i2c_write(0x0F12, 0x03CE); //#SARR_usDualGammaLutRGBIndoor_0__18_ 0x700032AC
s5k5caga_i2c_write(0x0F12, 0x03FF); //#SARR_usDualGammaLutRGBIndoor_0__19_ 0x700032AE
s5k5caga_i2c_write(0x0F12, 0x0000); //#SARR_usDualGammaLutRGBIndoor_1__0_ 0x700032B0
s5k5caga_i2c_write(0x0F12, 0x0004); //#SARR_usDualGammaLutRGBIndoor_1__1_ 0x700032B2
s5k5caga_i2c_write(0x0F12, 0x0010); //#SARR_usDualGammaLutRGBIndoor_1__2_ 0x700032B4
s5k5caga_i2c_write(0x0F12, 0x002A); //#SARR_usDualGammaLutRGBIndoor_1__3_ 0x700032B6
s5k5caga_i2c_write(0x0F12, 0x0062); //#SARR_usDualGammaLutRGBIndoor_1__4_ 0x700032B8
s5k5caga_i2c_write(0x0F12, 0x00D5); //#SARR_usDualGammaLutRGBIndoor_1__5_ 0x700032BA
s5k5caga_i2c_write(0x0F12, 0x0138); //#SARR_usDualGammaLutRGBIndoor_1__6_ 0x700032BC
s5k5caga_i2c_write(0x0F12, 0x0161); //#SARR_usDualGammaLutRGBIndoor_1__7_ 0x700032BE
s5k5caga_i2c_write(0x0F12, 0x0186); //#SARR_usDualGammaLutRGBIndoor_1__8_ 0x700032C0
s5k5caga_i2c_write(0x0F12, 0x01BC); //#SARR_usDualGammaLutRGBIndoor_1__9_ 0x700032C2
s5k5caga_i2c_write(0x0F12, 0x01E8); //#SARR_usDualGammaLutRGBIndoor_1__10_ 0x700032C4
s5k5caga_i2c_write(0x0F12, 0x020F); //#SARR_usDualGammaLutRGBIndoor_1__11_ 0x700032C6
s5k5caga_i2c_write(0x0F12, 0x0232); //#SARR_usDualGammaLutRGBIndoor_1__12_ 0x700032C8
s5k5caga_i2c_write(0x0F12, 0x0273); //#SARR_usDualGammaLutRGBIndoor_1__13_ 0x700032CA
s5k5caga_i2c_write(0x0F12, 0x02AF); //#SARR_usDualGammaLutRGBIndoor_1__14_ 0x700032CC
s5k5caga_i2c_write(0x0F12, 0x0309); //#SARR_usDualGammaLutRGBIndoor_1__15_ 0x700032CE
s5k5caga_i2c_write(0x0F12, 0x0355); //#SARR_usDualGammaLutRGBIndoor_1__16_ 0x700032D0
s5k5caga_i2c_write(0x0F12, 0x0394); //#SARR_usDualGammaLutRGBIndoor_1__17_ 0x700032D2
s5k5caga_i2c_write(0x0F12, 0x03CE); //#SARR_usDualGammaLutRGBIndoor_1__18_ 0x700032D4
s5k5caga_i2c_write(0x0F12, 0x03FF); //#SARR_usDualGammaLutRGBIndoor_1__19_ 0x700032D6
s5k5caga_i2c_write(0x0F12, 0x0000); //#SARR_usDualGammaLutRGBIndoor_2__0_ 0x700032D8
s5k5caga_i2c_write(0x0F12, 0x0004); //#SARR_usDualGammaLutRGBIndoor_2__1_ 0x700032DA
s5k5caga_i2c_write(0x0F12, 0x0010); //#SARR_usDualGammaLutRGBIndoor_2__2_ 0x700032DC
s5k5caga_i2c_write(0x0F12, 0x002A); //#SARR_usDualGammaLutRGBIndoor_2__3_ 0x700032DE
s5k5caga_i2c_write(0x0F12, 0x0062); //#SARR_usDualGammaLutRGBIndoor_2__4_ 0x700032E0
s5k5caga_i2c_write(0x0F12, 0x00D5); //#SARR_usDualGammaLutRGBIndoor_2__5_ 0x700032E2
s5k5caga_i2c_write(0x0F12, 0x0138); //#SARR_usDualGammaLutRGBIndoor_2__6_ 0x700032E4
s5k5caga_i2c_write(0x0F12, 0x0161); //#SARR_usDualGammaLutRGBIndoor_2__7_ 0x700032E6
s5k5caga_i2c_write(0x0F12, 0x0186); //#SARR_usDualGammaLutRGBIndoor_2__8_ 0x700032E8
s5k5caga_i2c_write(0x0F12, 0x01BC); //#SARR_usDualGammaLutRGBIndoor_2__9_ 0x700032EA
s5k5caga_i2c_write(0x0F12, 0x01E8); //#SARR_usDualGammaLutRGBIndoor_2__10_ 0x700032EC
s5k5caga_i2c_write(0x0F12, 0x020F); //#SARR_usDualGammaLutRGBIndoor_2__11_ 0x700032EE
s5k5caga_i2c_write(0x0F12, 0x0232); //#SARR_usDualGammaLutRGBIndoor_2__12_ 0x700032F0
s5k5caga_i2c_write(0x0F12, 0x0273); //#SARR_usDualGammaLutRGBIndoor_2__13_ 0x700032F2
s5k5caga_i2c_write(0x0F12, 0x02AF); //#SARR_usDualGammaLutRGBIndoor_2__14_ 0x700032F4
s5k5caga_i2c_write(0x0F12, 0x0309); //#SARR_usDualGammaLutRGBIndoor_2__15_ 0x700032F6
s5k5caga_i2c_write(0x0F12, 0x0355); //#SARR_usDualGammaLutRGBIndoor_2__16_ 0x700032F8
s5k5caga_i2c_write(0x0F12, 0x0394); //#SARR_usDualGammaLutRGBIndoor_2__17_ 0x700032FA
s5k5caga_i2c_write(0x0F12, 0x03CE); //#SARR_usDualGammaLutRGBIndoor_2__18_ 0x700032FC
s5k5caga_i2c_write(0x0F12, 0x03FF); //#SARR_usDualGammaLutRGBIndoor_2__19_ 0x700032FE

s5k5caga_i2c_write(0x0F12, 0x0000); //#SARR_usDualGammaLutRGBOutdoor_0__0_ 0x70003300
s5k5caga_i2c_write(0x0F12, 0x0004); //#SARR_usDualGammaLutRGBOutdoor_0__1_ 0x70003302
s5k5caga_i2c_write(0x0F12, 0x0010); //#SARR_usDualGammaLutRGBOutdoor_0__2_ 0x70003304
s5k5caga_i2c_write(0x0F12, 0x002A); //#SARR_usDualGammaLutRGBOutdoor_0__3_ 0x70003306
s5k5caga_i2c_write(0x0F12, 0x0062); //#SARR_usDualGammaLutRGBOutdoor_0__4_ 0x70003308
s5k5caga_i2c_write(0x0F12, 0x00D5); //#SARR_usDualGammaLutRGBOutdoor_0__5_ 0x7000330A
s5k5caga_i2c_write(0x0F12, 0x0138); //#SARR_usDualGammaLutRGBOutdoor_0__6_ 0x7000330C
s5k5caga_i2c_write(0x0F12, 0x0161); //#SARR_usDualGammaLutRGBOutdoor_0__7_ 0x7000330E
s5k5caga_i2c_write(0x0F12, 0x0186); //#SARR_usDualGammaLutRGBOutdoor_0__8_ 0x70003310
s5k5caga_i2c_write(0x0F12, 0x01BC); //#SARR_usDualGammaLutRGBOutdoor_0__9_ 0x70003312
s5k5caga_i2c_write(0x0F12, 0x01E8); //#SARR_usDualGammaLutRGBOutdoor_0__10_0x70003314
s5k5caga_i2c_write(0x0F12, 0x020F); //#SARR_usDualGammaLutRGBOutdoor_0__11_0x70003316
s5k5caga_i2c_write(0x0F12, 0x0232); //#SARR_usDualGammaLutRGBOutdoor_0__12_0x70003318
s5k5caga_i2c_write(0x0F12, 0x0273); //#SARR_usDualGammaLutRGBOutdoor_0__13_0x7000331A
s5k5caga_i2c_write(0x0F12, 0x02AF); //#SARR_usDualGammaLutRGBOutdoor_0__14_0x7000331C
s5k5caga_i2c_write(0x0F12, 0x0309); //#SARR_usDualGammaLutRGBOutdoor_0__15_0x7000331E
s5k5caga_i2c_write(0x0F12, 0x0355); //#SARR_usDualGammaLutRGBOutdoor_0__16_0x70003320
s5k5caga_i2c_write(0x0F12, 0x0394); //#SARR_usDualGammaLutRGBOutdoor_0__17_0x70003322
s5k5caga_i2c_write(0x0F12, 0x03CE); //#SARR_usDualGammaLutRGBOutdoor_0__18_0x70003324
s5k5caga_i2c_write(0x0F12, 0x03FF); //#SARR_usDualGammaLutRGBOutdoor_0__19_0x70003326
s5k5caga_i2c_write(0x0F12, 0x0000); //#SARR_usDualGammaLutRGBOutdoor_1__0_ 0x70003328
s5k5caga_i2c_write(0x0F12, 0x0004); //#SARR_usDualGammaLutRGBOutdoor_1__1_ 0x7000332A
s5k5caga_i2c_write(0x0F12, 0x0010); //#SARR_usDualGammaLutRGBOutdoor_1__2_ 0x7000332C
s5k5caga_i2c_write(0x0F12, 0x002A); //#SARR_usDualGammaLutRGBOutdoor_1__3_ 0x7000332E
s5k5caga_i2c_write(0x0F12, 0x0062); //#SARR_usDualGammaLutRGBOutdoor_1__4_ 0x70003330
s5k5caga_i2c_write(0x0F12, 0x00D5); //#SARR_usDualGammaLutRGBOutdoor_1__5_ 0x70003332
s5k5caga_i2c_write(0x0F12, 0x0138); //#SARR_usDualGammaLutRGBOutdoor_1__6_ 0x70003334
s5k5caga_i2c_write(0x0F12, 0x0161); //#SARR_usDualGammaLutRGBOutdoor_1__7_ 0x70003336
s5k5caga_i2c_write(0x0F12, 0x0186); //#SARR_usDualGammaLutRGBOutdoor_1__8_ 0x70003338
s5k5caga_i2c_write(0x0F12, 0x01BC); //#SARR_usDualGammaLutRGBOutdoor_1__9_ 0x7000333A
s5k5caga_i2c_write(0x0F12, 0x01E8); //#SARR_usDualGammaLutRGBOutdoor_1__10_0x7000333C
s5k5caga_i2c_write(0x0F12, 0x020F); //#SARR_usDualGammaLutRGBOutdoor_1__11_0x7000333E
s5k5caga_i2c_write(0x0F12, 0x0232); //#SARR_usDualGammaLutRGBOutdoor_1__12_0x70003340
s5k5caga_i2c_write(0x0F12, 0x0273); //#SARR_usDualGammaLutRGBOutdoor_1__13_0x70003342
s5k5caga_i2c_write(0x0F12, 0x02AF); //#SARR_usDualGammaLutRGBOutdoor_1__14_0x70003344
s5k5caga_i2c_write(0x0F12, 0x0309); //#SARR_usDualGammaLutRGBOutdoor_1__15_0x70003346
s5k5caga_i2c_write(0x0F12, 0x0355); //#SARR_usDualGammaLutRGBOutdoor_1__16_0x70003348
s5k5caga_i2c_write(0x0F12, 0x0394); //#SARR_usDualGammaLutRGBOutdoor_1__17_0x7000334A
s5k5caga_i2c_write(0x0F12, 0x03CE); //#SARR_usDualGammaLutRGBOutdoor_1__18_0x7000334C
s5k5caga_i2c_write(0x0F12, 0x03FF); //#SARR_usDualGammaLutRGBOutdoor_1__19_0x7000334E
s5k5caga_i2c_write(0x0F12, 0x0000); //#SARR_usDualGammaLutRGBOutdoor_2__0_ 0x70003350
s5k5caga_i2c_write(0x0F12, 0x0004); //#SARR_usDualGammaLutRGBOutdoor_2__1_ 0x70003352
s5k5caga_i2c_write(0x0F12, 0x0010); //#SARR_usDualGammaLutRGBOutdoor_2__2_ 0x70003354
s5k5caga_i2c_write(0x0F12, 0x002A); //#SARR_usDualGammaLutRGBOutdoor_2__3_ 0x70003356
s5k5caga_i2c_write(0x0F12, 0x0062); //#SARR_usDualGammaLutRGBOutdoor_2__4_ 0x70003358
s5k5caga_i2c_write(0x0F12, 0x00D5); //#SARR_usDualGammaLutRGBOutdoor_2__5_ 0x7000335A
s5k5caga_i2c_write(0x0F12, 0x0138); //#SARR_usDualGammaLutRGBOutdoor_2__6_ 0x7000335C
s5k5caga_i2c_write(0x0F12, 0x0161); //#SARR_usDualGammaLutRGBOutdoor_2__7_ 0x7000335E
s5k5caga_i2c_write(0x0F12, 0x0186); //#SARR_usDualGammaLutRGBOutdoor_2__8_ 0x70003360
s5k5caga_i2c_write(0x0F12, 0x01BC); //#SARR_usDualGammaLutRGBOutdoor_2__9_ 0x70003362
s5k5caga_i2c_write(0x0F12, 0x01E8); //#SARR_usDualGammaLutRGBOutdoor_2__10_0x70003364
s5k5caga_i2c_write(0x0F12, 0x020F); //#SARR_usDualGammaLutRGBOutdoor_2__11_0x70003366
s5k5caga_i2c_write(0x0F12, 0x0232); //#SARR_usDualGammaLutRGBOutdoor_2__12_0x70003368
s5k5caga_i2c_write(0x0F12, 0x0273); //#SARR_usDualGammaLutRGBOutdoor_2__13_0x7000336A
s5k5caga_i2c_write(0x0F12, 0x02AF); //#SARR_usDualGammaLutRGBOutdoor_2__14_0x7000336C
s5k5caga_i2c_write(0x0F12, 0x0309); //#SARR_usDualGammaLutRGBOutdoor_2__15_0x7000336E
s5k5caga_i2c_write(0x0F12, 0x0355); //#SARR_usDualGammaLutRGBOutdoor_2__16_0x70003370
s5k5caga_i2c_write(0x0F12, 0x0394); //#SARR_usDualGammaLutRGBOutdoor_2__17_0x70003372
s5k5caga_i2c_write(0x0F12, 0x03CE); //#SARR_usDualGammaLutRGBOutdoor_2__18_0x70003374
s5k5caga_i2c_write(0x0F12, 0x03FF); //#SARR_usDualGammaLutRGBOutdoor_2__19_0x70003376

s5k5caga_i2c_write(0x002A, 0x06A6);
s5k5caga_i2c_write(0x0F12, 0x00D8); // #SARR_AwbCcmCord_0_ H
s5k5caga_i2c_write(0x0F12, 0x00FC); // #SARR_AwbCcmCord_1_ A
s5k5caga_i2c_write(0x0F12, 0x0120); // #SARR_AwbCcmCord_2_ TL84/U30
s5k5caga_i2c_write(0x0F12, 0x014C); // #SARR_AwbCcmCord_3_ CWF
s5k5caga_i2c_write(0x0F12, 0x0184); // #SARR_AwbCcmCord_4_ D50
s5k5caga_i2c_write(0x0F12, 0x01AD); // #SARR_AwbCcmCord_5_ D65

s5k5caga_i2c_write(0x002A, 0x1034);  // Hong  1123           
s5k5caga_i2c_write(0x0F12, 0x00B5); // #SARR_IllumType[0]   
s5k5caga_i2c_write(0x0F12, 0x00CF); // #SARR_IllumType[1]   
s5k5caga_i2c_write(0x0F12, 0x0116); // #SARR_IllumType[2]   
s5k5caga_i2c_write(0x0F12, 0x0140); // #SARR_IllumType[3]   
s5k5caga_i2c_write(0x0F12, 0x0150); // #SARR_IllumType[4]   
s5k5caga_i2c_write(0x0F12, 0x0174); // #SARR_IllumType[5]   
s5k5caga_i2c_write(0x0F12, 0x018E); // #SARR_IllumType[6]   

s5k5caga_i2c_write(0x0F12, 0x00B8); // #SARR_IllumTypeF[0]  
s5k5caga_i2c_write(0x0F12, 0x00BA); // #SARR_IllumTypeF[1]  
s5k5caga_i2c_write(0x0F12, 0x00C0); // #SARR_IllumTypeF[2]  
s5k5caga_i2c_write(0x0F12, 0x00F0); // #SARR_IllumTypeF[3]  
s5k5caga_i2c_write(0x0F12, 0x0100); // #SARR_IllumTypeF[4]  
s5k5caga_i2c_write(0x0F12, 0x0100); // #SARR_IllumTypeF[5]  
s5k5caga_i2c_write(0x0F12, 0x0100); // #SARR_IllumTypeF[6]  


//================================================================================================
// SET AFIT
//================================================================================================
// Noise index
s5k5caga_i2c_write(0x002A, 0x0764);
s5k5caga_i2c_write(0x0F12, 0x0049);   // #afit_uNoiseIndInDoor[0] // 64
s5k5caga_i2c_write(0x0F12, 0x005F);   // #afit_uNoiseIndInDoor[1] // 165
s5k5caga_i2c_write(0x0F12, 0x00CB);   // #afit_uNoiseIndInDoor[2] // 377
s5k5caga_i2c_write(0x0F12, 0x01E0);   // #afit_uNoiseIndInDoor[3] // 616
s5k5caga_i2c_write(0x0F12, 0x0220);   // #afit_uNoiseIndInDoor[4] // 700

s5k5caga_i2c_write(0x002A, 0x0770);  // AFIT table start address // 7000_07C4
s5k5caga_i2c_write(0x0F12, 0x07C4);
s5k5caga_i2c_write(0x0F12, 0x7000);
// AFIT table (Variables)
s5k5caga_i2c_write(0x002A, 0x07C4);
s5k5caga_i2c_write(0x0F12, 0x0034);  // #TVAR_afit_pBaseVals[0]
s5k5caga_i2c_write(0x0F12, 0x0000);  // #TVAR_afit_pBaseVals[1]
s5k5caga_i2c_write(0x0F12, 0x0014);  // #TVAR_afit_pBaseVals[2]
s5k5caga_i2c_write(0x0F12, 0x0000);  // #TVAR_afit_pBaseVals[3]
s5k5caga_i2c_write(0x0F12, 0x0000);  // #TVAR_afit_pBaseVals[4]
s5k5caga_i2c_write(0x0F12, 0x00C1);  // #TVAR_afit_pBaseVals[5]
s5k5caga_i2c_write(0x0F12, 0x03FF);  // #TVAR_afit_pBaseVals[6]
s5k5caga_i2c_write(0x0F12, 0x009C);  // #TVAR_afit_pBaseVals[7]
s5k5caga_i2c_write(0x0F12, 0x0251);  // #TVAR_afit_pBaseVals[8]
s5k5caga_i2c_write(0x0F12, 0x03FF);  // #TVAR_afit_pBaseVals[9]
s5k5caga_i2c_write(0x0F12, 0x000C);  // #TVAR_afit_pBaseVals[10]
s5k5caga_i2c_write(0x0F12, 0x0010);  // #TVAR_afit_pBaseVals[11]
s5k5caga_i2c_write(0x0F12, 0x012C);  // #TVAR_afit_pBaseVals[12]
s5k5caga_i2c_write(0x0F12, 0x03E8);  // #TVAR_afit_pBaseVals[13]
s5k5caga_i2c_write(0x0F12, 0x0046);  // #TVAR_afit_pBaseVals[14]
s5k5caga_i2c_write(0x0F12, 0x005A);  // #TVAR_afit_pBaseVals[15]
s5k5caga_i2c_write(0x0F12, 0x0070);  // #TVAR_afit_pBaseVals[16]
s5k5caga_i2c_write(0x0F12, 0x0020);  // #TVAR_afit_pBaseVals[17]
s5k5caga_i2c_write(0x0F12, 0x0020);  // #TVAR_afit_pBaseVals[18]
s5k5caga_i2c_write(0x0F12, 0x01AA);  // #TVAR_afit_pBaseVals[19]
s5k5caga_i2c_write(0x0F12, 0x003C);  // #TVAR_afit_pBaseVals[20]
s5k5caga_i2c_write(0x0F12, 0x003C);  // #TVAR_afit_pBaseVals[21]
s5k5caga_i2c_write(0x0F12, 0x0000);  // #TVAR_afit_pBaseVals[22]
s5k5caga_i2c_write(0x0F12, 0x0000);  // #TVAR_afit_pBaseVals[23]
s5k5caga_i2c_write(0x0F12, 0x003E);  // #TVAR_afit_pBaseVals[24]
s5k5caga_i2c_write(0x0F12, 0x0008);  // #TVAR_afit_pBaseVals[25]
s5k5caga_i2c_write(0x0F12, 0x003C);  // #TVAR_afit_pBaseVals[26]
s5k5caga_i2c_write(0x0F12, 0x001E);  // #TVAR_afit_pBaseVals[27]
s5k5caga_i2c_write(0x0F12, 0x003C);  // #TVAR_afit_pBaseVals[28]
s5k5caga_i2c_write(0x0F12, 0x001E);  // #TVAR_afit_pBaseVals[29]
s5k5caga_i2c_write(0x0F12, 0x0A24);  // #TVAR_afit_pBaseVals[30]
s5k5caga_i2c_write(0x0F12, 0x1701);  // #TVAR_afit_pBaseVals[31]
s5k5caga_i2c_write(0x0F12, 0x0229);  // #TVAR_afit_pBaseVals[32]
s5k5caga_i2c_write(0x0F12, 0x1403);  // #TVAR_afit_pBaseVals[33]
s5k5caga_i2c_write(0x0F12, 0x0000);  // #TVAR_afit_pBaseVals[34]
s5k5caga_i2c_write(0x0F12, 0x0000);  // #TVAR_afit_pBaseVals[35]
s5k5caga_i2c_write(0x0F12, 0x0000);  // #TVAR_afit_pBaseVals[36]
s5k5caga_i2c_write(0x0F12, 0x00FF);  // #TVAR_afit_pBaseVals[37]
s5k5caga_i2c_write(0x0F12, 0x045A);  // #TVAR_afit_pBaseVals[38]
s5k5caga_i2c_write(0x0F12, 0x1414);  // #TVAR_afit_pBaseVals[39]
s5k5caga_i2c_write(0x0F12, 0x0301);  // #TVAR_afit_pBaseVals[40]
s5k5caga_i2c_write(0x0F12, 0xFF07);  // #TVAR_afit_pBaseVals[41]
s5k5caga_i2c_write(0x0F12, 0x081E);  // #TVAR_afit_pBaseVals[42]
s5k5caga_i2c_write(0x0F12, 0x0A14);  // #TVAR_afit_pBaseVals[43]
s5k5caga_i2c_write(0x0F12, 0x0F0F);  // #TVAR_afit_pBaseVals[44]
s5k5caga_i2c_write(0x0F12, 0x0A08);  // #TVAR_afit_pBaseVals[45]
s5k5caga_i2c_write(0x0F12, 0x0032);  // #TVAR_afit_pBaseVals[46]
s5k5caga_i2c_write(0x0F12, 0x000E);  // #TVAR_afit_pBaseVals[47]
s5k5caga_i2c_write(0x0F12, 0x0002);  // #TVAR_afit_pBaseVals[48]
s5k5caga_i2c_write(0x0F12, 0x00FF);  // #TVAR_afit_pBaseVals[49]
s5k5caga_i2c_write(0x0F12, 0x1102);  // #TVAR_afit_pBaseVals[50]
s5k5caga_i2c_write(0x0F12, 0x001B);  // #TVAR_afit_pBaseVals[51]
s5k5caga_i2c_write(0x0F12, 0x0900);  // #TVAR_afit_pBaseVals[52]
s5k5caga_i2c_write(0x0F12, 0x0600);  // #TVAR_afit_pBaseVals[53]
s5k5caga_i2c_write(0x0F12, 0x0504);  // #TVAR_afit_pBaseVals[54]
s5k5caga_i2c_write(0x0F12, 0x0306);  // #TVAR_afit_pBaseVals[55]
s5k5caga_i2c_write(0x0F12, 0x4603);  // #TVAR_afit_pBaseVals[56]
s5k5caga_i2c_write(0x0F12, 0x0480);  // #TVAR_afit_pBaseVals[57]
s5k5caga_i2c_write(0x0F12, 0x003C);  // #TVAR_afit_pBaseVals[58]
s5k5caga_i2c_write(0x0F12, 0x0080);  // #TVAR_afit_pBaseVals[59]
s5k5caga_i2c_write(0x0F12, 0x0101);  // #TVAR_afit_pBaseVals[60]
s5k5caga_i2c_write(0x0F12, 0x0707);  // #TVAR_afit_pBaseVals[61]
s5k5caga_i2c_write(0x0F12, 0x5001);  // #TVAR_afit_pBaseVals[62]
s5k5caga_i2c_write(0x0F12, 0xC850);  // #TVAR_afit_pBaseVals[63]
s5k5caga_i2c_write(0x0F12, 0x50C8);  // #TVAR_afit_pBaseVals[64]
s5k5caga_i2c_write(0x0F12, 0x0500);  // #TVAR_afit_pBaseVals[65]
s5k5caga_i2c_write(0x0F12, 0x0003);  // #TVAR_afit_pBaseVals[66]
s5k5caga_i2c_write(0x0F12, 0x5400);  // #TVAR_afit_pBaseVals[67]
s5k5caga_i2c_write(0x0F12, 0x0714);  // #TVAR_afit_pBaseVals[68]
s5k5caga_i2c_write(0x0F12, 0x32FF);  // #TVAR_afit_pBaseVals[69]
s5k5caga_i2c_write(0x0F12, 0x5A04);  // #TVAR_afit_pBaseVals[70]
s5k5caga_i2c_write(0x0F12, 0x201E);  // #TVAR_afit_pBaseVals[71]
s5k5caga_i2c_write(0x0F12, 0x4012);  // #TVAR_afit_pBaseVals[72]
s5k5caga_i2c_write(0x0F12, 0x0204);  // #TVAR_afit_pBaseVals[73]
s5k5caga_i2c_write(0x0F12, 0x1403);  // #TVAR_afit_pBaseVals[74]
s5k5caga_i2c_write(0x0F12, 0x0114);  // #TVAR_afit_pBaseVals[75]
s5k5caga_i2c_write(0x0F12, 0x0101);  // #TVAR_afit_pBaseVals[76]
s5k5caga_i2c_write(0x0F12, 0x4446);  // #TVAR_afit_pBaseVals[77]
s5k5caga_i2c_write(0x0F12, 0x646E);  // #TVAR_afit_pBaseVals[78]
s5k5caga_i2c_write(0x0F12, 0x0028);  // #TVAR_afit_pBaseVals[79]
s5k5caga_i2c_write(0x0F12, 0x030A);  // #TVAR_afit_pBaseVals[80]
s5k5caga_i2c_write(0x0F12, 0x0000);  // #TVAR_afit_pBaseVals[81]
s5k5caga_i2c_write(0x0F12, 0x141E);  // #TVAR_afit_pBaseVals[82]
s5k5caga_i2c_write(0x0F12, 0xFF07);  // #TVAR_afit_pBaseVals[83]
s5k5caga_i2c_write(0x0F12, 0x0432);  // #TVAR_afit_pBaseVals[84]
s5k5caga_i2c_write(0x0F12, 0x0014);  // #TVAR_afit_pBaseVals[85]
s5k5caga_i2c_write(0x0F12, 0x0F0F);  // #TVAR_afit_pBaseVals[86]
s5k5caga_i2c_write(0x0F12, 0x0440);  // #TVAR_afit_pBaseVals[87]
s5k5caga_i2c_write(0x0F12, 0x0302);  // #TVAR_afit_pBaseVals[88]
s5k5caga_i2c_write(0x0F12, 0x1414);  // #TVAR_afit_pBaseVals[89]
s5k5caga_i2c_write(0x0F12, 0x0101);  // #TVAR_afit_pBaseVals[90]
s5k5caga_i2c_write(0x0F12, 0x4601);  // #TVAR_afit_pBaseVals[91]
s5k5caga_i2c_write(0x0F12, 0x6E44);  // #TVAR_afit_pBaseVals[92]
s5k5caga_i2c_write(0x0F12, 0x2864);  // #TVAR_afit_pBaseVals[93]
s5k5caga_i2c_write(0x0F12, 0x0A00);  // #TVAR_afit_pBaseVals[94]
s5k5caga_i2c_write(0x0F12, 0x0003);  // #TVAR_afit_pBaseVals[95]
s5k5caga_i2c_write(0x0F12, 0x1E00);  // #TVAR_afit_pBaseVals[96]
s5k5caga_i2c_write(0x0F12, 0x0714);  // #TVAR_afit_pBaseVals[97]
s5k5caga_i2c_write(0x0F12, 0x32FF);  // #TVAR_afit_pBaseVals[98]
s5k5caga_i2c_write(0x0F12, 0x1404);  // #TVAR_afit_pBaseVals[99]
s5k5caga_i2c_write(0x0F12, 0x0F0A);  // #TVAR_afit_pBaseVals[100]
s5k5caga_i2c_write(0x0F12, 0x400F);  // #TVAR_afit_pBaseVals[101]
s5k5caga_i2c_write(0x0F12, 0x0204);  // #TVAR_afit_pBaseVals[102]
s5k5caga_i2c_write(0x0F12, 0x0003);  // #TVAR_afit_pBaseVals[103]
s5k5caga_i2c_write(0x0F12, 0x0000);  // #TVAR_afit_pBaseVals[104]
s5k5caga_i2c_write(0x0F12, 0x0000);  // #TVAR_afit_pBaseVals[105]
s5k5caga_i2c_write(0x0F12, 0x0014);  // #TVAR_afit_pBaseVals[106]
s5k5caga_i2c_write(0x0F12, 0x0000);  // #TVAR_afit_pBaseVals[107]
s5k5caga_i2c_write(0x0F12, 0x0000);  // #TVAR_afit_pBaseVals[108]
s5k5caga_i2c_write(0x0F12, 0x00C1);  // #TVAR_afit_pBaseVals[109]
s5k5caga_i2c_write(0x0F12, 0x03FF);  // #TVAR_afit_pBaseVals[110]
s5k5caga_i2c_write(0x0F12, 0x009C);  // #TVAR_afit_pBaseVals[111]
s5k5caga_i2c_write(0x0F12, 0x0251);  // #TVAR_afit_pBaseVals[112]
s5k5caga_i2c_write(0x0F12, 0x03FF);  // #TVAR_afit_pBaseVals[113]
s5k5caga_i2c_write(0x0F12, 0x000C);  // #TVAR_afit_pBaseVals[114]
s5k5caga_i2c_write(0x0F12, 0x0010);  // #TVAR_afit_pBaseVals[115]
s5k5caga_i2c_write(0x0F12, 0x012C);  // #TVAR_afit_pBaseVals[116]
s5k5caga_i2c_write(0x0F12, 0x03E8);  // #TVAR_afit_pBaseVals[117]
s5k5caga_i2c_write(0x0F12, 0x0046);  // #TVAR_afit_pBaseVals[118]
s5k5caga_i2c_write(0x0F12, 0x005A);  // #TVAR_afit_pBaseVals[119]
s5k5caga_i2c_write(0x0F12, 0x0070);  // #TVAR_afit_pBaseVals[120]
s5k5caga_i2c_write(0x0F12, 0x000A);  // #TVAR_afit_pBaseVals[121]
s5k5caga_i2c_write(0x0F12, 0x000A);  // #TVAR_afit_pBaseVals[122]
s5k5caga_i2c_write(0x0F12, 0x01AE);  // #TVAR_afit_pBaseVals[123]
s5k5caga_i2c_write(0x0F12, 0x001E);  // #TVAR_afit_pBaseVals[124]
s5k5caga_i2c_write(0x0F12, 0x001E);  // #TVAR_afit_pBaseVals[125]
s5k5caga_i2c_write(0x0F12, 0x0000);  // #TVAR_afit_pBaseVals[126]
s5k5caga_i2c_write(0x0F12, 0x0000);  // #TVAR_afit_pBaseVals[127]
s5k5caga_i2c_write(0x0F12, 0x003E);  // #TVAR_afit_pBaseVals[128]
s5k5caga_i2c_write(0x0F12, 0x0008);  // #TVAR_afit_pBaseVals[129]
s5k5caga_i2c_write(0x0F12, 0x003C);  // #TVAR_afit_pBaseVals[130]
s5k5caga_i2c_write(0x0F12, 0x001E);  // #TVAR_afit_pBaseVals[131]
s5k5caga_i2c_write(0x0F12, 0x003C);  // #TVAR_afit_pBaseVals[132]
s5k5caga_i2c_write(0x0F12, 0x001E);  // #TVAR_afit_pBaseVals[133]
s5k5caga_i2c_write(0x0F12, 0x0A24);  // #TVAR_afit_pBaseVals[134]
s5k5caga_i2c_write(0x0F12, 0x1701);  // #TVAR_afit_pBaseVals[135]
s5k5caga_i2c_write(0x0F12, 0x0229);  // #TVAR_afit_pBaseVals[136]
s5k5caga_i2c_write(0x0F12, 0x1403);  // #TVAR_afit_pBaseVals[137]
s5k5caga_i2c_write(0x0F12, 0x0000);  // #TVAR_afit_pBaseVals[138]
s5k5caga_i2c_write(0x0F12, 0x0000);  // #TVAR_afit_pBaseVals[139]
s5k5caga_i2c_write(0x0F12, 0x0000);  // #TVAR_afit_pBaseVals[140]
s5k5caga_i2c_write(0x0F12, 0x00FF);  // #TVAR_afit_pBaseVals[141]
s5k5caga_i2c_write(0x0F12, 0x045A);  // #TVAR_afit_pBaseVals[142]
s5k5caga_i2c_write(0x0F12, 0x1414);  // #TVAR_afit_pBaseVals[143]
s5k5caga_i2c_write(0x0F12, 0x0301);  // #TVAR_afit_pBaseVals[144]
s5k5caga_i2c_write(0x0F12, 0xFF07);  // #TVAR_afit_pBaseVals[145]
s5k5caga_i2c_write(0x0F12, 0x081E);  // #TVAR_afit_pBaseVals[146]
s5k5caga_i2c_write(0x0F12, 0x0A14);  // #TVAR_afit_pBaseVals[147]
s5k5caga_i2c_write(0x0F12, 0x0F0F);  // #TVAR_afit_pBaseVals[148]
s5k5caga_i2c_write(0x0F12, 0x0A05);  // //0A03 // #TVAR_afit_pBaseVals[149]
s5k5caga_i2c_write(0x0F12, 0x0046);  // #TVAR_afit_pBaseVals[150]
s5k5caga_i2c_write(0x0F12, 0x0018);  // #TVAR_afit_pBaseVals[151]
s5k5caga_i2c_write(0x0F12, 0x0002);  // #TVAR_afit_pBaseVals[152]
s5k5caga_i2c_write(0x0F12, 0x00FF);  // #TVAR_afit_pBaseVals[153]
s5k5caga_i2c_write(0x0F12, 0x1102);  // #TVAR_afit_pBaseVals[154]
s5k5caga_i2c_write(0x0F12, 0x001B);  // #TVAR_afit_pBaseVals[155]
s5k5caga_i2c_write(0x0F12, 0x0900);  // #TVAR_afit_pBaseVals[156]
s5k5caga_i2c_write(0x0F12, 0x0600);  // #TVAR_afit_pBaseVals[157]
s5k5caga_i2c_write(0x0F12, 0x0504);  // #TVAR_afit_pBaseVals[158]
s5k5caga_i2c_write(0x0F12, 0x0306);  // #TVAR_afit_pBaseVals[159]
s5k5caga_i2c_write(0x0F12, 0x4603);  // #TVAR_afit_pBaseVals[160]
s5k5caga_i2c_write(0x0F12, 0x0880);  // #TVAR_afit_pBaseVals[161]
s5k5caga_i2c_write(0x0F12, 0x0046);  // #TVAR_afit_pBaseVals[162]
s5k5caga_i2c_write(0x0F12, 0x0080);  // #TVAR_afit_pBaseVals[163]
s5k5caga_i2c_write(0x0F12, 0x0101);  // #TVAR_afit_pBaseVals[164]
s5k5caga_i2c_write(0x0F12, 0x0707);  // #TVAR_afit_pBaseVals[165]
s5k5caga_i2c_write(0x0F12, 0x3C01);  // #TVAR_afit_pBaseVals[166]
s5k5caga_i2c_write(0x0F12, 0xC83C);  // #TVAR_afit_pBaseVals[167]
s5k5caga_i2c_write(0x0F12, 0x50C8);  // #TVAR_afit_pBaseVals[168]
s5k5caga_i2c_write(0x0F12, 0x0500);  // #TVAR_afit_pBaseVals[169]
s5k5caga_i2c_write(0x0F12, 0x0004);  // #TVAR_afit_pBaseVals[170]
s5k5caga_i2c_write(0x0F12, 0x3C0A);  // #TVAR_afit_pBaseVals[171]
s5k5caga_i2c_write(0x0F12, 0x0714);  // #TVAR_afit_pBaseVals[172]
s5k5caga_i2c_write(0x0F12, 0x3214);  // #TVAR_afit_pBaseVals[173]
s5k5caga_i2c_write(0x0F12, 0x5A03);  // #TVAR_afit_pBaseVals[174]
s5k5caga_i2c_write(0x0F12, 0x1228);  // #TVAR_afit_pBaseVals[175]
s5k5caga_i2c_write(0x0F12, 0x4012);  // #TVAR_afit_pBaseVals[176]
s5k5caga_i2c_write(0x0F12, 0x0604);  // #TVAR_afit_pBaseVals[177]
s5k5caga_i2c_write(0x0F12, 0x1E06);  // #TVAR_afit_pBaseVals[178]
s5k5caga_i2c_write(0x0F12, 0x011E);  // #TVAR_afit_pBaseVals[179]
s5k5caga_i2c_write(0x0F12, 0x0101);  // #TVAR_afit_pBaseVals[180]
s5k5caga_i2c_write(0x0F12, 0x3A3C);  // #TVAR_afit_pBaseVals[181]
s5k5caga_i2c_write(0x0F12, 0x585A);  // #TVAR_afit_pBaseVals[182]
s5k5caga_i2c_write(0x0F12, 0x0028);  // #TVAR_afit_pBaseVals[183]
s5k5caga_i2c_write(0x0F12, 0x030A);  // #TVAR_afit_pBaseVals[184]
s5k5caga_i2c_write(0x0F12, 0x0000);  // #TVAR_afit_pBaseVals[185]
s5k5caga_i2c_write(0x0F12, 0x141E);  // #TVAR_afit_pBaseVals[186]
s5k5caga_i2c_write(0x0F12, 0xFF07);  // #TVAR_afit_pBaseVals[187]
s5k5caga_i2c_write(0x0F12, 0x0432);  // #TVAR_afit_pBaseVals[188]
s5k5caga_i2c_write(0x0F12, 0x1428);  // #TVAR_afit_pBaseVals[189]
s5k5caga_i2c_write(0x0F12, 0x0F0F);  // #TVAR_afit_pBaseVals[190]
s5k5caga_i2c_write(0x0F12, 0x0440);  // #TVAR_afit_pBaseVals[191]
s5k5caga_i2c_write(0x0F12, 0x0302);  // #TVAR_afit_pBaseVals[192]
s5k5caga_i2c_write(0x0F12, 0x1E1E);  // #TVAR_afit_pBaseVals[193]
s5k5caga_i2c_write(0x0F12, 0x0101);  // #TVAR_afit_pBaseVals[194]
s5k5caga_i2c_write(0x0F12, 0x3C01);  // #TVAR_afit_pBaseVals[195]
s5k5caga_i2c_write(0x0F12, 0x5A3A);  // #TVAR_afit_pBaseVals[196]
s5k5caga_i2c_write(0x0F12, 0x2858);  // #TVAR_afit_pBaseVals[197]
s5k5caga_i2c_write(0x0F12, 0x0A00);  // #TVAR_afit_pBaseVals[198]
s5k5caga_i2c_write(0x0F12, 0x0003);  // #TVAR_afit_pBaseVals[199]
s5k5caga_i2c_write(0x0F12, 0x1E00);  // #TVAR_afit_pBaseVals[200]
s5k5caga_i2c_write(0x0F12, 0x0714);  // #TVAR_afit_pBaseVals[201]
s5k5caga_i2c_write(0x0F12, 0x32FF);  // #TVAR_afit_pBaseVals[202]
s5k5caga_i2c_write(0x0F12, 0x2804);  // #TVAR_afit_pBaseVals[203]
s5k5caga_i2c_write(0x0F12, 0x0F1E);  // #TVAR_afit_pBaseVals[204]
s5k5caga_i2c_write(0x0F12, 0x400F);  // #TVAR_afit_pBaseVals[205]
s5k5caga_i2c_write(0x0F12, 0x0204);  // #TVAR_afit_pBaseVals[206]
s5k5caga_i2c_write(0x0F12, 0x0003);  // #TVAR_afit_pBaseVals[207]
s5k5caga_i2c_write(0x0F12, 0x0000);  // #TVAR_afit_pBaseVals[208]
s5k5caga_i2c_write(0x0F12, 0x0000);  // #TVAR_afit_pBaseVals[209]
s5k5caga_i2c_write(0x0F12, 0x0000);  // #TVAR_afit_pBaseVals[210]
s5k5caga_i2c_write(0x0F12, 0x0000);  // #TVAR_afit_pBaseVals[211]
s5k5caga_i2c_write(0x0F12, 0x0000);  // #TVAR_afit_pBaseVals[212]
s5k5caga_i2c_write(0x0F12, 0x00C1);  // #TVAR_afit_pBaseVals[213]
s5k5caga_i2c_write(0x0F12, 0x03FF);  // #TVAR_afit_pBaseVals[214]
s5k5caga_i2c_write(0x0F12, 0x009C);  // #TVAR_afit_pBaseVals[215]
s5k5caga_i2c_write(0x0F12, 0x0251);  // #TVAR_afit_pBaseVals[216]
s5k5caga_i2c_write(0x0F12, 0x03FF);  // #TVAR_afit_pBaseVals[217]
s5k5caga_i2c_write(0x0F12, 0x000C);  // #TVAR_afit_pBaseVals[218]
s5k5caga_i2c_write(0x0F12, 0x0010);  // #TVAR_afit_pBaseVals[219]
s5k5caga_i2c_write(0x0F12, 0x012C);  // #TVAR_afit_pBaseVals[220]
s5k5caga_i2c_write(0x0F12, 0x03E8);  // #TVAR_afit_pBaseVals[221]
s5k5caga_i2c_write(0x0F12, 0x0046);  // #TVAR_afit_pBaseVals[222]
s5k5caga_i2c_write(0x0F12, 0x005A);  // #TVAR_afit_pBaseVals[223]
s5k5caga_i2c_write(0x0F12, 0x0070);  // #TVAR_afit_pBaseVals[224]
s5k5caga_i2c_write(0x0F12, 0x000A);  // #TVAR_afit_pBaseVals[225]
s5k5caga_i2c_write(0x0F12, 0x000A);  // #TVAR_afit_pBaseVals[226]
s5k5caga_i2c_write(0x0F12, 0x0226);  // #TVAR_afit_pBaseVals[227]
s5k5caga_i2c_write(0x0F12, 0x001E);  // #TVAR_afit_pBaseVals[228]
s5k5caga_i2c_write(0x0F12, 0x001E);  // #TVAR_afit_pBaseVals[229]
s5k5caga_i2c_write(0x0F12, 0x0000);  // #TVAR_afit_pBaseVals[230]
s5k5caga_i2c_write(0x0F12, 0x0000);  // #TVAR_afit_pBaseVals[231]
s5k5caga_i2c_write(0x0F12, 0x004E);  // #TVAR_afit_pBaseVals[232]
s5k5caga_i2c_write(0x0F12, 0x0000);  // #TVAR_afit_pBaseVals[233]
s5k5caga_i2c_write(0x0F12, 0x003C);  // #TVAR_afit_pBaseVals[234]
s5k5caga_i2c_write(0x0F12, 0x001E);  // #TVAR_afit_pBaseVals[235]
s5k5caga_i2c_write(0x0F12, 0x003C);  // #TVAR_afit_pBaseVals[236]
s5k5caga_i2c_write(0x0F12, 0x001E);  // #TVAR_afit_pBaseVals[237]
s5k5caga_i2c_write(0x0F12, 0x0A24);  // #TVAR_afit_pBaseVals[238]
s5k5caga_i2c_write(0x0F12, 0x1701);  // #TVAR_afit_pBaseVals[239]
s5k5caga_i2c_write(0x0F12, 0x0229);  // #TVAR_afit_pBaseVals[240]
s5k5caga_i2c_write(0x0F12, 0x1403);  // #TVAR_afit_pBaseVals[241]
s5k5caga_i2c_write(0x0F12, 0x0000);  // #TVAR_afit_pBaseVals[242]
s5k5caga_i2c_write(0x0F12, 0x0000);  // #TVAR_afit_pBaseVals[243]
s5k5caga_i2c_write(0x0F12, 0x0906);  // #TVAR_afit_pBaseVals[244]
s5k5caga_i2c_write(0x0F12, 0x00FF);  // #TVAR_afit_pBaseVals[245]
s5k5caga_i2c_write(0x0F12, 0x045A);  // #TVAR_afit_pBaseVals[246]
s5k5caga_i2c_write(0x0F12, 0x1414);  // #TVAR_afit_pBaseVals[247]
s5k5caga_i2c_write(0x0F12, 0x0301);  // #TVAR_afit_pBaseVals[248]
s5k5caga_i2c_write(0x0F12, 0xFF07);  // #TVAR_afit_pBaseVals[249]
s5k5caga_i2c_write(0x0F12, 0x081E);  // #TVAR_afit_pBaseVals[250]
s5k5caga_i2c_write(0x0F12, 0x0A14);  // #TVAR_afit_pBaseVals[251]
s5k5caga_i2c_write(0x0F12, 0x0F0F);  // #TVAR_afit_pBaseVals[252]
s5k5caga_i2c_write(0x0F12, 0x0A07);  // //0A03 // #TVAR_afit_pBaseVals[253]
s5k5caga_i2c_write(0x0F12, 0x009A);  // #TVAR_afit_pBaseVals[254]
s5k5caga_i2c_write(0x0F12, 0x0028);  // #TVAR_afit_pBaseVals[255]
s5k5caga_i2c_write(0x0F12, 0x0002);  // #TVAR_afit_pBaseVals[256]
s5k5caga_i2c_write(0x0F12, 0x00FF);  // #TVAR_afit_pBaseVals[257]
s5k5caga_i2c_write(0x0F12, 0x1102);  // #TVAR_afit_pBaseVals[258]
s5k5caga_i2c_write(0x0F12, 0x001B);  // #TVAR_afit_pBaseVals[259]
s5k5caga_i2c_write(0x0F12, 0x0900);  // #TVAR_afit_pBaseVals[260]
s5k5caga_i2c_write(0x0F12, 0x0600);  // #TVAR_afit_pBaseVals[261]
s5k5caga_i2c_write(0x0F12, 0x0504);  // #TVAR_afit_pBaseVals[262]
s5k5caga_i2c_write(0x0F12, 0x0306);  // #TVAR_afit_pBaseVals[263] //*<BU5D05950 zhangsheng 20100422 begin*/ 
s5k5caga_i2c_write(0x0F12, 0x4602);  // //7903 // #TVAR_afit_pBaseVals[264] //*BU5D05950 zhangsheng 20100422 end>*/ 
s5k5caga_i2c_write(0x0F12, 0x1980);  // #TVAR_afit_pBaseVals[265]
s5k5caga_i2c_write(0x0F12, 0x0080);  // #TVAR_afit_pBaseVals[266]
s5k5caga_i2c_write(0x0F12, 0x0080);  // #TVAR_afit_pBaseVals[267]
s5k5caga_i2c_write(0x0F12, 0x0101);  // #TVAR_afit_pBaseVals[268]
s5k5caga_i2c_write(0x0F12, 0x0707);  // #TVAR_afit_pBaseVals[269]
s5k5caga_i2c_write(0x0F12, 0x2A01);  // #TVAR_afit_pBaseVals[270]
s5k5caga_i2c_write(0x0F12, 0x462A);  // #TVAR_afit_pBaseVals[271]
s5k5caga_i2c_write(0x0F12, 0x5046);  // #TVAR_afit_pBaseVals[272]
s5k5caga_i2c_write(0x0F12, 0x0500);  // #TVAR_afit_pBaseVals[273]
s5k5caga_i2c_write(0x0F12, 0x1A04);  // #TVAR_afit_pBaseVals[274]
s5k5caga_i2c_write(0x0F12, 0x280A);  // #TVAR_afit_pBaseVals[275]
s5k5caga_i2c_write(0x0F12, 0x080C);  // #TVAR_afit_pBaseVals[276]
s5k5caga_i2c_write(0x0F12, 0x1414);  // #TVAR_afit_pBaseVals[277]
s5k5caga_i2c_write(0x0F12, 0x7E03);  // #TVAR_afit_pBaseVals[278]
s5k5caga_i2c_write(0x0F12, 0x124A);  // #TVAR_afit_pBaseVals[279]
s5k5caga_i2c_write(0x0F12, 0x4012);  // #TVAR_afit_pBaseVals[280]
s5k5caga_i2c_write(0x0F12, 0x0604);  // #TVAR_afit_pBaseVals[281]
s5k5caga_i2c_write(0x0F12, 0x2806);  // #TVAR_afit_pBaseVals[282]
s5k5caga_i2c_write(0x0F12, 0x0128);  // #TVAR_afit_pBaseVals[283]
s5k5caga_i2c_write(0x0F12, 0x0101);  // #TVAR_afit_pBaseVals[284]
s5k5caga_i2c_write(0x0F12, 0x2224);  // #TVAR_afit_pBaseVals[285]
s5k5caga_i2c_write(0x0F12, 0x3236);  // #TVAR_afit_pBaseVals[286]
s5k5caga_i2c_write(0x0F12, 0x0028);  // #TVAR_afit_pBaseVals[287]
s5k5caga_i2c_write(0x0F12, 0x030A);  // #TVAR_afit_pBaseVals[288]
s5k5caga_i2c_write(0x0F12, 0x0410);  // #TVAR_afit_pBaseVals[289]
s5k5caga_i2c_write(0x0F12, 0x141E);  // #TVAR_afit_pBaseVals[290]
s5k5caga_i2c_write(0x0F12, 0xFF07);  // #TVAR_afit_pBaseVals[291]
s5k5caga_i2c_write(0x0F12, 0x0432);  // #TVAR_afit_pBaseVals[292]
s5k5caga_i2c_write(0x0F12, 0x547D);  // #TVAR_afit_pBaseVals[293]
s5k5caga_i2c_write(0x0F12, 0x0F0F);  // #TVAR_afit_pBaseVals[294]
s5k5caga_i2c_write(0x0F12, 0x0440);  // #TVAR_afit_pBaseVals[295]
s5k5caga_i2c_write(0x0F12, 0x0302);  // #TVAR_afit_pBaseVals[296]
s5k5caga_i2c_write(0x0F12, 0x2828);  // #TVAR_afit_pBaseVals[297]
s5k5caga_i2c_write(0x0F12, 0x0101);  // #TVAR_afit_pBaseVals[298]
s5k5caga_i2c_write(0x0F12, 0x2401);  // #TVAR_afit_pBaseVals[299]
s5k5caga_i2c_write(0x0F12, 0x3622);  // #TVAR_afit_pBaseVals[300]
s5k5caga_i2c_write(0x0F12, 0x2832);  // #TVAR_afit_pBaseVals[301]
s5k5caga_i2c_write(0x0F12, 0x0A00);  // #TVAR_afit_pBaseVals[302]
s5k5caga_i2c_write(0x0F12, 0x1003);  // #TVAR_afit_pBaseVals[303]
s5k5caga_i2c_write(0x0F12, 0x1E04);  // #TVAR_afit_pBaseVals[304]
s5k5caga_i2c_write(0x0F12, 0x0714);  // #TVAR_afit_pBaseVals[305]
s5k5caga_i2c_write(0x0F12, 0x32FF);  // #TVAR_afit_pBaseVals[306]
s5k5caga_i2c_write(0x0F12, 0x7D04);  // #TVAR_afit_pBaseVals[307]
s5k5caga_i2c_write(0x0F12, 0x0F5E);  // #TVAR_afit_pBaseVals[308]
s5k5caga_i2c_write(0x0F12, 0x400F);  // #TVAR_afit_pBaseVals[309]
s5k5caga_i2c_write(0x0F12, 0x0204);  // #TVAR_afit_pBaseVals[310]
s5k5caga_i2c_write(0x0F12, 0x0003);  // #TVAR_afit_pBaseVals[311]
s5k5caga_i2c_write(0x0F12, 0x0000);  // #TVAR_afit_pBaseVals[312]
s5k5caga_i2c_write(0x0F12, 0x0000);  // #TVAR_afit_pBaseVals[313]
s5k5caga_i2c_write(0x0F12, 0x0000);  // #TVAR_afit_pBaseVals[314]
s5k5caga_i2c_write(0x0F12, 0x0000);  // #TVAR_afit_pBaseVals[315]
s5k5caga_i2c_write(0x0F12, 0x0000);  // #TVAR_afit_pBaseVals[316]
s5k5caga_i2c_write(0x0F12, 0x00C1);  // #TVAR_afit_pBaseVals[317]
s5k5caga_i2c_write(0x0F12, 0x03FF);  // #TVAR_afit_pBaseVals[318]
s5k5caga_i2c_write(0x0F12, 0x009C);  // #TVAR_afit_pBaseVals[319]
s5k5caga_i2c_write(0x0F12, 0x0251);  // #TVAR_afit_pBaseVals[320]
s5k5caga_i2c_write(0x0F12, 0x03FF);  // #TVAR_afit_pBaseVals[321]
s5k5caga_i2c_write(0x0F12, 0x000C);  // #TVAR_afit_pBaseVals[322]
s5k5caga_i2c_write(0x0F12, 0x0010);  // #TVAR_afit_pBaseVals[323]
s5k5caga_i2c_write(0x0F12, 0x00C8);  // #TVAR_afit_pBaseVals[324]
s5k5caga_i2c_write(0x0F12, 0x03E8);  // #TVAR_afit_pBaseVals[325]
s5k5caga_i2c_write(0x0F12, 0x0046);  // #TVAR_afit_pBaseVals[326]
s5k5caga_i2c_write(0x0F12, 0x0050);  // #TVAR_afit_pBaseVals[327]
s5k5caga_i2c_write(0x0F12, 0x0070);  // #TVAR_afit_pBaseVals[328]
s5k5caga_i2c_write(0x0F12, 0x000A);  // #TVAR_afit_pBaseVals[329]
s5k5caga_i2c_write(0x0F12, 0x000A);  // #TVAR_afit_pBaseVals[330]
s5k5caga_i2c_write(0x0F12, 0x0226);  // #TVAR_afit_pBaseVals[331]
s5k5caga_i2c_write(0x0F12, 0x0014);  // #TVAR_afit_pBaseVals[332]
s5k5caga_i2c_write(0x0F12, 0x0014);  // #TVAR_afit_pBaseVals[333]
s5k5caga_i2c_write(0x0F12, 0x0000);  // #TVAR_afit_pBaseVals[334]
s5k5caga_i2c_write(0x0F12, 0x0000);  // #TVAR_afit_pBaseVals[335]
s5k5caga_i2c_write(0x0F12, 0x004E);  // #TVAR_afit_pBaseVals[336]
s5k5caga_i2c_write(0x0F12, 0x0000);  // #TVAR_afit_pBaseVals[337]
s5k5caga_i2c_write(0x0F12, 0x002D);  // #TVAR_afit_pBaseVals[338]
s5k5caga_i2c_write(0x0F12, 0x0019);  // #TVAR_afit_pBaseVals[339]
s5k5caga_i2c_write(0x0F12, 0x002D);  // #TVAR_afit_pBaseVals[340]
s5k5caga_i2c_write(0x0F12, 0x0019);  // #TVAR_afit_pBaseVals[341]
s5k5caga_i2c_write(0x0F12, 0x0A24);  // #TVAR_afit_pBaseVals[342]
s5k5caga_i2c_write(0x0F12, 0x1701);  // #TVAR_afit_pBaseVals[343]
s5k5caga_i2c_write(0x0F12, 0x0229);  // #TVAR_afit_pBaseVals[344]
s5k5caga_i2c_write(0x0F12, 0x1403);  // #TVAR_afit_pBaseVals[345]
s5k5caga_i2c_write(0x0F12, 0x0000);  // #TVAR_afit_pBaseVals[346]
s5k5caga_i2c_write(0x0F12, 0x0000);  // #TVAR_afit_pBaseVals[347]
s5k5caga_i2c_write(0x0F12, 0x0906);  // #TVAR_afit_pBaseVals[348]
s5k5caga_i2c_write(0x0F12, 0x00FF);  // #TVAR_afit_pBaseVals[349]
s5k5caga_i2c_write(0x0F12, 0x045A);  // #TVAR_afit_pBaseVals[350]
s5k5caga_i2c_write(0x0F12, 0x1414);  // #TVAR_afit_pBaseVals[351]
s5k5caga_i2c_write(0x0F12, 0x0301);  // #TVAR_afit_pBaseVals[352]
s5k5caga_i2c_write(0x0F12, 0xFF07);  // #TVAR_afit_pBaseVals[353]
s5k5caga_i2c_write(0x0F12, 0x081E);  // #TVAR_afit_pBaseVals[354]
s5k5caga_i2c_write(0x0F12, 0x0A14);  // #TVAR_afit_pBaseVals[355]
s5k5caga_i2c_write(0x0F12, 0x0F0F);  // #TVAR_afit_pBaseVals[356]
s5k5caga_i2c_write(0x0F12, 0x0A07);  // //0A00 // #TVAR_afit_pBaseVals[357]
s5k5caga_i2c_write(0x0F12, 0x009A);  // #TVAR_afit_pBaseVals[358]
s5k5caga_i2c_write(0x0F12, 0x0028);  // #TVAR_afit_pBaseVals[359]
s5k5caga_i2c_write(0x0F12, 0x0001);  // #TVAR_afit_pBaseVals[360]
s5k5caga_i2c_write(0x0F12, 0x00FF);  // #TVAR_afit_pBaseVals[361]
s5k5caga_i2c_write(0x0F12, 0x1002);  // #TVAR_afit_pBaseVals[362]
s5k5caga_i2c_write(0x0F12, 0x001E);  // #TVAR_afit_pBaseVals[363]
s5k5caga_i2c_write(0x0F12, 0x0900);  // #TVAR_afit_pBaseVals[364]
s5k5caga_i2c_write(0x0F12, 0x0600);  // #TVAR_afit_pBaseVals[365]
s5k5caga_i2c_write(0x0F12, 0x0504);  // #TVAR_afit_pBaseVals[366]
s5k5caga_i2c_write(0x0F12, 0x0307);  // #TVAR_afit_pBaseVals[367]//*<BU5D05950 zhangsheng 20100422 begin*/
s5k5caga_i2c_write(0x0F12, 0x6402);  // //8902 // #TVAR_afit_pBaseVals[368]//*BU5D05950 zhangsheng 20100422 end>*/
s5k5caga_i2c_write(0x0F12, 0x1980);  // #TVAR_afit_pBaseVals[369]
s5k5caga_i2c_write(0x0F12, 0x0080);  // #TVAR_afit_pBaseVals[370]
s5k5caga_i2c_write(0x0F12, 0x0080);  // #TVAR_afit_pBaseVals[371]
s5k5caga_i2c_write(0x0F12, 0x0101);  // #TVAR_afit_pBaseVals[372]
s5k5caga_i2c_write(0x0F12, 0x0707);  // #TVAR_afit_pBaseVals[373]
s5k5caga_i2c_write(0x0F12, 0x2A01);  // #TVAR_afit_pBaseVals[374]
s5k5caga_i2c_write(0x0F12, 0x262A);  // #TVAR_afit_pBaseVals[375]
s5k5caga_i2c_write(0x0F12, 0x5026);  // #TVAR_afit_pBaseVals[376]
s5k5caga_i2c_write(0x0F12, 0x0500);  // #TVAR_afit_pBaseVals[377]
s5k5caga_i2c_write(0x0F12, 0x1A04);  // #TVAR_afit_pBaseVals[378]
s5k5caga_i2c_write(0x0F12, 0x280A);  // #TVAR_afit_pBaseVals[379]
s5k5caga_i2c_write(0x0F12, 0x080C);  // #TVAR_afit_pBaseVals[380]
s5k5caga_i2c_write(0x0F12, 0x1414);  // #TVAR_afit_pBaseVals[381]
s5k5caga_i2c_write(0x0F12, 0x7E03);  // #TVAR_afit_pBaseVals[382]
s5k5caga_i2c_write(0x0F12, 0x124A);  // #TVAR_afit_pBaseVals[383]
s5k5caga_i2c_write(0x0F12, 0x4012);  // #TVAR_afit_pBaseVals[384]
s5k5caga_i2c_write(0x0F12, 0x0604);  // #TVAR_afit_pBaseVals[385]
s5k5caga_i2c_write(0x0F12, 0x3C06);  // #TVAR_afit_pBaseVals[386]
s5k5caga_i2c_write(0x0F12, 0x013C);  // #TVAR_afit_pBaseVals[387]
s5k5caga_i2c_write(0x0F12, 0x0101);  // #TVAR_afit_pBaseVals[388]
s5k5caga_i2c_write(0x0F12, 0x1C1E);  // #TVAR_afit_pBaseVals[389]
s5k5caga_i2c_write(0x0F12, 0x1E22);  // #TVAR_afit_pBaseVals[390]
s5k5caga_i2c_write(0x0F12, 0x0028);  // #TVAR_afit_pBaseVals[391]
s5k5caga_i2c_write(0x0F12, 0x030A);  // #TVAR_afit_pBaseVals[392]
s5k5caga_i2c_write(0x0F12, 0x0214);  // #TVAR_afit_pBaseVals[393]
s5k5caga_i2c_write(0x0F12, 0x0E14);  // #TVAR_afit_pBaseVals[394]
s5k5caga_i2c_write(0x0F12, 0xFF06);  // #TVAR_afit_pBaseVals[395]
s5k5caga_i2c_write(0x0F12, 0x0432);  // #TVAR_afit_pBaseVals[396]
s5k5caga_i2c_write(0x0F12, 0x547D);  // #TVAR_afit_pBaseVals[397]
s5k5caga_i2c_write(0x0F12, 0x150C);  // #TVAR_afit_pBaseVals[398]
s5k5caga_i2c_write(0x0F12, 0x0440);  // #TVAR_afit_pBaseVals[399]
s5k5caga_i2c_write(0x0F12, 0x0302);  // #TVAR_afit_pBaseVals[400]
s5k5caga_i2c_write(0x0F12, 0x3C3C);  // #TVAR_afit_pBaseVals[401]
s5k5caga_i2c_write(0x0F12, 0x0101);  // #TVAR_afit_pBaseVals[402]
s5k5caga_i2c_write(0x0F12, 0x1E01);  // #TVAR_afit_pBaseVals[403]
s5k5caga_i2c_write(0x0F12, 0x221C);  // #TVAR_afit_pBaseVals[404]
s5k5caga_i2c_write(0x0F12, 0x281E);  // #TVAR_afit_pBaseVals[405]
s5k5caga_i2c_write(0x0F12, 0x0A00);  // #TVAR_afit_pBaseVals[406]
s5k5caga_i2c_write(0x0F12, 0x1403);  // #TVAR_afit_pBaseVals[407]
s5k5caga_i2c_write(0x0F12, 0x1402);  // #TVAR_afit_pBaseVals[408]
s5k5caga_i2c_write(0x0F12, 0x060E);  // #TVAR_afit_pBaseVals[409]
s5k5caga_i2c_write(0x0F12, 0x32FF);  // #TVAR_afit_pBaseVals[410]
s5k5caga_i2c_write(0x0F12, 0x7D04);  // #TVAR_afit_pBaseVals[411]
s5k5caga_i2c_write(0x0F12, 0x0C5E);  // #TVAR_afit_pBaseVals[412]
s5k5caga_i2c_write(0x0F12, 0x4015);  // #TVAR_afit_pBaseVals[413]
s5k5caga_i2c_write(0x0F12, 0x0204);  // #TVAR_afit_pBaseVals[414]
s5k5caga_i2c_write(0x0F12, 0x0003);  // #TVAR_afit_pBaseVals[415]
s5k5caga_i2c_write(0x0F12, 0x0000);  // #TVAR_afit_pBaseVals[416]
s5k5caga_i2c_write(0x0F12, 0x0000);  // #TVAR_afit_pBaseVals[417]
s5k5caga_i2c_write(0x0F12, 0x0000);  // #TVAR_afit_pBaseVals[418] //high luma sta
s5k5caga_i2c_write(0x0F12, 0x0000);  // #TVAR_afit_pBaseVals[419]
s5k5caga_i2c_write(0x0F12, 0x0000);  // #TVAR_afit_pBaseVals[420]
s5k5caga_i2c_write(0x0F12, 0x00C1);  // #TVAR_afit_pBaseVals[421]
s5k5caga_i2c_write(0x0F12, 0x03FF);  // #TVAR_afit_pBaseVals[422]
s5k5caga_i2c_write(0x0F12, 0x009C);  // #TVAR_afit_pBaseVals[423]
s5k5caga_i2c_write(0x0F12, 0x0251);  // #TVAR_afit_pBaseVals[424]
s5k5caga_i2c_write(0x0F12, 0x03FF);  // #TVAR_afit_pBaseVals[425]
s5k5caga_i2c_write(0x0F12, 0x000C);  // #TVAR_afit_pBaseVals[426]
s5k5caga_i2c_write(0x0F12, 0x0010);  // #TVAR_afit_pBaseVals[427]
s5k5caga_i2c_write(0x0F12, 0x0032);  // #TVAR_afit_pBaseVals[428]
s5k5caga_i2c_write(0x0F12, 0x028A);  // #TVAR_afit_pBaseVals[429]
s5k5caga_i2c_write(0x0F12, 0x0032);  // #TVAR_afit_pBaseVals[430]
s5k5caga_i2c_write(0x0F12, 0x01F4);  // #TVAR_afit_pBaseVals[431]
s5k5caga_i2c_write(0x0F12, 0x0070);  // #TVAR_afit_pBaseVals[432]
s5k5caga_i2c_write(0x0F12, 0x0000);  // #TVAR_afit_pBaseVals[433]
s5k5caga_i2c_write(0x0F12, 0x0000);  // #TVAR_afit_pBaseVals[434]
s5k5caga_i2c_write(0x0F12, 0x01AA);  // #TVAR_afit_pBaseVals[435]
s5k5caga_i2c_write(0x0F12, 0x003C);  // #TVAR_afit_pBaseVals[436]
s5k5caga_i2c_write(0x0F12, 0x0050);  // #TVAR_afit_pBaseVals[437]
s5k5caga_i2c_write(0x0F12, 0x0000);  // #TVAR_afit_pBaseVals[438]
s5k5caga_i2c_write(0x0F12, 0x0000);  // #TVAR_afit_pBaseVals[439]
s5k5caga_i2c_write(0x0F12, 0x0044);  // #TVAR_afit_pBaseVals[440]
s5k5caga_i2c_write(0x0F12, 0x0014);  // #TVAR_afit_pBaseVals[441]
s5k5caga_i2c_write(0x0F12, 0x0046);  // #TVAR_afit_pBaseVals[442]
s5k5caga_i2c_write(0x0F12, 0x0019);  // #TVAR_afit_pBaseVals[443]
s5k5caga_i2c_write(0x0F12, 0x0046);  // #TVAR_afit_pBaseVals[444]
s5k5caga_i2c_write(0x0F12, 0x0019);  // #TVAR_afit_pBaseVals[445]
s5k5caga_i2c_write(0x0F12, 0x0A24);  // #TVAR_afit_pBaseVals[446]
s5k5caga_i2c_write(0x0F12, 0x1701);  // #TVAR_afit_pBaseVals[447]
s5k5caga_i2c_write(0x0F12, 0x0229);  // #TVAR_afit_pBaseVals[448]
s5k5caga_i2c_write(0x0F12, 0x0503);  // #TVAR_afit_pBaseVals[449]
s5k5caga_i2c_write(0x0F12, 0x080F);  // #TVAR_afit_pBaseVals[450]
s5k5caga_i2c_write(0x0F12, 0x0808);  // #TVAR_afit_pBaseVals[451]
s5k5caga_i2c_write(0x0F12, 0x0000);  // #TVAR_afit_pBaseVals[452]
s5k5caga_i2c_write(0x0F12, 0x00FF);  // #TVAR_afit_pBaseVals[453]
s5k5caga_i2c_write(0x0F12, 0x012D);  // #TVAR_afit_pBaseVals[454]
s5k5caga_i2c_write(0x0F12, 0x1414);  // #TVAR_afit_pBaseVals[455]
s5k5caga_i2c_write(0x0F12, 0x0301);  // #TVAR_afit_pBaseVals[456]
s5k5caga_i2c_write(0x0F12, 0xFF07);  // #TVAR_afit_pBaseVals[457]
s5k5caga_i2c_write(0x0F12, 0x061E);  // #TVAR_afit_pBaseVals[458]
s5k5caga_i2c_write(0x0F12, 0x0A1E);  // #TVAR_afit_pBaseVals[459]
s5k5caga_i2c_write(0x0F12, 0x0606);  // #TVAR_afit_pBaseVals[460]
s5k5caga_i2c_write(0x0F12, 0x0A07);  ////0A00 // #TVAR_afit_pBaseVals[461]
s5k5caga_i2c_write(0x0F12, 0x379A);  // #TVAR_afit_pBaseVals[462]
s5k5caga_i2c_write(0x0F12, 0x1028);  // #TVAR_afit_pBaseVals[463]
s5k5caga_i2c_write(0x0F12, 0x0001);  // #TVAR_afit_pBaseVals[464]
s5k5caga_i2c_write(0x0F12, 0x00FF);  // #TVAR_afit_pBaseVals[465]
s5k5caga_i2c_write(0x0F12, 0x1002);  // #TVAR_afit_pBaseVals[466]
s5k5caga_i2c_write(0x0F12, 0x001E);  // #TVAR_afit_pBaseVals[467]
s5k5caga_i2c_write(0x0F12, 0x0900);  // #TVAR_afit_pBaseVals[468]
s5k5caga_i2c_write(0x0F12, 0x0600);  // #TVAR_afit_pBaseVals[469]
s5k5caga_i2c_write(0x0F12, 0x0504);  // #TVAR_afit_pBaseVals[470]
s5k5caga_i2c_write(0x0F12, 0x0307);  // #TVAR_afit_pBaseVals[471] //*<BU5D05950 zhangsheng 20100422 begin*/
s5k5caga_i2c_write(0x0F12, 0x6401);  // #TVAR_afit_pBaseVals[472] //*BU5D05950 zhangsheng 20100422 end>*/ 
s5k5caga_i2c_write(0x0F12, 0x1980);  // #TVAR_afit_pBaseVals[473]
s5k5caga_i2c_write(0x0F12, 0x0080);  // #TVAR_afit_pBaseVals[474]
s5k5caga_i2c_write(0x0F12, 0x0080);  // #TVAR_afit_pBaseVals[475]
s5k5caga_i2c_write(0x0F12, 0x5050);  // #TVAR_afit_pBaseVals[476]
s5k5caga_i2c_write(0x0F12, 0x0101);  // #TVAR_afit_pBaseVals[477]
s5k5caga_i2c_write(0x0F12, 0x1C01);  // #TVAR_afit_pBaseVals[478]
s5k5caga_i2c_write(0x0F12, 0x191C);  // #TVAR_afit_pBaseVals[479]
s5k5caga_i2c_write(0x0F12, 0x210C);  // #TVAR_afit_pBaseVals[480]
s5k5caga_i2c_write(0x0F12, 0x0A00);  // #TVAR_afit_pBaseVals[481]
s5k5caga_i2c_write(0x0F12, 0x1E04);  // #TVAR_afit_pBaseVals[482]
s5k5caga_i2c_write(0x0F12, 0x0A08);  // #TVAR_afit_pBaseVals[483]
s5k5caga_i2c_write(0x0F12, 0x070C);  // #TVAR_afit_pBaseVals[484]
s5k5caga_i2c_write(0x0F12, 0x3264);  // #TVAR_afit_pBaseVals[485]
s5k5caga_i2c_write(0x0F12, 0x7E02);  // #TVAR_afit_pBaseVals[486]
s5k5caga_i2c_write(0x0F12, 0x104A);  // #TVAR_afit_pBaseVals[487]
s5k5caga_i2c_write(0x0F12, 0x4012);  // #TVAR_afit_pBaseVals[488]
s5k5caga_i2c_write(0x0F12, 0x0604);  // #TVAR_afit_pBaseVals[489]
s5k5caga_i2c_write(0x0F12, 0x4606);  // #TVAR_afit_pBaseVals[490]
s5k5caga_i2c_write(0x0F12, 0x0146);  // #TVAR_afit_pBaseVals[491]
s5k5caga_i2c_write(0x0F12, 0x0101);  // #TVAR_afit_pBaseVals[492]
s5k5caga_i2c_write(0x0F12, 0x1C18);  // #TVAR_afit_pBaseVals[493]
s5k5caga_i2c_write(0x0F12, 0x1819);  // #TVAR_afit_pBaseVals[494]
s5k5caga_i2c_write(0x0F12, 0x0028);  // #TVAR_afit_pBaseVals[495]
s5k5caga_i2c_write(0x0F12, 0x030A);  // #TVAR_afit_pBaseVals[496]
s5k5caga_i2c_write(0x0F12, 0x0514);  // #TVAR_afit_pBaseVals[497]
s5k5caga_i2c_write(0x0F12, 0x0C14);  // #TVAR_afit_pBaseVals[498]
s5k5caga_i2c_write(0x0F12, 0xFF05);  // #TVAR_afit_pBaseVals[499]
s5k5caga_i2c_write(0x0F12, 0x0432);  // #TVAR_afit_pBaseVals[500]
s5k5caga_i2c_write(0x0F12, 0x547D);  // #TVAR_afit_pBaseVals[501]
s5k5caga_i2c_write(0x0F12, 0x1514);  // #TVAR_afit_pBaseVals[502]
s5k5caga_i2c_write(0x0F12, 0x0440);  // #TVAR_afit_pBaseVals[503]
s5k5caga_i2c_write(0x0F12, 0x0302);  // #TVAR_afit_pBaseVals[504]
s5k5caga_i2c_write(0x0F12, 0x4646);  // #TVAR_afit_pBaseVals[505]
s5k5caga_i2c_write(0x0F12, 0x0101);  // #TVAR_afit_pBaseVals[506]
s5k5caga_i2c_write(0x0F12, 0x1801);  // #TVAR_afit_pBaseVals[507]
s5k5caga_i2c_write(0x0F12, 0x191C);  // #TVAR_afit_pBaseVals[508]
s5k5caga_i2c_write(0x0F12, 0x2818);  // #TVAR_afit_pBaseVals[509]
s5k5caga_i2c_write(0x0F12, 0x0A00);  // #TVAR_afit_pBaseVals[510]
s5k5caga_i2c_write(0x0F12, 0x1403);  // #TVAR_afit_pBaseVals[511]
s5k5caga_i2c_write(0x0F12, 0x1405);  // #TVAR_afit_pBaseVals[512]
s5k5caga_i2c_write(0x0F12, 0x050C);  // #TVAR_afit_pBaseVals[513]
s5k5caga_i2c_write(0x0F12, 0x32FF);  // #TVAR_afit_pBaseVals[514]
s5k5caga_i2c_write(0x0F12, 0x7D04);  // #TVAR_afit_pBaseVals[515]
s5k5caga_i2c_write(0x0F12, 0x145E);  // #TVAR_afit_pBaseVals[516]
s5k5caga_i2c_write(0x0F12, 0x4015);  // #TVAR_afit_pBaseVals[517]
s5k5caga_i2c_write(0x0F12, 0x0204);  // #TVAR_afit_pBaseVals[518]
s5k5caga_i2c_write(0x0F12, 0x0003);  // #TVAR_afit_pBaseVals[519]
// AFIT table (Constants)
s5k5caga_i2c_write(0x0F12, 0x7F7A); // #afit_pConstBaseVals[0]
s5k5caga_i2c_write(0x0F12, 0x7F9D); // #afit_pConstBaseVals[1]
s5k5caga_i2c_write(0x0F12, 0xBEFC); // #afit_pConstBaseVals[2]
s5k5caga_i2c_write(0x0F12, 0xF7BC); // #afit_pConstBaseVals[3]
s5k5caga_i2c_write(0x0F12, 0x7E06); // #afit_pConstBaseVals[4]
s5k5caga_i2c_write(0x0F12, 0x0053); // #afit_pConstBaseVals[5]
// Update Changed Registers
s5k5caga_i2c_write(0x002A, 0x0664);
s5k5caga_i2c_write(0x0F12, 0x013E); //seti_uContrastCenter

s5k5caga_i2c_write(0x002A, 0x04D6);
s5k5caga_i2c_write(0x0F12, 0x0001); // #REG_TC_DBG_ReInitCmd
s5k5caga_i2c_write(0x0028, 0xD000);
s5k5caga_i2c_write(0x002A, 0x1102);
s5k5caga_i2c_write(0x0F12, 0x00C0); // Use T&P index 22 and 23
s5k5caga_i2c_write(0x002A, 0x113C);
s5k5caga_i2c_write(0x0F12, 0x267C); // Trap 22 address 0x71aa
s5k5caga_i2c_write(0x0F12, 0x2680); // Trap 23 address 0x71b4
s5k5caga_i2c_write(0x002A, 0x1142); 
s5k5caga_i2c_write(0x0F12, 0x00C0); // Trap Up Set (trap Addr are > 0x10000) 
s5k5caga_i2c_write(0x002A, 0x117C); 
s5k5caga_i2c_write(0x0F12, 0x2CE8); // Patch 22 address (TrapAndPatchOpCodes array index 22) 
s5k5caga_i2c_write(0x0F12, 0x2CeC); // Patch 23 address (TrapAndPatchOpCodes array index 23) 
// Fill RAM with alternative op-codes
s5k5caga_i2c_write(0x0028, 0x7000); // start add MSW
s5k5caga_i2c_write(0x002A, 0x2CE8); // start add LSW
s5k5caga_i2c_write(0x0F12, 0x0007); // Modify LSB to control AWBB_YThreshLow
s5k5caga_i2c_write(0x0F12, 0x00e2); // 
s5k5caga_i2c_write(0x0F12, 0x0005); // Modify LSB to control AWBB_YThreshLowBrLow
s5k5caga_i2c_write(0x0F12, 0x00e2); // 
// Update T&P tuning parameters
s5k5caga_i2c_write(0x002A, 0x337A);
s5k5caga_i2c_write(0x0F12, 0x0006); // #Tune_TP_atop_dbus_reg // 6 is the default HW value
//============================================================
// Frame rate setting 
//============================================================
// How to set
// 1. Exposure value
// dec2hex((1 / (frame rate you want(ms))) * 100d * 4d)
// 2. Analog Digital gain
// dec2hex((Analog gain you want) * 256d)
//============================================================
// Set preview exposure time
s5k5caga_i2c_write(0x002A, 0x0530);
s5k5caga_i2c_write(0x002A, 0x0530);//lt_uMaxExp1//   
s5k5caga_i2c_write(0x0F12, 0x3415);        
s5k5caga_i2c_write(0x002A, 0x0534);//lt_uMaxExp2//   
s5k5caga_i2c_write(0x0F12, 0x682A);        
s5k5caga_i2c_write(0x002A, 0x167C);//lt_uMaxExp3//   
s5k5caga_i2c_write(0x0F12, 0x8235);        
s5k5caga_i2c_write(0x002A, 0x1680);//lt_uMaxExp4//   
s5k5caga_i2c_write(0x0F12, 0xc350);        
s5k5caga_i2c_write(0x0F12, 0x0000);        

s5k5caga_i2c_write(0x002A, 0x0538);//It_uCapMaxExp1 // 
s5k5caga_i2c_write(0x0F12, 0x3415);        
s5k5caga_i2c_write(0x002A, 0x053C);//It_uCapMaxExp2 // 
s5k5caga_i2c_write(0x0F12, 0x682A);        
s5k5caga_i2c_write(0x002A, 0x1684);//It_uCapMaxExp3 // 
s5k5caga_i2c_write(0x0F12, 0x8235);        
s5k5caga_i2c_write(0x002A, 0x1688);//It_uCapMaxExp4 // 
s5k5caga_i2c_write(0x0F12, 0xc350);        
s5k5caga_i2c_write(0x0F12, 0x0000); 

// Set gain
s5k5caga_i2c_write(0x002A, 0x0540);                     
s5k5caga_i2c_write(0x0F12, 0x0150);//lt_uMaxAnGain1_700lux//                
s5k5caga_i2c_write(0x0F12, 0x0190);//lt_uMaxAnGain2_400lux//          
s5k5caga_i2c_write(0x002A, 0x168C);                     
s5k5caga_i2c_write(0x0F12, 0x0250);//MaxAnGain3_200lux//             
s5k5caga_i2c_write(0x0F12, 0x0600);//MaxAnGain4 //

s5k5caga_i2c_write(0x002A, 0x0544);        
s5k5caga_i2c_write(0x0F12, 0x0100);//It_uMaxDigGain // 
s5k5caga_i2c_write(0x0F12, 0x8000); //Max Gain 8 // 
s5k5caga_i2c_write(0x002A, 0x1694);
s5k5caga_i2c_write(0x0F12, 0x0001); // #evt1_senHal_bExpandForbid
s5k5caga_i2c_write(0x002A, 0x051A);
s5k5caga_i2c_write(0x0F12, 0x0111); // #lt_uLimitHigh 
s5k5caga_i2c_write(0x0F12, 0x00F0); // #lt_uLimitLow

s5k5caga_i2c_write(0x002A, 0x3286);
s5k5caga_i2c_write(0x0F12, 0x0001);  //Pre/Post gamma on(¿øº¹) 

s5k5caga_i2c_write(0x002A, 0x020E);                                       
s5k5caga_i2c_write(0x0F12, 0x000F); //Contr 
//================================================================================================
// How to set
// 1. MCLK
// 2. System CLK
// 3. PCLK
//================================================================================================
//clk Settings
s5k5caga_i2c_write(0x002A, 0x01CC);     
s5k5caga_i2c_write(0x0F12, 0x5DC0);    //REG_TC_IPRM_InClockLSBs//input clock=24MHz          
s5k5caga_i2c_write(0x0F12, 0x0000);    //REG_TC_IPRM_InClockMSBs  
s5k5caga_i2c_write(0x002A, 0x01EE);   
s5k5caga_i2c_write(0x0F12, 0x0003);    //REG_TC_IPRM_UseNPviClocks         

s5k5caga_i2c_write(0x002A, 0x01F6);   
s5k5caga_i2c_write(0x0F12, 0x2710); //1F40 //#REG_TC_IPRM_OpClk4KHz_0		//1st system CLK 32MHz
s5k5caga_i2c_write(0x0F12, 0x2EA0); //3A88 //#REG_TC_IPRM_MinOutRate4KHz_0	//PVI clock 60MHz
s5k5caga_i2c_write(0x0F12, 0x2EE0); //3AA8 //#REG_TC_IPRM_MaxOutRate4KHz_0          

s5k5caga_i2c_write(0x0F12, 0x38A4);    //REG_TC_IPRM_OpClk4KHz_1     
s5k5caga_i2c_write(0x0F12, 0x589E);    //REG_TC_IPRM_MinOutRate4KHz_1 2   700001FE 
s5k5caga_i2c_write(0x0F12, 0x591E);    //REG_TC_IPRM_MaxOutRate4KHz_1 2   70000200 

s5k5caga_i2c_write(0x0F12, 0x38A4);   //REG_TC_IPRM_OpClk4KHz_2      2   70000202 
s5k5caga_i2c_write(0x0F12, 0x5d80);   //REG_TC_IPRM_MinOutRate4KHz_2 2   70000204 
s5k5caga_i2c_write(0x0F12, 0x5e00);   //REG_TC_IPRM_MaxOutRate4KHz_2 2   70000206 

s5k5caga_i2c_write(0x002A, 0x0208);         
s5k5caga_i2c_write(0x0F12, 0x0001);      //REG_TC_IPRM_InitParamsUpdated  

//Auto Flicker 60Hz Start
s5k5caga_i2c_write(0x0028, 0x7000);
s5k5caga_i2c_write(0x002A, 0x0C18); //#AFC_Default60Hz
s5k5caga_i2c_write(0x0F12, 0x0000); // #AFC_Default60Hz  1: Auto Flicker 60Hz start 0: Auto Flicker 50Hz start
s5k5caga_i2c_write(0x002A, 0x04D2); // #REG_TC_DBG_AutoAlgEnBits
s5k5caga_i2c_write(0x0F12, 0x067F);

s5k5caga_i2c_write(0x002A, 0x020E);       
s5k5caga_i2c_write(0x0F12, 0x0015);            //Contr

s5k5caga_i2c_write(0x002A, 0x0452);
s5k5caga_i2c_write(0x0F12, 0x0055);   //REG_TC_BRC_usPrevQuality //Best Quality : 85d; Good Quality : 50d;Poor Quality :20d  
s5k5caga_i2c_write(0x0F12, 0x0055);   //REG_TC_BRC_usCaptureQuality //Best Quality : 85d; Good Quality : 50d;Poor Quality :20d


//================================================================================================
// SET PREVIEW CONFIGURATION_0
// # Foramt: YUV422
// # Size:   800X600
// # FPS:    10-30fps(using normal_mode preview)
//================================================================================================
s5k5caga_i2c_write(0x002A, 0x026C);
s5k5caga_i2c_write(0x0F12, 800);   //REG_0TC_PCFG_usWidth        640
s5k5caga_i2c_write(0x0F12, 600);   //REG_0TC_PCFG_usHeight       480 2   7000026E    //             
s5k5caga_i2c_write(0x0F12, 0x0005);   //REG_0TC_PCFG_Format          2   70000270    //             
s5k5caga_i2c_write(0x0F12, 0x2EE0);//591E   //REG_0TC_PCFG_usMaxOut4KHzRate             
s5k5caga_i2c_write(0x0F12, 0x2EA0);//589E   //REG_0TC_PCFG_usMinOut4KHzRate             
s5k5caga_i2c_write(0x0F12, 0x0100);   //REG_0TC_PCFG_OutClkPerPix88  2   70000276    //             
s5k5caga_i2c_write(0x0F12, 0x0800);   //REG_0TC_PCFGew_uMaxBpp88       2   70000278    //             
s5k5caga_i2c_write(0x0F12, 0x0052);   //REG_0TC_PCFG_PVIMask         2   7000027A    //42 YUYV  
s5k5caga_i2c_write(0x0F12, 0x0010);   //REG_0TC_PCFG_OIFMask         2   7000027C    //             
s5k5caga_i2c_write(0x0F12, 0x01E0);   //REG_0TC_PCFG_usJpegPacketSize              2   7000027E    //             
s5k5caga_i2c_write(0x0F12, 0x0000);   //REG_0TC_PCFG_usJpegTotalPackets            2   70000280    //             
s5k5caga_i2c_write(0x0F12, 0x0000);   //REG_0TC_PCFG_uClockInd       2   70000282    //             
s5k5caga_i2c_write(0x0F12, 0x0000);   //REG_0TC_PCFG_usFrTimeType    2   70000284    //             
s5k5caga_i2c_write(0x0F12, 0x0001);   //REG_0TC_PCFG_FrRateQualityType             2   70000286    //             
s5k5caga_i2c_write(0x0F12, 0x03E8);   //REG_0TC_PCFG_usMaxFrTimeMsecMult10         2   70000288    //             
s5k5caga_i2c_write(0x0F12, 0x014D);   //REG_0TC_PCFG_usMinFrTimeMsecMult10         2   7000028A    //             
s5k5caga_i2c_write(0x0F12, 0x0000);   //REG_0TC_PCFG_bSmearOutput    2   7000028C    //             
s5k5caga_i2c_write(0x0F12, 0x0000);   //REG_0TC_PCFG_sSaturation     2   7000028E    //             
s5k5caga_i2c_write(0x0F12, 0x0000);   //REG_0TC_PCFG_sSharpBlur      2   70000290    //             
s5k5caga_i2c_write(0x0F12, 0x0000);   //REG_0TC_PCFG_sColorTemp      2   70000292    //             
s5k5caga_i2c_write(0x0F12, 0x0000);   //REG_0TC_PCFG_uDeviceGammaIndex             2   70000294    //             
s5k5caga_i2c_write(0x0F12, 0x0000);   //REG_0TC_PCFG_uPrevMirror     2   70000296    //             
s5k5caga_i2c_write(0x0F12, 0x0000);   //REG_0TC_PCFG_uCaptureMirror  2   70000298    //             
s5k5caga_i2c_write(0x0F12, 0x0000);   //REG_0TC_PCFG_uRotation       2   7000029A    //          

//================================================================================================
// SET PREVIEW CONFIGURATION_1
// # Foramt : YUV422
// # Size:    640*480
// # FPS :    15fps
//================================================================================================
s5k5caga_i2c_write(0x002A, 0x029C);
s5k5caga_i2c_write(0x0F12, 0x0280);   //REG_0TC_PCFG_usWidth        
s5k5caga_i2c_write(0x0F12, 0x01E0);   //REG_0TC_PCFG_usHeight        2   7000026E    //             
s5k5caga_i2c_write(0x0F12, 0x0005);   //REG_0TC_PCFG_Format          2   70000270    //             
s5k5caga_i2c_write(0x0F12, 0x591E);  //REG_0TC_PCFG_usMaxOut4KHzRate             
s5k5caga_i2c_write(0x0F12, 0x589E);  //REG_0TC_PCFG_usMinOut4KHzRate             
s5k5caga_i2c_write(0x0F12, 0x0100);   //REG_0TC_PCFG_OutClkPerPix88  2   70000276    //             
s5k5caga_i2c_write(0x0F12, 0x0800);   //REG_0TC_PCFG_uMaxBpp88       2   70000278    //       
s5k5caga_i2c_write(0x0F12, 0x0052);   //REG_0TC_PCFG_PVIMask         2   7000027A    //42 YUYV         
s5k5caga_i2c_write(0x0F12, 0x0010);   //REG_0TC_PCFG_OIFMask         2   7000027C    //             
s5k5caga_i2c_write(0x0F12, 0x01E0);   //REG_0TC_PCFG_usJpegPacketSize              2   7000027E    //             
s5k5caga_i2c_write(0x0F12, 0x0000);   //REG_0TC_PCFG_usJpegTotalPackets            2   70000280    //             
s5k5caga_i2c_write(0x0F12, 0x0001);   //REG_0TC_PCFG_uClockInd       2   70000282    //             
s5k5caga_i2c_write(0x0F12, 0x0000);   //REG_0TC_PCFG_usFrTimeType    2   70000284    //             
s5k5caga_i2c_write(0x0F12, 0x0001);   //REG_0TC_PCFG_FrRateQualityType             2   70000286    //             
s5k5caga_i2c_write(0x0F12, 0x029A);   //REG_0TC_PCFG_usMaxFrTimeMsecMult10         2   70000288    //             
s5k5caga_i2c_write(0x0F12, 0x029A);   //REG_0TC_PCFG_usMinFrTimeMsecMult10         2   7000028A    //             
s5k5caga_i2c_write(0x0F12, 0x0000);   //REG_0TC_PCFG_bSmearOutput    2   7000028C    //             
s5k5caga_i2c_write(0x0F12, 0x0000);   //REG_0TC_PCFG_sSaturation     2   7000028E    //             
s5k5caga_i2c_write(0x0F12, 0x0000);   //REG_0TC_PCFG_sSharpBlur      2   70000290    //             
s5k5caga_i2c_write(0x0F12, 0x0000);   //REG_0TC_PCFG_sColorTemp      2   70000292    //             
s5k5caga_i2c_write(0x0F12, 0x0000);   //REG_0TC_PCFG_uDeviceGammaIndex       2   70000294    //             
s5k5caga_i2c_write(0x0F12, 0x0000);   //REG_0TC_PCFG_uPrevMirror     2   70000296    //             
s5k5caga_i2c_write(0x0F12, 0x0000);   //REG_0TC_PCFG_uCaptureMirror  2   70000298    //             
s5k5caga_i2c_write(0x0F12, 0x0000);   //REG_0TC_PCFG_uRotation       2   7000029A    //     


//================================================================================================
// APPLY PREVIEW CONFIGURATION & RUN PREVIEW
//================================================================================================
s5k5caga_i2c_write(0x002A, 0x023C); 
s5k5caga_i2c_write(0x0F12, 0x0000);  // #REG_TC_GP_ActivePrevConfig // Select preview configuration_0
s5k5caga_i2c_write(0x002A, 0x0240); 
s5k5caga_i2c_write(0x0F12, 0x0001);  // #REG_TC_GP_PrevOpenAfterChange
s5k5caga_i2c_write(0x002A, 0x0230); 
s5k5caga_i2c_write(0x0F12, 0x0001);  // #REG_TC_GP_NewConfigSync // Update preview configuration
s5k5caga_i2c_write(0x002A, 0x023E); 
s5k5caga_i2c_write(0x0F12, 0x0001);  // #REG_TC_GP_PrevConfigChanged
s5k5caga_i2c_write(0x002A, 0x0220); 
s5k5caga_i2c_write(0x0F12, 0x0001);  // #REG_TC_GP_EnablePreview // Start preview
s5k5caga_i2c_write(0x0F12, 0x0001);  // #REG_TC_GP_EnablePreviewChanged

//================================================================================================      
// SET Capture CONFIGURATION_0    
// # Foramt : YUV
// # Size:    3M             
// # FPS :    5fps     
//================================================================================================  
s5k5caga_i2c_write(0x002A, 0x035C);   //Normal capture //     
s5k5caga_i2c_write(0x0F12, 0x0000);   //REG_0TC_CCFG_uCaptureMode    2   7000035C
s5k5caga_i2c_write(0x0F12, 0x0800);   //REG_0TC_CCFG_usWidth         2   7000035E          //       
s5k5caga_i2c_write(0x0F12, 0x0600);   //REG_0TC_CCFG_usHeight        2   70000360          //       
s5k5caga_i2c_write(0x0F12, 0x0005);   //REG_0TC_CCFG_Format          2   70000362          //       
s5k5caga_i2c_write(0x0F12, 0x591E);   //REG_0TC_CCFG_usMaxOut4KHzRate             
s5k5caga_i2c_write(0x0F12, 0x589E);   //REG_0TC_CCFG_usMinOut4KHzRate             
s5k5caga_i2c_write(0x0F12, 0x0100);   //REG_0TC_CCFG_OutClkPerPix88  2   70000368          //       
s5k5caga_i2c_write(0x0F12, 0x0800);   //REG_0TC_CCFG_uMaxBpp88       2   7000036A          //       
s5k5caga_i2c_write(0x0F12, 0x0052);   //REG_0TC_CCFG_PVIMask         2   7000036C          //    42 YUYV   
s5k5caga_i2c_write(0x0F12, 0x0000);   //REG_0TC_CCFG_OIFMask         2   7000036E          //       
s5k5caga_i2c_write(0x0F12, 0x01E0);   //REG_0TC_CCFG_usJpegPacketSize              2   70000370          //       
s5k5caga_i2c_write(0x0F12, 0x0000);   //REG_0TC_CCFG_usJpegTotalPackets            2   70000372          //       
s5k5caga_i2c_write(0x0F12, 0x0001);   //REG_0TC_CCFG_uClockInd       2   70000374          //       
s5k5caga_i2c_write(0x0F12, 0x0000);   //REG_0TC_CCFG_usFrTimeType    2   70000376          //       
s5k5caga_i2c_write(0x0F12, 0x0002);   //REG_0TC_CCFG_FrRateQualityType             2   70000378          //       
s5k5caga_i2c_write(0x0F12, 0x07D0);   //REG_0TC_CCFG_usMaxFrTimeMsecMult10         2   7000037A          //       
s5k5caga_i2c_write(0x0F12, 0x07D0);   //REG_0TC_CCFG_usMinFrTimeMsecMult10         2   7000037C          //       
s5k5caga_i2c_write(0x0F12, 0x0000);   //REG_0TC_CCFG_bSmearOutput    2   7000037E          //       
s5k5caga_i2c_write(0x0F12, 0x0000);   //REG_0TC_CCFG_sSaturation     2   70000380          //       
s5k5caga_i2c_write(0x0F12, 0x0000);   //REG_0TC_CCFG_sSharpBlur      2   70000382          //       
s5k5caga_i2c_write(0x0F12, 0x0000);   //REG_0TC_CCFG_sColorTemp      2   70000384          //       
s5k5caga_i2c_write(0x0F12, 0x0000);   //REG_0TC_CCFG_uDeviceGammaIndex             2   70000386          //            
//================================================================================================      
// SET Capture CONFIGURATION_1    
// # Foramt : YUV
// # Size:    2M             
// # FPS :    5fps     
//================================================================================================  
s5k5caga_i2c_write(0x002A, 0x0388);   //Normal capture //     
s5k5caga_i2c_write(0x0F12, 0x0000);   //REG_0TC_CCFG_uCaptureMode    2   7000035C
/*Add <BEGIN> by NTD.YuBin, 2012/8/9, bug 315049, change picture size from 2M to 3M*/
s5k5caga_i2c_write(0x0F12, 0x0800);   //REG_0TC_CCFG_usWidth         2   7000035E          //       
s5k5caga_i2c_write(0x0F12, 0x0600);   //REG_0TC_CCFG_usHeight        2   70000360          //       
/*Add <END> by NTD.YuBin*/
s5k5caga_i2c_write(0x0F12, 0x0005);   //REG_0TC_CCFG_Format          2   70000362          //       
s5k5caga_i2c_write(0x0F12, 0x2ee0);   //REG_0TC_CCFG_usMaxOut4KHzRate             
s5k5caga_i2c_write(0x0F12, 0x2ea0);   //REG_0TC_CCFG_usMinOut4KHzRate             
s5k5caga_i2c_write(0x0F12, 0x0100);   //REG_0TC_CCFG_OutClkPerPix88  2   70000368          //       
s5k5caga_i2c_write(0x0F12, 0x0800);   //REG_0TC_CCFG_uMaxBpp88       2   7000036A          //       
s5k5caga_i2c_write(0x0F12, 0x0052);   //REG_0TC_CCFG_PVIMask         2   7000036C          //  42 YUYV     
s5k5caga_i2c_write(0x0F12, 0x0010);   //REG_0TC_CCFG_OIFMask         2   7000036E          //       
s5k5caga_i2c_write(0x0F12, 0x01E0);   //REG_0TC_CCFG_usJpegPacketSize              2   70000370          //       
s5k5caga_i2c_write(0x0F12, 0x0000);   //REG_0TC_CCFG_usJpegTotalPackets            2   70000372          //       
s5k5caga_i2c_write(0x0F12, 0x0000);   //REG_0TC_CCFG_uClockInd       2   70000374          //       
s5k5caga_i2c_write(0x0F12, 0x0000);   //REG_0TC_CCFG_usFrTimeType    2   70000376          //       
s5k5caga_i2c_write(0x0F12, 0x0002);   //REG_0TC_CCFG_FrRateQualityType             2   70000378          //       
s5k5caga_i2c_write(0x0F12, 0x07D0);   //REG_0TC_CCFG_usMaxFrTimeMsecMult10         2   7000037A          //       
s5k5caga_i2c_write(0x0F12, 0x07D0);   //REG_0TC_CCFG_usMinFrTimeMsecMult10         2   7000037C          //       
s5k5caga_i2c_write(0x0F12, 0x0000);   //REG_0TC_CCFG_bSmearOutput    2   7000037E          //       
s5k5caga_i2c_write(0x0F12, 0x0000);   //REG_0TC_CCFG_sSaturation     2   70000380          //       
s5k5caga_i2c_write(0x0F12, 0x0000);   //REG_0TC_CCFG_sSharpBlur      2   70000382          //       
s5k5caga_i2c_write(0x0F12, 0x0000);   //REG_0TC_CCFG_sColorTemp      2   70000384          //       
s5k5caga_i2c_write(0x0F12, 0x0000);   //REG_0TC_CCFG_uDeviceGammaIndex             2   70000386          //         

//================================================================================================      
// SET Capture CONFIGURATION_2   
// # Foramt : YUV
// # Size:    1.3M             
// # FPS :   5fps      
//================================================================================================  
s5k5caga_i2c_write(0x002A, 0x03B4);   //Normal capture //     
s5k5caga_i2c_write(0x0F12, 0x0000);   //REG_0TC_CCFG_uCaptureMode    2   7000035C
s5k5caga_i2c_write(0x0F12, 0x0500);   //REG_0TC_CCFG_usWidth         2   7000035E          //       
s5k5caga_i2c_write(0x0F12, 0x0400);   //REG_0TC_CCFG_usHeight        2   70000360          //       
s5k5caga_i2c_write(0x0F12, 0x0005);   //REG_0TC_CCFG_Format          2   70000362          //       
s5k5caga_i2c_write(0x0F12, 0x32F8);   //REG_0TC_CCFG_usMaxOut4KHzRate             
s5k5caga_i2c_write(0x0F12, 0x3298);   //REG_0TC_CCFG_usMinOut4KHzRate             
s5k5caga_i2c_write(0x0F12, 0x0100);   //REG_0TC_CCFG_OutClkPerPix88  2   70000368          //       
s5k5caga_i2c_write(0x0F12, 0x0800);   //REG_0TC_CCFG_uMaxBpp88       2   7000036A          //       
s5k5caga_i2c_write(0x0F12, 0x0052);   //REG_0TC_CCFG_PVIMask         2   7000036C          //  42 YUYV     
s5k5caga_i2c_write(0x0F12, 0x0010);   //REG_0TC_CCFG_OIFMask         2   7000036E          //       
s5k5caga_i2c_write(0x0F12, 0x01E0);   //REG_0TC_CCFG_usJpegPacketSize              2   70000370          //       
s5k5caga_i2c_write(0x0F12, 0x0000);   //REG_0TC_CCFG_usJpegTotalPackets            2   70000372          //       
s5k5caga_i2c_write(0x0F12, 0x0000);   //REG_0TC_CCFG_uClockInd       2   70000374          //       
s5k5caga_i2c_write(0x0F12, 0x0000);   //REG_0TC_CCFG_usFrTimeType    2   70000376          //       
s5k5caga_i2c_write(0x0F12, 0x0002);   //REG_0TC_CCFG_FrRateQualityType             2   70000378          //       
s5k5caga_i2c_write(0x0F12, 0x07D0);   //REG_0TC_CCFG_usMaxFrTimeMsecMult10         2   7000037A          //       
s5k5caga_i2c_write(0x0F12, 0x07D0);   //REG_0TC_CCFG_usMinFrTimeMsecMult10         2   7000037C          //       
s5k5caga_i2c_write(0x0F12, 0x0000);   //REG_0TC_CCFG_bSmearOutput    2   7000037E          //       
s5k5caga_i2c_write(0x0F12, 0x0000);   //REG_0TC_CCFG_sSaturation     2   70000380          //       
s5k5caga_i2c_write(0x0F12, 0x0000);   //REG_0TC_CCFG_sSharpBlur      2   70000382          //       
s5k5caga_i2c_write(0x0F12, 0x0000);   //REG_0TC_CCFG_sColorTemp      2   70000384          //       
s5k5caga_i2c_write(0x0F12, 0x0000);   //REG_0TC_CCFG_uDeviceGammaIndex             2   70000386          //      

//================================================================================================      
// SET Capture CONFIGURATION_3  
// # Foramt : YUV
// # Size:    VGA             
// # FPS :    5fps      
//================================================================================================  
s5k5caga_i2c_write(0x002A, 0x03E0);   //Normal capture //     
s5k5caga_i2c_write(0x0F12, 0x0000);   //REG_0TC_CCFG_uCaptureMode    2   7000035C
s5k5caga_i2c_write(0x0F12, 0x0280);   //REG_0TC_CCFG_usWidth         2   7000035E          //       
s5k5caga_i2c_write(0x0F12, 0x01E0);   //REG_0TC_CCFG_usHeight        2   70000360          //       
s5k5caga_i2c_write(0x0F12, 0x0005);   //REG_0TC_CCFG_Format          2   70000362          //       
s5k5caga_i2c_write(0x0F12, 0x32F8);   //REG_0TC_CCFG_usMaxOut4KHzRate             
s5k5caga_i2c_write(0x0F12, 0x3298);   //REG_0TC_CCFG_usMinOut4KHzRate             
s5k5caga_i2c_write(0x0F12, 0x0100);   //REG_0TC_CCFG_OutClkPerPix88  2   70000368          //       
s5k5caga_i2c_write(0x0F12, 0x0800);   //REG_0TC_CCFG_uMaxBpp88       2   7000036A          //       
s5k5caga_i2c_write(0x0F12, 0x0052);   //REG_0TC_CCFG_PVIMask         2   7000036C          //  42 YUYV     
s5k5caga_i2c_write(0x0F12, 0x0010);   //REG_0TC_CCFG_OIFMask         2   7000036E          //       
s5k5caga_i2c_write(0x0F12, 0x01E0);   //REG_0TC_CCFG_usJpegPacketSize              2   70000370          //       
s5k5caga_i2c_write(0x0F12, 0x0000);   //REG_0TC_CCFG_usJpegTotalPackets            2   70000372          //       
s5k5caga_i2c_write(0x0F12, 0x0000);   //REG_0TC_CCFG_uClockInd       2   70000374          //       
s5k5caga_i2c_write(0x0F12, 0x0000);   //REG_0TC_CCFG_usFrTimeType    2   70000376          //       
s5k5caga_i2c_write(0x0F12, 0x0002);   //REG_0TC_CCFG_FrRateQualityType             2   70000378          //       
s5k5caga_i2c_write(0x0F12, 0x07D0);   //REG_0TC_CCFG_usMaxFrTimeMsecMult10         2   7000037A          //       
s5k5caga_i2c_write(0x0F12, 0x07D0);   //REG_0TC_CCFG_usMinFrTimeMsecMult10         2   7000037C          //       
s5k5caga_i2c_write(0x0F12, 0x0000);   //REG_0TC_CCFG_bSmearOutput    2   7000037E          //       
s5k5caga_i2c_write(0x0F12, 0x0000);   //REG_0TC_CCFG_sSaturation     2   70000380          //       
s5k5caga_i2c_write(0x0F12, 0x0000);   //REG_0TC_CCFG_sSharpBlur      2   70000382          //       
s5k5caga_i2c_write(0x0F12, 0x0000);   //REG_0TC_CCFG_sColorTemp      2   70000384          //       
s5k5caga_i2c_write(0x0F12, 0x0000);   //REG_0TC_CCFG_uDeviceGammaIndex             2   70000386          //      




s5k5caga_i2c_write(0x0028, 0xD000); 
s5k5caga_i2c_write(0x002A, 0x1000); 
s5k5caga_i2c_write(0x0F12, 0x0001);    //0TC_CCFG_sColorTemp      2   70000384          //       
s5k5caga_i2c_write(0x0F12, 0x0000);   //REG_0TC_CCFG_uDeviceGammaIndex             2   70000386          //      

CAMERADBG( " ======V04 : Poppy_5CA_Ini_V04_111209 is OK====== \n" );

}

