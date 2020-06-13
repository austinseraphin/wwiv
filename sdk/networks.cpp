/**************************************************************************/
/*                                                                        */
/*                          WWIV Version 5.x                              */
/*             Copyright (C)2014-2020, WWIV Software Services             */
/*                                                                        */
/*    Licensed  under the  Apache License, Version  2.0 (the "License");  */
/*    you may not use this  file  except in compliance with the License.  */
/*    You may obtain a copy of the License at                             */
/*                                                                        */
/*                http://www.apache.org/licenses/LICENSE-2.0              */
/*                                                                        */
/*    Unless  required  by  applicable  law  or agreed to  in  writing,   */
/*    software  distributed  under  the  License  is  distributed on an   */
/*    "AS IS"  BASIS, WITHOUT  WARRANTIES  OR  CONDITIONS OF ANY  KIND,   */
/*    either  express  or implied.  See  the  License for  the specific   */
/*    language governing permissions and limitations under the License.   */
/**************************************************************************/
#include "sdk/networks.h"

#include "core/datafile.h"
#include "core/file.h"
#include "core/jsonfile.h"
#include "core/log.h"
#include "core/stl.h"
#include "core/strings.h"
#include "sdk/config.h"
#include "sdk/filenames.h"
#include <stdexcept>
#include <string>
#include <vector>

using namespace wwiv::core;
using namespace wwiv::stl;
using namespace wwiv::strings;

// ReSharper disable once CppUnusedIncludeDirective
#include "sdk/networks_cereal.h"

namespace wwiv::sdk {

const int Networks::npos;  // reserve space.

Networks::Networks(const Config& config) : datadir_(config.datadir()) {
  if (!config.IsInitialized()) {
    return;
  }

  if (!File::Exists(FilePath(datadir_, NETWORKS_JSON)) &&
      !File::Exists(FilePath(datadir_, NETWORKS_DAT))) {
    return;
  }

  initialized_ = Load();
  if (!initialized_) {
    LOG(ERROR) << "Failed to read " << NETWORKS_JSON << " or " << NETWORKS_DAT;
  }
  initialized_ = true;
}

const net_networks_rec& Networks::at(const std::string& name) const {
  for (const auto& n : networks_) {
    if (iequals(name.c_str(), n.name)) {
      return n;
    }
  }
  throw std::out_of_range(StrCat("Unable to find network named: ", name));
}

net_networks_rec& Networks::at(const std::string& name) {
  for (auto& n : networks_) {
    if (iequals(name, n.name)) {
      return n;
    }
  }
  throw std::out_of_range(StrCat("Unable to find network named: ", name));
}

Networks::~Networks() = default;

 auto Networks::network_number(const std::string& network_name) const -> size_type {
   auto i = 0;
  for (const auto& n : networks_) {
    if (iequals(network_name, n.name)) {
      return i;
    }
    ++i;
  }
  return npos;
}

bool Networks::contains(const std::string& network_name) const {
  for (const auto& n : networks_) {
    if (iequals(network_name, n.name)) {
      return true;
    }
  }
  return false;
}

bool Networks::insert(int n, net_networks_rec r) {
  return insert_at(networks_, n, r);
}

bool Networks::erase(int n) {
  return erase_at(networks_, n);
}

bool Networks::Load() {
  if (LoadFromJSON()) {
    return true;
  }
  return LoadFromDat();
}

bool Networks::LoadFromJSON() {
  networks_.clear();
  JsonFile json(FilePath(datadir_, NETWORKS_JSON), "networks", networks_);
  return json.Load();
}

bool Networks::LoadFromDat() {
  DataFile<net_networks_rec_disk> file(FilePath(datadir_, NETWORKS_DAT),
                                       File::modeBinary | File::modeReadOnly, File::shareDenyNone);
  if (!file) {
    return false;
  }

  std::vector<net_networks_rec_disk> networks_disk;

  if (!file.ReadVector(networks_disk)) {
    return false;
  }
  for (const auto& n : networks_disk) {
    net_networks_rec r = {};
    r.type = static_cast<network_type_t>(n.type);
    strcpy(r.name, n.name);
    r.dir = n.dir;
    r.sysnum = n.sysnum;
    networks_.emplace_back(r);
  }
  return true;
}

bool Networks::Save() {
  const auto dat = SaveToDat();
  const auto json = SaveToJSON();

  return dat && json;
}

bool Networks::SaveToJSON() {
  JsonFile json(FilePath(datadir_, NETWORKS_JSON), "networks", networks_);
  return json.Save();
}

bool Networks::SaveToDat() {
  std::vector<net_networks_rec_disk> disk;

  for (const auto& from : networks_) {
    net_networks_rec_disk to{};
    to.type = static_cast<uint8_t>(from.type);
    strcpy(to.name, from.name);
    to_char_array(to.dir, from.dir);
    to.sysnum = from.sysnum;
    disk.emplace_back(to);
  }

  DataFile<net_networks_rec_disk> file(FilePath(datadir_, NETWORKS_DAT),
                                       File::modeBinary | File::modeReadWrite |
                                       File::modeCreateFile | File::modeTruncate,
                                       File::shareDenyReadWrite);
  if (!file) {
    return false;
  }
  return file.WriteVector(disk);
}

}
