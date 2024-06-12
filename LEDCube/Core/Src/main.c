/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <stdio.h>
#include <string.h>
#include <stdint.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
uint8_t rxData;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM3_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

char *demo_string = "Hello \r\n"; //demo string
uint8_t recv_char;
uint8_t recv_str[20];
int i=0;
char *on = "on";
char *off = "off";
uint8_t ft = 76;

typedef struct {
    const char* name;
    GPIO_TypeDef* port;
    uint16_t pin;
} PinMap;
PinMap pinMap[] = {
    {"PIN11", PIN11_GPIO_Port, PIN11_Pin},
    {"PIN12", PIN12_GPIO_Port, PIN12_Pin},
    {"PIN13", PIN13_GPIO_Port, PIN13_Pin},
    {"PIN14", PIN14_GPIO_Port, PIN14_Pin},
    {"PIN21", PIN21_GPIO_Port, PIN21_Pin},
    {"PIN22", PIN22_GPIO_Port, PIN22_Pin},
    {"PIN23", PIN23_GPIO_Port, PIN23_Pin},
    {"PIN24", PIN24_GPIO_Port, PIN24_Pin},
    {"PIN31", PIN31_GPIO_Port, PIN31_Pin},
    {"PIN32", PIN32_GPIO_Port, PIN32_Pin},
    {"PIN33", PIN33_GPIO_Port, PIN33_Pin},
    {"PIN34", PIN34_GPIO_Port, PIN34_Pin},
    {"PIN41", PIN41_GPIO_Port, PIN41_Pin},
    {"PIN42", PIN42_GPIO_Port, PIN42_Pin},
    {"PIN43", PIN43_GPIO_Port, PIN43_Pin},
    {"PIN44", PIN44_GPIO_Port, PIN44_Pin},
};
PinMap layers[] ={
		{"LAYER1", LAYER1_GPIO_Port, LAYER1_Pin},
		{"LAYER2", LAYER2_GPIO_Port, LAYER2_Pin},
		{"LAYER3", LAYER3_GPIO_Port, LAYER3_Pin},
		{"LAYER4", LAYER4_GPIO_Port, LAYER4_Pin}};
int mapSize = sizeof(pinMap) / sizeof(PinMap);
int layersSize = sizeof(layers) / sizeof(PinMap);


PinMap* findPinMap(const char* name) {
    for (int i = 0; i < sizeof(pinMap) / sizeof(PinMap); i++) {
        if (strcmp(name, pinMap[i].name) == 0) {
            return &pinMap[i];
        }
    }
    return NULL;
}


void disableAllPins() {
    for (int i = 0; i < mapSize; i++) {
        HAL_GPIO_WritePin(pinMap[i].port, pinMap[i].pin, GPIO_PIN_SET);
    }

    for(int i = 0; i < layersSize; i++) {
    	HAL_GPIO_WritePin(layers[i].port, layers[i].pin, GPIO_PIN_RESET);
    }
}

void enableAllPins() {
    for (int i = 0; i < mapSize; i++) {
        HAL_GPIO_WritePin(pinMap[i].port, pinMap[i].pin, GPIO_PIN_RESET);
    }

    for(int i = 0; i < layersSize; i++) {
    	HAL_GPIO_WritePin(layers[i].port, layers[i].pin, GPIO_PIN_SET);
    }
}
//turn columns off
void turnColumnsOff() {

	for (int i = 0; i < mapSize; i++) {
	        HAL_GPIO_WritePin(pinMap[i].port, pinMap[i].pin, GPIO_PIN_SET);
	    }

}
//flicker on
void flickerOn()
{
  int i = 150;
  while(i != 0)
  {
    enableAllPins();
    HAL_Delay(i);
    disableAllPins();
    HAL_Delay(i);
    i-= 5;
  }
}

void turnOnAndOffAllByLayerUpAndDownNotTimed()
{
  int x = 75;
  for(int i = 5; i != 0; i--)
  {
    enableAllPins();
    for(int i = layersSize; i!=0; i--)
    {
    	HAL_GPIO_WritePin(layers[i-1].port, layers[i-1].pin, GPIO_PIN_RESET);
    	HAL_Delay(x);
    }
    for(int i = 0; i< layersSize; i++)
    {
    	HAL_GPIO_WritePin(layers[i].port, layers[i].pin, GPIO_PIN_SET);
    	    	HAL_Delay(x);
    }
      for(int i = 0; i<layersSize; i++)
    {
    	  HAL_GPIO_WritePin(layers[i].port, layers[i].pin, GPIO_PIN_RESET);
    	      	    	HAL_Delay(x);
    }
    for(int i = layersSize; i!=0; i--)
    {
    	HAL_GPIO_WritePin(layers[i-1].port, layers[i-1].pin, GPIO_PIN_SET);
    	    	HAL_Delay(x);
    }
  }
}

//turn everything on and off by column sideways
void turnOnAndOffAllByColumnSideways()
{
  int x = 75;
  disableAllPins();
  //turn on layers
  for(int i = 0; i<layersSize; i++)
  {
	  HAL_GPIO_WritePin(layers[i].port, layers[i].pin, GPIO_PIN_SET);
  }
  for(int y = 0; y<3; y++)
  {
    //turn on 0-3
    for(int i = 0; i<4; i++)
    {
    	HAL_GPIO_WritePin(pinMap[i].port, pinMap[i].pin, GPIO_PIN_RESET);
    	HAL_Delay(x);
    }
    //turn on 4-7
    for(int i = 4; i<8; i++)
    {
    	HAL_GPIO_WritePin(pinMap[i].port, pinMap[i].pin, GPIO_PIN_RESET);
    	HAL_Delay(x);
    }
    //turn on 8-11
    for(int i = 8; i<12; i++)
    {
    	HAL_GPIO_WritePin(pinMap[i].port, pinMap[i].pin, GPIO_PIN_RESET);
    	HAL_Delay(x);
    }
    //turn on 12-15
    for(int i = 12; i<16; i++)
    {
    	HAL_GPIO_WritePin(pinMap[i].port, pinMap[i].pin, GPIO_PIN_RESET);
    	HAL_Delay(x);
    }
    //turn off 0-3
    for(int i = 0; i<4; i++)
    {
    	HAL_GPIO_WritePin(pinMap[i].port, pinMap[i].pin, GPIO_PIN_SET);
    	HAL_Delay(x);
    }
    //turn off 4-7
    for(int i = 4; i<8; i++)
    {
    	HAL_GPIO_WritePin(pinMap[i].port, pinMap[i].pin, GPIO_PIN_SET);
    	HAL_Delay(x);
    }
    //turn off 8-11
    for(int i = 8; i<12; i++)
    {
    	HAL_GPIO_WritePin(pinMap[i].port, pinMap[i].pin, GPIO_PIN_SET);
    	HAL_Delay(x);
    }
    //turn off 12-15
    for(int i = 12; i<16; i++)
    {
    	HAL_GPIO_WritePin(pinMap[i].port, pinMap[i].pin, GPIO_PIN_SET);
    	HAL_Delay(x);
    }
    //turn on 12-15
    for(int i = 12; i<16; i++)
    {
    	HAL_GPIO_WritePin(pinMap[i].port, pinMap[i].pin, GPIO_PIN_RESET);
    	HAL_Delay(x);
    }
    //turn on 8-11
    for(int i = 8; i<12; i++)
    {
    	HAL_GPIO_WritePin(pinMap[i].port, pinMap[i].pin, GPIO_PIN_RESET);
    	    	HAL_Delay(x);
    }
    //turn on 4-7
    for(int i = 4; i<8; i++)
    {
    	HAL_GPIO_WritePin(pinMap[i].port, pinMap[i].pin, GPIO_PIN_RESET);
    	    	HAL_Delay(x);
    }
    //turn on 0-3
    for(int i = 0; i<4; i++)
    {
    	HAL_GPIO_WritePin(pinMap[i].port, pinMap[i].pin, GPIO_PIN_RESET);
    	    	HAL_Delay(x);
    }
    //turn off 12-15
    for(int i = 12; i<16; i++)
    {
    	HAL_GPIO_WritePin(pinMap[i].port, pinMap[i].pin, GPIO_PIN_SET);
    	    	HAL_Delay(x);
    }
    //turn off 8-11
    for(int i = 8; i<12; i++)
    {
    	HAL_GPIO_WritePin(pinMap[i].port, pinMap[i].pin, GPIO_PIN_SET);
    	    	    	HAL_Delay(x);
    }
    //turn off 4-7
    for(int i = 4; i<8; i++)
    {
    	HAL_GPIO_WritePin(pinMap[i].port, pinMap[i].pin, GPIO_PIN_SET);
    	    	    	HAL_Delay(x);
    }
    //turn off 0-3
    for(int i = 0; i<4; i++)
    {
    	HAL_GPIO_WritePin(pinMap[i].port, pinMap[i].pin, GPIO_PIN_SET);
    	    	    	HAL_Delay(x);
    }
  }
}

//up and down single layer stomp
void layerstompUpAndDown()
{
  int x = 75;
  for(int i = 0; i<4; i++)
  {
	  HAL_GPIO_WritePin(layers[i].port, layers[i].pin, GPIO_PIN_RESET);
  }
  for(int y = 0; y<5; y++)
  {
    for(int count = 0; count<1; count++)
    {
      for(int i = 0; i<4; i++)
      {
    	  HAL_GPIO_WritePin(layers[i].port, layers[i].pin, GPIO_PIN_SET);
    	  HAL_Delay(x);
          HAL_GPIO_WritePin(layers[i].port, layers[i].pin, GPIO_PIN_RESET);
      }
      for(int i = 4; i !=0; i--)
      {
    	  HAL_GPIO_WritePin(layers[i-1].port, layers[i-1].pin, GPIO_PIN_SET);
    	  HAL_Delay(x);
    	  HAL_GPIO_WritePin(layers[i-1].port, layers[i-1].pin, GPIO_PIN_RESET);
      }
    }
    for(int i = 0; i<4; i++)
    {
    	HAL_GPIO_WritePin(layers[i].port, layers[i].pin, GPIO_PIN_SET);
    	HAL_Delay(x);
    }
    for(int i = 4; i!=0; i--)
    {
    	HAL_GPIO_WritePin(layers[i-1].port, layers[i-1].pin, GPIO_PIN_RESET);
    	HAL_Delay(x);
    }
  }
}

//flicker off
void flickerOff()
{
  enableAllPins();
  for(int i = 0; i!= 150; i+=5)
  {
    disableAllPins();
    HAL_Delay(i+50);
    enableAllPins();
    HAL_Delay(i);
  }
}
//around edge of the cube down
void aroundEdgeDown()
{
  for(int x = 200; x != 0; x -=50)
  {
    disableAllPins();
    for(int i = 4; i != 0; i--)
    {
    	HAL_GPIO_WritePin(layers[i-1].port, layers[i-1].pin, GPIO_PIN_SET);
    	HAL_GPIO_WritePin(pinMap[5].port, pinMap[5].pin, GPIO_PIN_RESET);
    	HAL_GPIO_WritePin(pinMap[6].port, pinMap[6].pin, GPIO_PIN_RESET);
    	HAL_GPIO_WritePin(pinMap[9].port, pinMap[9].pin, GPIO_PIN_RESET);
    	HAL_GPIO_WritePin(pinMap[10].port, pinMap[10].pin, GPIO_PIN_RESET);

    	HAL_GPIO_WritePin(pinMap[0].port, pinMap[0].pin, GPIO_PIN_RESET);
    	HAL_Delay(x);

    	HAL_GPIO_WritePin(pinMap[0].port, pinMap[0].pin, GPIO_PIN_SET);
    	HAL_GPIO_WritePin(pinMap[4].port, pinMap[4].pin, GPIO_PIN_RESET);
    	HAL_Delay(x);

    	HAL_GPIO_WritePin(pinMap[4].port, pinMap[4].pin, GPIO_PIN_SET);
    	HAL_GPIO_WritePin(pinMap[8].port, pinMap[8].pin, GPIO_PIN_RESET);
    	HAL_Delay(x);

    	HAL_GPIO_WritePin(pinMap[8].port, pinMap[8].pin, GPIO_PIN_SET);
    	HAL_GPIO_WritePin(pinMap[12].port, pinMap[12].pin, GPIO_PIN_RESET);
    	HAL_Delay(x);

    	HAL_GPIO_WritePin(pinMap[12].port, pinMap[12].pin, GPIO_PIN_SET);
    	HAL_GPIO_WritePin(pinMap[13].port, pinMap[13].pin, GPIO_PIN_RESET);
    	HAL_Delay(x);

    	HAL_GPIO_WritePin(pinMap[13].port, pinMap[13].pin, GPIO_PIN_SET);
    	HAL_GPIO_WritePin(pinMap[14].port, pinMap[14].pin, GPIO_PIN_RESET);
    	HAL_Delay(x);

    	HAL_GPIO_WritePin(pinMap[14].port, pinMap[14].pin, GPIO_PIN_SET);
    	HAL_GPIO_WritePin(pinMap[15].port, pinMap[15].pin, GPIO_PIN_RESET);
    	HAL_Delay(x);

    	HAL_GPIO_WritePin(pinMap[15].port, pinMap[15].pin, GPIO_PIN_SET);
    	HAL_GPIO_WritePin(pinMap[11].port, pinMap[11].pin, GPIO_PIN_RESET);
    	HAL_Delay(x);

    	HAL_GPIO_WritePin(pinMap[11].port, pinMap[11].pin, GPIO_PIN_SET);
    	HAL_GPIO_WritePin(pinMap[7].port, pinMap[7].pin, GPIO_PIN_RESET);
    	HAL_Delay(x);

    	HAL_GPIO_WritePin(pinMap[7].port, pinMap[7].pin, GPIO_PIN_SET);
    	HAL_GPIO_WritePin(pinMap[3].port, pinMap[3].pin, GPIO_PIN_RESET);
    	HAL_Delay(x);

    	HAL_GPIO_WritePin(pinMap[3].port, pinMap[3].pin, GPIO_PIN_SET);
    	HAL_GPIO_WritePin(pinMap[2].port, pinMap[2].pin, GPIO_PIN_RESET);
    	HAL_Delay(x);

    	HAL_GPIO_WritePin(pinMap[2].port, pinMap[2].pin, GPIO_PIN_SET);
    	HAL_GPIO_WritePin(pinMap[1].port, pinMap[1].pin, GPIO_PIN_RESET);
    	HAL_Delay(x);

    	HAL_GPIO_WritePin(pinMap[1].port, pinMap[1].pin, GPIO_PIN_SET);
    }
  }
}

//diagonal rectangle
void diagonalRectangle()
{
  int x = 350;
  disableAllPins();
  for(int count = 0; count<5; count++)
  {
    //top left
    for(int i = 0; i<8; i++)
    {
    	HAL_GPIO_WritePin(pinMap[i].port, pinMap[i].pin, GPIO_PIN_RESET);

    }
    HAL_GPIO_WritePin(layers[3].port, layers[3].pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(layers[2].port, layers[2].pin, GPIO_PIN_SET);
    HAL_Delay(x);

    disableAllPins();
    //middle middle
    for(int i = 4; i<12; i++)
    {
    	HAL_GPIO_WritePin(pinMap[i].port, pinMap[i].pin, GPIO_PIN_RESET);
    }
    HAL_GPIO_WritePin(layers[1].port, layers[1].pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(layers[2].port, layers[2].pin, GPIO_PIN_SET);

    HAL_Delay(x);

    disableAllPins();
    //bottom right
    for(int i = 8; i<16; i++)
    {
    	HAL_GPIO_WritePin(pinMap[i].port, pinMap[i].pin, GPIO_PIN_RESET);
    }
    HAL_GPIO_WritePin(layers[0].port, layers[0].pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(layers[1].port, layers[1].pin, GPIO_PIN_SET);
    HAL_Delay(x);

    disableAllPins();
    //bottom middle
    for(int i = 4; i<12; i++)
    {
    	HAL_GPIO_WritePin(pinMap[i].port, pinMap[i].pin, GPIO_PIN_RESET);
    }
    HAL_GPIO_WritePin(layers[0].port, layers[0].pin, GPIO_PIN_SET);
       HAL_GPIO_WritePin(layers[1].port, layers[1].pin, GPIO_PIN_SET);
       HAL_Delay(x);
       disableAllPins();
    //bottom left
    for(int i = 0; i<8; i++)
    {
    	HAL_GPIO_WritePin(pinMap[i].port, pinMap[i].pin, GPIO_PIN_RESET);
    }
    HAL_GPIO_WritePin(layers[0].port, layers[0].pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(layers[1].port, layers[1].pin, GPIO_PIN_SET);
    HAL_Delay(x);
    disableAllPins();
    //middle middle
    for(int i = 4; i<12; i++)
    {
    	HAL_GPIO_WritePin(pinMap[i].port, pinMap[i].pin, GPIO_PIN_RESET);
    }
    HAL_GPIO_WritePin(layers[1].port, layers[1].pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(layers[2].port, layers[2].pin, GPIO_PIN_SET);
    HAL_Delay(x);
    disableAllPins();
    //top right
    for(int i = 8; i<16; i++)
    {
    	HAL_GPIO_WritePin(pinMap[i].port, pinMap[i].pin, GPIO_PIN_RESET);
    }
    HAL_GPIO_WritePin(layers[2].port, layers[2].pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(layers[3].port, layers[3].pin, GPIO_PIN_SET);
        HAL_Delay(x);
        disableAllPins();
    //top middle
    for(int i = 4; i<12; i++)
    {
    	HAL_GPIO_WritePin(pinMap[i].port, pinMap[i].pin, GPIO_PIN_RESET);
    }
    HAL_GPIO_WritePin(layers[2].port, layers[2].pin, GPIO_PIN_SET);
           HAL_GPIO_WritePin(layers[3].port, layers[3].pin, GPIO_PIN_SET);
           HAL_Delay(x);
           disableAllPins();
  }
  //top left
  for(int i = 0; i<8; i++)
  {
	  HAL_GPIO_WritePin(pinMap[i].port, pinMap[i].pin, GPIO_PIN_RESET);
  }
  HAL_GPIO_WritePin(layers[3].port, layers[3].pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(layers[2].port, layers[2].pin, GPIO_PIN_SET);
  HAL_Delay(x);
  disableAllPins();
}

//propeller
void propeller()
{
  disableAllPins();
  int x = 90;
  for(int y = 4; y>0; y--)
  {
    for(int i = 0; i<6; i++)
    {
      //turn on layer
    	HAL_GPIO_WritePin(layers[y-1].port, layers[y-1].pin, GPIO_PIN_SET);
      //a1
      turnColumnsOff();
      HAL_GPIO_WritePin(pinMap[0].port, pinMap[0].pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(pinMap[5].port, pinMap[5].pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(pinMap[10].port, pinMap[10].pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(pinMap[15].port, pinMap[15].pin, GPIO_PIN_RESET);
      HAL_Delay(x);
      //b1
      turnColumnsOff();
      HAL_GPIO_WritePin(pinMap[4].port, pinMap[4].pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(pinMap[5].port, pinMap[5].pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(pinMap[10].port, pinMap[10].pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(pinMap[11].port, pinMap[11].pin, GPIO_PIN_RESET);
      HAL_Delay(x);
      //c1
      turnColumnsOff();
      HAL_GPIO_WritePin(pinMap[6].port, pinMap[6].pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(pinMap[7].port, pinMap[7].pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(pinMap[8].port, pinMap[8].pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(pinMap[9].port, pinMap[9].pin, GPIO_PIN_RESET);
      HAL_Delay(x);
      //d1
      turnColumnsOff();
      HAL_GPIO_WritePin(pinMap[3].port, pinMap[3].pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(pinMap[6].port, pinMap[6].pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(pinMap[9].port, pinMap[9].pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(pinMap[12].port, pinMap[12].pin, GPIO_PIN_RESET);
      HAL_Delay(x);
      //d2
      turnColumnsOff();
      HAL_GPIO_WritePin(pinMap[2].port, pinMap[2].pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(pinMap[6].port, pinMap[6].pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(pinMap[9].port, pinMap[9].pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(pinMap[13].port, pinMap[13].pin, GPIO_PIN_RESET);
      HAL_Delay(x);
      //d3
      turnColumnsOff();
      HAL_GPIO_WritePin(pinMap[1].port, pinMap[1].pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(pinMap[5].port, pinMap[5].pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(pinMap[10].port, pinMap[10].pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(pinMap[14].port, pinMap[14].pin, GPIO_PIN_RESET);
      HAL_Delay(x);
    }
  }
  //d4
  turnColumnsOff();
  HAL_GPIO_WritePin(pinMap[0].port, pinMap[0].pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(pinMap[5].port, pinMap[5].pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(pinMap[10].port, pinMap[10].pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(pinMap[15].port, pinMap[15].pin, GPIO_PIN_RESET);
  HAL_Delay(x);
}

//spiral in and out
void spiralInAndOut()
{
  enableAllPins();
  int x = 60;
  for(int i = 0; i<6; i++)
  {
    //spiral in clockwise
	  HAL_GPIO_WritePin(pinMap[0].port, pinMap[0].pin, GPIO_PIN_SET);
	  HAL_Delay(x);
	  HAL_GPIO_WritePin(pinMap[1].port, pinMap[1].pin, GPIO_PIN_SET);
	  HAL_Delay(x);
	  HAL_GPIO_WritePin(pinMap[2].port, pinMap[2].pin, GPIO_PIN_SET);
	  HAL_Delay(x);
	  HAL_GPIO_WritePin(pinMap[3].port, pinMap[3].pin, GPIO_PIN_SET);
	  HAL_Delay(x);
	  HAL_GPIO_WritePin(pinMap[7].port, pinMap[7].pin, GPIO_PIN_SET);
	  HAL_Delay(x);
	  HAL_GPIO_WritePin(pinMap[11].port, pinMap[11].pin, GPIO_PIN_SET);
	  HAL_Delay(x);
	  HAL_GPIO_WritePin(pinMap[15].port, pinMap[15].pin, GPIO_PIN_SET);
	  HAL_Delay(x);
	  HAL_GPIO_WritePin(pinMap[14].port, pinMap[14].pin, GPIO_PIN_SET);
	  HAL_Delay(x);
	  HAL_GPIO_WritePin(pinMap[13].port, pinMap[13].pin, GPIO_PIN_SET);
	  HAL_Delay(x);
	  HAL_GPIO_WritePin(pinMap[12].port, pinMap[12].pin, GPIO_PIN_SET);
	  HAL_Delay(x);
	  HAL_GPIO_WritePin(pinMap[8].port, pinMap[8].pin, GPIO_PIN_SET);
	  HAL_Delay(x);
	  HAL_GPIO_WritePin(pinMap[4].port, pinMap[4].pin, GPIO_PIN_SET);
	  HAL_Delay(x);
	  HAL_GPIO_WritePin(pinMap[5].port, pinMap[5].pin, GPIO_PIN_SET);
	  HAL_Delay(x);
	  HAL_GPIO_WritePin(pinMap[6].port, pinMap[6].pin, GPIO_PIN_SET);
	  HAL_Delay(x);
	  HAL_GPIO_WritePin(pinMap[10].port, pinMap[10].pin, GPIO_PIN_SET);
	  HAL_Delay(x);
	  HAL_GPIO_WritePin(pinMap[9].port, pinMap[9].pin, GPIO_PIN_SET);
	  HAL_Delay(x);
	  	  HAL_GPIO_WritePin(pinMap[9].port, pinMap[9].pin, GPIO_PIN_RESET);
	  	  HAL_Delay(x);
	  	  HAL_GPIO_WritePin(pinMap[10].port, pinMap[10].pin, GPIO_PIN_RESET);
	  	  HAL_Delay(x);
	  	  HAL_GPIO_WritePin(pinMap[6].port, pinMap[6].pin, GPIO_PIN_RESET);
	  	  HAL_Delay(x);
	  	  HAL_GPIO_WritePin(pinMap[5].port, pinMap[5].pin, GPIO_PIN_RESET);
	  	  HAL_Delay(x);
	  	  HAL_GPIO_WritePin(pinMap[4].port, pinMap[4].pin, GPIO_PIN_RESET);
	  	  HAL_Delay(x);
	  	  HAL_GPIO_WritePin(pinMap[8].port, pinMap[8].pin, GPIO_PIN_RESET);
	  	  HAL_Delay(x);
	  	  HAL_GPIO_WritePin(pinMap[12].port, pinMap[12].pin, GPIO_PIN_RESET);
	  	  HAL_Delay(x);
	  	  HAL_GPIO_WritePin(pinMap[13].port, pinMap[13].pin, GPIO_PIN_RESET);
	  	  HAL_Delay(x);
	  	  HAL_GPIO_WritePin(pinMap[14].port, pinMap[14].pin, GPIO_PIN_RESET);
	  	  HAL_Delay(x);
	      HAL_GPIO_WritePin(pinMap[15].port, pinMap[15].pin, GPIO_PIN_RESET);
	      HAL_Delay(x);
	  	  HAL_GPIO_WritePin(pinMap[11].port, pinMap[11].pin, GPIO_PIN_RESET);
	      HAL_Delay(x);
	  	  HAL_GPIO_WritePin(pinMap[7].port, pinMap[7].pin, GPIO_PIN_RESET);
	  	  HAL_Delay(x);
	  	  HAL_GPIO_WritePin(pinMap[3].port, pinMap[3].pin, GPIO_PIN_RESET);
	  	  HAL_Delay(x);
	  	  HAL_GPIO_WritePin(pinMap[2].port, pinMap[2].pin, GPIO_PIN_RESET);
	  	  HAL_Delay(x);
	      HAL_GPIO_WritePin(pinMap[1].port, pinMap[1].pin, GPIO_PIN_RESET);
	  	  HAL_Delay(x);
	      HAL_GPIO_WritePin(pinMap[0].port, pinMap[0].pin, GPIO_PIN_RESET);
	  	  HAL_Delay(x);
	  	      HAL_GPIO_WritePin(pinMap[0].port, pinMap[0].pin, GPIO_PIN_SET);
	  		  HAL_Delay(x);
	  		  HAL_GPIO_WritePin(pinMap[4].port, pinMap[4].pin, GPIO_PIN_SET);
	  		  HAL_Delay(x);
	  		  HAL_GPIO_WritePin(pinMap[8].port, pinMap[8].pin, GPIO_PIN_SET);
	  		  HAL_Delay(x);
	  		  HAL_GPIO_WritePin(pinMap[12].port, pinMap[12].pin, GPIO_PIN_SET);
	  		  HAL_Delay(x);
	  		  HAL_GPIO_WritePin(pinMap[13].port, pinMap[13].pin, GPIO_PIN_SET);
	  		  HAL_Delay(x);
	  		  HAL_GPIO_WritePin(pinMap[14].port, pinMap[14].pin, GPIO_PIN_SET);
	  		  HAL_Delay(x);
	  		  HAL_GPIO_WritePin(pinMap[15].port, pinMap[15].pin, GPIO_PIN_SET);
	  		  HAL_Delay(x);
	  		  HAL_GPIO_WritePin(pinMap[11].port, pinMap[11].pin, GPIO_PIN_SET);
	  		  HAL_Delay(x);
	  		  HAL_GPIO_WritePin(pinMap[7].port, pinMap[7].pin, GPIO_PIN_SET);
	  		  HAL_Delay(x);
	  		  HAL_GPIO_WritePin(pinMap[3].port, pinMap[3].pin, GPIO_PIN_SET);
	  		  HAL_Delay(x);
	  		  HAL_GPIO_WritePin(pinMap[2].port, pinMap[2].pin, GPIO_PIN_SET);
	  		  HAL_Delay(x);
	  		  HAL_GPIO_WritePin(pinMap[1].port, pinMap[1].pin, GPIO_PIN_SET);
	  		  HAL_Delay(x);
	  		  HAL_GPIO_WritePin(pinMap[5].port, pinMap[5].pin, GPIO_PIN_SET);
	  		  HAL_Delay(x);
	  		  HAL_GPIO_WritePin(pinMap[9].port, pinMap[9].pin, GPIO_PIN_SET);
	  		  HAL_Delay(x);
	  		  HAL_GPIO_WritePin(pinMap[10].port, pinMap[10].pin, GPIO_PIN_SET);
	  		  HAL_Delay(x);
	  		  HAL_GPIO_WritePin(pinMap[6].port, pinMap[6].pin, GPIO_PIN_SET);
	  		  HAL_Delay(x);
	  		          HAL_GPIO_WritePin(pinMap[6].port, pinMap[6].pin, GPIO_PIN_RESET);
	  			  	  HAL_Delay(x);
	  			  	  HAL_GPIO_WritePin(pinMap[10].port, pinMap[10].pin, GPIO_PIN_RESET);
	  			  	  HAL_Delay(x);
	  			  	  HAL_GPIO_WritePin(pinMap[9].port, pinMap[9].pin, GPIO_PIN_RESET);
	  			  	  HAL_Delay(x);
	  			  	  HAL_GPIO_WritePin(pinMap[5].port, pinMap[5].pin, GPIO_PIN_RESET);
	  			  	  HAL_Delay(x);
	  			  	  HAL_GPIO_WritePin(pinMap[1].port, pinMap[1].pin, GPIO_PIN_RESET);
	  			  	  HAL_Delay(x);
	  			  	  HAL_GPIO_WritePin(pinMap[2].port, pinMap[2].pin, GPIO_PIN_RESET);
	  			  	  HAL_Delay(x);
	  			  	  HAL_GPIO_WritePin(pinMap[3].port, pinMap[3].pin, GPIO_PIN_RESET);
	  			  	  HAL_Delay(x);
	  			  	  HAL_GPIO_WritePin(pinMap[7].port, pinMap[7].pin, GPIO_PIN_RESET);
	  			  	  HAL_Delay(x);
	  			  	  HAL_GPIO_WritePin(pinMap[11].port, pinMap[11].pin, GPIO_PIN_RESET);
	  			  	  HAL_Delay(x);
	  			      HAL_GPIO_WritePin(pinMap[15].port, pinMap[15].pin, GPIO_PIN_RESET);
	  			      HAL_Delay(x);
	  			  	  HAL_GPIO_WritePin(pinMap[14].port, pinMap[14].pin, GPIO_PIN_RESET);
	  			      HAL_Delay(x);
	  			  	  HAL_GPIO_WritePin(pinMap[13].port, pinMap[13].pin, GPIO_PIN_RESET);
	  			  	  HAL_Delay(x);
	  			  	  HAL_GPIO_WritePin(pinMap[12].port, pinMap[12].pin, GPIO_PIN_RESET);
	  			  	  HAL_Delay(x);
	  			  	  HAL_GPIO_WritePin(pinMap[8].port, pinMap[8].pin, GPIO_PIN_RESET);
	  			  	  HAL_Delay(x);
	  			      HAL_GPIO_WritePin(pinMap[4].port, pinMap[4].pin, GPIO_PIN_RESET);
	  			  	  HAL_Delay(x);
	  			      HAL_GPIO_WritePin(pinMap[0].port, pinMap[0].pin, GPIO_PIN_RESET);
	  			  	  HAL_Delay(x);
  }
}
//go through all leds one at a time
void goThroughAllLedsOneAtATime()
{
  int x = 15;
  disableAllPins();
  for(int y = 0; y<5; y++)
  {
    //0-3
    for(int count = 4; count != 0; count--)
    {
    	HAL_GPIO_WritePin(layers[count-1].port, layers[count-1].pin, GPIO_PIN_SET);
      for(int i = 0; i<4; i++)
      {
    	  HAL_GPIO_WritePin(pinMap[i].port, pinMap[i].pin, GPIO_PIN_RESET);
    	  HAL_Delay(x);
    	  HAL_GPIO_WritePin(pinMap[i].port, pinMap[i].pin, GPIO_PIN_SET);
        HAL_Delay(x);
      }
      HAL_GPIO_WritePin(layers[count-1].port, layers[count-1].pin, GPIO_PIN_RESET);
    }
    //4-7
    for(int count = 0; count < 4; count++)
    {
    	HAL_GPIO_WritePin(layers[count].port, layers[count].pin, GPIO_PIN_SET);
      for(int i = 4; i<8; i++)
      {
    	  HAL_GPIO_WritePin(pinMap[i].port, pinMap[i].pin, GPIO_PIN_RESET);
    	  HAL_Delay(x);
    	  HAL_GPIO_WritePin(pinMap[i].port, pinMap[i].pin, GPIO_PIN_SET);
    	  HAL_Delay(x);
      }
      HAL_GPIO_WritePin(layers[count].port, layers[count].pin, GPIO_PIN_RESET);
    }
    //8-11
    for(int count = 4; count != 0; count--)
    {
    	HAL_GPIO_WritePin(layers[count-1].port, layers[count-1].pin, GPIO_PIN_SET);
      for(int i = 8; i<12; i++)
      {
    	  HAL_GPIO_WritePin(pinMap[i].port, pinMap[i].pin, GPIO_PIN_RESET);
    	      	  HAL_Delay(x);
    	      	  HAL_GPIO_WritePin(pinMap[i].port, pinMap[i].pin, GPIO_PIN_SET);
    	      	  HAL_Delay(x);
      }
      HAL_GPIO_WritePin(layers[count-1].port, layers[count-1].pin, GPIO_PIN_RESET);
    }
    //12-15
    for(int count = 0; count < 4; count++)
    {
    	HAL_GPIO_WritePin(layers[count].port, layers[count].pin, GPIO_PIN_SET);
      for(int i = 12; i<16; i++)
      {
    	  HAL_GPIO_WritePin(pinMap[i].port, pinMap[i].pin, GPIO_PIN_RESET);
    	  HAL_Delay(x);
    	  HAL_GPIO_WritePin(pinMap[i].port, pinMap[i].pin, GPIO_PIN_SET);
    	  HAL_Delay(x);
      }
      HAL_GPIO_WritePin(layers[count].port, layers[count].pin, GPIO_PIN_RESET);
    }
  }
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Receive_IT(&huart1, &recv_char, 1);

//    HAL_UART_Receive_IT(&huart1, &recv_char, 1); //UART1 Interrupt call
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if(!strcmp(recv_str, on)){
	  		  spiralInAndOut();
	  		}
	  		if(!strcmp(recv_str, off)){
	  		   propeller();
	  	        }
	  //	flickerOn();
	//  turnOnAndOffAllByLayerUpAndDownNotTimed();
	 // turnOnAndOffAllByColumnSideways();
	 // layerstompUpAndDown();
	 // flickerOff();
	 // aroundEdgeDown();
	  //diagonalRectangle();
	 // propeller();
	  //spiralInAndOut();
	 // HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, SET);
	  //goThroughAllLedsOneAtATime();
	  //enableAllPins();

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 84;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 720;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 1999;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, PIN11_Pin|PIN12_Pin|PIN31_Pin|PIN13_Pin
                          |PIN32_Pin|LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LAYER1_Pin|LAYER2_Pin|LAYER3_Pin|PIN42_Pin
                          |PIN41_Pin|PIN34_Pin|PIN24_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LAYER4_Pin|PIN23_Pin|PIN14_Pin|PIN22_Pin
                          |PIN21_Pin|PIN33_Pin|PIN44_Pin|PIN43_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PIN11_Pin PIN12_Pin PIN31_Pin PIN13_Pin
                           PIN32_Pin LED_Pin */
  GPIO_InitStruct.Pin = PIN11_Pin|PIN12_Pin|PIN31_Pin|PIN13_Pin
                          |PIN32_Pin|LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : LAYER1_Pin LAYER2_Pin LAYER3_Pin PIN42_Pin
                           PIN41_Pin PIN34_Pin PIN24_Pin */
  GPIO_InitStruct.Pin = LAYER1_Pin|LAYER2_Pin|LAYER3_Pin|PIN42_Pin
                          |PIN41_Pin|PIN34_Pin|PIN24_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LAYER4_Pin PIN23_Pin PIN14_Pin PIN22_Pin
                           PIN21_Pin PIN33_Pin PIN44_Pin PIN43_Pin */
  GPIO_InitStruct.Pin = LAYER4_Pin|PIN23_Pin|PIN14_Pin|PIN22_Pin
                          |PIN21_Pin|PIN33_Pin|PIN44_Pin|PIN43_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
    if(huart->Instance == USART1 ){
        if(recv_char == '\r'){

	        if(!strcmp(recv_str, on)){
		   HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, SET);
		}
		if(!strcmp(recv_str, off)){
		    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, RESET);
	        }

		memset(recv_str, 0, i);
		i=0;
		}else{
		    if(recv_char == '\r' || recv_char == '\n'){
		} else{
		    recv_str[i++] = recv_char;
		}
	 }
	 HAL_UART_Receive_IT(&huart1, &recv_char, 1); //UART1 Interrupt call

    }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
