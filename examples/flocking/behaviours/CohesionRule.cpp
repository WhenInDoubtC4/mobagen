#include "CohesionRule.h"
#include "../gameobjects/Boid.h"

Vector2f CohesionRule::computeForce(const std::vector<Boid*>& neighborhood, Boid* boid) 
{
  if (neighborhood.empty()) return Vector2f::zero();

  Vector2f cohesionForce;

  //Calculate center of mass
  Vector2f centerOfMass;
  for (Boid* boid : neighborhood)
  {
    centerOfMass += boid->getPosition();
  }
  centerOfMass /= float(neighborhood.size());

  //Direction of force
  Vector2f direction = centerOfMass - boid->getPosition();
  Vector2f rawForce = direction / boid->getDetectionRadius();

  cohesionForce = direction.getMagnitude() > 0.f && direction.getMagnitude() <= boid->getDetectionRadius() ? rawForce : Vector2f::zero();
  cohesionForce *= weight;

  return cohesionForce;
}