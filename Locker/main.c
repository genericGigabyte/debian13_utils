//password locker program
//this program is used to store passwords for services locally
//all passwords stored are encrypted and can be copied to clipboard or deleted

//header files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "classes.h"
#include "md5.h"
#include <termios.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

//global declarations
struct User user;
static struct Password passwordTable[100];
int passTablePointer = 0;
const char detailsPath[] = "details.txt";
const char passwordsPath[] = "passwords.txt";

//function prototypes
void EncryptPass(char _password[17]);
void DecryptPass(char _password[17]);

#pragma region display functions

void ClearDisplay()
{
    //clears the display after any key is pressed
    getchar();
    system("clear");
}

void displayPasswords()
{
    printf("--== Passwords ==--\n");

    for (int i = 0; i < passTablePointer; i++)
    {
        char temp[17];
        printf("[%3d]\t\033[0;31m%16s\033[0m\t%16s\n", i + 1, passwordTable[i].password, passwordTable[i].service);
    }

    printf("\n--== Options ==--\n[1] Generate Password\n[2] Add Password\n[3] Delete Password\n[4] Display Password\n[5] Quit");
}

#pragma endregion

#pragma region signin signup

//getch implementation
int getch(void)
{
    int ch;

    //this is to change the settings so getchar does not require enter for the input to be read
    static struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON);
    newt.c_lflag &= ~(ECHO);
    tcsetattr(STDERR_FILENO, TCSANOW, &newt);

    //get character input
    ch = getchar();

    //reset settings back to normal
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return ch;
}

//this function handles converting a string of numbers into an equivalent integer
int StrToNum(char str[256])
{
    int total = 0;
    
    //loop through each number in the string and add it's numerical value to the total
    for (int i = 0; i < 3; i++)
    {
        total += ((int)str[i] - 48)*(int)pow(10,2-i);
    }

    //return the resulting integer
    return total;
}

//this function handles number input
int NumInput(char inp[256])
{
    const int maxLength = 3;
    char ch;
    int index = 0;

    while(1)
    {
        ch = getch();

        if (ch == 10)
        { //if the enter button is pressed
            break; //stop taking input
        } else if (ch == 127)
        { //if the backspace button is pressed
            if (index > 0)
            {
                index--;
                printf("\b \b");
            }
        } else if (ch == 32 || ch == 9 || index == maxLength || ch < 48 || ch > 57)
        { //if space or tab is pressed or a number is not entered
            continue; //ignore the input
        } else
        {
            inp[index] = ch;
            index++;
            printf("%c", ch);
        }
    }

    //move the contents of the array so that any gaps are on the left
    if (index != 0 || index != 3)
    {
        int count = 3 - index;
        while(count)
        {
            for (int i = 2; i > 0; i--)
            {
                inp[i] = inp[i - 1];
                inp[i - 1] = 0;
            }
            count--;
        }
    }

    //fill the unused spots with 0's
    if (index != 0)
    {
        for (int i = 3 - index; i > 0; i--)
        {
            inp[i - 1] = '0';
        }
    }
    
    inp[3] = '\0';
    printf("\n");

    //convert the entered number into an integer
    int test = StrToNum(inp);
    return test;
}

//password entry function
void EnterPassword(char _password[17])
{
    const int maxPassLength = 16;
    int passPos = 0;
    char ch;

    while (1)
    {
        ch = getch();

        if (ch == 10)
        { //if the enter button is pressed
            break; //stop taking input
        } else if (ch == 127)
        { //if the backspace button is pressed
            if (passPos > 0) {
                passPos--;
                printf("\b \b");
            }
        } else if (ch == 32 || ch == 9 || passPos == maxPassLength)
        { //if the space or tab key is pressed
            continue; //ignore the input
        } else
        {
            _password[passPos] = ch;
            passPos++;
            printf("*");
        }
    }
    _password[passPos] = '\0';
    printf("\n");
}

void EnterString(char _string[17])
{
    int count = 0;
    char ch;
    while (1)
    {
        ch = getch();

        if (ch == 10)
        { //if the enter button is pressed
            break; //stop taking input
        } else if (ch == 127)
        { //if the backspace button is pressed
            if (count > 0) {
                count--;
                printf("\b \b");
            }
        } else if (ch == 32 || ch == 9 || count == 16)
        { //if the space, tab, or enter key is pressed
            continue; //ignore the input
        } else
        {
            _string[count] = ch;
            count++;
            printf("%c", ch);
        }
    }
    _string[count] = '\0';
    printf("\n");
}

//sign in function
int SignIn()
{
    char buff[256];

    //get username and password input
    char username[17];
    printf("[Please login]\nUsername: ");
    EnterString(username);

    char password[17];
    printf("Password: ");
    EnterPassword(password);

    //open details file in read mode
    FILE* fptr = fopen(detailsPath, "r");

    //read username and password in the details file
    char usernameCheck[17];
    fgets(usernameCheck, 17, fptr);

    char passwordCheck[256];
    fgets(passwordCheck, 256, fptr);

    //hash the inputted password
    unsigned *d = md5(password, strlen(password));
    WBunion u;

    int i = 0;
    for (int j = 0; j < 4; j++)
    {
        u.w = d[j];
        for (int k = 0; k < 4; k++)
        {
            //convert the hexadecimal into a string of hexadecimal
            sprintf(buff + i, "%02x", u.b[k]);
            i += 2;
        }
    }
    buff[i] = '\0';

    //at the end we close the file, but we make sure there were no erros in doing so
    if (!fclose(fptr)) {
        perror("=> closing file 'details.txt'");
    } else
    {
        exit(1);
    }

    //check the username and password entered matches the ones on file
    //we add a \n on the end of the username and password entered because there will be a \n on the end of the one read from the details file
    if (strcmp(strcat(username,"\n"), usernameCheck) == 0 && strcmp(strcat(buff,"\n"), passwordCheck) == 0)
    {
        strcpy(user.password, buff);
        strcpy(user.username, username);
        return 1; //if so return 1 to indicate a successful sign in
    } else
    {
        return 0; //otherwise return 0 to indicate a failed sign in
    }
}

//sign up function
int SignUp()
{
    //get username and password input
    printf("[Please create a login]\nUsername: ");
    EnterString(user.username);

    char password[17];
    char password2[17];

    //we want to confirm password input twice
    printf("Password:\n");
    EnterPassword(password);
    printf("Please Re-enter your password:\n");
    EnterPassword(password2);

    //if the two passwords entered are the same
    if (strcmp(password, password2) == 0)
    { //tell the user the sign up was successful
        printf("Sign up successful\n");
       
        //hash the inputted password
        char buff[256];
        unsigned *d = md5(password, strlen(password));
        WBunion u;

        int i = 0;
        for (int j = 0; j < 4; j++)
        {
            u.w = d[j];
            for (int k = 0; k < 4; k++)
            {
                //convert the hexadecimal into a string of hexadecimal
                sprintf(buff + i, "%02x", u.b[k]);
                i += 2;
            }
        }
        buff[i] = '\0';

       //set the user password
       strcpy(user.password, buff);
    } else
    { //otherwise tell the user the passwords do not match
       printf("Passwords do not match\n");

       //return 0 to show sign up failed
       return 0;
    }

    //open the details file in write mode
    FILE* fptr = fopen(detailsPath, "w");

    //write the username and encrypted password to file
    fprintf(fptr, "%s\n%s\n", user.username, user.password);

    //at the end we close the file, but we make sure there were no erros in doing so
    if (!fclose(fptr)) {
        perror("=> closing file 'details.txt'");
    } else
    {
        exit(1);
    }

    //return 1 to show sign up was successfull
    return 1;
}

#pragma endregion

#pragma region password handling functions

void AddToPasswordTable(char *_password, char *_service)
{
    struct Password password;
    strcpy(password.service, _service);
    strcpy(password.password, _password);

    passwordTable[passTablePointer] = password;
    passTablePointer++;
}

void RemoveFromPasswordTable(int index)
{
    for (int count = index; count < passTablePointer; count++)
    {
        passwordTable[count] = passwordTable[count + 1];
    }
    passTablePointer--;
}

void EncryptPass(char _password[17])
{
    //get the length of the password to encrypt
    int passLength = strlen(_password);

    //create the key
    char key[256];
    strcpy(key, user.password);

    //perform caeser cipher
    for (int i = 0; i < passLength; i++)
    {
        char temp = _password[i];

        //find the number of times to rotate
        unsigned int rotation = ((int)key[i] % 4) + 16;

        //rotating 5 times, make sure to go back to beginning (ASCII value of 33) if we go over 126
        _password[i] = temp + rotation > 126 ? ((temp + rotation) - 126) + 33 : temp + rotation;
    }
}

void DecryptPass(char _encryptedPass[17])
{
    //get the length of the password to decrypt
    int passLength = strlen(_encryptedPass);

    //create the key
    char key[256];
    strcpy(key, user.password);

    //perform caeser cipher
    for (int i = 0; i < passLength; i++)
    {
        char temp = _encryptedPass[i];

        //find the number of times to rotate
        unsigned int rotation = ((int)key[i] % 4) + 16;

        //rotating 5 times in the opposite direction, making sure to go back to the end (ASCII value of 126) if we go under 33
        _encryptedPass[i] = temp - rotation < 33 ? 126 - (33 - (temp - rotation)) : temp - rotation;
    }
}

int ReadPasswords()
{
    FILE *fptr = fopen(passwordsPath, "r");

    fseek(fptr, 0, SEEK_END);
    int size = ftell(fptr);

    if (size == 0)
    { //if there are no passwords in file then there are no passwords to read
        return 0;
    } else
    {
        //go back to the start of the file
        fseek(fptr, 0, SEEK_SET);

        //count the number of lines in the file
        int numOfLines = 0;
        char c;
        do
        {
            c = getc(fptr);
            if (c == '\n')
            {
                numOfLines++;
            }
        } while (c != EOF);

        //go back to the start of the file
        fseek(fptr, 0, SEEK_SET);

        //divide number of lines by 2 to get how many times to loop
        //loop through each pair of lines
        for (int i = 0; i < numOfLines/2; i++)
        {
            //get password
            char password[17];

            //read a line for the password
            fscanf(fptr, "%s", password);

            //get service
            char service[17];

            //read a line for the service
            fscanf(fptr, "%s", service);

            //create a password struct and add it to the table
            AddToPasswordTable(password, service);
        }
    }

    //at the end we close the file, but we make sure there were no erros in doing so
    if (!fclose(fptr)) {
        perror("=> closing file 'details.txt'");
    } else
    {
        exit(1);
    }

    return 1;
}

void UpdatePasswordsFile()
{
    //open the passwords file in write mode
    FILE* fptr = fopen(passwordsPath, "w");

    //loop through all of the passwords in the password table
    for (int i = 0; i < passTablePointer; i++)
    {
        //write the relevant details in the file
        fprintf(fptr, "%s\n%s\n", passwordTable[i].password, passwordTable[i].service);
    }

    //close the file and ensure there are no errors in doing so
    if (!fclose(fptr)) {
        perror("=> closing file 'details.txt'");
    } else
    {
        exit(1);
    }
}

void GeneratePassword(char _service[17])
{
    const int PASSLENGTH = 17;
    char _password[PASSLENGTH];

    srand((unsigned int)time(NULL));
    for (int i = 0; i < PASSLENGTH; i++)
    {
        int randomizer = rand() % 4;
        switch (randomizer)
        {
            case 1: //add a lower case letter to the password
                _password[i] = (rand() % 26) + 97;
                break;
        
            case 2: //add an upper case letter to the password
                _password[i] = (rand() % 26) + 65;
                break;

            case 3: //add a number to the password
                _password[i] = (rand() % 9) + 48;
                break;
        
            default: //add a special character to the password
                _password[i] = (rand() % 6) + 33;
                break;
        }
    }
    _password[PASSLENGTH - 1] = '\0'; //add null character on the end
    printf("%s", _password);

    //add the generated and encrypted password to the table
    EncryptPass(_password);
    AddToPasswordTable(_password, _service);
}

#pragma endregion

//main function
int main(void)
{
    #pragma region signing in

    //check if there is something in the details file
    //aka is there an account already created
    FILE* fptr = fopen(detailsPath, "r");

    //check the position of the end of the file
    //if this is 0 then the file is empty
    fseek(fptr, 0, SEEK_END);
    int size = ftell(fptr);

    if (fclose(fptr)) {
        printf("Failed to close details.txt, exiting program");
        exit(1);
    }

    if (size == 0)
    {
        //if the details file is empty, sign up
        int signUpCheck = 0;
        while (signUpCheck == 0)
        {
            signUpCheck = SignUp();
            ClearDisplay();
        }
    } else
    {
        //otherwise we are going to sign in
        int signInCheck = 0;
        int attemptCount = 0;
        while (signInCheck == 0)
        {
            if (attemptCount == 3)
            { //if 3 login attempts were made then close the program
                printf("3 attempts made, locking account\n");
                return 0;
            }

            //run the sign in function
            signInCheck = SignIn();
            if (signInCheck == 0)
            {
                printf("Sign in failed! Please try again");
                attemptCount++; //keep track of failed login attempts
            } else
            {
                printf("Sign in successful!");
            }
            ClearDisplay();
        }
    }

    #pragma endregion

    #pragma region main program loop

    ReadPasswords();
    while (1)
    {
        displayPasswords();

        printf("\nChoose from one of the options above: ");
        char num[256]; //input buffer for numbers
        int choice = NumInput(num);
        char service[17];
        char password[17];

        switch (choice)
        {
            case 1: //generating a random password
                //get service
                printf("Enter a service: ");
                EnterString(service);

                //generate the password
                GeneratePassword(service);
                break;

            case 2: //adding a password
                //get service
                printf("Enter a service: ");
                EnterString(service);

                //get password
                printf("Enter a password: ");
                EnterPassword(password);

                //encrypt password
                EncryptPass(password);

                //add password to password table
                AddToPasswordTable(password, service);
                break;

            case 3: //deleting a password
                //find the password to delete
                printf("Enter the number of the entry you want to delete: ");

                do
                {
                    choice = NumInput(num);
                    if (choice < 1 || choice > passTablePointer)
                    {
                        printf("Invalid input, please try again: ");
                    }
                } while (choice < 1 || choice > passTablePointer);

                //remove the password from the password table
                RemoveFromPasswordTable(choice - 1);
                break;

            case 4: //displaying a password
                //find the password to display
                printf("Enter the number of the entry you want to display: ");

                do
                {
                    choice = NumInput(num);
                    if (choice < 1 || choice > passTablePointer)
                    {
                        printf("Invalid input, please try again: ");
                    }
                } while (choice < 1 || choice > passTablePointer);
                
                //display the password
                char buff[17];
                strcpy(buff, passwordTable[choice - 1].password);
                DecryptPass(buff);
                printf("\033[0;31m%s\033[0m", buff);
                break;
        
            case 5: //exit the program
                //update the passwords file
                UpdatePasswordsFile();

                //exit the program
                ClearDisplay();
                return 0;
                break;

            default:
                printf("Invalid Input");
                break; 
        }
        ClearDisplay();
    }

    #pragma endregion
}