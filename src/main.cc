/**
   \file main.cc
   entry point of mtp-sync
   Copyright 2021 by Vincent Fourmond

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see
   <http://www.gnu.org/licenses/>.
*/


#include <headers.hh>
#include <getopt.h>

#include <mtpdevice.hh>

int main(int argc, char **argv)
{
  MTPDevice device;
  while (1) {
      int option_index = 0;
      static struct option long_options[] = {
          {"list-devices",  false,       0,  'L'},
          {"list-files",  false,       0,  'l'},
          {0, 0, 0, 0 }
      };

      int c = getopt_long(argc, argv, "Ll",
                          long_options, &option_index);
      if (c == -1)
          break;
      switch(c) {
      case 'L': {
        std::vector<MTPDevice> devices = MTPDevice::availableDevices();
        std::cout << "Found: " << devices.size()
                  << " devices" << std::endl;
        for(MTPDevice & d : devices) {
          std::cout << " * model: " << d.modelName() << std::endl;
        }
        break;
      }
      case 'l': {
        // take the first device
        std::vector<MTPDevice> devices = MTPDevice::availableDevices();
        MTPDevice d = devices[0];
        d.dumpFileList();
      }
      }
  }
  return 0;
}
