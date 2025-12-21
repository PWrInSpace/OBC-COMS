#include "sx1280-radio-config.h"

SX1280_t sx1280_radio;

void Radio_Setup(void) {

    SX1280Init(&sx1280_radio,NULL);
    SX1280HalReset(&sx1280_radio);
    SX1280SetPacketType(&sx1280_radio, PACKET_TYPE_LORA);
    SX1280SetRfFrequency(&sx1280_radio, 2400000000);

    ModulationParams_t modParams;
    modParams.PacketType = PACKET_TYPE_LORA;
    modParams.Params.LoRa.SpreadingFactor = LORA_SF7;
    modParams.Params.LoRa.Bandwidth       = LORA_BW_0400;
    modParams.Params.LoRa.CodingRate      = LORA_CR_4_5;
    SX1280SetModulationParams(&sx1280_radio, &modParams);

    PacketParams_t pktParams;
    pktParams.PacketType = PACKET_TYPE_LORA;
    pktParams.Params.LoRa.PreambleLength = 12;
    pktParams.Params.LoRa.HeaderType     = LORA_PACKET_VARIABLE_LENGTH;
    pktParams.Params.LoRa.PayloadLength  = 255;
    pktParams.Params.LoRa.CrcMode        = LORA_CRC_ON;
    pktParams.Params.LoRa.InvertIQ       = LORA_IQ_NORMAL;
    SX1280SetPacketParams(&sx1280_radio, &pktParams);

    SX1280SetTxParams(&sx1280_radio, 13, RADIO_RAMP_20_US);
}