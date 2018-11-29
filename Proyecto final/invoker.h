#ifndef INVOKER_H
#define INVOKER_H

#include <algorithm>
#include <utility>
#include <iostream>
#include <string>
#include <cuda.h>

using namespace std;

template <class Function>
__host__ __device__
void invoke(Function f, pair <string, int> a)
{
  f(a);
}

template <class Function>
__host__ __device__
void invoke(Function f, string a)
{
  f(a);
}

#endif
