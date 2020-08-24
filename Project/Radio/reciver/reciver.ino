#include <VirtualWire.h> // khai báo thư viện VirtualWire

byte msg[VW_MAX_MESSAGE_LEN];// biến lưu dữ liệu nhận được
byte msgLen = VW_MAX_MESSAGE_LEN;

void setup()
{
  Serial.begin(9600);
  Serial.println("READY..........");
  vw_setup(1024); // cài đặt tốc độ truyền tín hiệu
  vw_set_rx_pin(2);// cài đặt chân digital để nhận tín hiệu
  vw_rx_start();// bắt đầu nhận tín hiệu
}

void loop()
{
  if (vw_get_message(msg, &msgLen)) // nếu có tín hiệu được truyền đến
  {
    Serial.print("got: ");
    for (int i = 0; i < msgLen; i++)
    {
      Serial.write(msg[i]);// in ký tự ra màn hình
    }
    Serial.println();
  }
}
