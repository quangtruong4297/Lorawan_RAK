void RTU_Send_Request(String _module, int Addr, int Func, int RegAddr, int RegValue, int _serial) {
  uint8_t Buff[20], Index;
  uint16_t crc16;
  Index = 0;
  memset(Buff, '\0', sizeof(Buff));
  if (_module == "modbus") {
    Buff[Index++] = Addr;
    Buff[Index++] = Func;
    Buff[Index++] = RegAddr / 256;
    Buff[Index++] = RegAddr & 0xFF;
    switch (Func)
    {
      case 0x03:  // Read Holding Registers
        Buff[Index++] = RegValue >> 8;
        Buff[Index++] = RegValue & 0xFF;
        break;
      case 0x16:  // Read Holding Registers
        Buff[Index++] = RegValue >> 8;
        Buff[Index++] = RegValue & 0xFF;
        break;
      case 0x04:  // Read Output Register
        Buff[Index++] = RegValue >> 8;
        Buff[Index++] = RegValue & 0xFF;
        break;

      case 0x05:  // Write Single coils
        Buff[Index++] = RegValue >> 8;
        Buff[Index++] = RegValue & 0xFF;
        break;

      case 0x06:  // Write Single Holding Register
        Buff[Index++] = RegValue >> 24;
        Buff[Index++] = RegValue >> 16;
        Buff[Index++] = RegValue >> 8;
        Buff[Index++] = RegValue & 0xFF;
        break;
    }

    crc16 = ModRTU_CRC(Buff, Index);
    Buff[Index++]  = crc16 & 0xFF;
    Buff[Index++]  = crc16 >> 8; // split crc into 2 bytes

  }

  switch(_serial){
//    case 0: 
//      Serial.write(Buff, Index);
//      break;
    case 1:
      VM_serial.write(Buff, Index);
      break;
    case 2:
      Serial2.write(Buff, Index);
      break;
    default:
      break;
  }
#if DEBUG
  Serial.println("RTU_Send_Request:");
  for(int i = 0 ; i < Index; i++){
    Serial.print(Buff[i], HEX); Serial.print(" ");
  }
#endif
}
void modbus(byte _data[], int len) {
  int c = ModRTU_CRC(_data, len);
//  data_send[len] = byte(c);
//  data_send[len + 1] = byte(c >> 8);

}

unsigned int ModRTU_CRC(byte buf[], int len) {
  unsigned int crc = 0xFFFF;
  for (int pos = 0; pos < len; pos++) {
    crc ^= (unsigned int)buf[pos];
    for (int i = 8; i != 0; i--) {
      if ((crc & 0x0001) != 0) {
        crc >>= 1;
        crc ^= 0xA001;
      }
      else
        crc >>= 1;
    }
  }
  return crc;
}
int checksum(byte buf[], int len) {
  int c = ModRTU_CRC(buf, len - 2);
  byte crc[2];
  crc[0] = byte(c);
  crc[1] = byte(c >> 8);
  if (buf[len - 2] == crc[0] && buf[len - 1] == crc[1]) {
    return 1;
  } else return 0;
}
