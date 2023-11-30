#pragma once

#include "prelude.h"
#include "vec.h"
#include "simd.h"

void ComputeScene();
Color ComputeRayColor(Ray ray);
f32 HitSphere(Vec3 center, f32 radius, Ray ray);
