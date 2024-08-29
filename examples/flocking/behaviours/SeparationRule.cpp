#include "SeparationRule.h"
#include "../gameobjects/Boid.h"
#include "../gameobjects/World.h"
#include "engine/Engine.h"

Vector2f SeparationRule::computeForce(const std::vector<Boid*>& neighborhood, Boid* boid) {
  // Try to avoid boids too close
  Vector2f separatingForce = Vector2f::zero();

  if (neighborhood.empty()) return separatingForce;

  for (Boid* neighborBoid : neighborhood)
  {
    Vector2f toNeighbor = boid->getPosition() - neighborBoid->getPosition();
    float distance = toNeighbor.getMagnitude();
    Vector2f force = toNeighbor.normalized() / distance;

    force = distance > 0.f && distance <= desiredMinimalDistance ? force : Vector2f::zero();

    separatingForce += force;
  }

  separatingForce = Vector2f::normalized(separatingForce);

  return separatingForce;
}

bool SeparationRule::drawImguiRuleExtra() {
  ImGui::SetCurrentContext(world->engine->window->imGuiContext);
  bool valusHasChanged = false;

  if (ImGui::DragFloat("Desired Separation", &desiredMinimalDistance, 0.05f)) {
    valusHasChanged = true;
  }

  return valusHasChanged;
}
