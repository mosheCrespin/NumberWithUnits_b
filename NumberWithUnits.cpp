
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include "NumberWithUnits.hpp"
#include <iomanip>

using namespace std;
namespace ariel{
    const double EPS=0.0001;
    static unordered_map<std::string,unordered_map<string,double>> data_table;//examplel---> km:{meter,1000} 1 km=1000 mater
    
    //constructors
    NumberWithUnits::NumberWithUnits(double num, const std::string &type){
        if(data_table.find(type)==data_table.end())//check if this type exists in our data table
        {
            throw invalid_argument("this type does not exists in our data table");
        }
        this->type=type;
        this->num=num;
    }
 
    /*
     * this method responsible for updating the data of our data_table
     * for every line in our file:
     * 1. enter to temporary vars the data for checking
     * 2. check if one of the types already exists in our table
     * 3. enter the data like this -> type1=amount_2 [type2], type2=1/amount_2[type1]
     * 4. for every neighbor of the exists member update for the new member, the update goes like that:
     * 4.1 let's say v is the iteration member, then take the units between v to exists -> say equal X
     * 4.2 then take the units between exist to not exists -> say equal Y
     * 4.3 now v=X*Y [not_exists]
     * 4.4 not_exists= 1/x*Y [v]
    */
    void NumberWithUnits::read_units(ifstream& units_file){
        double amount1{};
        string type1;
        string equal;
        string type2;
        double amount2{};
        if(!units_file){//check if the file exists
            exit(EXIT_FAILURE);
        }
        while(units_file >> amount1 >> type1 >> equal >> amount2 >> type2)//while everything is correct and there is more lines
        {
            string exist;
            string not_exist;
            if(data_table.find(type1)==data_table.end())//if type 1 is not exists
            {
                exist = type2;
                not_exist=type1;
            }else
            {
                exist=type1;
                not_exist=type2;
            }
            data_table[type1][type2]=amount2;
            data_table[type2][type1]=1/amount2;
            data_table[type1][type1]=1;
            data_table[type2][type2]=1;


            //update the rest of the family
            for(auto &v: data_table[exist])
            {
                if(v.first==not_exist){continue;}//if already exists there is nothing to do, then continue

                double temp=data_table[v.first][exist];
                data_table[v.first][not_exist]=temp*data_table[exist][not_exist];
                data_table[not_exist][v.first]=1/data_table[v.first][not_exist];
            }
        }




    }

    void NumberWithUnits::print_data_table() {
        cout << "this is our converted data" << endl;
        for (auto &v: data_table) {
            cout << "Unit->" << v.first << endl;

            for (auto &g : data_table[v.first]) {
                cout << "------>";
                cout << setprecision(2)<<  g.second << " " << g.first << endl;
            }
        }
    }

    /*
     * check if src and dest has the same dim if so returns true, O.W false
     */
    bool NumberWithUnits::does_same_dim(const string &src, const string &dest) {
        if(data_table.find(src)!=data_table.end())//if src exists
        {
            return data_table[src].find(dest)!=data_table[src].end(); //if dest also exists
        }

        return false; //O.W return false
    }

    /*
     * this method get 3 parameters:
     * src is the from dim
     * dest is the dest dim
     * amount is how many units from src we want to convert to dest
     * example:
     * lets say 1 dollar = 2 ILS
     * then if we want to convert 4 ILS to dollar
     * we send src= ILS , dest= Dollar, amount =4
     * we should get 2
     * so 4[shekel]=2[dollar]
    */
    double NumberWithUnits::conversion(const string &src, const string &dest, double amount) {

        if (!does_same_dim(src, dest)) {
            throw invalid_argument("Units do not match - ["+src+"] cannot be converted to ["+dest+"]");
        }
        return data_table[src][dest]*amount;
    }
    ///////////////////////////operators//////////////////////////////


    /////////////////////////unary operators///////////////////////////


    NumberWithUnits NumberWithUnits::operator+(){
        return NumberWithUnits(this->num,this->type);
    }

    //unary minus
    NumberWithUnits NumberWithUnits::operator-() {
        return NumberWithUnits(-this->num, this->type);
    }



    //////////////////////increment and decrements operators//////////////////

    NumberWithUnits& NumberWithUnits::operator-=(const NumberWithUnits &second) {
        this->num-=conversion(second.type, this->type, second.num);
        return *this;
    }

    NumberWithUnits& NumberWithUnits::operator+=(const NumberWithUnits &second){
        this->num+=conversion(second.type, this->type, second.num);
        return *this;
    }

    NumberWithUnits NumberWithUnits::operator-(const NumberWithUnits &second) {
        double temp= conversion(second.type, this->type, second.num);
        return NumberWithUnits(this->num-temp, this->type);
    }
    NumberWithUnits NumberWithUnits::operator+(const NumberWithUnits &second){
        double temp= conversion(second.type, this->type, second.num);
        return NumberWithUnits(this->num+temp, this->type);
    }

    ////////////////////Multiplication operator////////////////////

    //right side
    NumberWithUnits operator*(const double d, const NumberWithUnits &N) {
        return NumberWithUnits(d*N.num,N.type);
    }
    //left side so define as friend func
    NumberWithUnits NumberWithUnits::operator*(const double d) {
        return NumberWithUnits(d*this->num,this->type);
    }




    //////////////////comparator operators (<, >, <=, >=, ==, !=)////////////////
    bool NumberWithUnits::operator==(const NumberWithUnits &second) const{
        double temp= conversion(second.type, this->type, second.num);
        return (abs(this->num-temp))<=EPS;
    }

    bool NumberWithUnits::operator!=(const NumberWithUnits &second) const{
        return !(*this==second);
    }

    bool NumberWithUnits::operator<(const NumberWithUnits &second) const{
        double temp= conversion(second.type, this->type, second.num);
        return *this!=second && this->num<temp;
    }
    bool NumberWithUnits::operator>(const NumberWithUnits &second) const{
        double temp= conversion(second.type, this->type, second.num);
        return *this!=second && this->num>temp;
    }
    bool NumberWithUnits::operator>=(const NumberWithUnits &second) const{
        return (*this>second)||(*this==second);

    }
    bool NumberWithUnits::operator<=(const NumberWithUnits &second) const{
        return (*this<second)||(*this==second);
    }


    ///////////////////////////increment and decrement operators(++a, a++, a--, --a//////////////////////////////
    //prefix increment
    NumberWithUnits& NumberWithUnits::operator++(){
        this->num++;
        return *this;
    }


    NumberWithUnits& NumberWithUnits::operator--(){
        this->num--;
        return *this;
    }
    NumberWithUnits  NumberWithUnits::operator--(int flag){
        NumberWithUnits copy = NumberWithUnits(this->num,this->type);
        this->num--;
        return copy;
    }
    NumberWithUnits  NumberWithUnits::operator++(int flag){
        NumberWithUnits copy = NumberWithUnits(this->num,this->type);
        this->num++;
        return copy;
    }



    ///////////////ostream and isstream operators//////////////////
    ostream &operator<<(ostream &os,const NumberWithUnits &temp){
        return os<<temp.num<<"["<<temp.type<<"]";
    }

    /*
     * this method take an istream object and transform it into a NumberWithUnits object
     * the istream object looks like that A-> "amount[type]" or B-> "amount[type ]"
     * 1. enter the istream object into temporary variables for correctness checking
     * 2. check if it's option A or B
     * 3. if A then our type suppose to look like this type]
     * 3.1. we should remove the right bracket-> then find the right bracket pos and substr
     * 4. if B then our type will be in a correctness form
     * 4.1 just remove the right bracket from istream object
     * 5. check if our temporary type exists in our data table
     * 5.1 if so then update the received object to be the temporary amount and type
     * 5.2 O.W throw exception
     */




    istream &operator>>(istream &is, NumberWithUnits &temp){
        char left_bracket{};
        char right_bracket{};
        double num_t{};
        string type_t;
        is >> num_t >> left_bracket >> type_t;
        if(type_t.find(']')!=string::npos)//if there is a right bracket
        {
            size_t pos_ = type_t.find(']');
            type_t = type_t.substr(0, pos_);
        }else{
            is>>right_bracket;
        }

        if(data_table.find(type_t)==data_table.end())
        {
            throw invalid_argument("this type does not exists in our data table");
        }
        temp.num=num_t;
        temp.type=type_t;
        return is;
    }


}
