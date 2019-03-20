#ifndef SINGLETON_H
#define SINGLETON_H

#define SINGLETON(T) static T* instance() { static T i; return &i;}

#endif