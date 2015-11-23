int Moisture_ain=A0;
int ad_value;
void setup()
{
  pinMode(Moisture_ain,INPUT);
  Serial.begin(9600);
}
void loop()
{
  ad_value=analogRead(Moisture_ain);
  /*if(ad_value>200)
  {*/
    Serial.print("Pot 1 ");
    Serial.println(ad_value);
  /*}
  else
  {
    Serial.println("Thirsty");
  }*/
  delay(500);
}
