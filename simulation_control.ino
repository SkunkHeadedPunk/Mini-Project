///one thing I am not sure about is how we output u.  U is the volatge value. Also we need to get locationtogo() just have to make that 0-360 degrees




//locatation where to
int locationtogo() = 0; //fixme to call location


///controler vvalues
int Kp = 0.645;
int ki = 0.41;
//int Kd = 0; // not needed but may
int Step = 7;

//deltay value for controller
int delayValue = 20;


////other values below here
int I = 0;
int e_past = 0;
int Ts = 0;
int Tc = currentTimeatStart;      //FIXME

////reference variables otherwise known as the rotary encoader
int r = location; //FIXME
int y = tommycode(); //FIXME                                 This grabs the current location of the rotary encoader remeber its like a zillion turns per wheel rotation.


void setup() {

 Serial.begin(115200);
  Serial.println("Dual MC33926 Motor Shield");
  md.init();
}

void loop() {
 static int e;
 static int D;
 static int u;

 //eror determining part of code
 r = locationtogo() ;
 y = tommycode();
 e = r-y;
//////
 
//checks for error
 if (Ts>0){
  D = (e - e_past)/Ts;
  e_past = e;
 }
 else{
  D = 0;  //maybe we need to pop a kd value in there if its not working well so here it is
 }

//now calculate I
I = I + Ts*e;

//now get controller output
u = Kp*e +Ki*I;

Ts = currentTime()-Tc; //FIXME
Tc = currentTime(); //FIXME
millis(delayValue - Ts);

}
