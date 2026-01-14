#pragma once
#include "Core/Base.h"
#include "Core/Application.h"
#include "Core/Log.h"

#ifdef PLATFORM_WINDOWS

extern Engine::Application *Engine::createApplication();

int main(int argc, char **argv) {

  Engine::Log::init();

  auto app = Engine::createApplication();
}

#endif
