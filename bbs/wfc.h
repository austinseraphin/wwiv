/**************************************************************************/
/*                                                                        */
/*                              WWIV Version 5.x                          */
/*             Copyright (C)1998-2019, WWIV Software Services             */
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
#ifndef __INCLUDED_BBS_WFC_H__
#define __INCLUDED_BBS_WFC_H__

#include <memory>
#include "bbs/application.h"

#include "localui/curses_io.h"
#include "localui/curses_win.h"

void wfc_cls(Application* a);

namespace wwiv {
namespace bbs {

class WFC {
public:
  WFC(Application* a);
  virtual ~WFC();

  int doWFCEvents();

private:
  int LocalLogon();
  void DrawScreen();
  void Clear();

  Application* a_ = nullptr;
  int status_ = 0;
};

}  // namespace bbs
}  // namespace wwiv

#endif  // __INCLUDED_BBS_WFC_H__