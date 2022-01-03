/**
   \file mtpdevice.cc
   
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
#include <mtpdevice.hh>

/// A small wrapper that runs a libmtp function and raises an
/// exception if it failed.

template <typename... Args> void errorWrapMTP(LIBMTP_error_number_t (*func)(Args...), Args... args) {
  LIBMTP_error_number_t err = func(args...);
  if(err != LIBMTP_ERROR_NONE) {
    std::ostringstream ss;
    ss << "Error: " << err;
    throw std::runtime_error(ss.str());
  }
}

bool MTPDevice::initialized = false;

void MTPDevice::ensureInitialized()
{
  if(! initialized) {
    LIBMTP_Init();
    initialized = true;
  }
}


MTPDevice::MTPDevice() :
  device(NULL)
{
}

MTPDevice::MTPDevice(LIBMTP_mtpdevice_t * dev) :
  device(dev)
{
}

std::vector<MTPDevice> MTPDevice::availableDevices()
{
  std::vector<MTPDevice> rv;
  ensureInitialized();
  LIBMTP_mtpdevice_t * devices;
  errorWrapMTP(LIBMTP_Get_Connected_Devices, &devices);

  while(devices) {
    rv.push_back(MTPDevice(devices));
    devices = devices->next;
  }
  free(devices);
  return rv;
}

std::string MTPDevice::modelName() const
{
  if(! device)
    throw std::logic_error("Getting name of an invalid MTPDevice");
  char * mn = LIBMTP_Get_Modelname(device);
  if(! mn)
    throw std::runtime_error("Could not get model name");
  std::string rv = mn;
  free(mn);
  return rv;
}

void MTPDevice::cacheFolderList()
{
  if(! device)
    throw std::logic_error("Getting name of an invalid MTPDevice");

  // if(folderList.size() > 0)
  //   throw std::logic_error("Already cached ?");

  std::function<void (LIBMTP_folder_t*)> add =
    [this,&add](LIBMTP_folder_t* fld) {
      while (fld != NULL) {
        folderList[fld->folder_id] = fld;
        if(fld->child) {
          std::cout << "Adding child: " << fld->child << std::endl;
          add(fld->child);
        }
        fld = fld->sibling;
      }
    };

  LIBMTP_folder_t* fld = LIBMTP_Get_Folder_List(device);
  add(fld);
}


void MTPDevice::dumpFileList()
{
  if(! device)
    throw std::logic_error("Getting name of an invalid MTPDevice");

  LIBMTP_file_t *filelist;

  std::cout << "Files: " << std::endl;


  cacheFolderList();
  for(auto v : folderList) {
    LIBMTP_folder_t* fld = v.second;
    std::cout << " * #" << fld->folder_id
              << ": " << fld->name << " ("
              << fld->parent_id << ", "
              << fld->storage_id << ")" << std::endl;
  }


  filelist = LIBMTP_Get_Files_And_Folders(device, 0, 0);
  while (filelist != NULL) {
    LIBMTP_file_t *tmp;
    // Do something on each element in the list here...
    tmp = filelist;
    std::cout << " * #" << tmp->item_id
              << ": " << tmp->filename << " ("
              << tmp->parent_id << ")" << std::endl;
    
    filelist = filelist->next;
    LIBMTP_destroy_file_t(tmp);
  }

}


