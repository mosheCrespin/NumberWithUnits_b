
#ifndef SETUP_NUMBERWITHUNITS_HPP
#define SETUP_NUMBERWITHUNITS_HPP
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
namespace ariel{
    class NumberWithUnits{
    private:
        std::string type;
        double num;
    public :
        NumberWithUnits()= default;
        NumberWithUnits(double num, const std::string &type);
        ~ NumberWithUnits()= default;;
        static void read_units(std::ifstream& units_file);
        static void print_data_table();
        static bool does_same_dim(const std::string &src, const std::string &dest);
        static double conversion(const std::string &src, const std::string &dest, double amount);
        NumberWithUnits operator-() ; //unary minus
        NumberWithUnits& operator-=(const NumberWithUnits &second); //decrements
        NumberWithUnits operator-(const NumberWithUnits &second) ; //binary decrements
        NumberWithUnits operator+(); //unary plus
        NumberWithUnits& operator+=(const NumberWithUnits &second); //add to exists (this)
        NumberWithUnits operator+(const NumberWithUnits &second); //binary plus
        NumberWithUnits& operator++();//prefix increment
        NumberWithUnits operator++(int flag);// postfix increment
        NumberWithUnits& operator--();//prefix increment
        NumberWithUnits operator--(int flag); //postfix increment
        bool operator==(const NumberWithUnits &second) const;
        bool operator!=(const NumberWithUnits &second) const;
        bool operator<(const NumberWithUnits &second) const;
        bool operator>(const NumberWithUnits &second) const;
        bool operator>=(const NumberWithUnits &second) const;
        bool operator<=(const NumberWithUnits &second) const;
        NumberWithUnits operator*(double d);
        friend NumberWithUnits operator*(double d,const NumberWithUnits &N);
        friend std::ostream &operator<<(std::ostream &os,const NumberWithUnits &temp);
        friend std::istream &operator>>(std::istream &is, NumberWithUnits &temp);// not const because we do want to modify this ob
    };
}



#endif //SETUP_NUMBERWITHUNITS_HPP
