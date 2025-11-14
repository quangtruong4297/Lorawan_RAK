void setup_VM(){
  VM_serial.begin(9600);
}
void read_modbus_VM(){
  RTU_Send_Request("modbus", 1, 3, 51,8,1);
}
int wait_for_serial_VM_serial()
{
  int i = 0;
  int ret;
  for (i = 0; i < TIME_HANDLE; i++) {
    ret = serialEvent_VM_serial();
    if (ret >= 0)  return ret;
  }
  return -1;
}

int serialEvent_VM_serial() {

  byte request[MAX_BUFFER_SIZE];
  memset(request, 0, MAX_BUFFER_SIZE);
  byte buffSize = 0;

  while (VM_serial.available())  {

    byte recv = (byte)VM_serial.read();
    request[buffSize] = recv;
    buffSize++;
    if (buffSize >= MAX_BUFFER_SIZE) {
      break;
    }
    delay(5);
  }

  if (buffSize > 0) {
    VM_serial_handle(request, buffSize);
    return 1;
  }
  return -1;
}
void VM_serial_handle(byte buf[], unsigned char bufferSize) {
#if DEBUG
  for(int i = 0 ; i < bufferSize; i++){
    Serial.print(buf[i], HEX); Serial.print(" ");
  }
#endif

  if (!checksum(buf, bufferSize)) {
    return;
  }
  byte _id = buf[0];
  byte _func = buf[1];
  byte _len = buf[2];
  if (_id != 0x01) {
    return;
  }

  if (_func == 0x03) {
    if(buf[2] == 0x10)
      handle_function_read(buf, bufferSize);
  }
}

void handle_function_read(byte buf[], unsigned char bufferSize){
  if(bufferSize < 21) return;
  value_US[0].freq = buf[3] << 8 | buf[4];
  value_US[1].freq = buf[5] << 8 | buf[6];
  value_US[2].freq = buf[7] << 8 | buf[8];
  value_US[3].freq = buf[9] << 8 | buf[10];
  
  value_US[0].temp = buf[11] << 8 | buf[12];
  value_US[1].temp = buf[13] << 8 | buf[14];
  value_US[2].temp = buf[15] << 8 | buf[16];
  value_US[3].temp = buf[17] << 8 | buf[18];
  
}

void handle_function_modify(byte _new_id){

}

void send_to_485_VM_serial(byte _msg[], int _len) {
  VM_serial.write(_msg, _len);
}
void send_to_485_CR_serial(String str) {
  VM_serial.println(str);
}
