#include "iostream"
#include "mbed.h"
#include <cmath>

BufferedSerial esp(PA_9, PA_10, 115200);

int main()
{   
    int16_t pwm[4] = {0, 0, 0, 0};
    float pojisyon[3] = {0,0,0};
    float omuni_value = 15000;
    char buf[64];
    int buf_index = 0;
    int number = 0;

    CAN can(PA_11, PA_12, (int)1e6); // canを出力するピンを指定
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
                        // number = atoi(buf);
                        for (int i = 0; i <= 2; i ++)
                        {
                            number = atoi(buf) - (i * 255);
                            if((number) <= 127 && -127 <= (number)){
                                if ((20 < number) || (-20 > number))
                                {
                                    pojisyon[i] = number;
                                }else{
                                    pojisyon[i] = 0;
                                }
                            }
                        }                              
                    }else if(buf_index < sizeof(buf) - 1){
                        buf[buf_index++] = c;
                    }
                }
            }
        }


        float power = hypot(pojisyon[0],pojisyon[1])/127;
        float angle = (pojisyon[1],pojisyon[0]) * (180/M_PI); 
        int num2 = (int)(-5000 * pojisyon[2] / 127); 

        // 単位円に関するベクトルの考え方によりsin-45,cos-45,sin-45,sin-45=sin-45,sin45,sin-45,sin45
        // モーターの回転を考慮すると(sinθに180を足し-sinθとする)sin135,sin45,sin-45,sin225
        float tire_angle[4] = {135,45,-45,225};

        for (int i = 0; i < 4; i++)
        {
            
            int num = (int)(sin((M_PI/180)*(tire_angle[i] + angle)) * power * omuni_value); 

            if(num != 0){
                pwm[i] = num;
            }else
            {
                pwm[i] = num2;
            }

            if (pwm[i] > 15000)
            {
                pwm[i] = 0;
            }else if (pwm[i] < -15000)
            {
                pwm[i] = 0;                
            }
        }

        CANMessage msg(4, (const uint8_t *)pwm, 8); //特に理由がない限りwhile直下
        can.write(msg); //特に理由がない限りwhile直下
    }
}