#include <time.h>

/**
 * Parse a string to a integer.
 * @param string to parse
 * @return int
 */
int stringToInt(char *string){
    int num = 0, i = 0;
    while (*(string + i) != 0){
        num = num * 10 + (*(string + i) - '0') ;
        i++;
    }
    return num;
}

/**
 * Parses string with data in the format dd-mm-aaaa hh:mm:ss to the struct tm.
 * @param string string to parse.
 * @param tm struct to use.
 */
void string_time(char *string, struct tm *tm){
    // Parse day.
    tm->tm_mday = ((*string++ - '0') * 10);
    tm->tm_mday += ((*string++ - '0'));
    // Skip -
    string++;
    // Parse month.
    tm->tm_mon = ((*string++ - '0') * 10) ;
    tm->tm_mon += ((*string++ - '0'));
    // Skip -
    string++;
    // Parse year.
    tm->tm_year = ((*string++ - '0') * 1000);
    tm->tm_year += ((*string++ - '0') * 100);
    tm->tm_year += ((*string++ - '0') * 10);
    tm->tm_year += ((*string++ - '0'));
    // Skip space
    string++;
    // Parse hour.
    tm->tm_hour = ((*string++ - '0') * 10);
    tm->tm_hour += ((*string++ - '0'));
    // Skip :
    string++;
    // Parse min.
    tm->tm_min = ((*string++ - '0') * 10);
    tm->tm_min += ((*string++ - '0'));
    // Skip :
    string++;
    // Parse seconds.
    tm->tm_sec = ((*string++ - '0') * 10);
    tm->tm_sec += ((*string - '0'));
    // Set unused struct members.
    tm->tm_wday = 0;
    tm->tm_yday = 0;
    tm->tm_isdst = 0;
}

int main (){
    // "dd-mm-aaaa hh:mm:ss”
    struct tm timestamp;
    char date[] = "12-08-1994 23:58:00";
    string_time(date, &timestamp);
    return 0;
}