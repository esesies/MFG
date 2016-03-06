#ifndef UTILS_H
#define UTIIS_H

#include <string>
#include <vector>
#include <algorithm>

class Utils
{
public:
  //static functions: We don´t need to create instances of the object to use the functions.

  //unsigned int Split
  //split a string <txt> everywhere a certain character is found.
  //store the resulting substrings in the vector <strs>
  //returns the  size of the vector
  static unsigned int Split(const std::string& txt, std::vector<std::string>& strs, char ch)
  {
    int pos = txt.find(ch); //find the first instance of this ch. Returns the position.
                            //if it cannot find it, returns std::string::npos
    int initialPos = 0;
    strs.clear();  //Clear the vector
    while (pos != std::string::npos)
    {
      strs.push_back(txt.substr(initialPos, pos - initialPos));
      initialPos = pos + 1;
      pos = txt.find(ch, initialPos);
    }
    //Add the last one
    strs.push_back(txt.substr(initialPos, std::min<int>(pos, txt.size() - (initialPos + 1))));

    return strs.size();
  }
};


#endif //UTILS_H
