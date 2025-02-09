#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

const int not_a_reading = -7777;
const int not_a_mounth = -1;


struct Day {
    vector<double> hour {vector<double>(24, not_a_reading)};
};

struct Month {
    int month {not_a_mounth};
    vector<Day> day {32};
};

struct Year {
    int year {-1};
    vector<Month> month{12};
};
void print_year(ofstream& ofs, Year y)
{
    ofs << "YEAR = " << '"' << y.year << '"';
    for(int i = 0; i < y.month.size(); i++)
    {
        if(y.month[i].month != not_a_mounth)
        {
            ofs << " - MONTH = " << '"' << y.month[i].month << '"';
            for(int j = 0; j < y.month[i].day.size(); j++)
            {
                for(int k = 0; k < y.month[i].day[j].hour.size(); k++)
                {
                    if(y.month[i].day[j].hour[k] != not_a_reading)
                    {
                        ofs << " - DAY = " << '"' << j << '"';
                        ofs << " - HOUR = " << '"' << k << '"';
                        ofs << " - TEMP = " << '"' << y.month[i].day[j].hour[k] << '"';
                    }
                }
            }
        }
    }
}

struct Reading {
    int day;
    int hour;
    double temperature;
};

void end_of_loop(istream& ist, char term, const string& str)
{
    if (ist.fail()) {
        ist.clear();
        char ch;
        if (ist >> ch && ch == term) return;
        cout << str << endl;
        return;
    }
    // char ch;
    // if (ist >> ch && ch == term) return;
    //     cout << str << endl;
    //     return;
}

istream& operator>>(istream& is, Reading& r)
{
    char ch;
    is >> ch;
    if (ch && ch != '(') {
        is.unget();
        is.clear(ios_base::failbit);
        return is;
    }

    char ch2;

    int d, h;
    double t;
    is >> d >> h >> t >> ch2;
    if (!is || ch2 != ')')
    {
        wcout << L"Ошибка" << endl;
        return is;
    }
    r.day = d;
    r.hour = h;
    r.temperature = t;
    return is;
}

// int month_to_int(string str)
// {
//     enum class Months {jan, feb, mar, apr, may, jun, jul, aug, set, oct, nov, dec};
// }

bool is_valid(Reading& rd)
{
    if(rd.day > 31 || rd.day < 1) return false;
    if(rd.hour > 23 || rd.hour < 0) return false;
    if(rd.temperature < -200 || rd.temperature > 200) return false;
    return true;
}

int month_to_int(string str)
{
    vector<string> month_input_tb1 = {"jan", "feb", "mar", "apr", "may", "jun",
    "jul", "aug", "sep", "oct", "nov", "dec"};

    for(int i = 0; i < 12; i++) if(month_input_tb1[i] == str) return i;
    return -1;
}

istream& operator>>(istream& is, Month& m)
{
    char ch;
    if(is>>ch && ch != '{') {
        is.unget();
        is.clear(ios_base::failbit);
        return is;
    }

    string month_marker, mm;
    is >> month_marker >> mm;
    if(!is || month_marker != "month") {
        wcout << L"Неверное начало Month" << endl;
        return is;
    }

    m.month = month_to_int(mm);

    Reading r;
    int duplicates = 0;
    int invalide = 0;
    for (Reading r; is >> r; ) {
        if(is_valid(r)) {
            if (m.day[r.day].hour[r.hour] != not_a_reading) {
                ++duplicates;
            }
            m.day[r.day].hour[r.hour] = r.temperature;
        }
        else
            ++invalide;
    }
    if (invalide) {
        wcout << L"Неверные данные в Month", invalide;
        return is;
    }
    if (duplicates) {
        wcout << L"Повторяющиеся показания в Month", duplicates;
        return is;
    }
    
    end_of_loop(is, '}', "Неправильный конец Month");
    return is;
}

istream& operator>>(istream& is, Year& y)
{
    char ch;
    if(is >> ch && ch != '{') {
        is.unget();
        is.clear(ios_base::failbit);
        return is;
    }
    string year_marker;
    int yy;
    is >> year_marker >> yy;
    if (!is || year_marker != "year") {
        wcout << L"Неверное начало Year" << endl;
        return is;
    }
    y.year = yy;

    while(true) {
        Month mth;
        if(!(is >> mth)) break;
        y.month[mth.month] = mth;
    }

    end_of_loop(is, '}', "Неправильный конец Year");
    return is;
}

int main()
{
    setlocale(LC_ALL, "");
    string iname;
    wcout << L"Введите имя входного файла\n";
    cin >> iname;
    ifstream ifs {iname};

    string oname;
    wcout << L"Введите имя выходного файла\n";
    
    cin >> oname;
    ofstream ofs {oname};
    vector<Year> ys;
    while(true) {
        Year y;
        if(!(ifs>>y)) break;
        ys.push_back(y);
    }
    wcout << L"Считано " << ys.size() << L" годичных записей\n";

    for(Year& y : ys) print_year(ofs, y);
}