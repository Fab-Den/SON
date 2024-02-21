import controlP5.*;
import processing.serial.*;

Serial port;

ControlP5 cp5;
PFont font;
String port_number = "COM3"; //A vérif à l'execution
int list_height = 23;
Button b1, b2;

void setup(){
 
  size(250,450);
  
  port = new Serial(this, port_number, 9600);
  cp5 = new ControlP5(this);
  font = createFont("arial", 15);
  
  b1 = cp5.addButton("ovni")
    .setPosition(50,80)
    .setSize(50,40)
    .setLabel("off")
    .setFont(font)
    .setSwitch(true)
    .setColorBackground(color(255,0,0))
    .setColorActive(color(0,255,0))
    ;
    
  b2 = cp5.addButton("rev")
    .setPosition(150,80)
    .setLabel("off")
    .setSize(50,40)
    .setFont(font)
    .setSwitch(true)
    .setColorBackground(color(255,0,0))
    .setColorActive(color(0,255,0))
    ;
    
  cp5.addSlider("volume")
    .setPosition(70, 180)
    .setSize(20, 150)
    .setRange(0, 100)
    .setValue(50)
    .setLabel("vol")
    .setLabelVisible(true)
    .setColorBackground(color(255,0,0))
    .setColorForeground(color(0,255,0))
    .setColorValue(color(0,0,0))
    .setColorActive(color(0,0,255))
    .setFont(font)
    ;
  
      cp5.addSlider("roomsize")
    .setPosition(160, 180)
    .setSize(20, 150)
    .setRange(0, 75)
    .setValue(30)
    .setColorBackground(color(255,0,0))
    .setColorForeground(color(0,255,0))
    .setColorValue(color(0,0,0))
    .setColorActive(color(0,0,255))
    .setFont(font)
    ;
    

  cp5.addDropdownList("mode")
    .setPosition(60,350)
    .setBarHeight(list_height)
    .setItemHeight(list_height)
    .setWidth(130)
    .setFont(createFont("calibri light", 12))
    .setLabel("select mode")
    .addItem("Normal mode", 1)
    .addItem("Major mode", 2)
    .addItem("Minor mode", 3)
    .setDefaultValue(0)
    .setOpen(false)
    .setDirection(-1)
  ;
  
}

void draw(){
  background(255, 255, 255); //background color RGB
   
  fill(0,0,0);
  textFont(createFont("calibri", 22));
  text("Autotune control panel", 25, 20);
  textFont(font);
  text("No smoothing", 35, 60, 10);
  text("Reverb", 150, 60);
  text("Volume", 50,150);
  text("Reverb control", 120, 150);
}
  
  
void volume(int volume){
  println(volume);
  port.write(volume); 
}

void roomsize(int roomsize){
  port.write(roomsize+100);
}

void ovni(boolean ovni){
  if (ovni) {
    println("send signal for true");
    port.write(205);
    b1.setLabel("on");
  } else {
    println("send signal for false");
    port.write(206);
    b1.setLabel("off");
  }
}

void rev(boolean rev){
  if (rev) {
    println("send signal for true");
    port.write(207);
    b2.setLabel("on");
  } else {
    println("send signal for false");
    port.write(208);
    b2.setLabel("off");
  }
}

void mode(int mode){
  if (mode == 0) {
    port.write(211);
  } else if (mode == 1){
    port.write(212);
  } else {
    port.write(213);
  }
}
