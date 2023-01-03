#include <cmath>
#include <ctime>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

using namespace std;

static void list_dir(const char *path) {
  struct dirent *entry;
  DIR *dir = opendir(path);
  if (dir == NULL) {
    return;
  }

  while ((entry = readdir(dir)) != NULL) {
    printf("%s\n", entry->d_name);
  }

  closedir(dir);
}

int main() {
  string name = "";
  double version = 0.4;
  string command = "";
  double defaultNumVar = 0;
  vector<string> folders;
  vector<string> keys;
  vector<string> values;
  int lengthOfVars = 0;
  cout << "Enter name: ";
  cin >> name;
  cout << endl;
  cout << "Welcome to the Command Line Operating System Beta, " << name << "."
       << endl;
  cout << "Version: " << version << ", Name: " + name + "." << endl;
  while (true) {
    cout << "> ";
    cin >> command;

    if (command == "help" || command == "h") {
      cout << "clear - Clears the command line interface." << endl;
      cout << "help or h - Opens this help text." << endl;
      cout << "about - Prints text that would show when CLOS starts." << endl;
      cout << "exit - Exits CLOS. Optional delay in seconds available." << endl;
      cout << "add - Add two numbers together. Optional extra addition "
              "available."
           << endl;
      cout << "subtract or minus - Similar to add but with minus sign." << endl;
      cout << "multiply - Multiplies two numbers." << endl;
      cout << "divide - Divides two numbers. Throws error if division by zero "
              "occurs."
           << endl;
      cout << "power or pwr - Raises first number to power of second number."
           << endl;
      cout << "root - Takes root of first number using second number." << endl;
      cout << "var - Creates a new temporary variable." << endl;
      cout << "print - Prints a variable made using var command." << endl;
      cout << "restart - Restarts CLOS. Optional delay in seconds available."
           << endl;
      cout << "whoami - Shows the entered name." << endl;
      cout << "file - Creates a new file or rewrites exsisting one. Path can "
              "be specified and is entierly optional."
           << endl;
      cout << "read - Reads a file. Path can be specified and is entierly "
              "optional."
           << endl;
      cout << "mkdir - Makes a new directory. Special characters are not "
              "allowed. No nested directories are allowed, either."
           << endl;
      cout << "delete - Deletes a file given the filename and optional file "
              "path."
           << endl;
      cout << "rmdir - Removes the given directory. If the directory is not "
              "empty, it fails to do so."
           << endl;
      cout << "ls - Lists the contents of a given folder." << endl;
      cout << "version or ver - Gives current OS version." << endl;
    }

    if (command == "exit") { // start of EXIT
      int delay;
      cout << "Delay? ";
      cin >> delay;
      sleep(delay);
      cout << "Exiting CLOS" << endl;
      sleep(1);
      cout.flush();
      system("clear");
      return 0;
    } // end of EXIT

    else if (command == "add") { // start of ADD
      bool isContinuing = false;
      while (true) {
        double b;
        if (!isContinuing) {
          cout << "Two numbers: ";
          cin >> defaultNumVar >> b;
          cout << endl;
          cout << defaultNumVar + b << endl;
        } else if (isContinuing) {
          cout << "Additional number: ";
          cin >> b;
          cout << endl;
          cout << defaultNumVar + b << endl;
        }
        string cont = "";
        cout << "Continue(Y/n)? ";
        cin >> cont;
        if (cont == "Y" || cont == "y") {
          defaultNumVar = defaultNumVar + b;
          isContinuing = true;
          continue;
        } else {
          cout << "Final result: " << defaultNumVar + b << endl;
          defaultNumVar = defaultNumVar + b;
          break;
        }
      }
    } // end of ADD

    else if (command == "subtract" || command == "minus") { // start of SUBTRACT
      bool isContinuing = false;
      while (true) {
        double b;
        if (!isContinuing) {
          cout << "Two numbers: ";
          cin >> defaultNumVar >> b;
          cout << endl;
          cout << defaultNumVar - b << endl;
        } else if (isContinuing) {
          cout << "Additional number: ";
          cin >> b;
          cout << endl;
          cout << defaultNumVar - b << endl;
        }
        string cont = "";
        cout << "Continue(Y/n)? ";
        cin >> cont;
        if (cont == "Y" || cont == "y") {
          defaultNumVar = defaultNumVar - b;
          isContinuing = true;
          continue;
        } else {
          cout << "Final result: " << defaultNumVar - b << endl;
          defaultNumVar = defaultNumVar - b;
          break;
        }
      }
    } // end of SUBTRACT

    else if (command == "multiply") { // start of MULTIPLY
      bool isContinuing = false;
      while (true) {
        double b;
        if (!isContinuing) {
          cout << "Two numbers: ";
          cin >> defaultNumVar >> b;
          cout << endl;
          cout << defaultNumVar * b << endl;
        } else if (isContinuing) {
          cout << "Additional number: ";
          cin >> b;
          cout << endl;
          cout << defaultNumVar * b << endl;
        }
        string cont = "";
        cout << "Continue(Y/n)? ";
        cin >> cont;
        if (cont == "Y" || cont == "y") {
          defaultNumVar = defaultNumVar * b;
          isContinuing = true;
          continue;
        } else {
          cout << "Final result: " << defaultNumVar * b << endl;
          defaultNumVar = defaultNumVar * b;
          break;
        }
      }
    } // end of MULTIPLY

    else if (command == "divide") { // start of DIVIDE
      bool isContinuing = false;
      while (true) {
        double b;
        if (!isContinuing) {
          cout << "Two numbers: ";
          cin >> defaultNumVar >> b;
          cout << endl;
          if (b == 0) {
            while (b == 0) {
              cout << "Division by Zero is undefined. Enter new second number:";
              cin >> b;
            }
          }
          cout << defaultNumVar / b << endl;
        } else if (isContinuing) {
          cout << "Additional number: ";
          cin >> b;
          if (b == 0) {
            while (b == 0) {
              cout
                  << "Division by Zero is undefined. Enter new second number: ";
              cin >> b;
            }
          }
          cout << endl;
          cout << defaultNumVar / b << endl;
        }
        string cont = "";
        cout << "Continue(Y/n)? ";
        cin >> cont;
        if (cont == "Y" || cont == "y") {
          defaultNumVar = defaultNumVar / b;
          isContinuing = true;
          continue;
        } else {
          cout << "Final result: " << defaultNumVar / b << endl;
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
      double b;
      cout << "Two numbers: ";
      cin >> defaultNumVar >> b;
      cout << endl;
      cout << "Result: " << pow(defaultNumVar, b) << endl;
      defaultNumVar = pow(defaultNumVar, b);
    } // end of POWER

    else if (command == "root") { // start of ROOT
      double b;
      cout << "Two numbers: ";
      cin >> defaultNumVar >> b;
      if (defaultNumVar < 0) {
        while (defaultNumVar < 0) {
          cout << "Root of negative number does not exist (in Reals). Enter "
                  "new first number:";
          cin >> defaultNumVar;
        }
      }
      b = 1 / b;
      cout << endl;
      cout << "Result: " << pow(defaultNumVar, b) << endl;
      defaultNumVar = pow(defaultNumVar, b);
    } // end of ROOT

    else if (command == "clear" || command == "cls") { // start of CLEAR
      cout.flush();
      system("clear");
    } // end of CLEAR

    else if (command == "about") { // start of ABOUT
      cout << "CLOS System - Command Line Operating System" << endl;
      cout << "Version: " << version << ", Name: " + name + "." << endl;
    } // end of ABOUT

    else if (command == "var") { // start of VAR
      string key = "", val = "";
      cout << "Key and Value: ";
      cin >> key >> val;
      keys.push_back(key);
      values.push_back(val);
      lengthOfVars += 1;
      cout << "Done." << endl;
    } // end of VAR

    else if (command == "print") { // start of PRINT
      cout << "Key? ";
      string key = "";
      cin >> key;
      int i = 0;
      for (i = 0; i < lengthOfVars; i++) {
        if (keys[i] == key) {
          cout << "Value: " << values[i] << endl;
          ;
          break;
        }
      }
      if (i == lengthOfVars) {
        cout << "That variable cannot be found." << endl;
      }
    } // end of PRINT

    else if (command == "restart") { // start of RESTART
      int delay;
      cout << "Delay? ";
      cin >> delay;
      sleep(delay);
      cout << "Exiting CLOS" << endl;
      sleep(1);
      cout.flush();
      system("clear");
      sleep(4);
      cout << "restarted" << endl;
      cout << "Enter name:";
      cin >> name;
      cout << endl;
      cout << "Welcome to the Command Line Operating System, " << name << "."
           << endl;
      cout << "Version: " << version << ", Name: " + name + "." << endl;
    } // end of RESTART

    else if (command == "whoami") { // start of WHOAMI
      cout << name << endl;
    } // end of WHOAMI

    else if (command == "file") { // start of FILE
      cout << "Filename (with extension and optional path): ";
      string fileName = "";
      cin >> fileName;
      ofstream tempFile(fileName);
      cout.flush();
      system("clear");
      cout << "Editing " + fileName << endl;
      cout << "To end type q and hit Enter on new line." << endl;
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
      tempFile.close();
    } // end of File

    else if (command == "read") { // start of READ
      cout << "Filename (with extension and optional path): ";
      string fileName = "";
      cin >> fileName;
      string output = "";
      ifstream tempFile(fileName);
      while (getline(tempFile, output)) {
        cout << output << endl;
      }
    } // end of READ

    else if (command == "mkdir") { // start of MKDIR
      cout << "Folder name (no spaces): ";

      string temp = "";
      cin >> temp;
      char *folder = const_cast<char *>(temp.c_str());
      int check = 0;
      check = mkdir(folder);
      if (!check) {
        cout << "Directory " << folder << " created." << endl;
      } else {
        cout << "Unable to create directory " << folder << "." << endl;
      }
    } // end of MKDIR

    else if (command == "delete") { // start of DELETE
      cout << "Filename (with extension): ";
      string temp = "";
      cin >> temp;
      char *filename = const_cast<char *>(temp.c_str());
      int check = 0;
      check = remove(filename);
      if (!check) {
        cout << "File " << filename << " deleted." << endl;
      } else {
        cout << "Unable to delete the file " << filename
             << ". Maybe filename was mistyped?" << endl;
      }
    } // end of DELETE

    else if (command == "rmdir") { // start of RMDIR
      cout << "Folder name (Folder MUST be empty): ";
      string temp = "";
      cin >> temp;
      char *folder = const_cast<char *>(temp.c_str());
      int check = 0;
      check = rmdir(folder);
      if (!check) {
        cout << "Folder " << folder << " deleted." << endl;
      } else {
        cout << "Unable to remove directory " << folder
             << ". Maybe foldername was mistyped?" << endl
             << "If not, make sure the folder is empty." << endl;
      }
    } // end of RMDIR

    else if (command == "ls") { // start of LS
      cout << "Folder name (no spaces): ";

      string temp = "";
      cin >> temp;
      char *folder = const_cast<char *>(temp.c_str());
      list_dir(folder);
    } // end of LS

    else if (command == "version" || command == "ver") { // start of VERSION
      cout << "Version: " << version << endl;
    } // end of VERSION

    else { // if all ELSEIF statements fail
      cout << "That command is not in the system." << endl;
    }
  }
}