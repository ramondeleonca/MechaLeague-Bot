#pragma once
#ifndef MECHALIB_LISTENER_H
#define MECHALIB_LISTENER_H

#include <Arduino.h>

class Listener {
    protected:
        bool (*supplier)();

        void (*onTrueCallbacks)();
};

#endif