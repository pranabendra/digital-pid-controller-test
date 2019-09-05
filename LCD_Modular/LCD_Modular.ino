#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

class Line {
  public:
    String line;
    int cursor_position;
    int decimal_position;
    int lower_limit;
    int upper_limit;
    bool enable;
    String front;
    String back;

    void initialize_line(String init_line) {
      line = init_line;
      cursor_position = 13;
      // decimal_position = line.find('.');

      for (int i = 0; i < decimal_position; i++) {
        if (isdigit(line[i])) {
          lower_limit = i;
          break;
        }
      }

      decimal_position = 14;
      // lower_limit = 9;
      upper_limit = 19;
      front = " 123456789";
      back =  "0123456789";
    }

    void select_line() {
      line[0] = '-';
      line[1] = '>';
    }

    void deselect_line() {
      line[0] = ' ';
      line[1] = ' ';
    }

    void clear_number() {

    }

    int get_cursor_position() {
      return cursor_position;
    }

    void move_cursor_right() {
      cursor_position = ((cursor_position + 3) % 11) + 9;
      if (cursor_position == decimal_position) {
        cursor_position++;
      }
    }

    void move_cursor_left() {
      cursor_position = ((cursor_position + 1) % 11) + 9;
      if (cursor_position == decimal_position) {
        cursor_position--;
      }
    }

    void increment_digit() {

    }

    void decrement_digit() {

    }
};

// LCD Parameters
String line3 = "  SET RUN AUTO PID  ";
String modifiedLine = "";
unsigned long startTime = 0;
bool visited[4] = {false};
int lineNum = 0;
int index = 0;
int presentState = 0;
// String line = "";
int current_line = 0;
bool run_status = 0;

// Keypad Parameters
const int right = 39;
const int left = 41;
const int down = 43;
const int up = 45;
const int select_parameter = 47;
const int set_run = 49;
const int clear_digit = 51;
const int auto_set = 53;

int previous_input[8];
int current_input[8] = {0};
int button_press[8] = {0};

int button;
int button_pins[8] = {right, left, down, up, select_parameter, set_run, clear_digit, auto_set};

bool latch_status = false;
int latch_button = -1;

Line line[3];

void setup()
{

  line[0].initialize_line("  Kp   : 12345.06789");
  line[1].initialize_line("  Ti(s): 12345.06789");
  line[2].initialize_line("  Td(s): 12345.06789");
  line[0].select_line();
  lcd.init();
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print(line[0].line);
  lcd.setCursor(0, 1);
  lcd.print(line[1].line);
  lcd.setCursor(0, 2);
  lcd.print(line[2].line);
  lcd.setCursor(0, 3);
  lcd.print(line3);

  pinMode(right, INPUT);
  pinMode(left, INPUT);
  pinMode(down, INPUT);
  pinMode(up, INPUT);
  pinMode(select_parameter, INPUT);
  pinMode(set_run, INPUT);
  pinMode(clear_digit, INPUT);
  pinMode(auto_set, INPUT);

  Serial.begin(9600);
}

void loop()
{
  for (button = 0; button < 8; button++)
  {
    current_input[button] = digitalRead(button_pins[button]);
  }

  for (button = 0; button < 8; button++)
  {
    if (previous_input[button] - current_input[button] == -1 && !latch_status)
    {
      latch_status = true;
      latch_button = button;
    }

    if (latch_status)
    {
      if (button == latch_button)
      {
        button_press[button] = previous_input[button] - current_input[button];
      }
      else
      {
        button_press[button] = 0;
      }

      if (button_press[button] == 1)
      {
        latch_status = false;
      }
    }
  }

  // Begin

  if (button_press[0] == 1)
  {
    Serial.println("Button pressed = RIGHT");
    line[lineNum].move_cursor_right();
  }
  else if (button_press[1] == 1)
  {
    Serial.println("Button pressed = LEFT");
    line[lineNum].move_cursor_left();
  }
  else if (button_press[2] == 1)
  {
    Serial.println("Button pressed = DOWN");
  }
  else if (button_press[3] == 1)
  {
    Serial.println("Button pressed = UP");
  }
  else if (button_press[4] == 1)
  {
    Serial.println("Button pressed = SELECT");
    if (!run_status)
    {
      line[lineNum].deselect_line();
      lcd.setCursor(0, lineNum);
      lcd.print(line[lineNum].line);

      visited[lineNum] = false;
      lineNum = (lineNum + 1) % 3;
      line[lineNum].select_line();

      Serial.println("LINE NUMBER = " + lineNum);
      Serial.println(line[lineNum].line);
    }
  }
  else if (button_press[5] == 1)
  {
    Serial.println("Button pressed = SET/RUN");
    run_status = 1 - run_status;
    Serial.println(run_status);
    if (!run_status)
    {
      line[lineNum].select_line();
    }
    else
    {
      line[lineNum].deselect_line();
    }

  }
  else if (button_press[6] == 1)
  {
    Serial.println("Button pressed = CLEAR");
  }
  else if (button_press[7] == 1)
  {
    Serial.println("Button pressed = AUTO");
  }

  if (!visited[lineNum] && !run_status)
  {
    visited[lineNum] = true;
    index = line[lineNum].cursor_position;
    modifiedLine = line[lineNum].line;
    modifiedLine[index] = ' ';
    presentState = 1;
    startTime = millis();
    lcd.setCursor(0, lineNum);
    lcd.print(' ');

    // Serial.println(line0);
    Serial.println(modifiedLine);
    Serial.println("HI "+index);
    // Serial.println(startTime);
  }

  if ( (millis() - startTime >= 500) )
  {
    // Serial.println(startTime);

    index = line[lineNum].cursor_position;
    modifiedLine = line[lineNum].line;
    modifiedLine[index] = ' ';
    
    startTime = millis();
    presentState = 1 - presentState;
    if (presentState && !run_status)
    {
      lcd.setCursor(0, lineNum);
      lcd.print(modifiedLine);
      Serial.println(modifiedLine);
    }
    else
    {
      lcd.setCursor(0, lineNum);
      lcd.print(line[lineNum].line);
      Serial.println(line[lineNum].line);
    }
  }

  // End

  memset(button_press, 0, sizeof(button_press));
  memcpy(previous_input, current_input, sizeof(current_input));

  // delay(10);
}
