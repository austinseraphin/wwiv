/**************************************************************************/
/*                                                                        */
/*                          WWIV Version 5.x                              */
/*               Copyright (C)2020, WWIV Software Services                */
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
#ifndef INCLUDED_NET_CORE_NETDAT_H
#define INCLUDED_NET_CORE_NETDAT_H

#include "core/clock.h"
#include "core/textfile.h"
#include <memory>
#include <ostream>
#include <filesystem>
#include <sstream>

struct net_networks_rec;

namespace wwiv::net {

class NetworkStat final {
public:
  NetworkStat() = default;
  ~NetworkStat() = default;
  int k() const;

  int files{0};
  int bytes{0};

};

/**
 * Implements writing to and rolling over the netdat?.log files for
 * WWIVnet message reporting.
 */
class NetDat final {
public:
  explicit NetDat(std::filesystem::path gfiles, const net_networks_rec& net, wwiv::core::Clock& clock);

  ~NetDat();

  bool WriteStats();
  void add_file_bytes(int node, int bytes);
  bool empty() const;
  std::string ToDebugString() const;

private:
  bool rollover();
  void WriteHeader();
  void WriteLine(const std::string& s);
  [[nodiscard]] std::unique_ptr<TextFile> open(const std::string& mode) const;


  std::filesystem::path gfiles_;
  const net_networks_rec& net_;
  core::Clock& clock_;
  std::unique_ptr<TextFile> file_;
  std::map<int, NetworkStat> stats_;
};


}


#endif // INCLUDED_NET_CORE_NETDAT_H