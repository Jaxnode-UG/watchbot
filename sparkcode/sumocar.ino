int rcCarControl(String command);

/* Globals -------------------------------------------------------------------*/
Servo leftServo;
Servo rightServo;

/* This function is called once at start up ----------------------------------*/
void setup()
{
  //Register Spark function
  //Spark.function("sumocar", rcCarControl);
  Spark.function("sumocar", setMotorpower);
  leftServo.attach(D0);
  rightServo.attach(D1);
  //pinMode(D7,OUTPUT);
}

/* This function loops forever --------------------------------------------*/
void loop()
{
  // Nothing to do here
}

/*******************************************************************************
 * Function Name  : rcCarControl
 * Description    : Parses the incoming API commands and sets the motor control
                    pins accordingly
 * Input          : RC Car commands
                    e.g.: rc,FORWARD
                          rc,BACK
 * Output         : Motor signals
 * Return         : 1 on success and -1 on fail
 *******************************************************************************/
int rcCarControl(String command)
{
  if(command.substring(0,4) == "STOP")
  {

    leftServo.write(90);
    rightServo.write(90);

    return 1;
  }

  if(command.substring(0,4) == "BACK")
  {
    leftServo.write(0);
    rightServo.write(180);
    
    return 1;
  }

  if(command.substring(0,7) == "FORWARD")
  {
    leftServo.write(180);
    rightServo.write(0);

    return 1;
  }

  if(command.substring(0,5) == "RIGHT")
  {
    leftServo.write(180);
    rightServo.write(180);

    return 1;
  }

  if(command.substring(0,4) == "LEFT")
  {
    leftServo.write(0);
    rightServo.write(0);
    return 1;
  }

  // If none of the commands were executed, return false
  return -1;
}

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
