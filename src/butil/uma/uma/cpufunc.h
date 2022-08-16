/*
 *  Copyright (c) 2022 NetEase Inc.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/*
 * Author: Xu Yifeng
 */

#ifndef UMA_CPUFUNC_H
#define UMA_CPUFUNC_H

static __inline int
fls(int mask)
{
	return (mask == 0 ? mask : (int)__builtin_clz((u_int)mask) + 1);
}

static __inline int
flsl(long mask)
{
	return (mask == 0 ? mask : (int)__builtin_clzl((u_long)mask) + 1);
}

#endif
