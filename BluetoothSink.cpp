#include "BluetoothSink.h"

void BluetoothSink::setupBluetoothSink(){
  a2dp_sink.set_bits_per_sample(32);  
  a2dp_sink.start("BT32bit");  
}