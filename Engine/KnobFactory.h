//  Natron
//
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
/*
 * Created by Alexandre GAUTHIER-FOICHAT on 6/1/2012.
 * contact: immarespond at gmail dot com
 *
 */

#ifndef NATRON_ENGINE_KNOBFACTORY_H_
#define NATRON_ENGINE_KNOBFACTORY_H_

#include <string>
#include <map>
#ifndef Q_MOC_RUN
#include <boost/shared_ptr.hpp>
#endif
#include "Global/GlobalDefines.h"

class KnobHelper;
class KnobHolder;
namespace Natron {
class LibraryBinary;
}

/******************************KNOB_FACTORY**************************************/


class KnobFactory
{
public:
    KnobFactory();

    ~KnobFactory();

    template <typename K>
    boost::shared_ptr<K> createKnob(KnobHolder*  holder,
                                    const std::string &description,
                                    int dimension = 1,
                                    bool declaredByPlugin = true) const
    {
        return boost::dynamic_pointer_cast<K>( createKnob(K::typeNameStatic(),holder,description,dimension,declaredByPlugin) );
    }

private:
    boost::shared_ptr<KnobHelper> createKnob(const std::string &id,KnobHolder* holder,
                                             const std::string &description, int dimension = 1,bool declaredByPlugin = true) const WARN_UNUSED_RETURN;
    const std::map<std::string, Natron::LibraryBinary *> &getLoadedKnobs() const
    {
        return _loadedKnobs;
    }

    void loadKnobPlugins();

    void loadBultinKnobs();

private:
    std::map<std::string, Natron::LibraryBinary *> _loadedKnobs;
};


#endif // NATRON_ENGINE_KNOBFACTORY_H_
