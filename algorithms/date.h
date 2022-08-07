#pragma once

#include <string>
#include <vector>
#include <algorithm>

/**
 * @brief The Date class gives an implementstion of date by day, month and year
 *
 * @author  Evgeny Gerasimov
 * @version 1.0; 10.04.2022
 */
class Date {
private:
    /**
     * Worst case time complexity - O(1)
     *
     * @brief jdn returns julian day number by date
     * @param day   - day of the date
     * @param month - month of the date
     * @param year  - year of the date
     * @return julian day number by date
     */
    static long long jdn(int day, int month, int year) {
        long long a = (14 - (long long)month) / 12;
        long long y = (long long)year + 4800 - a;
        long long m = (long long)month + 12*a - 3;

        return (long long)day + (long long)((153*m + 2) / 5) + 365*y + (long long)(y / 4) -
                (long long)(y / 100) + (long long)(y / 400) - 32045;
    }

public:
    long long date; // julian day number

    Date() = default;
    Date(int year, int month, int day) {
        date = jdn(day, month, year);
    }

    void increaseDay() {
        date++;
    }

    /**
     * Worst case time complexity - O(1)
     *
     * @brief operator < compares 2 dates
     * @param date1 - first date
     * @param date2 - second date
     * @return true if date1 is less than date2
     */
    friend bool operator < (const Date& date1, const Date& date2) {
        return date1.date < date2.date;
    }

    /**
     * Worst case time complexity - O(1)
     *
     * @brief operator == compares 2 dates
     * @param date1 - first date
     * @param date2 - second date
     * @return true if date1 is equal to date2
     */
    friend bool operator == (const Date& date1, const Date& date2) {
        return date1.date == date2.date;
    }

    /**
     * Worst case time complexity - O(1)
     *
     * @brief operator <= compares 2 dates
     * @param date1 - first date
     * @param date2 - second date
     * @return true if date1 is less or equal than date2
     */
    friend bool operator <= (const Date& date1, const Date& date2) {
        return date1 < date2 || date1 == date2;
    }

    /**
     * Worst case time complexity - O(1)
     *
     * @brief operator > compares 2 dates
     * @param date1 - first date
     * @param date2 - second date
     * @return true if date1 is greater than date2
     */
    friend bool operator > (const Date& date1, const Date& date2) {
        return !(date1 <= date2);
    }

    /**
     * Worst case time complexity - O(1)
     *
     * @brief GetDateFromString returns Date from given string date given in format YYYY-MM-DD
     * @param string_date date in format YYYY-MM-DD
     * @return Date from given string date given in format YYYY-MM-DD
     */
    static Date GetDateFromString(std::string string_date) {
        std::string year(string_date.cbegin(), string_date.cbegin() + 4);
        std::string month(string_date.cbegin() + 5, string_date.cbegin() + 7);
        std::string day(string_date.cbegin() + 8, string_date.cend());

        int year_int  = stoi(year);
        int month_int = stoi(month);
        int day_int   = stoi(day);

        Date time(year_int, month_int, day_int);

        return time;
    }
};
















