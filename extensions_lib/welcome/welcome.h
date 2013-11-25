#ifndef WELCOME_H
#define WELCOME_H

#include "core/extension/extension.h"

namespace quran {
class Chapter;
}

class QuranReciter;
class QuranReader;

class EXTENSION_EXPORT Welcome : public Extension
{
public:
    Welcome(QWidget *parent, data::DataHolder* dataHolder);
    virtual ~Welcome();
    virtual void initialize(void);
};
#endif // WELCOME_H
