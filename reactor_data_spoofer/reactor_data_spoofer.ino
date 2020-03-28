int k;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(2000000);
  randomSeed(analogRead(A0));
}

/* Reactor Systems Interface Packet */

enum RSIPacketClass {
  Status = 'S',
  Data = 'D'
};

enum RSIPacketDescriptor
{
  Temperature = 1,
  FlowRate = 2,
  Pressure = 3,
  Scale = 4
};

typedef struct RSIPacket {
  uint8_t p_class;
  uint8_t p_descriptor;
  uint8_t ndata;
  uint8_t ldata;
  uint8_t data[40];
};

float virt_temp = 23, virt_flow = 0;
void loop() {
  RSIPacket packet = RSIPacket();
  packet.p_class = Data;
  packet.p_descriptor = Temperature;

  struct TemperaturePacket {
    uint16_t temperature_id;
    uint32_t timestamp;
    float datapoint;
  };
  packet.ndata = 4;
  packet.ldata = sizeof(struct TemperaturePacket);

  //    Serial.println();
  //    Serial.print("Class: ");
  //    Serial.println(packet.p_class);
  //    Serial.print("Descriptor: ");
  //    Serial.println(packet.p_descriptor);
  //    Serial.print("Size: ");
  //    Serial.print(packet.ndata);
  //    Serial.print("x");
  //    Serial.println(packet.ldata);
  for (int i = 0; i < packet.ndata; i++)
  {
    struct TemperaturePacket tp = TemperaturePacket();
    tp.temperature_id = i + 1;
    tp.timestamp = millis();
    tp.datapoint = virt_temp * i + (random(0, 10) - 5);
    memcpy(
      packet.data + i * sizeof(struct TemperaturePacket),
      &tp, sizeof(struct TemperaturePacket));
    delay(random(0, 50));

    //        Serial.print("\tID: ");
    //        Serial.println(tp.temperature_id);
    //        Serial.print("\tTimestamp: ");
    //        Serial.println(tp.timestamp);
    //        Serial.print("\tValue: ");
    //        Serial.println(tp.datapoint);
  }
  virt_temp += (random(0, 1) * (1 + fabs(virt_temp - 100) / 100.0));

  Serial.print("RSIP>>");
  Serial.write((uint8_t *) &packet, sizeof(RSIPacket));

  delay(random(100, 250));


  // FLOW RATE
  packet.p_class = Data;
  packet.p_descriptor = FlowRate;

  struct FlowPacket {
    uint16_t flow_id;
    uint32_t timestamp;
    float datapoint;
  };
  packet.ndata = 4;
  packet.ldata = sizeof(struct FlowPacket);

  //    Serial.println();
  //    Serial.print("Class: ");
  //    Serial.println(packet.p_class);
  //    Serial.print("Descriptor: ");
  //    Serial.println(packet.p_descriptor);
  //    Serial.print("Size: ");
  //    Serial.print(packet.ndata);
  //    Serial.print("x");
  //    Serial.println(packet.ldata);
  for (int i = 0; i < packet.ndata; i++)
  {
    struct FlowPacket fp = FlowPacket();
    fp.flow_id = i + 1;
    fp.timestamp = millis();
    fp.datapoint = virt_flow + (random(0, 10) - 5) / 20.0;
    memcpy(
      packet.data + i * sizeof(struct FlowPacket),
      &fp, sizeof(struct FlowPacket));
    delay(random(0, 50));

    //        Serial.print("\tID: ");
    //        Serial.println(tp.temperature_id);
    //        Serial.print("\tTimestamp: ");
    //        Serial.println(tp.timestamp);
    //        Serial.print("\tValue: ");
    //        Serial.println(tp.datapoint);
  }
  virt_flow += ((float)random(0, 10) / 10.0) * (fabs(virt_flow - 2) / 2.0);
  Serial.print("RSIP>>");
  Serial.write((uint8_t *) &packet, sizeof(RSIPacket));
  delay(random(100, 250));


  // Scale
  packet.p_class = Data;
  packet.p_descriptor = Scale;

  struct ScalePacket {
    uint16_t scale_id;
    uint32_t timestamp;
    float datapoint;
  };
  packet.ndata = 4;
  packet.ldata = sizeof(struct ScalePacket);

  //    Serial.println();
  //    Serial.print("Class: ");
  //    Serial.println(packet.p_class);
  //    Serial.print("Descriptor: ");
  //    Serial.println(packet.p_descriptor);
  //    Serial.print("Size: ");
  //    Serial.print(packet.ndata);
  //    Serial.print("x");
  //    Serial.println(packet.ldata);
  for (int i = 0; i < packet.ndata; i++)
  {
    struct ScalePacket s = ScalePacket();
    s.scale_id = i + 1;
    s.timestamp = millis();
    s.datapoint = i + k;
    k= s.datapoint;
    memcpy(
      packet.data + i * sizeof(struct ScalePacket),
      &s, sizeof(struct ScalePacket));
    delay(random(0, 50));

    //        Serial.print("\tID: ");
    //        Serial.println(tp.temperature_id);
    //        Serial.print("\tTimestamp: ");
    //        Serial.println(tp.timestamp);
    //        Serial.print("\tValue: ");
    //        Serial.println(tp.datapoint);
  }
  Serial.print("RSIP>>");
  Serial.write((uint8_t *) &packet, sizeof(RSIPacket));
  delay(random(100, 250));
}
