#include <M5EPD.h>
#include <WiFi.h>

M5EPD_Canvas canvas(&M5.EPD);

int point[2][2];

void setup()
{
  // If WiFi is enabled it might interfere with touch interrupt
  WiFi.mode(WIFI_MODE_NULL);

  M5.begin(true, false, true, false, false);
  M5.EPD.SetRotation(90);
  M5.TP.SetRotation(90);
  M5.EPD.Clear(true);
  canvas.createCanvas(540, 960);
  canvas.setTextSize(5);
  canvas.drawString("Touch The Screen!", 20, 400);
  canvas.pushCanvas(0, 0, UPDATE_MODE_DU4);
}

void loop()
{
  M5.update();

  if(M5.BtnL.wasPressed() == true)
  {
    Serial.println("Touch: normal mode");
    M5.TP.sleep(false);
  }
  if(M5.BtnR.wasPressed() == true)
  {
    Serial.println("Touch: sleep mode");
    M5.TP.sleep(true);
  }
  if(M5.TP.avaliable())
  {
    if(!M5.TP.isFingerUp())
    {
      bool is_update = false;

      M5.TP.update();
      canvas.fillCanvas(0);
      for(int i = 0; i < 2; i++)
      {
        tp_finger_t FingerItem = M5.TP.readFinger(i);

        if((point[i][0] != FingerItem.x) || (point[i][1] != FingerItem.y))
        {
          is_update = true;
          point[i][0] = FingerItem.x;
          point[i][1] = FingerItem.y;
          canvas.fillRect(FingerItem.x - 50, FingerItem.y - 50, 100, 100, 15);
          Serial.printf("Finger ID:%d-->X: %d*C  Y: %d  Size: %d\n", FingerItem.id, FingerItem.x, FingerItem.y , FingerItem.size);
        }
      }
      if(is_update == true)
      {
        canvas.pushCanvas(0,0,UPDATE_MODE_DU4);
      }
    }
  }
}
