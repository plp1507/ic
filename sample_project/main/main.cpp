/*
 * SPDX-FileCopyrightText: 2021-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <string.h>
#include <stdio.h>
#include <Arduino.h>
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_gap_bt_api.h"
#include "esp_bt_device.h"
#include "esp_spp_api.h"
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

unsigned long Ts = 1000;


extern "C" void app_main(){
	
	Serial.begin(115200);
	pinMode(34, INPUT);
	SerialBT.begin("esp_");

	while(1){
	unsigned long tempo = millis();

	Serial.printf("%d", analogRead(34));
	Serial.printf("\n");

	while(millis()-tempo<Ts){}
	}
}
