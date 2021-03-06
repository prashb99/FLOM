#include <hive_map.hpp>
#include "radio_endpoint.h"
#include "messages.h"
#include "locations.h"

hmap::Location database(DATABASE);
RadioEndPoint radio_endpoint;
const byte address[6] = "00001";

void on_occupancy_msg(void *raw_msg)
{
    occupancy::Msg &o_msg = *static_cast<occupancy::Msg *>(raw_msg);
    Serial.write((byte *)(&o_msg.body), sizeof(occupancy::Body)); //writing a message
}

void setup()
{
    radio_endpoint.setup(address);
    Serial.begin(9600);

    database.subscribe<occupancy::Msg>(&on_occupancy_msg);
    database.bind(radio_endpoint); //setting up radio channel for communication
}

const unsigned int cycle_delay = 250;

void loop()
{
    //Serial.println(sizeof(occupancy::Body));
    delay(cycle_delay);
    database.cycle();
}

/*
#include "hive_map.h"
#include "hive_map.c"

#define PIR 2 //connect PIR sensor to pin 2 of arduino
#define LED 13 //connect LED for testing

int PIR_STATE = LOW;
int val = 0;
byte timer = 0;
bool cycle_flag = true;

typedef struct Occupancy_Space { //creating an occupancy space
    HiveMapSpaceId space;
    bool isOccupied;
} Occupancy_Space;

HiveMapNode(Occupancy_Space) node; //room defined by occupancy space

void setup_interrupts() {
  noInterrupts(); // disable interrupts
  // set up timer interput phase
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 31250; // compare match register (16MHz/256/2Hz)
  TCCR1B |= (1 << WGM12); // CTC mode
  TCCR1B |= (1 << CS12); // 256 prescaler
  TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
  interrupts(); // re-endable interrupts
}

ISR(TIMER1_COMPA_vect)
{
  TCNT1 = 34286; // timer duration
  cycle_flag = true;
  ++timer;
}

void setup(){
    pinMode(LED, OUTPUT);
    pinMode(PIR, INPUT);
    setup_interrupts();
    node.loc = 10; //room number
    node.goal_loc = 1; //floor number
    node.state_received = NULL;
    Serial.begin(9600);
}

void loop(){
    val = digitalRead(PIR);
    if (val == HIGH) {            // check if the input is HIGH
        digitalWrite(LED, HIGH);  // turn LED ON
        if (PIR_STATE == LOW) {
            Serial.println("Motion detected!");
            PIR_STATE = HIGH;
            node.state.isOccupied = true;
            update_node(&node, sizeof(node)); //update node
        }
    } 
    else {
        digitalWrite(LED, LOW); // turn LED OFF
        if (PIR_STATE == HIGH){
            Serial.println("Motion ended!");
            PIR_STATE = LOW;
            node.state.isOccupied = false;
            update_node(&node, sizeof(node));
        }
    }
    if(cycle_flag) {
        cycle_node(&node, sizeof(node));
        cycle_flag = false;
    }
}
*/
