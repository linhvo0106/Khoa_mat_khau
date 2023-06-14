#define LCD_RS_PIN PIN_D0
#define LCD_RW_PIN PIN_D1
#define LCD_ENABLE_PIN PIN_D2
#define LCD_DATA4 PIN_D7
#define LCD_DATA5 PIN_D6
#define LCD_DATA6 PIN_D5
#define LCD_DATA7 PIN_D4

// dinh nghia ten cac phim bam
#define KEY0 0
#define KEY1 1
#define KEY2 2
#define KEY3 3
#define KEY4 4
#define KEY5 5
#define KEY6 6
#define KEY7 7
#define KEY8 8
#define KEY9 9
#define KEYSTAR 10
#define KEYHASH 11
#define NOKEY 12
#define KEYA 13
#define KEYB 14
#define KEYC 15
#define KEYD 16

#define KC1 PIN_B4
#define KC2 PIN_B5
#define KC3 PIN_B6
#define KC4 PIN_B7
#define KR1 PIN_B0
#define KR2 PIN_B1
#define KR3 PIN_B2
#define KR4 PIN_B3

// khai bao dinh nghia cac trang thai mach
#define STAMAIN 0
#define STAINPUTPASS 1
#define STAUNLOCK 2
#define STACHANGEPASS 3
#define STAMENU1 4
#define STAADDRFID 5


#include <main.h>
#include <lcd.c>
#use rs232(UART1,baud=9600,parity=N,bits=8,stream=UART_PORT,errors)

int8 CurrentState=STAMAIN; // Bien luu trang thai cua mach hien tai
int8 NextState=STAMAIN; // Bien luu trang thai cua mach tiep theo


int8 CurrentKey; //bien luu gia tri phim bam
int8 CurrentCodeIndex; //vi tri so hien tai
int8 CurrentPass[6]; // bien luu ma hien tai

int8 AdminPass[6]={1,2,3,4,5,6};

int8 WrongPassCount=0;


void WritePassEEPROM()
{
    write_eeprom(0, AdminPass[0]);
    write_eeprom(1, AdminPass[1]);
    write_eeprom(2, AdminPass[2]);
    write_eeprom(3, AdminPass[3]);
    write_eeprom(4, AdminPass[4]);
    write_eeprom(5, AdminPass[5]);
}


void ReadPassfromEEPROM()
{

 int8 EEPROMData;
 // doc pass phu 1
 EEPROMData=read_eeprom(0);
 if(EEPROMData==0xFF) // ROM trang khong co du lieu
 {
   WritePassEEPROM();// ghi gia tri mac dinh vao eeprom
 }
 else // neu rom co du lieu thi doc  ma mat ma luu vao mang
 {

    AdminPass[0]=read_eeprom(0);
    AdminPass[1]=read_eeprom(1);
    AdminPass[2]=read_eeprom(2);
    AdminPass[3]=read_eeprom(3);
    AdminPass[4]=read_eeprom(4);
    AdminPass[5]=read_eeprom(5);
 }

}



int8 ScanKeyboard(void)
{
 output_low(KC1);
 if(!input(KR1)) 
 { 
  while(!input(KR1));
  delay_ms(10);
  return KEY1;
 }
 
 if(!input(KR2)) 
  { 
   while(!input(KR2));
   delay_ms(10);
   return KEY4;
  }
  
 if(!input(KR3))
 {
  while(!input(KR3));
  delay_ms(10);
  return KEY7;
 }
 
 if(!input(KR4)) 
  {
  while(!input(KR4));
  delay_ms(10);
  return KEYSTAR;
 }
 
 output_high(KC1);
 output_low(KC2);
 
 if(!input(KR1)) 
 { 
  while(!input(KR1));
  delay_ms(10);
  return KEY2;
 }
 
 if(!input(KR2)) 
  { 
   while(!input(KR2));
   delay_ms(10);
   return KEY5;
  }
  
 if(!input(KR3))
 {
  while(!input(KR3));
  delay_ms(10);
  return KEY8;
 }
 
 if(!input(KR4)) 
  {
  while(!input(KR4));
  delay_ms(10);
  return KEY0;
 }
 
 output_high(KC2);
 output_low(KC3);
 
 if(!input(KR1)) 
 { 
  while(!input(KR1));
  delay_ms(10);
  return KEY3;
 }
 
 if(!input(KR2)) 
  { 
   while(!input(KR2));
   delay_ms(10);
   return KEY6;
  }
  
 if(!input(KR3))
 {
  while(!input(KR3));
  delay_ms(10);
  return KEY9;
 }
 
 if(!input(KR4)) 
  {
  while(!input(KR4));
  delay_ms(10);
  return KEYHASH;
 }
 
 output_high(KC3);
 output_low(KC4);
 
 if(!input(KR1)) 
 { 
  while(!input(KR1));
  delay_ms(10);
  return KEYA;
 }
 
 if(!input(KR2)) 
  { 
   while(!input(KR2));
   delay_ms(10);
   return KEYB;
  }
  
 if(!input(KR3))
 {
  while(!input(KR3));
  delay_ms(10);
  return KEYC;
 }
 
 if(!input(KR4)) 
  {
  while(!input(KR4));
  delay_ms(10);
  return KEYD;
 }
 
 output_high(KC4);
 
 return NOKEY;

}

void OnBuzzer()
{
    output_high(PIN_C2);// Mo Loa
}

void OffBuzzer()
{
    output_Low(PIN_C2);// tat Loa
}


void Beep(int BeepCount) // phat tieng pip theo so lan
{
 int i;
 for(i=0;i<BeepCount;i++)
 {
    OnBuzzer();
    delay_ms(200);
    OffBuzzer();  
    delay_ms(200);
 }
}


void OnLedRed()
{
 output_High(PIN_C4);
}

void OffLedRed()
{
 output_Low(PIN_C4);
}


void OnLedGreen()
{
 output_High(PIN_C5);
}

void OffLedGreen()
{
 output_Low(PIN_C5);
}



int8 CheckPass() // ham kiem tra mat ma, ket qua tra ve 0 - mat ma sai, tra ve 1 mat ma dung
{
 
  int i;
  for(i=0;i<6;i++) // kiem tra pass add min
  {
   if(CurrentPass[i]!=AdminPass[i]) // neu phat hien 1 mat ma sai thi thoat vong lam
   {
     return 0;
   }
  }
  return 1;
  
}

void ResetInput() // ham reset du lieu dang nhap
{
  CurrentCodeIndex=0;
  CurrentPass[0]=0xFF;
  CurrentPass[1]=0xFF;
  CurrentPass[2]=0xFF;
  CurrentPass[3]=0xFF;
  CurrentPass[4]=0xFF;
  CurrentPass[5]=0xFF;
}

void DisplayMain()
{
   lcd_putc("\f");
   lcd_putc("   KHOA MAT MA  ");  
   lcd_gotoxy(1, 2);
   lcd_putc(" BAM A Doi Pass ");
}

void DisplayInputPass()
{
   lcd_putc("\f");
   lcd_putc("  NHAP MAT MA");  
   lcd_gotoxy(1, 2);

}



void DisplayChangePass()
{
   lcd_putc("\f");
   lcd_putc("NHAP PASS MOI");  
   lcd_gotoxy(1, 2);
}



void main()
{

   int8 CheckPassResult;
   int8 i;
   SET_TRIS_D(0x00);
   SET_TRIS_C(0x80); 
   Set_tris_B(0x0F);// cai dat port B 0-3 input, 4-7 output
   port_B_pullups(0xFF);   
   output_high(KC1);// dua dua cac chan cot phim len cao
   output_high(KC2);
   output_high(KC3);
   output_high(KC4);
   
   OffLedRed();
   OffLedGreen();
    //write_eeprom(0,0xFF); // reset pass
   
   lcd_init(); // khoi tao LCD
   DisplayMain();
   ResetInput();                   
   OffBuzzer();
   ReadPassfromEEPROM();
   CurrentState=STAMAIN;

   
   while(true)
   {

      
      // phan kim tra phim bam va mat ma
      CurrentKey=Scankeyboard(); // doc ban phim
      if(CurrentKey!=NOKEY) // neu phim tra ve khac NOKEY ( tuc la co bam phim)
      {
        switch(CurrentKey) // kiem tra xem la phim gi
        {
            case KEY0: // Neu la ca phim so
            case KEY1:
            case KEY2:
            case KEY3:
            case KEY4:
            case KEY5:
            case KEY6:
            case KEY7:
            case KEY8:
            case KEY9:
            {
               if(CurrentState==STAMAIN) // neu dang o che do cho hoac che do nhap pass thi cho phep nhap
               {
                 CurrentState=STAINPUTPASS; // cai trang tahi hien tai la nhap pass
                 NextState=STAUNLOCK;
                 DisplayInputPass();
               }
               else if(CurrentState==STAINPUTPASS||CurrentState==STACHANGEPASS)
               {
                 if(CurrentCodeIndex<6) //neu nhap chua du 6 ky tu
                 {
                    CurrentPass[CurrentCodeIndex]=CurrentKey; // luu so vua bam vao mang
                    lcd_putc('*'); // hien thi dau sao cho phim da bam
                    CurrentCodeIndex++;// chuyen sang vi tri tiep theo
                 }
               }
               break;
            }
            
            case KEYHASH:// Neu bam phim thang
            {
               if(CurrentState==STAINPUTPASS&&NextState==STAUNLOCK) // neu dang nhap pass de mo
              {
                CheckPassResult=CheckPass(); // kiem tra mat ma nhap vao
                if(CheckPassResult==0) //neu sai pass thi thong bao
                {
                   
                   
                   if(WrongPassCount<3) WrongPassCount++;
                   if(WrongPassCount==1)
                   {
                   
                      lcd_gotoxy(1, 2);
                      lcd_putc("SAI MAT MA!     ");
                      OnLedRed();
                      OnBuzzer();
                      delay_ms(3000);
                      ResetInput();  // xoa du lieu da nhap
                      OffLedRed();
                      OffBuzzer();
                      DisplayInputPass();
                      lcd_gotoxy(1, 2);
                   }
                   else
                   {
                      lcd_gotoxy(1, 2);
                      lcd_putc("SAI MAT MA!   s ");
                      OnBuzzer();
                      OnLedRed();
                      for(i=15;i>0;i--)
                      {
                       lcd_gotoxy(14, 2);
                       lcd_putc(i/10+48);
                       lcd_putc(i%10+48);
                       delay_ms(1000);
                      }
                      ResetInput();  // xoa du lieu da nhap
                      CurrentState=STAMAIN;
                      DisplayMain();
                      OffLedRed();
                      OffBuzzer();
                   }
                }
                else
                {
                   WrongPassCount=0; // xoa so lan sai
                   lcd_putc("\f"); // xoa man hinh
                   lcd_gotoxy(1, 1);
                   lcd_putc("DANG MO CUA:  s");
                   Beep(1); // beep loa 1 tieng
                   OnLedGreen();
                   for(i=10;i>0;i--)
                   {
                     lcd_gotoxy(13, 1);
                     lcd_putc(i/10+48);
                     lcd_putc(i%10+48);
                     delay_ms(1000);
                   }
                   ResetInput();  // xoa du lieu da nhap
                   CurrentState=STAMAIN;
                   DisplayMain();
                   OffLedGreen();
                }
              }
              else if(CurrentState==STAINPUTPASS&&NextState==STACHANGEPASS)
              {
                   CheckPassResult=CheckPass(); // kiem tra mat ma nhap vao
                   if(CheckPassResult==1) //neu nhap dung pass addmin
                   {
                      DisplayChangePass();
                      Beep(1); // beep loa 1 tieng
                      ResetInput();  // xoa du lieu da nhap
                      CurrentState=STACHANGEPASS; // tro ve man hinh change pass
                   }
                   else
                   {
            
                      if(WrongPassCount<3) WrongPassCount++;
                      if(WrongPassCount==1)
                      {
                      
                         lcd_gotoxy(1, 2);
                         lcd_putc("SAI MAT MA!     ");
                         OnLedRed();
                         OnBuzzer();
                         delay_ms(3000);
                         ResetInput();  // xoa du lieu da nhap
                         OffLedRed();
                         OffBuzzer();
                         DisplayInputPass();
                         lcd_gotoxy(1, 2);
                      }
                      else
                      {
                         lcd_gotoxy(1, 2);
                         lcd_putc("SAI MAT MA!   s ");
                         OnBuzzer();
                         OnLedRed();
                         for(i=15;i>0;i--)
                         {
                          lcd_gotoxy(14, 2);
                          lcd_putc(i/10+48);
                          lcd_putc(i%10+48);
                          delay_ms(1000);
                         }
                         ResetInput();  // xoa du lieu da nhap
                         CurrentState=STAMAIN;
                         DisplayMain();
                         OffLedRed();
                         OffBuzzer();
                      }
                   }  
              
              }
              else if(CurrentState==STACHANGEPASS)
              {
                 if(CurrentCodeIndex<6) //neu nhap chua du 6 ky tu
                 {
                      lcd_gotoxy(1, 2);
                      lcd_putc("CHUA DU 6 KY TU!");
                      Beep(3); // beep loa 3 tieng
                      delay_ms(1000);
                      ResetInput();  // xoa du lieu da nhap
                      DisplayChangePass();
                 }
                 else
                 {
                     AdminPass[0]=CurrentPass[0];
                     AdminPass[1]=CurrentPass[1];
                     AdminPass[2]=CurrentPass[2];
                     AdminPass[3]=CurrentPass[3];
                     AdminPass[4]=CurrentPass[4];
                     AdminPass[5]=CurrentPass[5];
                     WritePassEEPROM();
                     lcd_gotoxy(1, 2);
                     lcd_putc(" DA LUU MA MOI! ");
                     Beep(2);
                     delay_ms(1000);
                     CurrentState=STAMAIN;
                     ResetInput();  // xoa du lieu da nhap
                     DisplayMain();
                 }
              }
             
              break;
            } 
             case KEYSTAR:// Neu bam phim sao
            {
              if(CurrentState==STAINPUTPASS)
              {
               ResetInput();  // xoa du lieu da nhap
               CurrentState=STAMAIN;
               DisplayMain();
              }
              else if(CurrentState==STACHANGEPASS)
              {
               Beep(2);
               CurrentState=STAMAIN;
               ResetInput();  // xoa du lieu da nhap
               DisplayMain();
              }
              break;
            } 
            case KeyA:
            {
              if(CurrentState==STAMAIN)
              {
               CurrentState=STAINPUTPASS; // cai trang tahi hien tai la nhap pass
               NextState=STACHANGEPASS;
               DisplayInputPass();
              }
              break;
            }
            case KeyB:
            {
              if(CurrentState==STAINPUTPASS)
              {
               if(CurrentCodeIndex>0) // neu da nhap ky tu thi xoa 1 ky tu
               {
                 CurrentCodeIndex--;
                 CurrentPass[CurrentCodeIndex]=0xFF; // xoa ky tu trong mang
                 lcd_gotoxy(CurrentCodeIndex+1, 2);
                 lcd_putc(" "); // xoa hien thi
                 lcd_gotoxy(CurrentCodeIndex+1, 2);
               }
              }
              break;
            }
            
        }   
      }
    
   }
}
  
  
  

