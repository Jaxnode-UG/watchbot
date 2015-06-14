int rcCarControl(String command);

/* Globals -------------------------------------------------------------------*/
Servo leftServo;
Servo rightServo;

/* This function is called once at start up ----------------------------------*/
void setup()
{
  //Register Spark function
  Spark.function("sumocar", setMotorpower);
  leftServo.attach(D0);
  rightServo.attach(D1);
}

/* This function loops forever --------------------------------------------*/
void loop()
{
  // Nothing to do here
}

/*******************************************************************************
 * Function Name  : setMotorpower
 * Description    : Parses the incoming API commands and sets the motor control
                    pins accordingly
 * Input          : RC Car commands
                    e.g.: args=0,180
                          args=90,135
 * Output         : Motor signals
 * Return         : 1 on success and -1 on fail
 *******************************************************************************/
int setMotorpower(String command) {

  int loc1 = 0;
  int left = 0;
  int right = 0;

  loc1 = command.indexOf(",");
  left = command.substring(0,loc1).toInt();
  right = command.substring(loc1+1).toInt();
  leftServo.write(left);
  rightServo.write(right);
  
  return 1;
}
