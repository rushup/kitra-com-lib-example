#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <stdint.h>
#include "kitra_input.h"
#include "kitra_output.h"
#include "kitra_packet_generator.h"
#include "kitra_small_utility.h"
#include "serial.h"

serial kitra_serial;

void kitra_platform_send(char* buffer, uint32_t length)
{
  /*Print through serial*/
  kitra_serial.serialPuts(buffer);
  k_unlock_tx();
}

std::string wait_for_packet()
{
    std::string cmd;
    bool begin = false;
    while(1)
    {
        int n = kitra_serial.serialDataAvail();
        for(int i=0; i<n;i++)
        {
            int ris = kitra_serial.serialGetchar();
            if(ris >= 0 && ris <= 127)
            {
                if(cmd.length() > 1000) //something went wrong
                    cmd.clear();

                if(ris == '$')
                {
                    begin = true;
                    cmd.clear();
                }
                if(begin == true)
                {
                    cmd+=(char)ris;
                }
                if(ris == '\n')
                {
                    begin = false;
                    return cmd;
                }
            }
        }
    }

}


int main () {
    char response_obj[500];
    uint32_t packet_size;
    uint32_t optional_mask;

    if(kitra_serial.serialOpen("/dev/ttySAC3",115200) != -1)
    {
        printf("SERIAL FOUND\n");
        k_input_ldrgb_enable_disable msg1 = {K_INPUT_LDRGB_ENABLE_DISABLE,0,1};
        k_send_packet(&msg1,0);

        if(k_parse_packet_safe(wait_for_packet().c_str(),(void*)response_obj,&packet_size, &optional_mask) == PARSE_OK)
        {
            k_output_ack* ack = (k_output_ack*) response_obj;
            if(ack->id == K_OUTPUT_ACK)
            {
                printf("LED ENABLE ACKED\n");
            }
        }

        k_input_ldrgb_set msg2 = {K_INPUT_LDRGB_SET,0,0x0000FF,50,1};
        k_send_packet(&msg2,0);

        if(k_parse_packet_safe(wait_for_packet().c_str(),(void*)response_obj,&packet_size, &optional_mask) == PARSE_OK)
        {
            k_output_ack* ack = (k_output_ack*) response_obj;
            if(ack->id == K_OUTPUT_ACK)
            {
                printf("LED SET ACKED\n");
            }
        }
    }

    return 0;
}
