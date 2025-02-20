/*=========================================================================

  Library:   CTK

  Copyright (c) Kitware Inc.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0.txt

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

=========================================================================*/

#ifndef __ctkCompilerDetections_p_h
#define __ctkCompilerDetections_p_h

//
// This file is not part of the CTK API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

/*
 * C++11 keywords and expressions
 */
#ifdef Q_NULLPTR
# define CTK_NULLPTR Q_NULLPTR
#else
# define CTK_NULLPTR NULL
#endif

#if (__cplusplus >= 201103L) || ( defined(_MSC_VER) && _MSC_VER >= 1700 )
# define CTK_OVERRIDE override
#else
# define CTK_OVERRIDE
#endif

#endif
