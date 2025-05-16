/*
    * Name: Josh Evangelista, 5008496241, CS 135 5000 - 2024 Fall, ASSIGNMENT_06
    * Description: A program that takes in a 3 column zoo datafile to 
    *              output the  food mass and costs for each animal, 
    *              then calculates total costs for the day and 
    *              estimates monthly costs. If the values in the 
    *              food costs and masses are negative, such values will 
    *              be outputted but ignored when calculating total
    *              daily costs and monthly estimates. After the file
    *              is read, it will ask the user if they want to process
    *              another file. This will repeat until the user does not
    *              want to process any more files.
    * Input:       File location (std::string, something like 
    *              "zooData_v2.txt" or "/home/user/zooData_v2.txt")
    * 
    *              file contents (i.e. the data the file location 
    *              points to), formatted in 3 columns 
    *              "Animal food_pounds cost", separated by spaces
    *              
    *              user input on whether to process another file or
    *              not.
    * Output:      Error messages in case of invalid user input on
    *              continuing or not, or invalid file locations.
    *              
    *              Output of valid 3 column file, whose columns are
    *              "Animal", "food pounds", and "cost", in that order,
    *              as well as calculations of any total costs and
    *              estimates of monthly costs and food pound 
    *              requirements. If the data file has a negative 
    *              pounds or cost, that negative value will be ignored. 
*/
#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <string>

/*  function description: Takes in long long int 
                          cents into "AB.XY" string, in dollars.
                          Derived from my assignment04 main.cpp.
    parameters: long long int cent, simply the amount of U.S. cents
    return value: std::string dollar_amount, which is cents converted
                  into dollars. For example, if cent is 3002, then
                  dollar_amount is "30.02".
*/
std::string ll_cent_to_str_dollar(long long int cent) {
    std::string dollar_amount = "";
    /*all digits except the last two*/
    dollar_amount += std::to_string((cent-(cent%100))/100);
    dollar_amount += ".";
    /*in case last two digits of cent are something like 09*/
    if ((cent%100)-(cent%10)==0) {
        dollar_amount += "0";
        dollar_amount += std::to_string(cent%10);
    } else {
        /*in case last two digits of cent are something like 39*/
        dollar_amount += std::to_string(cent%100);
    }
    return dollar_amount;
}


/*  function description: Modifies &take_another_datafile based on user
                          input. If user input is Y or y, change
                          reference parameter to true. If N or n instead,
                          change reference parameter to false. If user
                          input is not Y/y/N/n, keep asking for proper
                          input.
    parameters: bool &take_another_datafile, which is modified by user
                input as to whether the program should process another 
                data file or not. If the user wants to process another
                data file, the value &take_another_file points at
                becomes true; otherwise, it becomes false.
    return value: Not available.
*/
void ask_for_another_datafile(bool &take_another_datafile) {
    char ch = 'n';
    
    bool repeat = false;
    do {
        std::cout << "Would you like to process another datafile? Y/y/N/n";
        std::cout << "\n***** ";
        std::cin >> ch;
        switch(tolower(ch)) {
            case 'y':
                take_another_datafile=true;
                repeat=false;
                break;
            case 'n':
                take_another_datafile=false;
                repeat=false;
                break;
            default:
                std::cout << "\n"
                          << "ERROR: Invalid entry. Please try again."
                          << '\n';
                repeat = true;
                std::cin.clear();
                break;
        } 
    } while (repeat==true);
}

/*  function description: Gets the n-th token of a string, a string
                          that is a list of words separated by 
                          spaces. 
    parameters:   std::string input_s, which is a list of words
                  separated by spaces, like "Lorem ipsum dolor"
                  
                  unsigned int n, which determines which token
                  in the string to obtain. 
    return value: a std::string value, which is either the n-th
                  token of a string, or the last token if n is
                  greater than the tokens input_s has. For example,
                  if input_s is "lorem ipsum dolor", and
                  n is 2, "ipsum" will be the return value. If
                  n is more than the amount of tokens, like
                  with "lorem ipsum dolor" and n=5, the last
                  token "dolor" will be returned instead.
*/
std::string get_nth_token(std::string input_s, unsigned int n) {
    std::string token = ""; 
    /* temp_token exists in case there is a word token followed
       by only delimiters in this case. If it was not emptied,
       it will give the last token. This happens when there are delimiters
       following the token*/
    std::string temp_token = "";
    char delimiter = ' ';
    unsigned int token_number = 0;
    
    bool on_new_token = false;
    unsigned long int i=0;
    /* added && i<input_s.length() to avoid a infinite loop should token_number
       be less than n after going through the entire string*/
    while (token_number<n && i<input_s.length()) {
        if (input_s[i] != delimiter) {
            temp_token = "";
            if (on_new_token == true) {
                token = "";
                on_new_token = false;
            }
            token.std::string::append(1, input_s[i]);
            if (i == input_s.length()-1) {
                token_number++;
            }
        } else {
            if (token.length()>0) {
                token_number++;
                if (token_number== n) {
                    break;
                }
                temp_token = token;
            }
            token = "";
            on_new_token = true;
        }
        i++;
    }

    if (temp_token.length() > token.length()) {
        /*replace token with temp_token in case of actual token
          being nothing*/
        token = temp_token;
    }  
    return token;

}
/*  function description: Outputs the opening statement of the zoo 
                          program.
    parameters:   No parameters
    return value: No return values
*/
void opening_statement() {
    for (int i=0; i<60; i++) {
        std::cout << '*';
    }
    std::cout << '\n';
    std::cout << " Welcome the UNLV CS Free Range Wild Animal Park!\n";
    for (int i=0; i<60; i++) {
        std::cout << '+';
    }
    std::cout << '\n';
}

/*  function description: Takes user input for file_location, then 
                          checks if a given file_location 
                          is valid. If not valid, it will keep
                          prompting user for valid file_location.
    parameters: std::string &file_location, which is always replaced
                by user input. When a valid file location input is 
                given, &file_location's value becomes that value.
    return value: Not available.
*/
void get_input_file(std::string &file_location) {
    file_location = "";
    bool retry = false;
    do {
        std::cout << "Please enter an input filename.\n";
        std::cout << "***** ";
        std::cin >> file_location;
        std::ifstream input_file(file_location);
        if (!input_file.is_open()) {
            std::cout << "ERROR opening the input file.\n";
            file_location = "";
            retry = true;
        } else {
            input_file.close();
            std::cout << "\n Input file given: "
                      << file_location;
            retry = false;
        }
    } while(retry == true);
}
/* Takes in a location of an input file, outputs its contents
   with some formatting, and calculates the sums of
   pounds and costs for the day to estimate the monthly
   food volume and cost requirements.*/
/*  function description: Takes in a location of an input file,
                          assumed to be valid, outputs its contents
                          with some formatting, and calculates the sums
                          of pounds and costs for the day to estimate the
                          monthly food mass and cost requirements. If
                          there are negative food mass or food cost values,
                          for purposes of calculating daily food mass and 
                          costs, and estimates of monthly food mass and 
                          costs, they are ignored.
    parameters: std::string &input_file, which is the location of the
                file to process.
    return value: Not available.
*/
void display_input_file(const std::string &input_file) {
    
    long double total_day_food_pounds = 0;
    long long int total_day_cost_cent = 0;
    
    long double estimate_month_food_pounds = 0;
    long long int estimate_month_cost_cent = 0;
    
    std::ifstream in_file_s(input_file);
    std::string line = "";

    std::cout << "+";
    for (int i=0; i<48; i++) {
        std::cout << "-";
    }
    std::cout << "+";
    std::cout << '\n';
    std::cout << "  Animal            Food 'lbs.'         Cost '$'\n";
    std::cout << "++---------------+---------------+---------------+"
              << '\n';

    long double raw_dollar_cost = 0;
    while(std::getline(in_file_s, line)) {
        /* crude measure to prevent empty lines from causing
           program shut downs. Does not help against non-empty
           file-formatting problems.*/
        if (line.length() > 0) {
            long double pounds = 10*std::stold(get_nth_token(line,2));
            pounds = std::round(pounds);
            pounds = pounds/10;

            long double cost = 100*std::stold(get_nth_token(line, 3));
            cost = std::round(cost);

            long long cents = static_cast<long long>(cost);

            std::string string_cost = ll_cent_to_str_dollar(cents);

            std::string animal_type = get_nth_token(line, 1);

            std::cout << ' '
                      << std::left 
                      << std::setw(16)

                      << animal_type; //animal type
            std::cout << std::right 
                      << std::setw(12)
                      << std::fixed
                      << std::setprecision(1)
                      << pounds;  //pounds of food for animal
            std::cout << std::right
                      << std::setw(14)
                      << "$"
                      << string_cost;

            
            std::cout << '\n';
        } else {
            break;
        }
        
        /*I was told to watch for negative values, and example 4 does
          not use negative values at all.*/
        if (std::stold(get_nth_token(line, 2)) > 0) {
            total_day_food_pounds += std::stold(get_nth_token(line, 2));
        }

        if (std::stold(get_nth_token(line, 3)) > 0) {
            raw_dollar_cost += std::stold(get_nth_token(line, 3));
        }
    }
    std::cout << "+";
    for (int i=0; i<48; i++) {
        std::cout << "-";
    }
    std::cout << "+\n";

    /*month is about 30 days for our purposes*/
    estimate_month_food_pounds = total_day_food_pounds*30; 

    {
        /*I put this in a code block so that if I ever use something
          nondescript like "temp" again, it will be 
          immediately obvious what its used for.*/
        long double temp=raw_dollar_cost*100; //convert to cent
        /*round temp to nearest cent, then cast to long long type*/
        total_day_cost_cent = static_cast<long long>(std::round(temp));

        /*same thing as above except in parts, and multiplying by 30
          for the estimates of monthly food costs*/
        temp=raw_dollar_cost*30*100;
        temp=std::round(temp);
        estimate_month_cost_cent = static_cast<long long>(temp);
    }

    std::cout << "\nFeeding Totals\n";
    for (int i=0; i<50; i++) {
        std::cout << "+";
    }
    std::cout << '\n';

    std::cout << "The total zoo food today was: "
              << std::round(total_day_food_pounds*10)/10 //round to tenth
              << " lbs."
              << '\n'
              << "Estimated zoo food for a month is: "
              << std::round(estimate_month_food_pounds*10)/10
              << " lbs."
              << '\n';
    std::cout << '\n';
    std::cout << "The total zoo food cost today was: "
              << ll_cent_to_str_dollar(total_day_cost_cent)
              << '\n'
              << "Estimated zoo food cost for a month is: "
              << ll_cent_to_str_dollar(estimate_month_cost_cent)
              << '\n'
              << '\n';
    
    for (int i=0; i<50; i++) {
        std::cout << "+";
    }
    std::cout << "\n";
    std::cout << std::setw(49)
              << "Remember to always feed safely!";
    std::cout << '\n';
    for (int i=0; i<50; i++) {
        std::cout << "*";
    }
    std::cout << '\n';   

    in_file_s.close(); //close file object

}

/*  function description: The first function that executes.
    parameters:  No parameters, meaning no command line arguments.
    return value: Always the integer 0, since the program was never
                  meant to output any other values.
*/
int main() {
    /*exists in main() to see if another file should be taken or not*/
    bool take_another_data_file = false;

    /*the location of the file. It supports absolute paths, such as 
      /home/user/zooData.txt, but not something like ~/zooData.txt.*/
    std::string file_location = "";  
    
    std::cout << '\n';
    opening_statement();
    std::cout << '\n';
    
    get_input_file(file_location);
    std::cout << '\n'
              << '\n';
    display_input_file(file_location);
    
    std::cout << '\n';
    
    /*ask_for_another_datafile modifies take_another_data_file,
      because it is passed by reference*/
    ask_for_another_datafile(take_another_data_file);
    std::cout << '\n';
    /*keep asking until user doesn't want to process any more
      data files.*/
    while (take_another_data_file==true) {
        get_input_file(file_location);
        std::cout << "\n\n";
        display_input_file(file_location);
        std::cout << '\n';
        ask_for_another_datafile(take_another_data_file);
        if (take_another_data_file == true) {
            std::cout << '\n';
        }
    }
    return 0;
}
