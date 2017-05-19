#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <map>
#include <vector>
#include <ios>
#include <cmath>

class result {
public:
  result() { }
  double locx, locy, locz;
  double getCoord1() const {
    switch (mode) {
    case 1: // XY
      return locx;
    case 2: // XZ
      return locx;
    case 3: // YZ
      return locy;
    }
    return 0;
  }
  double getCoord2() const {
    switch (mode) {
    case 1: // XY
      return locy;
    case 2: // XZ
      return locz;
    case 3: // YZ
      return locz;
    }
    return 0;
  }
  double distance(const result& other) const {
    double d1 = getCoord1() - other.getCoord1();
    double d2 = getCoord2() - other.getCoord2();
    return std::sqrt(d1*d1+d2*d2);
  }
  int mode;
  std::vector<double> values;
};

std::map<int,result> results1;
std::map<int,result> results2;
std::map<int,int> match;

int main(int argc, char* argv[])
{
  if (argc!=4) {
    std::cerr << "usage: ANSYSResultsMerger XY|XZ|YZ result1.txt result2.txt" << std::endl;
    return 0;
  }

  int mode = 0;
  std::string smode = argv[1];
  if (smode=="XY") {
    mode = 1;
  } else if (smode=="XZ") {
    mode = 2;
  } else if (smode=="YZ") {
    mode = 3;
  }
  if (mode==0) {
    std::cerr << "usage: ANSYSResultsMerger XY|XZ|YZ result1.txt result2.txt" << std::endl;
    return 0;
  }

  std::string line;
  int id;
  double value;

  int idx = 0;
  std::ifstream ifile1(argv[2]);
  getline(ifile1, line);
  while (getline(ifile1, line)) {
    result r;
    std::istringstream iss(line);
    iss >> value;
    r.locx = value;
    iss >> value;
    r.locy = value;
    iss >> value;
    r.locz = value;
    while (iss >> value) r.values.push_back(value);
    r.mode = mode;
    results1[idx] = r;
    idx++;
  }
  ifile1.close();

  idx = 0;
  std::ifstream ifile2(argv[3]);
  getline(ifile2, line);
  while (getline(ifile2, line)) {
    result r;
    std::istringstream iss(line);
    iss >> value;
    r.locx = value;
    iss >> value;
    r.locy = value;
    iss >> value;
    r.locz = value;
    while (iss >> value) r.values.push_back(value);
    r.mode = mode;
    results2[idx] = r;
    idx++;
  }
  ifile2.close();

  int nEntries1 = results1.size();

  int nResults1 = results1.begin()->second.values.size();
  int nResults2 = results2.begin()->second.values.size();

  int count = 0;
  for (std::map<int,result>::iterator it1 = results1.begin();
       it1!=results1.end();
       ++it1) {
    result& r1 = it1->second;

    std::map<double,int> dmap;
    double d;
    for (std::map<int,result>::iterator it2 = results2.begin();
         it2!=results2.end();
         ++it2) {
      result& r2 = it2->second;

      d = r1.distance(r2);

      dmap[d] = it2->first;
      if (d==0.) break;
    }

    match[it1->first] = dmap.begin()->second;

    dmap.clear();

    count++;

    std::cerr << "\r" << 100*count/nEntries1 << "\% done" << std::flush;
    //if (count==5000) break;
  }
  std::cerr << "\r" << std::flush;

  std::cout << std::setw(16) << "LOCX1"
      << std::setw(16) << "LOCY1"
      << std::setw(16) << "LOCZ1";
  for (int i=0;i<nResults1;i++) {
    std::cout << std::setw(15) << "RESULT1_" << i;
  }
  std::cout << std::setw(16) << "LOCX2"
      << std::setw(16) << "LOCY2"
      << std::setw(16) << "LOCZ2";
  for (int i=0;i<nResults2;i++) {
    std::cout << std::setw(15) << "RESULT2_" << i;
  }
  std::cout << std::endl;

  for (std::map<int,int>::iterator it = match.begin();
       it!=match.end();
       ++it) {

    result & r1 = results1[it->first];
    result & r2 = results2[it->second];

    std::cout << std::setw(16) << std::scientific << r1.locx
        << std::setw(16) << std::scientific << r1.locy
        << std::setw(16) << std::scientific << r1.locz;

    for (std::vector<double>::iterator itV = r1.values.begin();
         itV!=r1.values.end();
         ++itV) {
      std::cout << std::setw(16) << std::scientific << *itV;
    }

    std::cout << std::setw(16) << std::scientific << r2.locx
        << std::setw(16) << std::scientific << r2.locy
        << std::setw(16) << std::scientific << r2.locz;

    for (std::vector<double>::iterator itV = r2.values.begin();
         itV!=r2.values.end();
         ++itV) {
      std::cout << std::setw(16) << std::scientific << *itV;
    }

    std::cout << std::endl;
  }
}
