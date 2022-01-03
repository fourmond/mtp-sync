/**
   \file mtpdevice.hh
   The MTPDevice class,
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

#ifndef _MTPDEVICE_HH
#define _MTPDEVICE_HH

/// Represents an MTP device, the basic
class MTPDevice {
  /// True when the library has been initialized
  static bool initialized;

  /// Ensures the library is initialized
  static void ensureInitialized();

  /// The underlying device
  LIBMTP_mtpdevice_t * device;

  std::map<uint32_t, LIBMTP_folder_t*> folderList;


  void cacheFolderList();

  MTPDevice(LIBMTP_mtpdevice_t * dev);
public:


  /// Returns the list of available devices
  static std::vector<MTPDevice> availableDevices();

  /// Returns the first device
  static MTPDevice getFirst();

  /// Constructs an invalid MTPDevice
  MTPDevice();

  /// Returns true if this device is valid, i.e. actually connected to
  /// something.
  bool isValid() const;

  /// Returns the model name
  std::string modelName() const;


  /// Just dumps all the files
  void dumpFileList();
  
};


#endif
