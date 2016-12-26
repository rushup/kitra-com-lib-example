#include <stdio.h>
#include "kitra_input.h"
#include "kitra_output.h"
#include "kitra_packet_generator.h"
#include "kitra_small_utility.h"

void kitra_platform_send(char* buffer, uint32_t length)
{
  /*Print through serial*/
}


int main() {
    char chks[4];
    k_input_ldrgb_enable_disable msg = {K_INPUT_LDRGB_ENABLE_DISABLE,0,1};
    k_get_checksum((char*)&msg,sizeof(k_input_ldrgb_enable_disable),chks);
    printf("Hello, Lib working %d! \n",msg.enabled);

    return 0;
}
