#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <ios>

class node {
public:
  node() { id = -1; locx = 0.0; locy = 0.0; locz = 0.0; validity = 0; }
  int id;
  double locx, locy, locz;
  int validity;
};

class result {
public:
  result() { id = -1; validity = false; }
  int id;
  std::vector<double> values;
  std::vector<bool> rvalid;
  bool validity;
};

std::map<int,node> nodes;
std::map<int,result> results;

int main(int argc, char* argv[])
{
  if (argc<5) {
    std::cerr << "usage: ANSYSResultsMerger LOCX.txt LOCY.txt LOCZ.txt result1.txt ..." << std::endl;
  }

  std::string line;
  int id;
  double value;
  int nResults = 0;

  std::ifstream ifileLOCX(argv[1]);
  getline(ifileLOCX, line);
  while (ifileLOCX >> id >> value) {
    nodes[id].id = id;
    nodes[id].locx = value;
    nodes[id].validity++;
  }
  ifileLOCX.close();

  std::ifstream ifileLOCY(argv[2]);
  getline(ifileLOCY, line);
  while (ifileLOCY>> id >> value) {
    nodes[id].id = id;
    nodes[id].locy = value;
    nodes[id].validity++;
  }
  ifileLOCY.close();

  std::ifstream ifileLOCZ(argv[3]);
  getline(ifileLOCZ, line);
  while (ifileLOCZ>> id >> value) {
    nodes[id].id = id;
    nodes[id].locz = value;
    nodes[id].validity++;
  }
  ifileLOCZ.close();

  std::ifstream ifileResult(argv[4]);
  getline(ifileResult, line);
  while (ifileResult>> id >> value) {
    results[id].id = id;
    results[id].values.push_back(value);
    results[id].rvalid.push_back(true);
  }
  ifileResult.close();
  nResults++;

  if (argc>5) {
    for (int i=5;i<argc;i++) {
      std::ifstream ifileResult(argv[i]);
      getline(ifileResult, line);
      while (ifileResult>> id >> value) {
        std::map<int,result>::iterator itFind = results.find(id);
        if (itFind!=results.end()) {
          itFind->second.values.push_back(value);
          itFind->second.rvalid.push_back(true);
        }
      }
      ifileResult.close();
      nResults++;
    }
  }

  for (std::map<int,result>::iterator it = results.begin();
       it!=results.end();
       ++it) {

    std::map<int,node>::iterator itFind = nodes.find(it->second.id);
    if (itFind!=nodes.end() &&
        itFind->second.validity == 3 &&
        it->second.values.size()==nResults) {

      it->second.validity = true;

      std::cout << std::scientific << itFind->second.locx << ", "
          << std::scientific << itFind->second.locy << ", "
          << std::scientific << itFind->second.locz;

      for (std::vector<double>::iterator itV = it->second.values.begin();
           itV!=it->second.values.end();
           ++itV) {
        std::cout << ", " << std::scientific << *itV;
      }
      std::cout << std::endl;
    }
  }
}
