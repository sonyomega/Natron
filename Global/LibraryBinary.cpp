
//  Powiter
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
/*
 *Created by Alexandre GAUTHIER-FOICHAT on 6/1/2012.
 *contact: immarespond at gmail dot com
 *
 */
#include "LibraryBinary.h"

#include <cassert>
#include <iostream>

#include "Global/GlobalDefines.h"
#ifdef __POWITER_UNIX__
#include <dlfcn.h>
#endif


using namespace std;
using namespace Powiter;

LibraryBinary::LibraryBinary(LibraryBinary::LibraryType type)
: _type(type)
, _library(0)
, _valid(false)
{
    
}

LibraryBinary::LibraryBinary(const std::map<std::string,value_type>& functions)
: _type(LibraryBinary::BUILTIN)
, _library(0)
, _valid(false)
{
    _functions = functions;
}

LibraryBinary::LibraryBinary(const std::string& binaryPath)
: _type(LibraryBinary::EXTERNAL)
, _library(0)
, _valid(false)
{
    loadBinary(binaryPath);
}

LibraryBinary::LibraryBinary(const std::string& binaryPath,
                             const std::vector<std::string>& funcNames)
: _type(LibraryBinary::EXTERNAL)
, _library(0)
, _valid(false)
{
    
    if(!loadBinary(binaryPath)){
        return;
    }
    loadFunctions(funcNames);
}

bool LibraryBinary::loadBinary(const std::string& binaryPath) {
    assert(!_valid);
    if(_type != EXTERNAL){
        std::cout << "Trying to load a binary but the library is a built-in library." << std::endl;
        return false;
    }
    
    _binaryPath = binaryPath;
#ifdef __POWITER_WIN32__
    _library = LoadLibrary(binaryPath.c_str());
#elif defined(__POWITER_UNIX__)
    _library = dlopen(binaryPath.c_str(),RTLD_LAZY);
#endif
    if(!_library){
        std::cout << "Couldn't open library " << binaryPath  << ": " << dlerror() << std::endl;
        _valid = false;
        return false;
    }
    _valid = true;
    return true;
}

bool LibraryBinary::loadFunctions(const std::vector<std::string>& funcNames) {
    bool ret = true;
    assert(_valid);
    for (U32 i = 0; i < funcNames.size(); ++i) {
#ifdef __POWITER_WIN32__
        value_type v = (value_type)GetProcAddress(_library,funcNames[i].c_str())
#elif defined(__POWITER_UNIX__)
        value_type v = (value_type)dlsym(_library,funcNames[i].c_str());
#endif
        if(!v){
            std::cout << "Couldn't find function " << funcNames[i] << " in binary " << _binaryPath  << std::endl;
            ret = false;
            continue;
        }
        _functions.insert(std::make_pair(funcNames[i],v));
    }
    return ret;
}


LibraryBinary::~LibraryBinary() {
    if (!_valid) {
        return;
    }
    assert(_library);
#ifdef __POWITER_WIN32__
    FreeLibarary(_library);
#elif defined(__POWITER_UNIX__)
    dlclose(_library);
#endif
}