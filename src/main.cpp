#include "iostream"
#include "mbed.h"
#include <cmath>
# include <bits/stdc++.h>
#include <vector>
#include <stdio.h>
#include <time.h>
// BufferedSerial esp(USBTX, USBRX, 115200);
BufferedSerial esp(PA_9, PA_10, 115200);

// PA_3,PA_2,GAD,5V	→4ピン ロリコンにつけるPA_3はA層、,PA_2はB層
// PA_10,PA_9,GAD,5V →esp32　PA_10をespのTXにPA_9をespのRXにさす
// PB_3,PB_5　→2ピン　PB_5が右側になるように（負）
// PB_4,PB_10 →2ピン　PB_10が右側になるように（負）
// PA_8,PC_7　→2ピン　PC_7が右側になるように（負）
// PB_6,PA_6　→2ピン　PA_6が右側になるように（負）
// PA_5,PB_9　→2ピン　PB_9が右側になるように（負）
// PA_6,GAD    →2ピン
// PB_5,GAD    →2ピン
// q破壊工作とまりー
// k破壊工作動きー
// r浮気動きー
// l浮気逆供御気ー
// n浮気とまりー
// s浮気おそうごき
// m浮気遅く逆にー
// PwmOut pwm_outputs[10] = {PB_3,PB_5,PB_4,PB_10,PA_8,PC_7,PB_6,PA_6,PA_5,PB_9};

int16_t pwm[4] = {0, 0, 0, 0};
int16_t pwm2[4] = {0, 0, 0, 0};
float pojisyon[3] = {0,0,0};
float pojisyon1[3] = {0,0,0};
bool supido_dowun = false;
// bool chack[4] = {false,false,false,false};
// clock_t start;

int main()
{   
    // for (auto pin : pwm_pins) {
    //     pwm_outputs.emplace_back(pin);
    // }
    // pwm_outputs[0]->write(0.5f);

    struct pwm_data {
        char label;
        int assign;
        float value;
        //PwmOut pwm_outputs;
    };
    struct time_data {
        bool chack; 
        clock_t start;
        //PwmOut pwm_outputs;
    };

    time_data times[]{
        {false,0},
        {false,0},
        {false,0},
        {false,0},
    };
    time_data times1[]{
        {false,0},
        {false,0},
        {false,0},
        {false,0},
    };

    pwm_data datas[] = {
        {'r',0,9000},
        {'l',0,-9000},
        {'s',0,9000},
        {'m',0,-9000},
        {'n',0,0},
        {'k',1,-20000},
        {'q',1,0},
        {'w',1,-20000},
        {'u',3,20000},
        {'d',3,-20000},
        {'o',3,0},
        {'x',0,15000},
        {'w',0,-15000},
        {'t',2,20000},
        {'c',2,0}
    };
    // bool chack1[10] = {false,false,false,false,false,false,false,false,false,false};

    float omuni_value = 15000;
    float omuni_value1 = 6000;
    // float omuni_value[4] = {10000,10000,10000,10000};
    char buf[64];
    int buf_index = 0;
    int number;

    CAN can(PA_11, PA_12, (int)1e6); // canを出力するピンを指定
    CAN can2(PB_12, PB_13, (int)1e6); // canを出力するピンを指定
    CANMessage msg;// 変数「msg」の作成
    CANMessage msg2;// 変数「msg」の作成
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
                        // printf(buf);
                        number = atoi(buf);
                        if(c=='\n'){
                            for (int i = 0; i <= 512; i += 256)
                            {
                                
                                if((number - i) <= 127 && -127 <= (number - i)){
                                    // printf("%d\nkazu",number);
                                    if ((20 < (number - i)) || (-20 > (number - i)))
                                    {
                                        
                                        pojisyon[i/256] = (number - i);


                                    }else{
                                        pojisyon[i/256] = 0;
                                    }
                                }
                            }                            
                        }else{
                            for (int i = 0; i <= 512; i += 256)
                            {
                                
                                if((number - i) <= 127 && -127 <= (number - i)){
                                    // printf("%d\nkazu",number);
                                    if ((20 < (number - i)) || (-20 > (number - i)))
                                    {
                                        
                                        pojisyon1[i/256] = (number - i);


                                    }else{
                                        pojisyon1[i/256] = 0;
                                    }
                                }
                            }                              
                        }
                            // printf("\n");
                         
                  
                    }
                }else if(std::isalpha(c)){
                    // if (c ==  "a")
                    // {
                    //     omuni_value = 5000;
                    // }else if (c == "b"){
                    //     omuni_value = 10000;
                    // }
                    if (c ==  'a')
                    {
                        supido_dowun = true;
                        // printf("supido_dowun= true\n");
                    }else if (c ==  'b'){
                        supido_dowun = false;
                        // printf("supido_dowun= false\n");
                    }
                    
                    for (int i = 0; i <= 15; i++)
                    {
                        if (c == datas[i].label)
                        {
                            int nakanisi = datas[i].value;
                            if(nakanisi > 0 || nakanisi < 0){
                
                                if(times1[datas[i].assign].chack == false){
                                    times1[datas[i].assign].start = clock();
                                    times1[datas[i].assign].chack = true;
                                }
                                clock_t end = clock();
                                double time = static_cast<double>(end - times1[datas[i].assign].start) / CLOCKS_PER_SEC * 1000.0;
                                // printf("%f\n",time);
                                if (time >= 500)
                                {
                                    time = 500;
                                }
                                if (datas[i].assign != 2)
                                {
                                    nakanisi = nakanisi * (time/500);
                                }
                                
                            }else{
                                times1[datas[i].assign].chack = false;
                            }

                            if(supido_dowun == true){
                                pwm2[datas[i].assign] =  nakanisi / 3;
                                omuni_value = 6000;
                            }else{
                                pwm2[datas[i].assign] = nakanisi;
                                omuni_value = 15000;
                            }
                            // printf("%d\n",pwm2[collect_value[i][0]]);
                        }
                    }
                    
                }else if(buf_index < sizeof(buf) - 1){

                    buf[buf_index++] = c;

                }

            }
        }
        // ThisThread::sleep_for(chrono::milliseconds(1000));
        for (int i = 0; i < 2; i++)
        {
            
        }
        float power[2] = {(hypot(pojisyon[0],pojisyon[1])/127),
                          (hypot(pojisyon1[0],pojisyon1[1])/127)};
        float angle[2] = {  atan2(pojisyon[1],pojisyon[0]) * (180/M_PI),
                            atan2(pojisyon1[1],pojisyon1[0]) * (180/M_PI)}; 
        int num2[2] = {(int)(-47.2440944883 * pojisyon[2]),
                       (int)(10 * pojisyon1[2])};

        // 単位円に関するベクトルの考え方によりsin-45,cos-45,sin-45,sin-45=sin-45,sin45,sin-45,sin45
        // モーターの回転を考慮すると(sinθに180を足し-sinθとする)sin135,sin45,sin-45,sin225
        float tire_angle[4] = {135,45,-45,225};

        for (int i = 0; i < 4; i++)
        {
            
            int num = (int)(sin((M_PI/180)*(tire_angle[i] + angle[0])) * power[0] * omuni_value); 
            int num3 = (int)(sin((M_PI/180)*(tire_angle[i] + angle[1])) * power[1] * omuni_value1); 
            // int num = (int)(sin((M_PI/180)*(tire_angle[i] + angle)) * power * omuni_value[i]); 
            num = num + num3;
            if(num > 0 || num < 0){
                
                if(times[i].chack == false){
                    times[i].start = clock();
                    times[i].chack = true;
                }
                clock_t end = clock();
                double time = static_cast<double>(end - times[i].start) / CLOCKS_PER_SEC * 1000.0;
                // printf("%f\n",time);
                if (time >= 500)
                {
                    time = 500;
                }
                num = num * (time/500);
            }else{
                times[i].chack = false;
            }

            if(num != 0){
                pwm[i] = num;
            }else{
                pwm[i] = num2[0] + num2[1];
            }
            // printf("%d\n",i);
            // printf("%f\n",angle);
            

            if (pwm[i] > 25000)
            {
                // printf("hight_dangerous");
                pwm[i] = 25000;
            }else if (pwm[i] < -25000)
            {
                // printf("low_dangerous");
                pwm[i] = -25000;                
            }
        }
        // if(num > 7000 || num < -7000){
        //     int arisawa = 1;
        //     if (num < 0)
        //     {
        //         arisawa = -1;
        //     }
            
        //     if(times[i].chack == false){
        //         times[i].start = clock();
        //         times[i].chack = true;
        //     }
        //     clock_t end = clock();
        //     double time = static_cast<double>(end - times[i].start) / CLOCKS_PER_SEC * 1000.0;
        //     // printf("%f\n",time);
        //     if (time >= 2000)
        //     {
        //         time = 2000;
        //     }
        //     num = ((num - (7000 * arisawa)) * (time/2000)) + 7000 * arisawa;
        // }else{
        //     times[i].chack = false;
        // }
        // for (int i = 0; i < 4; i++)
        // {
        //     // if(supido_dowun == true)
        //     // {
        //         pwm[i] = pwm[i] * 0.3f; 
        //     // }           
        // }
        // for (int i = 0; i < 4; i++)
        // {
        //     // if(supido_dowun == true)
        //     // {
        //         pwm2[i] = pwm2[i] * 0.3f; 
        //         printf("%d\n",pwm2[i]);
        //     // }           
        // }
        // printf("%d\n",pwm[0]);
        CANMessage msg(4, (const uint8_t *)pwm, 8); //特に理由がない限りwhile直下
        CANMessage msg2(4, (const uint8_t *)pwm2, 8); //特に理由がない限りwhile直下
        can.write(msg); //特に理由がない限りwhile直下
        can2.write(msg2); //特に理由がない限りwhile直下
        
    }
}