#include "iostream"
#include "mbed.h"
#include <cmath>
# include <bits/stdc++.h>
BufferedSerial pc(USBTX, USBRX, 115200);
BufferedSerial esp(PA_9, PA_10, 115200);

int16_t pwm[4] = {0, 0, 0, 0};
float pojisyon[3] = {0,0,0};

int main()
{   
    float omuni_value = 10000;
    char buf[64];
    int buf_index = 0;
    int number;

    CAN can(PB_12, PB_13, (int)1e6); // canを出力するピンを指定
    CANMessage msg;// 変数「msg」の作成
    while (1)
    {
        if(esp.readable()){
            char c;
            int num = esp.read(&c, 1);
            if(num>0){
                if(c == '\n' || c == '\r'){
                    if (buf_index > 0)
                    {
                        buf[buf_index] = '\0';
                        buf_index = 0;
                        number = atoi(buf); 
                        for (int i = 0; i <= 512; i += 256)
                        {
                            
                            if((number - i) <= 127 && -127 <= (number - i)){
                                // printf("%d\nkazu",number);
                                if ((10 < (number - i)) || (-10 > (number - i)))
                                {
                                    
                                    pojisyon[i/256] = (number - i);


                                }else{
                                    pojisyon[i/256] = 0;
                                }
                            }
                        }
                    }
                }else if(buf_index < sizeof(buf) - 1){
                    buf[buf_index++] = c;
                }

            }
        }
        ThisThread::sleep_for(chrono::milliseconds(10));

        float power = (hypot(pojisyon[0],pojisyon[1])/127);
        float angle = (atan(pojisyon[1]/pojisyon[0]) * 180/M_PI); 

        // 単位円に関するベクトルの考え方によりsin-45,cos-45,sin-45,sin-45=sin-45,sin45,sin-45,sin45
        // モーターの回転を考慮すると(sinθに180を足し-sinθとする)sin135,sin45,sin-45,sin225
        float tire_angle[4] = {135,45,-45,225};

        for (int i = 0; i < 4; i++)
        {
            float num = (sin((M_PI/180)*(tire_angle[i] + angle)) * power * omuni_value) ; 

            printf("%d\n",i);

            float num2 = omuni_value * pojisyon[2] / 127;
            pwm[i] = num + num2;

            if (pwm[i] > 20000)
            {
                printf("hight_dangerous");
                pwm[i] = 20000;
            }else if (pwm[i] < -20000)
            {
                printf("low_dangerous");
                pwm[i] = -20000;                
            }
        }
        CANMessage msg(320, (const uint8_t *)pwm, 8); //特に理由がない限りwhile直下
        can.write(msg); //特に理由がない限りwhile直下
    }
} 