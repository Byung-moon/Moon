/*
  Copyright (C) 2009 Sung Ho Park
  Contact: ubinos.org@gmail.com

  This file is part of the exe_helloworld component of the Ubinos.

  GNU General Public License Usage
  This file may be used under the terms of the GNU
  General Public License version 3.0 as published by the Free Software
  Foundation and appearing in the file license_gpl3.txt included in the
  packaging of this file. Please review the following information to
  ensure the GNU General Public License version 3.0 requirements will be
  met: http://www.gnu.org/copyleft/gpl.html.

  GNU Lesser General Public License Usage
  Alternatively, this file may be used under the terms of the GNU Lesser
  General Public License version 2.1 as published by the Free Software
  Foundation and appearing in the file license_lgpl.txt included in the
  packaging of this file. Please review the following information to
  ensure the GNU Lesser General Public License version 2.1 requirements
  will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.

  Commercial Usage
  Alternatively, licensees holding valid commercial licenses may
  use this file in accordance with the commercial license agreement
  provided with the software or, alternatively, in accordance with the
  terms contained in a written agreement between you and rightful owner.
*/

/* -------------------------------------------------------------------------
	Include
 ------------------------------------------------------------------------- */
#include "../ubiconfig.h"

// standard c library include
#include <stdio.h>
#include <stdlib.h>
#include <sam4e.h>

// ubinos library include
#include "itf_ubinos/itf/bsp.h"
#include "itf_ubinos/itf/ubinos.h"
#include "itf_ubinos/itf/bsp_fpu.h"
#include "itf_ubinos/itf/ubik_mutex.h"
#include "itf_ubinos/itf/bsp_intr.h"

// chipset driver include
#include "ioport.h"
#include "pio/pio.h"

// new estk driver include
#include "lib_new_estk_api/itf/new_estk_led.h"
#include "lib_new_estk_api/itf/new_estk_glcd.h"
#include "lib_switch/itf/lib_switch.h"
#include "lib_motor_driver/itf/lib_motor_driver.h"
#include "lib_new_estk_api/itf/new_estk_ioport_set.h"
#include "lib_sensor/itf/lib_sensor.h"
#include "lib_EV3_sensor/itf/lib_EV3_sensor.h"
#include "lib_i2c/itf/lib_i2c.h"

// new bluetooth library include
#include "lib_bluetooth/itf/BT_Module_Interface.h"
#include "lib_bluetooth/itf/lib_BT.h"

//bluetooth module id
#define BLE_MODULE_ID_0		0X01
#define BLE_MODULE_ID_1		0X02
#define BLE_MODULE_ID_2		0X03
#define BLE_MODULE_ID_3		0X58

// custom library header file include
//#include "../../lib_default/itf/lib_default.h"

// user header file include

/* -------------------------------------------------------------------------
	Global variables
 ------------------------------------------------------------------------- */
#define R_MOTOR_PORT	0	//�������� ��Ʈ
#define L_MOTOR_PORT	1	//�޸�����Ʈ

#define LINETRACER_PORT	0	//����Ʈ���̼� ��Ʈ
#define LIGHT_SENSOR_PORT	1	//������ ��Ʈ
#define SOUND_SENSOR_PORT	3	//��ġ���� �� Ʈ


#define DEFAULT_SPEED	300//�⺻ ���� �ӵ�

#define STATE_NORMAL	0	//�ֹ� ��
#define STATE_ORDERED	1	//�ֹ� ������



#define STATE_WRONG		2	//�κ��� ��ǥ�������� �־�����
#define STATE_GOAL		3	//�κ��� ��ǥ������ ����������


#define EMPTY 0			//�ʹ� ����
#define FILLED 1		//�ʹ� ����



ev3_color_t ev3_color;	// ����κ���ġ
color_mode_t sensor_mode = COL_COLOR; //�÷����� ���
#define MAC_ARRAY_SIZE	5 // �÷����� ���

#define LOCK	1
#define UNLOCK	0

#define PLAY_TIME	60 // ���ð�
#define TIME_OUT	700//ȸ���Լ� Ÿ�Ӿƿ�


int flag = LOCK;
int plate;			//�ʹ� �׸� ����
int signal;			//��ȣ��
int table_num;		//���̺� ��ȣ  2 , 3 , 4
int current_state = STATE_NORMAL; //= STATE_CORRECT;
int current_color = WHITE;
int previous_color = WHITE;

int Gain1 = 5;	// ȸ�� �Լ� ���� ��� (����)
int Gain2 = 7;	// ȸ�� �Լ� ���� ��� (�ӵ�)

int linearr[8]; // ����Ʈ���̼� �迭
//-----------------------Blutooth �߰��� �۷ι� ����------------
uint8_t print_packet[DATA_SEND_BUFFER_SIZE] = {0,};
//int motorset=0;

//MAin Task Message Queue
msgq_pt BT_user_event_queue;
//Mutex for print_packe
mutex_pt _g_mutex;

//------------------------------------------------

/* -------------------------------------------------------------------------
	Prototypes
 ------------------------------------------------------------------------- */
//�½�ũ
void start_end_task(void);
void move_robot_task(void);
void check_state_task(void);

//�Լ�
void waiting(void);
void turn_right(int degree);
void turn_left(int degree);

void go_forward(int speed);
void go_forward_check(int speed, int table_num);
void go_backward(int speed);

//���ͷ�Ʈ ��ƾ
void sw1_isr(void);
void sw2_isr(void);

/* -------------------------------------------------------------------------
	Function Definitions
 ------------------------------------------------------------------------- */
int usrmain(int argc, char * argv[]) {
	int r;

	printf("\n\n\n\r");
	printf("================================================================================\n\r");
	printf("exe_ubinos_test (build time: %s %s)\n\r", __TIME__, __DATE__);
	printf("================================================================================\n\r");

	// �ʱ�ȭ
	motor_init();
	//encoder_init();
	sensor_init(EV3_LINE_FOLLOWER_SENSOR,0,NXT_DIGITAL_SENSOR_SONA,0); // ����Ʈ���̼�, ������ ���� �ʱ�ȭ
	ev3_sensor_init(LIGHT_SENSOR_PORT, sensor_mode); //������ �ʱ�ȭ
	glcd_init();
	//switch_init(sw1_isr, sw2_isr);

	//����Ʈ���̼� ��� ����
	modeSelectLine(1);

	// �½�ũ ����
	r = task_create(NULL, start_end_task, NULL, task_gethighestpriority(), 500, NULL);
			if (0 != r) {
				logme("fail at task_create\r\n");
			}

	r = task_create(NULL, check_state_task, NULL, task_gethighestpriority(), 500, NULL);
			if (0 != r) {
				logme("fail at task_create\r\n");
			}

	r = task_create(NULL, move_robot_task, NULL, task_gethighestpriority() , 500, NULL);
			if (0 != r) {
				logme("fail at task_create\r\n");
			}

	r = msgq_create(&BT_user_event_queue, sizeof(BT_Evt_t), MAIN_MSGQ_MAX_COUNT);
			if (0 != r) {
				logme("fail at task_create\r\n");
			}

	ubik_comp_start();
	glcd_printf("\nStart Robot\r\n");

	return 0;
}



// ��ġ���� üũ, ��� ���� �� 1�е� ����, �κ��� �ʱ���ġ ( ����0 )���� ����

void start_end_task(void) {
	//flag = LOCK;
	//waiting();

	//bsp_busywaitms(10);
	//turn_right(70);
	//sensor_init(EV3_LINE_FOLLOWER_SENSOR,0,0,0);
	//modeSelectLine(1);
	//glcd_printf("go");
	for(;;){
	if(!((current_state == STATE_ORDERED) && (table_num == ev3_sensor_get(LIGHT_SENSOR_PORT)))){
	go_forward(DEFAULT_SPEED);
	//bsp_busywaitms(1);
	}
	task_sleep(10);
	}
}


//�κ��� ���� ���¿� ���� ������ ����

void move_robot_task(void){



	while(1){
		switch(current_state){
	case STATE_NORMAL:
		//glcdGotoChar(4,2);
		//glcd_printf("GO");
		//go_forward(DEFAULT_SPEED);

		//bsp_busywaitms(20);
		task_sleep(10);

		break;
	case STATE_ORDERED:
		//glcdGotoChar(2,2);
		//glcd_printf("stop");
		if(table_num == ev3_sensor_get(LIGHT_SENSOR_PORT)){
		motor_set(R_MOTOR_PORT, 0);
		motor_set(L_MOTOR_PORT, 0);
		}
		break;
	case STATE_GOAL:
		glcdGotoChar(0,0);
		glcd_printf("Val : %d \r\n", ev3_sensor_get(0));
		//go_forward(DEFAULT_SPEED);
		bsp_busywaitms(3);

		//go_backward(DEFAULT_SPEED);
		bsp_busywaitms(1);
		break;

	default:
		glcdGotoChar(0,0);
		glcd_printf("Val : %d \r\n", ev3_sensor_get(0));
		go_forward(DEFAULT_SPEED);
		bsp_busywaitms(1);
		break;
		}
	task_sleep(2);
	}
}

//������ ���� �о�鿩 ���� �κ� ��ġ �� ���� üũ

void check_state_task(void){

	int i, j, cnt, state = 0;

	int r =0;
		module_id_st BT_ID;
		BT_Evt_t BT_usr_msgRXBuffer = { 0, };

		//set BT_module_ID
		BT_ID.module_id[0] = BLE_MODULE_ID_0;
		BT_ID.module_id[1] = BLE_MODULE_ID_1;
		BT_ID.module_id[2] = BLE_MODULE_ID_2;
		BT_ID.module_id[3] = BLE_MODULE_ID_3;

		task_sleep(200);
		BT_INIT(INIT_ROLE_PERIPHERAL, BT_ID, BT_user_event_queue);

		BT_ADV_START();

		for(;;){
				r = msgq_receive(BT_user_event_queue, (unsigned char*) &BT_usr_msgRXBuffer);
				if (0 != r){
				logme ("fail at msgq_receive\r\n" ) ;
				} else{
				   switch (BT_usr_msgRXBuffer.status){
				   case BT_EVT_PE_DATA_READ: {
				   //motor setting
				   mutex_lock(_g_mutex);
				   signal = (int)((BT_usr_msgRXBuffer.msg[1] << 8) | (BT_usr_msgRXBuffer.msg[0]));

				   for (int i=0; i<20; i++ ){
				  		      print_packet[i]=BT_usr_msgRXBuffer.msg[i];
				  		   }

				  		   mutex_unlock(_g_mutex);
				  		   glcdGotoChar(2,2);
				  		   glcd_printf("%d",signal); //signal �������
				  		   table_num = signal; // ���̺� ��ȣ�� ����
				  		   current_state = STATE_ORDERED; // ���� ���� ����
				  		   break;
				  		   //send same msg
				  		   //BT_DATA_SEND(INIT_ROLE_PERIPHERAL, print_packet);

				  		   task_sleep(10);
				  		   }
				  		   break;

				  		   case BT_EVT_DISCONNECTED:
				  			   break;

				  			    case BT_EVT_CONNECTED:

				  			       break;
				  			    }
				  		}
				  	}

	/*while(1){
		for(i = 0; i < MAC_ARRAY_SIZE; i++){
			color_array[i] = ev3_sensor_get(0);
			task_sleep(5);
			}

		for(i = 0; i < (MAC_ARRAY_SIZE - 1); i++){
			cnt = 1;
				for(j = i + 1; j < MAC_ARRAY_SIZE; j++){
					if(color_array[i]==color_array[j])
						cnt++;
					}
			if(cnt > (MAC_ARRAY_SIZE / 2)){
				current_color = color_array[i];
				break;
				}
			}

	state = current_color - previous_color;
	switch(current_color){
		case WHITE:
			current_state = STATE_NORMAL;
			break;
		case YELLOW:
		case GREEN:
		case BLUE:
		case RED:
			current_state = (state <= 0) ? STATE_CORRECT : STATE_WRONG;
			break;
		case BLACK:
			current_state = STATE_GOAL;
			break;
		default:
			break;
			}
	previous_color = current_color;
	task_sleep(20);
	}*/
}




//�κ��� ������ ������
void go_forward(int speed){
	for(;;){
	line_get(LINETRACER_PORT,linearr);
	if(abs(linearr[0])>=22){
	motor_set(R_MOTOR_PORT, 0.3*speed - 1.5*(linearr[0]-128));
	motor_set(L_MOTOR_PORT, 0.3*speed + 1.5*(linearr[0]-128));
	}
	else if(abs(linearr[0])<22){
	motor_set(R_MOTOR_PORT, 1.3*speed - 0.25*(linearr[0]-128));
	motor_set(L_MOTOR_PORT, 1.3*speed + 0.25*(linearr[0]-128));
	}

	task_sleep(10);
	}
}

void go_forward_check(int speed, int table_num){

}


// ��ư 1�� ������ ������ ����Ѵ�
void waiting(void){
	while(flag == LOCK){
	}
flag = LOCK;
}

//��ư 1�� 2�� ���ͷ�Ʈ ���� ��ƾ
void sw1_isr(void){
	flag = UNLOCK;
}

void sw2_isr(void){
}





//����κ��� ���������� �Է��� ������ŭ ȸ��
void turn_right(int degree){
	int speed = 300;
	int diff_degree = 0;
	int timeout = 0;

	encoder_reset(R_MOTOR_PORT);
	encoder_reset(L_MOTOR_PORT);
	timeout = TIME_OUT;

	do{
		diff_degree = (encoder_get(R_MOTOR_PORT) - encoder_get(L_MOTOR_PORT)) - (degree * Gain1);
		speed = Gain2 * diff_degree;
		motor_set(R_MOTOR_PORT, (-speed));
		motor_set(L_MOTOR_PORT, (speed));
		timeout--;
	}while((diff_degree != 0) && (timeout !=0));
}



//����κ��� �������� �Է��� ������ŭ ȸ��
void turn_left(int degree){
	int speed = 0;
	int diff_degree = 0;
	int timeout = 0;

	encoder_reset(R_MOTOR_PORT);
	encoder_reset(L_MOTOR_PORT);
	timeout = TIME_OUT;

	do{
		diff_degree = (encoder_get(L_MOTOR_PORT) - encoder_get(R_MOTOR_PORT)) - (degree * Gain1);
		speed = Gain2 * diff_degree;
		motor_set(R_MOTOR_PORT, (speed));
		motor_set(L_MOTOR_PORT, (-speed));
		timeout--;
	}while((diff_degree != 0) && (timeout !=0));
}








