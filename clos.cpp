#include <cmath>
#include <complex>
#include <cstdlib>
#include <ctime>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <random>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

// Green:32, Red:31, Blue:34, White:37.

using namespace std;

// variables used for user variables
vector<string> keys;
vector<string> values;
int lengthOfVars = 0;

// Function to initialize the grid with '*'
vector<vector<char>> initializeGrid(int width, int height) {
    return vector<vector<char>>(height, vector<char>(width, '*'));
}

// Function to plot the line y = mx + b on the grid with decimal precision
void plotLine(vector<vector<char>>& grid, double m, double b, double precision) {
    int width = grid[0].size();
    int height = grid.size();
    int originX = width / 2;
    int originY = height / 2;

    // Draw x and y axes
    for (int x = 0; x < width; ++x) {
        grid[originY][x] = '-';
    }
    for (int y = 0; y < height; ++y) {
        grid[y][originX] = '|';
    }
    
    // Adjust coordinates to match the grid
    for (double x = -originX; x <= originX; x += precision) {
        int gridX = static_cast<int>(round(x + originX));
        int gridY = static_cast<int>(round(m * x + b));
        int gridYInverse = originY - gridY;

        if (gridX >= 0 && gridX < width && gridYInverse >= 0 && gridYInverse < height) {
            grid[gridYInverse][gridX] = '@'; // Plot the line point
        }
    }

    grid[originY][originX] = '+'; // Intersection of axes
}

// Function to print the grid
void printGrid(const vector<vector<char>>& grid) {
    int size = grid.size();
    int range = size / 2; // for 20x20, the range will be from -10 to 10

    // Print grid with y-axis labels
    for (int i = 0; i < size; ++i) {
        int yAxisLabel = range - i;
        if (i == range) {
            cout << "   "; // For center axis
        } else {
            if(yAxisLabel!=-10){
                cout << setw(2) << yAxisLabel << ' ';
            }else{
                cout << setw(2) << 10 << ' ';
            }
        }
        for (int j = 0; j < size; ++j) {
            if (grid[i][j] == '@') {
                cout << "\033[32m" << grid[i][j] << "\033[37m";
            } else {
                cout << grid[i][j];
            }
            cout << ' ';
        }
        cout << '\n';
    }

    // Print x-axis labels
    /*cout << "   ";
    for (int j = 0; j < size; ++j) {
        int xAxisLabel = j - range; // for 20x20, labels range from -10 to 10
        if (j == range) {
            cout << "0 ";
        } else {
            cout << setw(2) << xAxisLabel << ' ';
        }
    }*/
    cout << '\n';
}

static void
list_dir(const char *path) { // helper function one for the LS command
  struct dirent *entry;
  DIR *dir = opendir(path);
  if (dir == NULL) {
    return;
  }

  while ((entry = readdir(dir)) != NULL) {
    printf("%s\n", entry->d_name);
  }

  closedir(dir);
} // end of helper function one

string get_var(
    string key) { // helper function two for the VAR command and crud operations
  int i = 0;
  for (i = 0; i < lengthOfVars; i++) {
    if (keys[i] == key) {
      return values[i];
      break;
    }
  }
  if (i == lengthOfVars) {
    return "-1";
  }
  return "-1";
} // end of helper function two

int gcd(int a, int b) { // helper function three for the LCM and GCD commands
  if (b == 0) {
    return a;
  }
  return gcd(b, a % b);
} // end of helper function three

int fact(int a) { // helper founction for combination, permutation, and
                  // factorial commands
  if (a == 0 || a == 1) {
    return 1;
  } else {
    return a * fact(a - 1);
  }
}

int main() { // start of main OS code
  string version = "0.11";
  string command = "";
  double defaultNumVar = 0;
  vector<string> folders;
  double piVal = 3.141592653579;

  mkdir("home/", 0777);

  cout << "\033[37m" << '\n';
  cout << R"(          _             _             _            _
        /\ \           _\ \          /\ \         / /\
       /  \ \         /\__ \        /  \ \       / /  \
      / /\ \ \       / /_ \_\      / /\ \ \     / / /\ \__
     / / /\ \ \     / / /\/_/     / / /\ \ \   / / /\ \___\
    / / /  \ \_\   / / /         / / /  \ \_\  \ \ \ \/___/
   / / /    \/_/  / / /         / / /   / / /   \ \ \
  / / /          / / / ____    / / /   / / /_    \ \ \
 / / /________  / /_/_/ ___/\ / / /___/ / //_/\__/ / /
/ / /_________\/_______/\__\// / /____\/ / \ \/___/ /
\/____________/\_______\/    \/_________/   \_____\/
                                                            )"
       << '\n';
  cout << "Welcome to the Command Line Operating System." << '\n';
  cout << "Version: " << version << "." << '\n';
  while (true) {
    cout << "user@clos> ";
    cin >> command;

    if (command == "help" || command == "h") { // start of HELP
      cout << "This is the entire list of commands that CLOS has currently "
              "implemented. If you wish to see a command/functionality "
              "implemented in CLOS, kindly contact the developer."
           << '\n'
           << '\n';
      cout << "clear - Clears the command line interface." << '\n';
      cout << "help or h - Opens this help text." << '\n';
      cout << "about - Prints text that would show when CLOS starts." << '\n';
      cout << "exit - Exits CLOS. Optional delay in seconds available." << '\n';
      cout << "add - Add two numbers together. Optional extra addition "
              "available."
           << '\n';
      cout << "subtract or minus - Similar to add but with minus sign." << '\n';
      cout << "multiply - Multiplies two numbers." << '\n';
      cout << "divide - Divides two numbers. Throws error if division by zero "
              "occurs."
           << '\n';
      cout << "power or pwr - Raises first number to power of second number."
           << '\n';
      cout << "root - Takes root of first number using second number." << '\n';
      cout << "abs or absolute - Gives the absolute value of a given number."
           << '\n';
      cout << "round - Rounds a given number." << '\n';
      cout << "var - Creates a new temporary variable." << '\n';
      cout << "print - Prints a variable made using var command." << '\n';
      cout << "restart - Restarts CLOS. Optional delay in seconds available."
           << '\n';
      cout << "file - Creates a new file or rewrites exsisting one. Path can "
              "be specified and is entierly optional."
           << '\n';
      cout << "read - Reads a file. Path can be specified and is entierly "
              "optional."
           << '\n';
      cout << "mkdir - Makes a new directory. Special characters are not "
              "allowed. No nested directories are allowed, either."
           << '\n';
      cout << "delete - Deletes a file given the filename and optional file "
              "path."
           << '\n';
      cout << "rmdir - Removes the given directory. If the directory is not "
              "empty, it fails to do so."
           << '\n';
      cout << "ls - Lists the contents of a given folder." << '\n';
      cout << "iround - Rounds by concatenation (removing decimal part)." << '\n';
      cout << "solve - Equation Solver v0.3. Solves linear and quadratic "
              "functions with support of non real coefficients and answers."
           << '\n';
      cout << "doublesolve - Double Equation Solver v0.1. Solves a system of "
              "linear equations given the equations and bounds."
           << '\n';
      cout << "linegraph - Graphs a Linear Equation given the standard form."
           << '\n';
      cout << "funcstat - FUNCSTAT feature. Given the equation, upper, and "
              "lower bound, it outputs the list of (x,y) coordinate pairs of "
              "that function within the limits."
           << '\n';
      cout << "minmax - MINMAX feature. Given the quadratic equation, it can "
              "find the maximum or minumum of the function."
           << '\n';
      cout << "log - Finds natural and base 10 (or other base) logarithm of "
              "number."
           << '\n';
      cout << "sin or sine - Sine function." << '\n';
      cout << "cos or cosine - Cosine function." << '\n';
      cout << "tan or tangent - Tangent function." << '\n';
      cout << "csc or cosecant - Cosecant function." << '\n';
      cout << "sec or secant - Secant function." << '\n';
      cout << "cot or cotagent - Cotangent function." << '\n';
      cout << "asin or arcsine - Arc Sine function." << '\n';
      cout << "acos or arccosine - Arc Cosine function." << '\n';
      cout << "atan or arctangent - Arc Tangent function." << '\n';
      cout << "degrad - Converts Degrees to Radians and vice versa." << '\n';
      cout << "combination - Gives the combination n choose r given n and r."
           << '\n';
      cout << "permutation - Gives the permutation n permute r given n and r."
           << '\n';
      cout << "factorial - Gives the factorial of a given number n." << '\n';
      cout << "rand - Gives random number within given upper and lower bound."
           << '\n';
      cout << "summation or sum - Computes the summation value of a given "
              "function (linear or quadratic) or a constant."
           << '\n';
      cout << "deriv or derivative - Definite Derivative Calculator" << '\n';
      cout << "integral - Definite Integral Calculator" << '\n';
      cout << "indefder or indefderiv - Indefinite Derivative Calculator" << '\n';
      cout << "indefintegral - Indefinite Integral Calculator" << '\n';
      cout << "lcm - Finds the LCM of two given numbers." << '\n';
      cout << "gcd - Finds the GCD of two given numbers." << '\n';
      cout << "time or date - Displays the time and date of the system in GMT."
           << '\n';
      cout << "version or ver - Gives current OS version." << '\n';
    } // end of HELP

    else if (command == "exit") { // start of EXIT
      int delay;
      cout << "Delay? ";
      cin >> delay;
      sleep(delay);
      cout << "Exiting CLOS" << '\n';
      sleep(1);
      cout.flush();
      system("clear");
      return 0;
    } // end of EXIT

    else if (command == "add") { // start of ADD
      bool isContinuing = false;
      while (true) {
        double b = 0;
        if (!isContinuing) {
          cout << "\033[32m"
               << "Two numbers (enter as 'a b' without quotes): "
               << "\033[37m";
          string temp1 = "", temp2 = "";
          cin >> temp1 >> temp2;
          cout << '\n';

          if (temp1 == "pi" && temp2 != "pi") {
            defaultNumVar = piVal;
            b = stoi(temp2);
          } else if (temp2 == "pi" && temp1 != "pi") {
            defaultNumVar = stoi(temp1);
            b = piVal;
          } else if (temp1 == "pi" && temp2 == "pi") {
            defaultNumVar = piVal;
            b = piVal;
          } else if (temp1 == "var" && temp2 != "var") {
            cout << "Variable for first value: ";
            string key = "";
            cin >> key;
            defaultNumVar = stoi(get_var(key));
            b = stoi(temp2);
          } else if (temp2 == "var" && temp1 != "var") {
            defaultNumVar = stoi(temp1);
            cout << "Variable for second value: ";
            string key = "";
            cin >> key;
            b = stoi(get_var(key));
          } else if (temp1 == "var" && temp2 == "var") {
            cout << "Variable for first value: ";
            string key1 = "";
            cin >> key1;
            cout << "Variable for second value: ";
            string key2 = "";
            cin >> key2;
            defaultNumVar = stoi(get_var(key1));
            b = stoi(get_var(key2));
          } else {
            defaultNumVar = stoi(temp1);
            b = stoi(temp2);
          }

          cout << defaultNumVar + b << '\n';
        } else if (isContinuing) {
          cout << "\033[32m"
               << "Additional number (enter as 'a' without quotes): "
               << "\033[37m";
          string temp1 = "";
          cin >> temp1;
          if (temp1 == "pi") {
            b = piVal;
          } else if (temp1 == "var") {
            cout << "Variable for additional value: ";
            string key = "";
            cin >> key;
            b = stoi(get_var(key));
          } else {
            b = stoi(temp1);
          }
          cout << '\n';
          cout << defaultNumVar + b << '\n';
        }
        string cont = "";
        cout << "\033[32m"
             << "Continue(Y/n)? "
             << "\033[37m";
        cin >> cont;
        if (cont == "Y" || cont == "y") {
          defaultNumVar = defaultNumVar + b;
          isContinuing = true;
          continue;
        } else {
          cout << "\033[34m"
               << "Final result: " << defaultNumVar + b << "\033[37m" << '\n';
          defaultNumVar = defaultNumVar + b;
          break;
        }
      }
    } // end of ADD

    else if (command == "subtract" || command == "minus") { // start of SUBTRACT
      bool isContinuing = false;
      while (true) {
        double b = 0;
        if (!isContinuing) {
          cout << "\033[32m"
               << "Two numbers (enter as 'a b' without quotes): "
               << "\033[37m";
          string temp1 = "", temp2 = "";
          cin >> temp1 >> temp2;
          cout << '\n';

          if (temp1 == "pi" && temp2 != "pi") {
            defaultNumVar = piVal;
            b = stoi(temp2);
          } else if (temp2 == "pi" && temp1 != "pi") {
            defaultNumVar = stoi(temp1);
            b = piVal;
          } else if (temp1 == "pi" && temp2 == "pi") {
            defaultNumVar = piVal;
            b = piVal;
          } else if (temp1 == "var" && temp2 != "var") {
            cout << "Variable for first value: ";
            string key = "";
            cin >> key;
            defaultNumVar = stoi(get_var(key));
            b = stoi(temp2);
          } else if (temp2 == "var" && temp1 != "var") {
            defaultNumVar = stoi(temp1);
            cout << "Variable for second value: ";
            string key = "";
            cin >> key;
            b = stoi(get_var(key));
          } else if (temp1 == "var" && temp2 == "var") {
            cout << "Variable for first value: ";
            string key1 = "";
            cin >> key1;
            cout << "Variable for second value: ";
            string key2 = "";
            cin >> key2;
            defaultNumVar = stoi(get_var(key1));
            b = stoi(get_var(key2));
          } else {
            defaultNumVar = stoi(temp1);
            b = stoi(temp2);
          }

          cout << defaultNumVar - b << '\n';
        } else if (isContinuing) {
          cout << "\033[32m"
               << "Additional number (enter as 'a' without quotes): "
               << "\033[37m";
          string temp1 = "";
          cin >> temp1;
          if (temp1 == "pi") {
            b = piVal;
          } else if (temp1 == "var") {
            cout << "Variable for additional value: ";
            string key = "";
            cin >> key;
            b = stoi(get_var(key));
          } else {
            b = stoi(temp1);
          }
          cout << '\n';
          cout << defaultNumVar - b << '\n';
        }
        string cont = "";
        cout << "\033[32m"
             << "Continue(Y/n)? "
             << "\033[37m";
        cin >> cont;
        if (cont == "Y" || cont == "y") {
          defaultNumVar = defaultNumVar - b;
          isContinuing = true;
          continue;
        } else {
          cout << "\033[34m"
               << "Final result: " << defaultNumVar - b << "\033[37m" << '\n';
          defaultNumVar = defaultNumVar - b;
          break;
        }
      }
    } // end of SUBTRACT

    else if (command == "multiply") { // start of MULTIPLY
      bool isContinuing = false;
      while (true) {
        double b = 0;
        if (!isContinuing) {
          cout << "\033[32m"
               << "Two numbers (enter as 'a b' without quotes): "
               << "\033[37m";
          string temp1 = "", temp2 = "";
          cin >> temp1 >> temp2;
          cout << '\n';

          if (temp1 == "pi" && temp2 != "pi") {
            defaultNumVar = piVal;
            b = stoi(temp2);
          } else if (temp2 == "pi" && temp1 != "pi") {
            defaultNumVar = stoi(temp1);
            b = piVal;
          } else if (temp1 == "pi" && temp2 == "pi") {
            defaultNumVar = piVal;
            b = piVal;
          } else if (temp1 == "var" && temp2 != "var") {
            cout << "Variable for first value: ";
            string key = "";
            cin >> key;
            defaultNumVar = stoi(get_var(key));
            b = stoi(temp2);
          } else if (temp2 == "var" && temp1 != "var") {
            defaultNumVar = stoi(temp1);
            cout << "Variable for second value: ";
            string key = "";
            cin >> key;
            b = stoi(get_var(key));
          } else if (temp1 == "var" && temp2 == "var") {
            cout << "Variable for first value: ";
            string key1 = "";
            cin >> key1;
            cout << "Variable for second value: ";
            string key2 = "";
            cin >> key2;
            defaultNumVar = stoi(get_var(key1));
            b = stoi(get_var(key2));
          } else {
            defaultNumVar = stoi(temp1);
            b = stoi(temp2);
          }

          cout << defaultNumVar * b << '\n';
        } else if (isContinuing) {
          cout << "\033[32m"
               << "Additional number (enter as 'a' without quotes): "
               << "\033[37m";
          string temp1 = "";
          cin >> temp1;
          if (temp1 == "pi") {
            b = piVal;
          } else if (temp1 == "var") {
            cout << "Variable for additional value: ";
            string key = "";
            cin >> key;
            b = stoi(get_var(key));
          } else {
            b = stoi(temp1);
          }
          cout << '\n';
          cout << defaultNumVar * b << '\n';
        }
        string cont = "";
        cout << "\033[32m"
             << "Continue(Y/n)? "
             << "\033[37m";
        cin >> cont;
        if (cont == "Y" || cont == "y") {
          defaultNumVar = defaultNumVar * b;
          isContinuing = true;
          continue;
        } else {
          cout << "\033[34m"
               << "Final result: " << defaultNumVar * b << "\033[37m" << '\n';
          defaultNumVar = defaultNumVar * b;
          break;
        }
      }
    } // end of MULTIPLY

    else if (command == "divide") { // start of DIVIDE
      bool isContinuing = false;
      while (true) {
        double b = 1;
        if (!isContinuing) {
          cout << "\033[32m"
               << "Two numbers (enter as 'a b' without quotes): "
               << "\033[37m";
          string temp1 = "", temp2 = "";
          cin >> temp1 >> temp2;
          cout << '\n';

          if (temp1 == "pi" && temp2 != "pi") {
            defaultNumVar = piVal;
            b = stoi(temp2);
          } else if (temp2 == "pi" && temp1 != "pi") {
            defaultNumVar = stoi(temp1);
            b = piVal;
          } else if (temp1 == "pi" && temp2 == "pi") {
            defaultNumVar = piVal;
            b = piVal;
          } else if (temp1 == "var" && temp2 != "var") {
            cout << "Variable for first value: ";
            string key = "";
            cin >> key;
            defaultNumVar = stoi(get_var(key));
            b = stoi(temp2);
          } else if (temp2 == "var" && temp1 != "var") {
            defaultNumVar = stoi(temp1);
            cout << "Variable for second value: ";
            string key = "";
            cin >> key;
            b = stoi(get_var(key));
          } else if (temp1 == "var" && temp2 == "var") {
            cout << "Variable for first value: ";
            string key1 = "";
            cin >> key1;
            cout << "Variable for second value: ";
            string key2 = "";
            cin >> key2;
            defaultNumVar = stoi(get_var(key1));
            b = stoi(get_var(key2));
          } else {
            defaultNumVar = stoi(temp1);
            b = stoi(temp2);
          }

          if (b == 0) {
            while (b == 0) {
              cout << "Division by Zero is undefined. Enter new second number:";
              cin >> b;
            }
          }
          cout << defaultNumVar / b << '\n';
        } else if (isContinuing) {
          cout << "\033[32m"
               << "Additional number (enter as 'a' without quotes): "
               << "\033[37m";
          string temp1 = "";
          cin >> temp1;
          if (temp1 == "pi") {
            b = piVal;
          } else if (temp1 == "var") {
            cout << "Variable for additional value: ";
            string key = "";
            cin >> key;
            b = stoi(get_var(key));
          } else {
            b = stoi(temp1);
          }
          if (b == 0) {
            while (b == 0) {
              cout
                  << "Division by Zero is undefined. Enter new second number: ";
              cin >> b;
            }
          }
          cout << '\n';
          cout << defaultNumVar / b << '\n';
        }
        string cont = "";
        cout << "\033[32m"
             << "Continue(Y/n)? "
             << "\033[37m";
        cin >> cont;
        if (cont == "Y" || cont == "y") {
          defaultNumVar = defaultNumVar / b;
          isContinuing = true;
          continue;
        } else {
          cout << "\033[34m"
               << "Final result: " << defaultNumVar / b << "\033[37m" << '\n';
          if (b != 0) {
            defaultNumVar = defaultNumVar / b;
          } else {
            defaultNumVar = 0;
          }
          break;
        }
      }
    } // end of DIVIDE

    else if (command == "power" || command == "pwr") { // start of POWER
      double b = 0;
      cout << "\033[32,"
           << "Two numbers (enter as 'a b' without quotes, for a to power of b): "
           << "\0333[37m";
      string temp1 = "", temp2 = "";
      cin >> temp1 >> temp2;
      cout << '\n';

      if (temp1 == "pi" && temp2 != "pi") {
        defaultNumVar = piVal;
        b = stoi(temp2);
      } else if (temp2 == "pi" && temp1 != "pi") {
        defaultNumVar = stoi(temp1);
        b = piVal;
      } else if (temp1 == "pi" && temp2 == "pi") {
        defaultNumVar = piVal;
        b = piVal;
      } else if (temp1 == "var" && temp2 != "var") {
        cout << "Variable for first value: ";
        string key = "";
        cin >> key;
        defaultNumVar = stoi(get_var(key));
        b = stoi(temp2);
      } else if (temp2 == "var" && temp1 != "var") {
        defaultNumVar = stoi(temp1);
        cout << "Variable for second value: ";
        string key = "";
        cin >> key;
        b = stoi(get_var(key));
      } else if (temp1 == "var" && temp2 == "var") {
        cout << "Variable for first value: ";
        string key1 = "";
        cin >> key1;
        cout << "Variable for second value: ";
        string key2 = "";
        cin >> key2;
        defaultNumVar = stoi(get_var(key1));
        b = stoi(get_var(key2));
      } else {
        defaultNumVar = stoi(temp1);
        b = stoi(temp2);
      }

      cout << "\033[34m"
           << "Result: " << pow(defaultNumVar, b) << "\033[37m" << '\n';
      defaultNumVar = pow(defaultNumVar, b);
    } // end of POWER

    else if (command == "root") { // start of ROOT
      double b = 0;
      cout << "\033[32m"
           << "Two numbers (enter as 'a b' without quotes, for a root of b): "
           << "\033[37m";
      string temp1 = "", temp2 = "";
      cin >> temp1 >> temp2;
      cout << '\n';

      if (temp1 == "pi" && temp2 != "pi") {
        defaultNumVar = piVal;
        b = stoi(temp2);
      } else if (temp2 == "pi" && temp1 != "pi") {
        defaultNumVar = stoi(temp1);
        b = piVal;
      } else if (temp1 == "pi" && temp2 == "pi") {
        defaultNumVar = piVal;
        b = piVal;
      } else if (temp1 == "var" && temp2 != "var") {
        cout << "Variable for first value: ";
        string key = "";
        cin >> key;
        defaultNumVar = stoi(get_var(key));
        b = stoi(temp2);
      } else if (temp2 == "var" && temp1 != "var") {
        defaultNumVar = stoi(temp1);
        cout << "Variable for second value: ";
        string key = "";
        cin >> key;
        b = stoi(get_var(key));
      } else if (temp1 == "var" && temp2 == "var") {
        cout << "Variable for first value: ";
        string key1 = "";
        cin >> key1;
        cout << "Variable for second value: ";
        string key2 = "";
        cin >> key2;
        defaultNumVar = stoi(get_var(key1));
        b = stoi(get_var(key2));
      } else {
        defaultNumVar = stoi(temp1);
        b = stoi(temp2);
      }

      if (defaultNumVar < 0) {
        while (defaultNumVar < 0) {
          cout << "Root of negative number does not exist (in Reals)."
               << "\033[32m"
               << "Enter "
                  "new first number:"
               << "\033[37m";
          cin >> defaultNumVar;
        }
      }
      b = 1 / b;
      cout << '\n';
      cout << "\033[34m"
           << "Result: " << pow(defaultNumVar, b) << "\033[37m" << '\n';
      defaultNumVar = pow(defaultNumVar, b);
    } // end of ROOT

    else if (command == "abs" || command == "absolute") { // start of ABS
      cout << "\033[32m"
           << "Number: "
           << "\033[37m";
      double b = 0;
      cin >> b;

      if (b >= 0) {
        cout << "\033[34m" << b << "\033[37m" << '\n';
      } else {
        cout << "\033[34m" << -1 * b << "\033[37m" << '\n';
      }
    } // end of ABS

    else if (command == "round") { // start of ROUND
      cout << "\033[32m"
           << "Number: "
           << "\033[37m";
      double b = 0;
      cin >> b;

      cout << "\033[34m" << round(b) << "\033[37m" << '\n';
    } // end of ROUND
      
    else if (command == "iround") { // start of IROUND
        
    } // end of IROUND

    else if (command == "clear" || command == "cls") { // start of CLEAR
      cout.flush();
      system("clear");
    } // end of CLEAR

    else if (command == "about") { // start of ABOUT
      cout << R"(          _             _             _            _
        /\ \           _\ \          /\ \         / /\
       /  \ \         /\__ \        /  \ \       / /  \
      / /\ \ \       / /_ \_\      / /\ \ \     / / /\ \__
     / / /\ \ \     / / /\/_/     / / /\ \ \   / / /\ \___\
    / / /  \ \_\   / / /         / / /  \ \_\  \ \ \ \/___/
   / / /    \/_/  / / /         / / /   / / /   \ \ \
  / / /          / / / ____    / / /   / / /_    \ \ \
 / / /________  / /_/_/ ___/\ / / /___/ / //_/\__/ / /
/ / /_________\/_______/\__\// / /____\/ / \ \/___/ /
\/____________/\_______\/    \/_________/   \_____\/
                                                            )"
           << '\n';
      cout << "CLOS System - Command Line Operating System" << '\n';
      cout << "Version: " << version << "." << '\n';
    } // end of ABOUT

    else if (command == "var") { // start of VAR
      string key = "", val = "";
      cout << "\033[32m"
           << "Key and Value: "
           << "\033[37m";
      cin >> key >> val;
      keys.push_back(key);
      values.push_back(val);
      lengthOfVars += 1;
      cout << "\033[34m"
           << "Done."
           << "\033[37m" << '\n';
    } // end of VAR

    else if (command == "print") { // start of PRINT
      cout << "\033[32m"
           << "Variable name: "
           << "\033[37m";
      string key = "";
      cin >> key;
      int i = 0;
      for (i = 0; i < lengthOfVars; i++) {
        if (keys[i] == key) {
          cout << "\033[32m"
               << "Value: " << values[i] << "\033[37m" << '\n';
          break;
        }
      }
      if (i == lengthOfVars) {
        cout << "\033[31m"
             << "That variable cannot be found."
             << "\033[37m" << '\n';
      }
    } // end of PRINT

    else if (command == "restart") { // start of RESTART
      int delay;
      cout << "Delay? ";
      cin >> delay;
      sleep(delay);
      cout << "Exiting CLOS" << '\n';
      sleep(1);
      cout.flush();
      system("clear");
      sleep(4);
      cout << "\033[34m"
           << "restarted"
           << "\033[37m" << '\n';
      cout << '\n';
      cout << R"(          _             _             _            _
        /\ \           _\ \          /\ \         / /\
       /  \ \         /\__ \        /  \ \       / /  \
      / /\ \ \       / /_ \_\      / /\ \ \     / / /\ \__
     / / /\ \ \     / / /\/_/     / / /\ \ \   / / /\ \___\
    / / /  \ \_\   / / /         / / /  \ \_\  \ \ \ \/___/
   / / /    \/_/  / / /         / / /   / / /   \ \ \
  / / /          / / / ____    / / /   / / /_    \ \ \
 / / /________  / /_/_/ ___/\ / / /___/ / //_/\__/ / /
/ / /_________\/_______/\__\// / /____\/ / \ \/___/ /
\/____________/\_______\/    \/_________/   \_____\/
                                                            )"
           << '\n';
      cout << "Welcome to the Command Line Operating System." << '\n';
      cout << "Version: " << version << "." << '\n';
    } // end of RESTART

    else if (command == "file") { // start of FILE
      cout << "\033[32m"
           << "Filename (with extension and optional path): "
           << "\033[37m";
      string fileName = "";
      cin >> fileName;
      fileName = "home/" + fileName;
      ofstream tempFile(fileName);
      cout.flush();
      system("clear");
      cout << "Editing " + fileName << '\n';
      cout << "To end type q and hit Enter on new line."
           << "\033[32m" << '\n';
      while (true) {
        string input;
        getline(cin, input);
        if (input == "q") {
          cout.flush();
          system("clear");
          break;
        }
        tempFile << input + "\n";
      }
      cout << "\033[37m";
      tempFile.close();
    } // end of FILE

    else if (command == "read") { // start of READ
      cout << "\033[32m"
           << "Filename (with extension and optional path): "
           << "\033[37m";
      string fileName = "";
      cin >> fileName;
      fileName = "home/" + fileName;
      string output = "";
      ifstream tempFile(fileName);
      while (getline(tempFile, output)) {
        cout << output << '\n';
      }
    } // end of READ

    else if (command == "mkdir") { // start of MKDIR
      cout << "\033[32m"
           << "Folder name (no spaces): "
           << "\033[37m";

      string temp = "";
      cin >> temp;
      temp = "home/" + temp;
      char *folder = const_cast<char *>(temp.c_str());
      int check = 0;
      check = mkdir(folder, 0777);
      if (!check) {
        cout << "Directory " << folder << " created." << '\n';
      } else {
        cout << "\033[31m"
             << "Unable to create directory " << folder << "." << '\n'
             << "Make sure that the folder does not have any special "
                "characters except '-' and '_'."
             << "\033[37m" << '\n';
      }
    } // end of MKDIR

    else if (command == "delete") { // start of DELETE
      cout << "\033[32m"
           << "Filename (with extension): "
           << "\033[37m";
      string temp = "";
      cin >> temp;
      temp = "home/" + temp;
      char *filename = const_cast<char *>(temp.c_str());
      int check = 0;
      check = remove(filename);
      if (!check) {
        cout << "File " << filename << " deleted." << '\n';
      } else {
        cout << "\033[31m"
             << "Unable to delete the file " << filename
             << ". Maybe the filename was mistyped?" << '\n'
             << "If not, make sure that you are the correct user and the file "
                "exists."
             << "\033[37m" << '\n';
      }
    } // end of DELETE

    else if (command == "rmdir") { // start of RMDIR
      cout << "\033[32m"
           << "Folder name (Folder MUST be empty): "
           << "\033[37m";
      string temp = "";
      cin >> temp;
      temp = "home/" + temp;
      char *folder = const_cast<char *>(temp.c_str());
      int check = 0;
      check = rmdir(folder);
      if (!check) {
        cout << "Folder " << folder << " deleted." << '\n';
      } else {
        cout << "\033[31m"
             << "Unable to remove the directory " << folder
             << ". Maybe the foldername was mistyped?" << '\n'
             << "If not, make sure that you are the correct user, the folder "
                "exists, and it is empty."
             << "\033[37m" << '\n';
      }
    } // end of RMDIR

    else if (command == "ls") { // start of LS
      cout << "\033[32m"
           << "Folder name (There should be no spaces. Type '/' for the home "
              "folder): "
           << "\033[37m";

      string temp = "";
      cin >> temp;
      temp = "home/" + temp;
      char *folder = const_cast<char *>(temp.c_str());
      list_dir(folder);
    } // end of LS

    else if (command == "solve") { // start of SOLVE
      system("clear");
      cout << "Equation Solver v0.3" << '\n';
      cout << "This solver can be used to calculate answers for both Linear "
              "and Quadratic equations."
           << '\n';
      cout << "\033[31m"
           << "(IMPORTANT NOTE) Imaginary answers for Quadratic equations are "
              "printed "
              "and typed in "
              "the "
              "form of (r,i), where r is the real part and i is the "
              "imaginary part."
           << "\033[37m" << '\n';
      cout
          << "Example: (2,3) would be 2+3i. (4,0) would be 4. (0,5) would be 5i"
          << '\n';
      cout << '\n';
      cout << "\033[32m"
           << "Please enter the equation type (linear or quadratic): "
           << "\033[37m";
      string type = "";
      cin >> type;
      if (type == "linear") {
        double m = 1, b = 0;
        cout << "Function type chosen: Linear (y=mx + b)" << '\n';
        cout << "\033[32m"
             << "Enter the coefficient for x term (m. default m=1): "
             << "\033[37m";
        cin >> m;
        if (m == 0) {
          m = 1;
          cout << "\033[31m"
               << "Cannot use zero for m value. Using one. Continue (Y/N)?"
               << "\033[37m";
          string continu = "";
          cin >> continu;
          if (continu == "Y" || continu == "y") {
            cout << "\033[32m"
                 << "Enter the value for constant (b): "
                 << "\033[37m";
            cin >> b;
            double answer = (-1 * b) / m;
            cout << '\n'
                 << "\033[32m"
                 << "Answer is x=" << answer << "."
                 << "\033[37m" << '\n';
          } else {
            cout << '\n';
            system("clear");
          }
        } else {
          cout << "\033[32m"
               << "Enter the value for constant (b): "
               << "\033[37m";
          cin >> b;
          double answer = (-1 * b) / m;
          cout << '\n'
               << "\033[34m"
               << "Answer is x=" << answer << "."
               << "\033[37m" << '\n';
        }
      } else if (type == "quadratic") {
          complex<double> a = (static_cast<void>(0), 0), b = (static_cast<void>(0), 0), c = (static_cast<void>(0), 0);
        cout << "Function type chosen: Quadratic (y=ax^2 + bx + c)" << '\n';
        cout << "\033[32m"
             << "Enter the coefficient for x squared term (a. default a=1): "
             << "\033[37m";
        cin >> a;
        if (a == complex<double>(0, 0)) {
          a = complex<double>(1, 0);
          cout << "\033[31m"
               << "Cannot use zero for a value. Using one. Continue (Y/N)?"
               << "\033[37m";
          string continu = "";
          cin >> continu;
          if (continu == "Y" || continu == "y") {
            cout << "\033[32m"
                 << "Enter the coefficient for x term (b): "
                 << "\033[37m";
            cin >> b;
            cout << "\033[32m"
                 << "Enter the value for constant (c): "
                 << "\033[37m";
            cin >> c;

            complex<double> discr =
                sqrt((b * b) - (complex<double>(4, 0) * a * c));
            complex<double> numer1 = (complex<double>(-1, 0) * b) + discr;
            complex<double> numer2 = (complex<double>(-1, 0) * b) - discr;

            complex<double> ans1 = numer1 / (complex<double>(2, 0) * a);
            complex<double> ans2 = numer2 / (complex<double>(2, 0) * a);
            if (ans1 == ans2) {
              cout << '\n'
                   << "\033[34m"
                   << "Answer is x=" << ans1 << "."
                   << "\033[37m" << '\n';
            } else if (ans1 != ans2) {
              cout << '\n'
                   << "\033[34m"
                   << "Answers are x=" << ans1 << ", " << ans2 << "."
                   << "\033[37m" << '\n';
            }
          } else {
            cout << '\n';
            system("clear");
          }
        } else {
          cout << "\033[32m"
               << "Enter the coefficient for x term (b): "
               << "\033[37m";
          cin >> b;
          cout << "\033[32m"
               << "Enter the value for constant (c): "
               << "\033[37m";
          cin >> c;

          complex<double> discr =
              sqrt((b * b) - (complex<double>(4, 0) * a * c));
          complex<double> numer1 = (complex<double>(-1, 0) * b) + discr;
          complex<double> numer2 = (complex<double>(-1, 0) * b) - discr;

          complex<double> ans1 = numer1 / (complex<double>(2, 0) * a);
          complex<double> ans2 = numer2 / (complex<double>(2, 0) * a);
          if (ans1 == ans2) {
            cout << '\n'
                 << "\033[34m"
                 << "Answer is x=" << ans1 << "."
                 << "\033[37m" << '\n';
          } else if (ans1 != ans2) {
            cout << '\n'
                 << "\033[34m"
                 << "Answers are x=" << ans1 << ", " << ans2 << "."
                 << "\033[37m" << '\n';
          }
        }
      }
    } // end of SOLVE

    else if (command == "funcstat") { // start of FUNCSTAT
      cout << "\033[32m"
           << "Please enter the equation type (linear or quadratic): "
           << "\033[37m";
      string type = "";
      cin >> type;
      if (type == "linear") {
        double m = 1, b = 0;
        cout << "Function type chosen: Linear (y=mx + b)" << '\n';
        cout << "\033[32m"
             << "Enter the coefficient for x term (m. default m=1): "
             << "\033[37m";
        cin >> m;
        if (m == 0) {
          m = 1;
          cout << "\033[31m"
               << "Cannot use zero for m value. Using one. Continue (Y/N)?"
               << "\033[37m";
          string continu = "";
          cin >> continu;
          if (continu == "Y" || continu == "y") {
            cout << "\033[32m"
                 << "Enter the value for constant (b): "
                 << "\033[37m";
            cin >> b;

            cout << '\n'
                 << '\n'
                 << "\033[32m"
                 << "Enter Upper bound for x: "
                 << "\033[37m";
            double upper = 0;
            cin >> upper;
            cout << "\033[32m"
                 << "Enter Lower bound for x: "
                 << "\033[37m";
            double lower = 0;
            cin >> lower;
            if (upper < lower) {
              cout << "\033[31m"
                   << "Upper bound " << upper << " is less than Lower bound "
                   << lower << ". Cannot continue."
                   << "\033[37m" << '\n';
            } else {
              cout << "\033[34m"
                   << "X | Y" << '\n';
              for (double i = lower; i <= upper; i++) {
                cout << i << " | " << i * m + b << '\n';
              }
              cout << "\033[37m";
            }
          } else {
            cout << '\n';
            system("clear");
          }
        } else {
          cout << "\033[32m"
               << "Enter the value for constant (b): "
               << "\033[37m";
          cin >> b;

          cout << '\n'
               << '\n'
               << "\033[32m"
               << "Enter Upper bound for x: "
               << "\033[37m";
          double upper = 0;
          cin >> upper;
          cout << "\033[32m"
               << "Enter Lower bound for x: "
               << "\033[37m";
          double lower = 0;
          cin >> lower;
          if (upper < lower) {
            cout << "\033[31m"
                 << "Upper bound " << upper << " is less than Lower bound "
                 << lower << ". Cannot continue."
                 << "\033[37m" << '\n';
          } else {
            cout << "\033[34m"
                 << "X | Y" << '\n';
            for (double i = lower; i <= upper; i++) {
              cout << i << " | " << i * m + b << '\n';
            }
          }
          cout << "\033[37m";
        }
      } else if (type == "quadratic") {
        double a = 0, b = 0, c = 0;
        cout << "Function type chosen: Quadratic (y=ax^2 + bx + c)" << '\n';
        cout << "\033[32m"
             << "Enter the coefficient for x squared term (a. default a=1): "
             << "\033[37m";
        cin >> a;
        if (a == 0) {
          a = 1;
          cout << "\033[31m"
               << "Cannot use zero for a value. Using one. Continue (Y/N)?"
               << "\033[37m";
          string continu = "";
          cin >> continu;
          if (continu == "Y" || continu == "y") {
            cout << "\033[32m"
                 << "Enter the coefficient for x term (b): "
                 << "\033[37m";
            cin >> b;
            cout << "\033[32m"
                 << "Enter the value for constant (c): "
                 << "\033[37m";
            cin >> c;

            cout << '\n'
                 << '\n'
                 << "\033[32m"
                 << "Enter Upper bound for x: "
                 << "\033[37m";
            double upper = 0;
            cin >> upper;
            cout << "\033[32m"
                 << "Enter Lower bound for x: "
                 << "\033[37m";
            double lower = 0;
            cin >> lower;
            if (upper < lower) {
              cout << "\033[31m"
                   << "Upper bound " << upper << " is less than Lower bound "
                   << lower << ". Cannot continue."
                   << "\033[37m" << '\n';
            } else {
              cout << "\033[34m"
                   << "X | Y" << '\n';
              for (double i = lower; i <= upper; i++) {
                cout << i << " | " << i * i * a + i * b + c << '\n';
              }
            }
            cout << "\033[37m";
          } else {
            cout << '\n';
            system("clear");
          }
        } else {
          cout << "\033[32m"
               << "Enter the coefficient for x term (b): "
               << "\033[37m";
          cin >> b;
          cout << "\033[32m"
               << "Enter the value for constant (c): "
               << "\033[37m";
          cin >> c;

          cout << '\n'
               << '\n'
               << "\033[32m"
               << "Enter Upper bound for x: "
               << "\033[37m";
          double upper = 0;
          cin >> upper;
          cout << "\033[32m"
               << "Enter Lower bound for x: "
               << "\033[37m";
          double lower = 0;
          cin >> lower;
          if (upper < lower) {
            cout << "\033[31m"
                 << "Upper bound " << upper << " is less than Lower bound "
                 << lower << ". Cannot continue."
                 << "\033[37" << '\n';
          } else {
            cout << "\033[32m"
                 << "X | Y" << '\n';
            for (double i = lower; i <= upper; i++) {
              cout << i << " | " << i * i * a + i * b + c << '\n';
            }
          }
          cout << "\033[37m";
        }
      }
    } // end of FUNCSTAT

    else if (command == "minmax") { // start of MINMAX
      int a = 0, b = 0, c = 0;
      cout << "Minimum and Maximum finder for Quadratic equations (y=ax^2 + bx "
              "+ c)"
           << '\n';
      cout << "\033[32m"
           << "Enter the coefficient for x squared term (a. default a=1): "
           << "\033[37m";
      cin >> a;
      if (a == 0) {
        a = 1;
        cout << "\033[31m"
             << "Cannot use zero for a value. Using one. Continue (Y/N)?"
             << "\033[37m";
        string continu = "";
        cin >> continu;
        if (continu == "Y" || continu == "y") {
          cout << "\033[34m"
               << "Enter the coefficient for x term (b): "
               << "\033[37m";
          cin >> b;
          cout << "\033[34m"
               << "Enter the value for constant (c): "
               << "\033[37m";
          cin >> c;

          double xval = -1 * (b / (2 * a));
          double yval = a * xval * xval + b * xval + c;

          if (a > 0) {
            cout << "\033[34m"
                 << "Found minimum value of " << yval << " at " << xval << "."
                 << "\033[37m" << '\n';
          } else {
            cout << "\033[34m"
                 << "Found maximum value of " << yval << " at " << xval << "."
                 << "\033[37m" << '\n';
          }
        } else {
          cout << '\n';
          system("clear");
        }
      } else {
        cout << "\033[32m"
             << "Enter the coefficient for x term (b): "
             << "\033[37m";
        cin >> b;
        cout << "\033[32m"
             << "Enter the value for constant (c): "
             << "\033[37m";
        cin >> c;

        double xval = -1 * (b / (2 * a));
        double yval = a * xval * xval + b * xval + c;

        if (a > 0) {
          cout << "\033[34m"
               << "Found minimum value of " << yval << " at " << xval << "."
               << "\033[37m" << '\n';
        } else {
          cout << "\033[34m"
               << "Found maximum value of " << yval << " at " << xval << "."
               << "\033[37m" << '\n';
        }
      }
    } // end of MINMAX

    else if (command == "log") { // start of LOG
      cout << "\033[32m"
           << "Are you using natural logarithm(Y/n)? "
           << "\033[37m";
      string temp = "";
      cin >> temp;
      if (temp == "Y" || temp == "y") {
        cout << "\033[32m"
             << "Enter number (decimal): "
             << "\033[37m";
        double b = 0;
        cin >> b;
        cout << "\033[34m" << log(b) << "\033[37m" << '\n';
      } else {
        cout << "\033[32m"
             << "Enter base number (whole): "
             << "\033[37m";
        double base = 0;
        cin >> base;
        cout << "\033[32m"
             << "Enter input number (decimal): "
             << "\033[37m";
        double input = 0;
        cin >> input;

        if (base == 10) {
          cout << "\033[34m" << log10(input) << "\033[37m" << '\n';
        } else {
          cout << "\033[34m" << log10(input) / log10(base) << "\033[37m"
               << '\n';
        }
      }
    } // end of LOG

    else if (command == "sin" || command == "sine") { // start of SINE
      cout << "\033[32m"
           << "Enter angle in radians for sine (decimal): "
           << "\033[37m";
      double input = 0;
      cin >> input;

      double result = sin(input);
      cout << "\033[34m"
           << "Answer: " << result << "."
           << "\033[37m" << '\n';
    } // end of SINE

    else if (command == "cos" || command == "cosine") { // start of COS
      cout << "\033[32m"
           << "Enter angle in radians for cosine (decimal): "
           << "\033[37m";
      double input = 0;
      cin >> input;

      double result = cos(input);
      cout << "\033[34m"
           << "Answer: " << result << "."
           << "\033[37m" << '\n';
    } // end of COS

    else if (command == "tan" || command == "tangent") { // start of TAN
      cout << "\033[32m"
           << "Enter angle in radians for tangent (decimal): "
           << "\033[37m";
      double input = 0;
      cin >> input;

      double result = tan(input);
      cout << "\033[34m"
           << "Answer: " << result << "."
           << "\033[37m" << '\n';
    } // end of TAN

    else if (command == "csc" || command == "cosecant") { // start of CSC
      cout << "\033[32m"
           << "Enter angle in radians for cosecant (decimal): "
           << "\033[37m";
      double input = 0;
      cin >> input;

      double result = 1 / (sin(input));
      cout << "\033[34m"
           << "Answer: " << result << "."
           << "\033[37m" << '\n';
    } // end of CSC

    else if (command == "sec" || command == "secant") { // start of SEC
      cout << "\033[32m"
           << "Enter angle in radians (decimal) for secant (decimal): "
           << "\033[37m";
      double input = 0;
      cin >> input;

      double result = 1 / (cos(input));
      cout << "\033[34m"
           << "Answer: " << result << "."
           << "\033[37m" << '\n';
    } // end of SEC

    else if (command == "cot" || command == "cotangent") { // start of COT
      cout << "\033[32m"
           << "Enter angle in radians for cotangent (decimal): "
           << "\033[37m";
      double input = 0;
      cin >> input;

      double result = 1 / (tan(input));
      cout << "\033[34m"
           << "Answer: " << result << "."
           << "\033[37m" << '\n';
    } // end of COT

    else if (command == "asin" || command == "arcsine") { // start of ASIN
      cout << "\033[32m"
           << "Enter sine value (decimal): "
           << "\033[37m";
      double input = 0;
      cin >> input;

      if (input > 1 || input < -1) {
        cout << "\033[31m"
             << "Entered sine value is greater than one/less than negative one. Non real answer."
             << "\033[37m" << '\n';
      } else {
        double result = asin(input);
        cout << "\033[34m"
             << "Answer: " << result << "."
             << "\033[37m" << '\n';
      }
    } // end of ASIN

    else if (command == "acos" || command == "arccosine") { // start of ACOS
      cout << "\033[32m"
           << "Enter cosine value (decimal): "
           << "\033[37m";
      double input = 0;
      cin >> input;

      if (input > 1 || input < -1) {
        cout << "\033[31m"
             << "Entered cosine value is greater than one/less than negative one. Non real answer."
             << "\033[37m" << '\n';
      } else {
        double result = acos(input);
        cout << "\033[34m"
             << "Answer: " << result << "."
             << "\033[37m" << '\n';
      }
    } // end of ACOS

    else if (command == "atan" || command == "arctangent") { // start of ATAN
      cout << "\033[32m"
           << "Enter tangent value (decimal): "
           << "\033[37m";
      double input = 0;
      cin >> input;

      double result = atan(input);
      cout << "\033[34m"
           << "Answer: " << result << "."
           << "\033[37m" << '\n';
    } // end of ATAN

    else if (command == "degrad") { // start of DEGRAD
      cout << "\033[32m"
           << "Convert from radians or to radians (F for From/T for To)? "
           << "\033[37m";
      string input = "";
      cin >> input;

      if (input == "F" || input == "f") {
        double rad = 0;
        cout << "Enter radian value (decimal): ";
        cin >> rad;

        double deg = rad * (180 / piVal);
        cout << "\033[34m"
             << "Degrees: " << deg << '.' << "\033[37m" << '\n';
      } else if (input == "T" || input == "t") {
        double deg = 0;
        cout << "Enter degree value (decimal): ";
        cin >> deg;

        double rad = deg * (piVal / 180);
        cout << "\033[34m"
             << "Radians: " << rad << '.' << "\033[37m" << '\n';
      } else {
        cout << "Invalid character. Please try again.";
      }
    } // end of DEGRAD

    else if (command == "combination") { // start of COMBINATION
      cout << "Combination n choose r." << '\n';
      int n = 0, r = 0;

      cout << "Enter value for n: ";
      cin >> n;
      cout << "Enter value for r: ";
      cin >> r;

      if (n < r) {
        cout << "\033[31m"
             << "n is greater than r. Please try again."
             << "\033[37m" << '\n';
      } else {
        int result = fact(n) / (fact(r) * fact(n - r));
        cout << "\033[34m" << result << "\033[37m" << '\n';
      }
    } // end of COMBINATION

    else if (command == "permutation") { // start of PERMUTATION
      cout << "Permutation n permute r." << '\n';
      int n = 0, r = 0;

      cout << "Enter value for n (whole): ";
      cin >> n;
      cout << "Enter value for r (whole): ";
      cin >> r;

      if (n < r) {
        cout << "\033[31m"
             << "n is greater than r. Please try again."
             << "\033[37m" << '\n';
      } else {
        int result = fact(n) / fact(n - r);
        cout << "\033[34m" << result << "\033[37m" << '\n';
      }
    } // end of PERMUTATION

    else if (command == "factorial") { // start of FACTORIAL
      cout << "Enter number (whole): ";
      int n = 0;
      cin >> n;

      int result = fact(n);
      cout << "\033[34m" << result << "\033[37m" << '\n';
    } // end of FACTORIAL

    else if (command == "rand") { // start of RAND
      int lower = -1;
      int upper = 1;
      cout << "Enter lower bound for random number (whole): ";
      cin >> lower;
      cout << "Enter upper bound (whole): ";
      cin >> upper;

      if (lower > upper) {
        cout
            << "\033[31m"
            << "Sorry, upper bound is lower than lower bound. Please try again."
            << "\033[37m" << '\n';
      } else {
        random_device rand_dev;
        mt19937 generator(rand_dev());
        uniform_int_distribution<int> distr(lower, upper);

        cout << "\033[34m" << distr(generator) << "\033[37m" << '\n';
      }
    } // end of RAND

    else if (command == "summation" || command == "sum") { // start of SUMMATION
      cout << "\033[32m"
           << "Enter equation type (linear or quadratic. Use linear if you "
              "want a constant number): "
           << "\033[37m";
      string type = "";
      cin >> type;
      if (type == "linear") {
        cout << "Function type chosen: Linear (mx+b)" << '\n';
        double m = 0, b = 0;
        cout << "\033[32m"
             << "Enter value for x coefficient (enter 0 if you want a constant "
                "number): "
             << "\033[37m";
        cin >> m;
        cout << "\033[32m"
             << "Enter value for constant (b): "
             << "\033[37m";
        cin >> b;

        int lower = -1, upper = 1;
        cout << "\033[32m"
             << "Enter lower bound (inclusive): "
             << "\033[37m";
        cin >> lower;
        cout << "\033[32m"
             << "Enter upper bound (inclusive): "
             << "\033[37m";
        cin >> upper;

        if (upper < lower) {
          cout << "\033[31m"
               << "Sorry, upper bound is lower than lower bound. Please try "
                  "again."
               << "\033[37m" << '\n';
        } else {
          double answer = 0;
          for (int i = lower; i <= upper; i++) {
            answer += m * i + b;
          }
          cout << "\033[34m"
               << "Summation result: " << answer << "."
               << "\033[37m" << '\n';
        }
      } else if (type == "quadratic") {
        double a = 0, b = 0, c = 0;
        cout << "Function type chosen: Quadratic (y=ax^2 + bx + c)" << '\n';
        cout << "\033[32m"
             << "Enter the coefficient for x squared term (a. default a=1): "
             << "\033[37m";
        cin >> a;
        if (a == 0) {
          a = 1;
          cout << "\033[31m"
               << "Cannot use zero for a value. Using one. Continue (Y/N)?"
               << "\033[37m";
          string continu = "";
          cin >> continu;
          if (continu == "Y" || continu == "y") {
            cout << "\033[32m"
                 << "Enter the coefficient for x term (b): "
                 << "\033[37m";
            cin >> b;
            cout << "\033[32m <<"
                    "Enter the value for constant (c): "
                 << "\033[37m";
            cin >> c;

            int lower = -1, upper = 1;
            cout << "\033[32m"
                 << "Enter lower bound (inclusive): "
                 << "\033[37m";
            cin >> lower;
            cout << "\033[32m"
                 << "Enter upper bound (inclusive): "
                 << "\033[37m";
            cin >> upper;

            if (upper < lower) {
              cout
                  << "\033[31m"
                  << "Sorry, upper bound is lower than lower bound. Please try "
                     "again."
                  << "\033[37m" << '\n';
            } else {
              double answer = 0;
              for (int i = lower; i <= upper; i++) {
                answer += a * i * i + b * i + c;
              }
              cout << "\033[34m"
                   << "Summation result: " << answer << "."
                   << "\033[37m" << '\n';
            }
          } else {
            cout << '\n';
            system("clear");
          }
        } else {
          cout << "\033[32m"
               << "Enter the coefficient for x term (b): "
               << "\033[37m";
          cin >> b;
          cout << "\033[32m"
               << "Enter the value for constant (c): "
               << "\033[37m";
          cin >> c;

          int lower = -1, upper = 1;
          cout << "\033[32m"
               << "Enter lower bound (inclusive): "
               << "\033[37m";
          cin >> lower;
          cout << "\033[32m"
               << "Enter upper bound (inclusive): "
               << "\033[37m";
          cin >> upper;

          if (upper < lower) {
            cout << "\033[31m"
                 << "Sorry, upper bound is lower than lower bound. Please try "
                    "again."
                 << "\033[37m" << '\n';
          } else {
            double answer = 0;
            for (int i = lower; i <= upper; i++) {
              answer += a * i * i + b * i + c;
            }
            cout << "\033[34m"
                 << "Summation result: " << answer << "."
                 << "\033[37m" << '\n';
          }
        }
      }
    } // end of SUMMATION

    else if (command == "linegraph") { // start of LINEGRAPH
      vector<vector<char>> graph = {
          {'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '|',
           '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
          {'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '|',
           '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
          {'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '|',
           '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
          {'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '|',
           '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
          {'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '|',
           '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
          {'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '|',
           '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
          {'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '|',
           '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
          {'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '|',
           '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
          {'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '|',
           '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
          {'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '|',
           '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
          {'-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '|',
           '-', '-', '-', '-', '-', '-', '-', '-', '-', '-'},
          {'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '|',
           '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
          {'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '|',
           '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
          {'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '|',
           '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
          {'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '|',
           '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
          {'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '|',
           '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
          {'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '|',
           '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
          {'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '|',
           '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
          {'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '|',
           '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
          {'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '|',
           '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
          {'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '|',
           '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'},
      };
      int m = 1, b = 0;
      cout << "Line Grapher v0.1" << '\n';
      cout << "This tool can be used to graph linear equations." << '\n';
      cout << "\033[32m"
           << "Enter the coefficient for x term (m. default m=1): "
           << "\033[37m";
      cin >> m;
      if (m == 0) {
        m = 1;
        cout << "\033[31m"
             << "Cannot use zero for m value. Using one. Continue (Y/N)?"
             << "\033[37m";
        string continu = "";
        cin >> continu;
        if (continu == "Y" || continu == "y") {
          cout << "\033[32m"
               << "Enter the value for constant (b): "
               << "\033[37m";
          cin >> b;

          for (int w = 0; w <= 20; w++) {
            int i = w - 10;
            if (i < 0) {
              int tempgraph = i + 10;
              int ytemp = m * i + b;
              if (ytemp < -10 || ytemp > 10) {
                continue;
              } else {
                if (ytemp < 0) {
                  //int ytempgraph = -1 * (ytemp - 10);
                  graph[ytemp][tempgraph] = 'X';
                }
                if (ytemp == 0) {
                  graph[10][tempgraph] = 'X';
                }
                if (ytemp > 0) {
                  int ytempgraph = 10 - ytemp;
                  graph[ytempgraph][tempgraph] = 'X';
                }
              }
            } else if (i == 0) {
              int ytemp = m * 0 + b;
              if (ytemp < -10 || ytemp > 10) {
                continue;
              } else {
                if (ytemp < 0) {
                  int ytempgraph = -1 * (ytemp - 10);
                  graph[ytempgraph][10] = 'X';
                }
                if (ytemp == 0) {
                  graph[10][10] = 'X';
                }
                if (ytemp > 0) {
                  int ytempgraph = 10 - ytemp;
                  graph[ytempgraph][10] = 'X';
                }
              }
            } else if (i > 0) {
              int tempgraph = 10 + i;
              int ytemp = m * i + b;
              if (ytemp < -10 || ytemp > 10) {
                continue;
              } else {
                if (ytemp < 0) {
                  int ytempgraph = -1 * (ytemp - 10);
                  graph[ytempgraph][tempgraph] = 'X';
                }
                if (ytemp == 0) {
                  graph[10][tempgraph] = 'X';
                }
                if (ytemp > 0) {
                  int ytempgraph = 10 - ytemp;
                  graph[ytempgraph][tempgraph] = 'X';
                }
              }
            }
          }

          for (int i = 0; i <= 20; i++) {
            int indice = -1 * (i - 10);
            if (indice < 0 && indice != -10) {
              cout << " " << indice << " ";
            } else if (indice == 10) {
              cout << "  " << indice << " ";
            }
            for (int j = 0; j <= 20; j++) {
              if (graph[i][j] == 'X') {
                cout << "\033[32m" << graph[i][j] << "\033[37m"
                     << " ";
              } else {
                cout << graph[i][j] << " ";
              }
            }
            cout << '\n';
          }
        } else {
          cout << '\n';
          system("clear");
        }
      } else {
        int b = 0;
        cout << "\033[32m"
             << "Enter the value for constant (b): "
             << "\033[37m";
        cin >> b;

        for (int w = 0; w <= 20; w++) {
          int i = w - 10;
          if (i < 0) {
            int tempgraph = i + 10;
            int ytemp = m * i + b;
            if (ytemp < -10 || ytemp > 10) {
              continue;
            } else {
              if (ytemp < 0) {
                int ytempgraph = -1 * (ytemp - 10);
                graph[ytempgraph][tempgraph] = 'X';
              }
              if (ytemp == 0) {
                graph[10][tempgraph] = 'X';
              }
              if (ytemp > 0) {
                int ytempgraph = 10 - ytemp;
                graph[ytempgraph][tempgraph] = 'X';
              }
            }
          } else if (i == 0) {
            int ytemp = m * 0 + b;
            if (ytemp < -10 || ytemp > 10) {
              continue;
            } else {
              if (ytemp < 0) {
                int ytempgraph = -1 * (ytemp - 10);
                graph[ytempgraph][10] = 'X';
              }
              if (ytemp == 0) {
                graph[10][10] = 'X';
              }
              if (ytemp > 0) {
                int ytempgraph = 10 - ytemp;
                graph[ytempgraph][10] = 'X';
              }
            }
          } else if (i > 0) {
            int tempgraph = 10 + i;
            int ytemp = m * i + b;
            if (ytemp < -10 || ytemp > 10) {
              continue;
            } else {
              if (ytemp < 0) {
                //int ytempgraph = -1 * (ytemp - 10);
                graph[ytemp][tempgraph] = 'X';
              }
              if (ytemp == 0) {
                graph[10][tempgraph] = 'X';
              }
              if (ytemp > 0) {
                int ytempgraph = 10 - ytemp;
                graph[ytempgraph][tempgraph] = 'X';
              }
            }
          }
        }
        for (int i = 0; i <= 20; i++) {
          int indice = -1 * (i - 10);
          if (indice < 0 && indice != -10) {
            cout << " " << indice << " ";
          } else if (indice == 10) {
            cout << " " << 10 << " ";
          } else if (indice == -10) {
            cout << -10 << " ";
          } else {
            cout << "  " << indice << " ";
          }
          for (int j = 0; j <= 20; j++) {
            if (graph[i][j] == 'X') {
              cout << "\033[32m" << graph[i][j] << "\033[37m"
                   << " ";
            } else {
              cout << graph[i][j] << " ";
            }
          }
          cout << '\n';
        }
      }
    } // end of LINEGRAPH

    else if (command == "circle") { // start of CIRCLE
      double h = 0, k = 0, r = 1;
      cout << "Circle Informant Utility" << '\n';
      cout << "This tool can be used to get information of circles in standard "
              "form. Decimals supported"
           << '\n';
      cout << "\033[32m"
           << "Enter the coefficient for x constant term (h in (x-h)^2): "
           << "\033[37m";
      cin >> h;
      cout << "\033[32m"
           << "Enter the coefficient for y constant term (k in (y-k)^2): "
           << "\033[37m";
      cin >> k;

      int ech = -1.0 * h, kay = -1.0 * k;
      if (ech == -0.1) {
        ech = 0;
      }
      if (kay == -0.1) {
        kay = 0;
      }

      cout << "\033[32m"
           << "Enter r value (r in (x-h)^2 + (y-k)^2 = r^2): "
           << "\033[37m";
      cin >> r;
      if (r == 0) {
        cout << "\033[31m"
             << "Cannot use zero for r. Using one. Continue (Y/N)?"
             << "\033[37m";
        string continu = "";
        cin >> continu;
        if (continu == "Y" || continu == "y") {
          r = 1;
          cout << "\033[34m"
               << "Radius of Circle: " << r << "."
               << "\033[37m"
               << "\n";
          cout << "\033[34m"
               << "Origin of Circle: (" << ech << ", " << kay << ")."
               << "\033[37m"
               << "\n";
          cout << "\033[34m"
               << "Top Corner: (" << ech << ", " << kay + r << ")."
               << "\033[37m"
               << "\n";
          cout << "\033[34m"
               << "Bottom Corner: (" << ech << ", " << kay - r << ")."
               << "\033[37m"
               << "\n";
          cout << "\033[34m"
               << "Left Corner: (" << ech - r << ", " << kay << ")."
               << "\033[37m"
               << "\n";
          cout << "\033[34m"
               << "Right Corner: (" << ech + r << ", " << kay << ")."
               << "\033[37m"
               << "\n";
        } else {
          cout << '\n';
          system("clear");
        }
      } else {
        cout << "\033[34m"
             << "Radius of Circle: " << r << "."
             << "\033[37m"
             << "\n";
        cout << "\033[34m"
             << "Origin of Circle: (" << ech << ", " << kay << ")."
             << "\033[37m"
             << "\n";
        cout << "\033[34m"
             << "Top Corner: (" << ech << ", " << kay + r << ")."
             << "\033[37m"
             << "\n";
        cout << "\033[34m"
             << "Bottom Corner: (" << ech << ", " << kay - r << ")."
             << "\033[37m"
             << "\n";
        cout << "\033[34m"
             << "Left Corner: (" << ech - r << ", " << kay << ")."
             << "\033[37m"
             << "\n";
        cout << "\033[34m"
             << "Right Corner: (" << ech + r << ", " << kay << ")."
             << "\033[37m"
             << "\n";
      }
    } // end of CIRCLE
      
    else if (command=="integral" || command=="integrand"){
        string functionType;
        cout << "\033[32m" << "Linear or Quadratic? " << "\033[37m";
        cin >> functionType;
        
        if(functionType=="Linear"){
            double m, b, x;
            cout << "Function type chosen: Linear (mx+b)\n";
            cout << "\033[32m" << "Enter value for x coefficient: " << "\033[37m";
            cin >> m;
            cout << "\033[32m" << "Enter value for constant (b): " << "\033[37m";
            cin >> b;
            cout << "\033[32m" << "Enter x coordinate: " << "\033[37m";
            cin >> x;
            cout << "\033[34m" << "Result: " << (m/2)*x*x + b*x << "\033[37m" << "\n";
        } else if (functionType == "Quadratic"){
            double a, b, c, x;
            cout << "Function type chosen: Quadratic (mx+b)\n";
            cout << "\033[32m" << "Enter value for x^2 coefficient: " << "\033[37m";
            cin >> a;
            cout << "\033[32m" << "Enter value for x coefficient: " << "\033[37m";
            cin >> b;
            cout << "\033[32m" << "Enter value for constant (c): " << "\033[37m";
            cin >> c;
            cout << "\033[32m" << "Enter x coordinate: " << "\033[37m";
            cin >> x;
            cout << "\033[34m" << "Result: " << (a/3)*x*x*x + b*x*x + c*x << "\033[37m" << "\n";
        }
    }
      
    else if (command=="deriv" || command=="derivative"){ // start of DERIV
        string functionType;
        cout << "\033[32m" << "Linear or Quadratic? " << "\033[37m";
        cin >> functionType;

        if (functionType == "Linear") {
            double m, b, x;
            cout << "Function type chosen: Linear (mx+b)\n";
            cout << "\033[32m" << "Enter value for x coefficient: " << "\033[37m";
            cin >> m;
            cout << "\033[32m" << "Enter value for constant (b): " << "\033[37m";
            cin >> b;
            cout << "\033[32m" << "Enter x coordinate: " << "\033[37m";
            cin >> x;
            cout << "\033[34m" << "Result: " << m << "\033[37m" << "\n";
        } else if (functionType == "Quadratic") {
            double a, b, c, x;
            cout << "Function type chosen: Quadratic (ax^2+bx+c)\n";
            cout << "\033[32m" << "Enter value for x^2 coefficient (a): " << "\033[37m";
            cin >> a;
            cout << "\033[32m" << "Enter value for x coefficient (b): " << "\033[37m";
            cin >> b;
            cout << "\033[32m" << "Enter value for constant (c): " << "\033[37m";
            cin >> c;
            cout << "\033[32m" << "Enter x coordinate: " << "\033[37m";
            cin >> x;
            cout << "\033[34m" << "Result: " << 2*a*x+b << "\033[37m" << "\n";
        } else {
            cout << "\033[31m" << "Invalid function type entered. Try again." << "\033[37m" << "\n";
        }
    } // end of DERIV
      
    else if (command=="indefder" || command=="indefinitederiv") { // start of INDEFDER
        cout << "Indefinite Derivative Calculator for Standard Polynomials" << '\n';
        cout << "\033[31m" << "Due to limitations of the input system in C++, it is required to mention the number of polynomials beforehand." << "\033[37m" << '\n';
        cout << "Furthermore, all polynomials must be in the form of Ax^b, were A and b are constants." << '\n';
        
        int n=0;
        cout << '\n' << "\033[32m" << "Enter the number of polynomials: " << "\033[37m";
        cin >> n;
        
        vector<int> coeffs={};
        vector<int> pwrs={};
        
        for(int i=0; i<n; i++){
            int a=0, b=0;
            cout << "\033[32m" << "Enter coefficient for term " << i+1 << " (whole): " << "\033[37m";
            cin >> a;
            cout << "\033[32m" << "Enter power for term " << i+1 << " (whole): " << "\033[37m";
            cin >> b;
            
            coeffs.push_back(a);
            pwrs.push_back(b);
        }
        
        cout << "\n" << "\033[34m" << "Result: " << "\033[37m";
        for(int i=0; i<n-1; i++){
            cout << coeffs[i]*pwrs[i] << "x^" << pwrs[i]-1;
            if(i==n-2){
                cout << "\n";
            }else{
                cout << " + ";
            }
        }
        
    } // end of INDEFDER
      
    else if (command == "indefintegral" || command == "indefintegral"){
        cout << "Indefinite Integral Calculator for Standard Polynomials" << '\n';
        cout << "\033[31m" << "Due to limitations of the input system in C++, it is required to mention the number of polynomials beforehand." << "\033[37m" << '\n';
        cout << "Furthermore, all polynomials must be in the form of Ax^b, were A and b are constants." << '\n';
        
        int n=0;
        cout << '\n' << "\033[32m" << "Enter the number of polynomials: " << "\033[37m";
        cin >> n;
        
        vector<int> coeffs={};
        vector<int> pwrs={};
        
        for(int i=0; i<n; i++){
            int a=0, b=0;
            cout << "\033[32m" << "Enter coefficient for term " << i+1 << " (whole): " << "\033[37m";
            cin >> a;
            cout << "\033[32m" << "Enter power for term " << i+1 << " (whole): " << "\033[37m";
            cin >> b;
            
            coeffs.push_back(a);
            pwrs.push_back(b);
        }
        
        cout << "\n" << "\033[34m" << "Result: " << "\033[37m";
        for(int i=0; i<n+1; i++){
            cout << coeffs[i]/(pwrs[i]+1) << "x^" << pwrs[i]+1;
            if(i==n){
                cout << "\n";
            }else{
                cout << " + ";
            }
        }
    }

    else if (command == "ellipse") { // start of ELLIPSE
      double h = 0, k = 0, a = 0, b = 0;
      cout << "Ellipse Informant Utility" << '\n';
      cout
          << "This tool can be used to get information of ellipses in standard "
             "form. Decimals supported"
          << '\n';
      cout << "\033[32m"
           << "Enter the coefficient for x constant term (h in (x-h)^2/a^2): "
           << "\033[37m";
      cin >> h;
      cout << "\033[32m"
           << "Enter the coefficient for y constant term (k in (y-k)^2/b^2): "
           << "\033[37m";
      cin >> k;

      int ech = 1.0 * h, kay = 1.0 * k;
      /*if (ech == -0.0) {
        ech = 0;
      }
      if (kay == -0.0) {
        kay = 0;
      }*/

      cout << "\033[32m"
           << "Enter the focal factor for x (a in (x-h)^2/a^2 + (y-k)^2/b^2 = "
              "r^2): "
           << "\033[37m";
      cin >> a;
      cout << "\033[32m"
           << "Enter the focal factor for y (b in (x-h)^2/a^2 + (y-k)^2/b^2 = "
              "r^2): "
           << "\033[37m";
      cin >> b;

      double c = 0;
      if (a >= b) {
        c = sqrt((a * a) - (b * b));
      } else {
        c = sqrt((b * b) - (a * a));
      }

      cout << "\033[34m"
           << "Origin of Ellipse: (" << ech << ", " << kay << ")."
           << "\033[37m"
           << "\n";
      if (b > a) {
        cout << "\033[34m"
             << "Foci 1 of Ellipse: (" << ech << ", " << kay + c << ")."
             << "\033[37m"
             << "\n";
        cout << "\033[34m"
             << "Foci 2 of Ellipse: (" << ech << ", " << kay - c << ")."
             << "\033[37m"
             << "\n";
      } else if (a > b) {
        cout << "\033[34m"
             << "Foci 1 of Ellipse: (" << ech + c << ", " << kay << ")."
             << "\033[37m"
             << "\n";
        cout << "\033[34m"
             << "Foci 2 of Ellipse: (" << ech - c << ", " << kay << ")."
             << "\033[37m"
             << "\n";
      } else {
        cout << "\033[34m"
             << "This tool calculates information for ellipses. The given "
                "information is relevant to a circle. Please use the Circle "
                "Informant Utility instead with the command 'circle'."
             << "\033[37m"
             << "\n";
        break;
      }

      if (b > a) {
        cout << "\033[34m"
             << "Top Corner: (" << ech << ", " << kay + b << ")."
             << "\033[37m"
             << "\n";
        cout << "\033[34m"
             << "Bottom Corner: (" << ech << ", " << kay - b << ")."
             << "\033[37m"
             << "\n";
        cout << "\033[34m"
             << "Left Corner: (" << ech - a << ", " << kay << ")."
             << "\033[37m"
             << "\n";
        cout << "\033[34m"
             << "Right Corner: (" << ech + a << ", " << kay << ")."
             << "\033[37m"
             << "\n";
      } else if (a > b) {
        cout << "\033[34m"
             << "Top Corner: (" << ech << ", " << kay + a << ")."
             << "\033[37m"
             << "\n";
        cout << "\033[34m"
             << "Bottom Corner: (" << ech << ", " << kay - a << ")."
             << "\033[37m"
             << "\n";
        cout << "\033[34m"
             << "Left Corner: (" << ech - b << ", " << kay << ")."
             << "\033[37m"
             << "\n";
        cout << "\033[34m"
             << "Right Corner: (" << ech + b << ", " << kay << ")."
             << "\033[37m"
             << "\n";
      } else {
        cout << "\033[34m"
             << "This tool calculates information for ellipses. The given "
                "information is relevant to a circle. Please use the Circle "
                "Informant Utility instead with the command 'circle'."
             << "\033[37m"
             << "\n";
        break;
      }

    } // end of ELLIPSE

    else if (command == "doublesolve") { // start of DOUBLESOLVE
      system("clear");
      cout << "Double equation Solver v0.1" << '\n';
      cout << "This solver can be used to calculate answers given two Linear "
              "equations."
           << '\n';

      double m1 = 1, b1 = 0;
      cout << "\033[32m"
           << "Enter the coefficient for x term in equation 1 (y1 = mx + b. "
              "default m=1): "
           << "\033[37m";
      cin >> m1;
      if (m1 == 0) {
        m1 = 1;
        cout << "\033[31m"
             << "Cannot use zero for m value. Using one. Continue (Y/N)?"
             << "\033[37m";
        string continu = "";
        cin >> continu;
        if (continu == "Y" || continu == "y") {
          cout << "\033[32m"
               << "Enter the value for constant in equation 1: "
               << "\033[37m";
          cin >> b1;

          double m2 = 1, b2 = 0;
          cout
              << "\033[32m"
              << "Enter the coefficient for x term in equation 2 (y2 = mx + b. "
                 "default m=1): "
              << "\033[37m";
          cin >> m2;
          if (m2 == 0) {
            m2 = 1;
            cout << "\033[31m"
                 << "Cannot use zero for m value. Using one. Continue (Y/N)?"
                 << "\033[37m";
            string continu2 = "";
            cin >> continu2;
            if (continu2 == "Y" || continu2 == "y") {
              cout << "\033[32m"
                   << "Enter the value for constant in equation 2: "
                   << "\033[37m";
              cin >> b2;

              int lower = -1, upper = 1;
              cout << "\033[32m"
                   << "Enter lower bound (inclusive): "
                   << "\033[37m";
              cin >> lower;
              cout << "\033[32m"
                   << "Enter upper bound (inclusive): "
                   << "\033[37m";
              cin >> upper;

              if (upper < lower) {
                cout << "\033[31m"
                     << "Sorry, upper bound is lower than lower bound. Please "
                        "try "
                        "again."
                     << "\033[37m" << '\n';
              } else {
                if (m1 == m2) {
                  cout << "\033[34m"
                       << "Linear equations are parallel to each other. No "
                          "solutions."
                       << "\033[37m" << '\n';
                } else if (m1 == m2 && b1 == b2) {
                  cout << "\033[34m"
                       << "Answer: All Real Numbers"
                       << "\033[37m" << '\n';
                } else {
                  for (int i = lower; i <= upper; i++) {
                    double eq1 = m1 * i + b1;
                    double eq2 = m2 * i + b2;
                    if (eq1 == eq2) {
                      cout << "\033[34m"
                           << "Answer: (" << i << ", " << eq1 << ")."
                           << "\033[37m" << '\n';
                      break;
                    }
                  }
                }
              }
            } else {
              cout << '\n';
              system("clear");
            }
          } else {
            cout << "\033[32m"
                 << "Enter the value for constant in equation 2: "
                 << "\033[37m";
            cin >> b2;

            int lower = -1, upper = 1;
            cout << "\033[32m"
                 << "Enter lower bound (inclusive): "
                 << "\033[37m";
            cin >> lower;
            cout << "\033[32m"
                 << "Enter upper bound (inclusive): "
                 << "\033[37m";
            cin >> upper;

            if (upper < lower) {
              cout << "\033[31m"
                   << "Sorry, upper bound is lower than lower bound. Please "
                      "try "
                      "again."
                   << "\033[37m" << '\n';
            } else {
              if (m1 == m2) {
                cout << "\033[34m"
                     << "Linear equations are parallel to each other. No "
                        "solutions."
                     << "\033[37m" << '\n';
              } else if (m1 == m2 && b1 == b2) {
                cout << "\033[34m"
                     << "Answer: All Real Numbers"
                     << "\033[37m" << '\n';
              } else {
                for (int i = lower; i <= upper; i++) {
                  double eq1 = m1 * i + b1;
                  double eq2 = m2 * i + b2;
                  if (eq1 == eq2) {
                    cout << "\033[34m"
                         << "Answer: (" << i << ", " << eq1 << ")."
                         << "\033[37m" << '\n';
                    break;
                  }
                }
              }
            }
          }
        } else {
          cout << '\n';
          system("clear");
        }
      } else {
        cout << "\033[32m"
             << "Enter the value for constant in equation 1: "
             << "\033[37m";
        cin >> b1;

        double m2 = 1, b2 = 0;
        cout << "\033[32m"
             << "Enter the coefficient for x term in equation 2 (y2 = mx + b. "
                "default m=1): "
             << "\033[37m";
        cin >> m2;
        if (m2 == 0) {
          m2 = 1;
          cout << "\033[31m"
               << "Cannot use zero for m value. Using one. Continue (Y/N)?"
               << "\033[37m";
          string continu2 = "";
          cin >> continu2;
          if (continu2 == "Y" || continu2 == "y") {
            cout << "\033[32m"
                 << "Enter the value for constant in equation 2: "
                 << "\033[37m";
            cin >> b2;

            int lower = -1, upper = 1;
            cout << "\033[32m"
                 << "Enter lower bound (inclusive): "
                 << "\033[37m";
            cin >> lower;
            cout << "\033[32m"
                 << "Enter upper bound (inclusive): "
                 << "\033[37m";
            cin >> upper;

            if (upper < lower) {
              cout << "\033[31m"
                   << "Sorry, upper bound is lower than lower bound. Please "
                      "try "
                      "again."
                   << "\033[37m" << '\n';
            } else {
              if (m1 == m2) {
                cout << "\033[34m"
                     << "Linear equations are parallel to each other. No "
                        "solutions."
                     << "\033[37m" << '\n';
              } else if (m1 == m2 && b1 == b2) {
                cout << "\033[34m"
                     << "Answer: All Real Numbers"
                     << "\033[37m" << '\n';
              } else {
                for (int i = lower; i <= upper; i++) {
                  double eq1 = m1 * i + b1;
                  double eq2 = m2 * i + b2;
                  if (eq1 == eq2) {
                    cout << "\033[34m"
                         << "Answer: (" << i << ", " << eq1 << ")."
                         << "\033[37m" << '\n';
                    break;
                  }
                }
              }
            }
          } else {
            cout << '\n';
            system("clear");
          }
        } else {
          cout << "\033[32m"
               << "Enter the value for constant in equation 2: "
               << "\033[37m";
          cin >> b2;

          int lower = -1, upper = 1;
          cout << "\033[32m"
               << "Enter lower bound (inclusive): "
               << "\033[37m";
          cin >> lower;
          cout << "\033[32m"
               << "Enter upper bound (inclusive): "
               << "\033[37m";
          cin >> upper;

          if (upper < lower) {
            cout << "\033[31m"
                 << "Sorry, upper bound is lower than lower bound. Please "
                    "try "
                    "again."
                 << "\033[37m" << '\n';
          } else {
            if (m1 == m2) {
              cout << "\033[34m"
                   << "Linear equations are parallel to each other. No "
                      "solutions."
                   << "\033[37m" << '\n';
            } else if (m1 == m2 && b1 == b2) {
              cout << "\033[34m"
                   << "Answer: All Real Numbers"
                   << "\033[37m" << '\n';
            } else {
              for (int i = lower; i <= upper; i++) {
                double eq1 = m1 * i + b1;
                double eq2 = m2 * i + b2;
                if (eq1 == eq2) {
                  cout << "\033[34m"
                       << "Answer: (" << i << ", " << eq1 << ")."
                       << "\033[37m" << '\n';
                  break;
                }
              }
            }
          }
        }
      }
    } // end of DOUBLESOLVE

    else if (command == "lcm") { // start of LCM
      int a = 0, b = 0;
      cout << "Two numbers: ";
      cin >> a >> b;

      int lcm = (a / gcd(a, b)) * b;
      cout << "\033[34m"
           << "LCM: " << lcm
           << "\033[37m" << '\n';
    } // end of LCM

    else if (command == "gcd") { // start of GCD
      int a = 0, b = 0;
      cout << "Two numbers: ";
      cin >> a >> b;

      cout << "\033[34m"
           << "GCD: " << gcd(a, b)
           << "\033[37m" << '\n';
    } // end of GCD

    else if (command == "time" || command == "date") { // start of TIME/DATE
      time_t curr_time;
      curr_time = time(NULL);

      tm *tm_gmt = gmtime(&curr_time);
      cout << "Current time and date: " << tm_gmt->tm_mon + 1 << ", "
           << tm_gmt->tm_mday << ", " << tm_gmt->tm_year + 1900 << '\n'
           << tm_gmt->tm_hour << ":" << tm_gmt->tm_min << ":" << tm_gmt->tm_sec
           << " GMT" << '\n';
    } // end of TIME/DATE

    else if (command == "version" || command == "ver") { // start of VERSION
      cout << "Version: " << version << '\n';
    } // end of VERSION

    else if (command == "corn") { // start of easter egg 1
      system("clear");
      cout << "Beijing Corn" << '\n'
           << '\n'
           << "[narrator] Welcome to Beijing Corn." << '\n'
           << "The corn grow in the magical land of Beijing." << '\n'
           << "The soil is so pure, you can taste the failure." << '\n'
           << "All the cows here play piano, so we thought having some "
              "chicken "
              "give us better atmosphere."
           << '\n'
           << "But they just ate all the corn." << '\n'
           << '\n'
           << "sowehavethreecansleft-" << '\n'
           << '\n'
           << "Meet our local farmer:" << '\n'
           << "[farmer] Every morning we bring the water up from the mountain"
           << '\n'
           << "Three AM every morning I come on the mountain to join the "
              "rabbits to sing Despacito."
           << '\n'
           << "an ancient ritual that really distills the disappointment into "
              "the corn."
           << "\n"
           << "our corn so dumb it grows out of trees." << '\n'
           << "in fact I didn't even plant corn." << '\n'
           << "I planted rice!" << '\n'
           << "[narrator] Stressed out at work, need to relax? Beijing Corn "
              "has calming "
              "quality. Did your son get a B in math? Give him Beijing Corn, "
              "disappointment, guaranteed."
           << '\n'
           << '\n'
           << "   Steven He's Father" << '\n'
           << "   Founder of Beijing Corn and Failure Management Lab" << '\n'
           << "   Walked to school twenty miles up hill both ways with one "
              "foot [his other foot was starting a business]."
           << '\n';
    } // end of easter egg 1

    else { // if all ELSEIF statements fail
      cout << "\033[31m"
           << "That command is not in the system."
           << "\033[37m" << '\n'
           << "Make sure that you typed the command properly." << '\n'
           << "If it is correct, Check the documents shown in the 'help' "
              "command to see if the desired command is available."
           << '\n';
    }
  }
} // end of main OS code
