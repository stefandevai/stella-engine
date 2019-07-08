#pragma once

#include <stella/stella.h>
#include <stella/systems.h>

class Sandbox : public stella::core::Game
{
  public:
    Sandbox();
    ~Sandbox();

  private:
    void update(const double dt) override;
};
