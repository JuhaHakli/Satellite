#include <iostream>
#include <limits>
#include <fstream>
#include <cmath>
#include <vector>
#include <sstream>
#include <stdlib.h>

using namespace std;

const int EARTH_RADIUS = 6371;
const double TO_RAD = M_PI / 180.0f;;

struct vec3
{
    double x; double y; double z; vec3 operator+(vec3); vec3 operator-(vec3); vec3 operator-(); double operator*(vec3); vec3 cross_product(vec3); double length();
};

vec3 vec3::operator+(vec3 param)
{
    vec3 temp;
    temp.x = x + param.x;
    temp.y = y + param.y;
    temp.z = z + param.z;
    return temp;
}

vec3 vec3::operator-(vec3 param)
{
    vec3 temp;
    temp.x = x - param.x;
    temp.y = y - param.y;
    temp.z = z - param.z;
    return temp;
}

vec3 vec3::operator-()
{
    vec3 temp;
    temp.x = -x;
    temp.y = -y;
    temp.z = -z;
    return temp;
}

double vec3::operator*(vec3 param)
{
    vec3 temp;
    temp.x = x * param.x;
    temp.y = y * param.y;
    temp.z = z * param.z;
    return (temp.x + temp.y + temp.z);
}

vec3 vec3::cross_product(vec3 param)
{
    vec3 temp;
    temp.x = y*param.z - z*param.y;
    temp.y = z*param.x - x*param.z;
    temp.z = x*param.y - y*param.x;
    return temp;
}

double vec3::length(){return sqrt(x*x + y*y + z*z);}

struct point
{
    vec3 loc;
    string name;
    //vector<point> distance_between;
    vector<bool> eyes_on;
};

//Transform geographical coordinates to Cartesian
vec3 geo_to_car(double lat, double lon, double alt)
{
    //convert to radians;
    cout << "MUUTETTIIN LAT: " << lat << " LON: " << lon << " ALT: " << alt << endl;
    lat = lat * TO_RAD;
    lon = lon * TO_RAD;
    double r = EARTH_RADIUS + alt;
    vec3 temp;
    temp.x = r * cos(lat) * cos(lon);
    temp.y = r * cos(lat) * sin(lon);
    temp.z = r * sin(lat);
    cout << "MUUTETTIIN LAT: " << lat << " LON: " << lon << " ALT: " << alt << endl;
    return temp;
}

//Do the satellites have lino of sight?
bool in_view(vec3 alpha, vec3 beta)
{

    vec3 numerator = -alpha.cross_product(-beta);
    vec3 denominator = beta - alpha;
    double distance = numerator.length() / denominator.length();

    //cout << distance << endl;

    if(distance > EARTH_RADIUS)
        return true;
    //else
        //return false;
    vec3 A = beta - alpha;
    vec3 B = -alpha;
    vec3 C = -beta;
    if(A * B > 0 && A * C < 0)
        return false;

    return true;

}

vec3 move_altitude(vec3 param, double alt)
{
    vec3 movement = param;
    double length = param.length();
    param.x = param.x/length*alt;
    param.y = param.y/length*alt;
    param.z = param.z/length*alt;
    return param;
}

int main()
{
    vector<point> points;
    string line;
    ifstream data;
    data.open("data7.txt");

    if(data.is_open())
    {
        point add;
        string value;
        double lat;
        double lon;
        double alt;
        while(getline(data, line))
        {
            if(line[0] == '#')
                ;
            else if(line[0] == 'S')
            {
                stringstream ss(line);

                getline(ss, add.name, ',');
                getline(ss, value, ',');
                lat = strtod(value.c_str(), NULL);
                getline(ss, value, ',');
                lon = strtod(value.c_str(), NULL);
                getline(ss, value);
                alt = strtod(value.c_str(), NULL);

                add.loc = geo_to_car(lat, lon, alt);
                //add.loc = add.loc + move_altitude(add.loc, alt);

                points.push_back(add);
            }
            else
            {
                stringstream ss(line);

                getline(ss, add.name, ',');
                getline(ss, value, ',');
                lat = strtod(value.c_str(), NULL);
                getline(ss, value, ',');
                lon = strtod(value.c_str(), NULL);

                add.loc = geo_to_car(lat, lon, 0.0f);
                //add.loc = add.loc + move_altitude(add.loc, 0.0);
                cout << "STARTLOC: " << add.loc.x << "," << add.loc.y << "," << add.loc.z << endl;

                add.name = "START";

                points.push_back(add);

                add.name = "END";
                lat = strtod(value.c_str(), NULL);
                getline(ss, value, ',');
                lon = strtod(value.c_str(), NULL);

                add.loc = geo_to_car(lat, lon, 0.0f);
                //add.loc = add.loc + move_altitude(add.loc, 0.0);
                cout << "ELOC: " << add.loc.x << "," << add.loc.y << "," << add.loc.z << endl;

                points.push_back(add);
            }
        }
    }

    for(int i = 0; i < points.size(); ++i)
    {
        for(int j = 0; j < points.size(); ++j)
        {
            points[i].eyes_on.push_back(in_view(points[i].loc, points[j].loc));
        }
    }

    for(int i = 0; i < points.size(); ++i)
    {
        cout << points[i].name << " " << points[i].loc.x << " " << points[i].loc.y << " " << points[i].loc.z << endl;
        for(int j = 0; j < points[i].eyes_on.size(); ++j)
        {
            cout << points[i].eyes_on[j];
        }
        cout << endl;
    }
    //19
    int i = 8;

    cout << points[i].name << " " << points[i].loc.x << " " << points[i].loc.y << " " << points[i].loc.z << endl;
    for(int j = 0; j < points[i].eyes_on.size(); ++j)
    {
        cout << points[j].name << ": ";
        if(i == j)
            cout << "ITSE";
        else if(points[i].eyes_on[j] == true)
            cout << "NAKYY";
        else
            cout << "EI NAY";
        cout << endl;
    }
    cout << endl;

    return 0;
}
