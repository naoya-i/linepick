
#include <stdio.h>

#include <sstream>
#include <fstream>
#include <iostream>

#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include "optparse.h"

int main(int argc, char** argv) {
  optparse_t opts("Usage:\n  linepick"
                                    " -i <INPUT FILE>"
                                    " -d <TOTAL DIVISION>"
                                    " -p <WHERE TO PICK FROM THE DIVISION>"
                  ,
                  "p:i:d:", "idp", argc, argv);
  if(!opts.isGood()) return 0;
  
  std::ifstream ifsInput(opts.of('i').c_str(), std::ios::binary|std::ios::ate);
  size_t filesize = ifsInput.tellg();
  ifsInput.close();
  
  int                 numParallel = atoi(opts.of('d').c_str());
  size_t              stride      = filesize/numParallel + 1,
    pos                           = atoi(opts.of('p').c_str()),
    offset                        = stride * pos;
  std::ifstream       ifsTarget(opts.of('i').c_str(), std::ios::binary);

  //
  // Seek to the head.
  ifsTarget.seekg(offset);

  std::string ln;

  // If the offset is non-zero, skip the current line.
  if(0 != offset)
    getline(ifsTarget, ln);

  // Push each line into the sub process.
  while(ifsTarget.tellg() <= offset+stride && getline(ifsTarget, ln))
    std::cout << ln << std::endl;

  ifsTarget.close();

  return 0;
}
