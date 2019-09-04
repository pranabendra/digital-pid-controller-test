#include <iostream>
#include <cstring>
#include <cctype>
using namespace std;

class Line {
    public:
        string line;
        int cursor_position;
        int decimal_position;
        int lower_limit;
        int upper_limit;
        bool enable;
        string front;
        string back;

        void initialize_line(string init_line) {
            line = init_line;
            cursor_position = 0;
            // decimal_position = line.find('.');

            for(int i=0; i<decimal_position; i++) {
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
                cursor_position++;
            }
        }

        void increment_digit() {
            
        }

        void decrement_digit() {

        }
};

void print(Line l[])
{
    int i;
    for(i=0; i<3; i++)
    {
        cout<<l[i].line<<endl;
    }
    cout<<endl;
}

int main() {
    Line line[3];

    line[0].initialize_line("  Kp   : 12345.06789");
    line[1].initialize_line("  Ti(s): 12345.06789");
    line[2].initialize_line("  Td(s): 12345.06789");

    print(line);
    line[1].select_line();
    print(line);
    line[2].deselect_line();
    print(line);
    
    return 0;
}
