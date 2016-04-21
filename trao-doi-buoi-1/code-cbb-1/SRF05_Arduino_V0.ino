#include <Wire.h>  
#include <LiquidCrystal_I2C.h> // Using version 1.2.1
#include <EEPROM.h>

#define trigger 8
#define echo 7
#define button 2
#define led 13
#define sleep 700 //Doi mot khoang thoi gian la sleep roi do tiep, tranh song con du lai
//#define N 5 //So lan do khoang cach tu cam bien sieu am

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  //Khai bao dia chi I2C cua LCD

float error_epsilon[] = {0.5, 0.4, 0.3, 0.2, 0.1, 0.05}; //Cac gia tri sai so cho phep
int N_error,N,N_do=5;
float A[5],A_tmp[5];
int signal = 0; //Tin hieu tu nut nhan
int addr1 = 0, addr2=100; //Dia chi EEROM o vi tri 0
int n=0;
float liquid,liquid1,liquid2;
float a,b,tmp;

void setup(){
	Serial.begin(9600);
	// Khai bao che do chan cua cam bien sieu am
	pinMode(trigger, OUTPUT);
	pinMode(echo, INPUT);
	pinMode(button, INPUT_PULLUP);
	pinMode(led, OUTPUT);

	lcd.begin(16,2); // Khai bao LCD 16 cot 2 dong
	lcd.backlight();

	N_error = sizeof(error_epsilon)/sizeof(float); //Lay so phan tu cua mang error_epsilon
	N=sizeof(A)/sizeof(float);
}

void loop(){
	signal = digitalRead(button);
	if (signal == LOW){
		n=n+1;
		if (n==1){ //Ghi vao o nho vi tri la 0
			lcd.clear();
			//Khi nhan nut nhan thi hien thi dong chu: To Measure ...
			lcd.setCursor(0,0);
			lcd.print("To Measure ...");
			Distances();
			liquid1 = giaithuat();
			DisplayLCD(liquid1);
			EEPROM.put(addr1, liquid1);
		}
		else
			if (n==2){ //Ghi vao o nho vi tri la 100
				lcd.clear();
				//Khi nhan nut nhan thi hien thi dong chu: To Measure ...
				lcd.setCursor(0,0);
				lcd.print("To Measure ...");
				Distances();
				liquid2 = giaithuat();
				DisplayLCD(liquid2);
				EEPROM.put(addr2, liquid2);
			}
			else
				if (n>2){ //Neu n>1 va n=2k+1 thi hoan vi gia tri duoc luu trong 2 o nho cho nhau
					a = EEPROM.get(addr1,liquid1);
					b = EEPROM.get(addr2,liquid2);

					//Hoan vi 2 vi tri a,b
					tmp = a;
					a = b;
					b = tmp;

					liquid1=a;
					liquid2=b;

					EEPROM.put(addr1,liquid1);
					EEPROM.put(addr2,liquid2);
					lcd.clear();
					//Khi nhan nut nhan thi hien thi dong chu: To Measure ...
					lcd.setCursor(0,0);
					lcd.print("To Measure ...");
					Distances();
					liquid2 = giaithuat();
					DisplayLCD(liquid2);
					EEPROM.put(addr2, liquid2);
				}
	}
	if (n>0){
		a = EEPROM.get(addr1,liquid1); //Gia tri truoc do
		b = EEPROM.get(addr2,liquid2); //Gia tri moi do duoc

		DisplayLCD(b);

		Serial.print("Gia tri moi do duoc la: ");
		Serial.println(b);

		Serial.print("Gia tri do duoc truoc do la: ");
		Serial.println(a);

		Serial.print('\n');
	}
}

float Distance_SRF(){
	unsigned long times;
	float distance;

	digitalWrite(trigger,HIGH);
	delayMicroseconds(1000); //Giu mot khoang thoi gian de phat xung di
	digitalWrite(trigger,LOW);
	times=pulseIn(echo,HIGH);//Lay thoi gian chan Echo tu HIGH --> LOW
	distance=0.0344*(times/2); // Van toc song am: v = 344 m/s
	return distance;
}
void Distances(){ //Do khoang cach N lan va khoang cach luu vao mang A
	for (int i=0;i<N_do;i++){
		A[i]=Distance_SRF();
		delay(sleep);
	}
}

float giaithuat(){ //Giai thuat loc nhieu, cai tien so lieu do tu cam bien
	/*Giai thuat loc nhieu:
	 * Input: Mang gia tri A co n phan tu
	 * Processing:
	 * Thuc hien vong lap theo sai so: 50% 40% 30% 20% 10% 5%
	 * Lap theo tung gia tri A[i]
	 * + Tinh trung binh bo A[i]
	 * + So sanh: Neu |trung_binh_bo_A[i]-Ai| > trunh_binh_bo_A[i]*sai_so =====> Loai bo A[i] ra khoi tap dang xet
	 * + Giu lai cac phan tu con dang duoc xet va thuc hien tiep vong lap
	 * Output: Gia tri trung binh cua cac phan tu con duoc giu lai
	 */
	int i,j,k,t,m,kt=0;
	float average;

	for (i=0;i<N_error;i++){//Lap theo tung gia tri sai so
		if (N>=2){ //Mang 2 phai tu 2 phan tu tro len moi xet tiep
			for (j=0;j<N;j++){//Lap theo tung gia tri khoang cach
				float s=0;
				for (k=0;k<N;k++){
					if (j != k){//Khong tinh phan tu A[j] dang xet
						s=s+A[k];
					}
				}
				s=s/(N-1);//Gia tri trung binh khong tinh A[j];
				if (abs(s-A[j])>(s*error_epsilon[i])){
					kt=1;
					m=0;
					for (k=0;k<N;k++){
						if (k != j){
							A_tmp[m]=A[k];
							m++;
						}
					}
					for (k=0;k<m;k++) A[k]=A_tmp[k];
				}
			}
		}
		if (kt==1) N=m;
		else N=sizeof(A)/sizeof(float);
		average=0;
		for (k=0;k<N;k++){
			average=average+A[k];
		}
		average=average/N;
	}
	return average;
}

void DisplayLCD(float liquid3){
	lcd.setCursor(0,0);
	lcd.print("Distance (cm):");
	lcd.setCursor(0,1);
	lcd.print(liquid3);
}


