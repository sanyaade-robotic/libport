/*
 * Copyright (C) 2010, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <iostream>
#include "auto_pimpl.h"

int main ()
{
  pimpl  p;

  std::cout << p.get_value() << std::endl;

  return 0;
}
