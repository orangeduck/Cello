#include "Cello.h"

static const double solar_mass = 4 * M_PI * M_PI;
static const double days_per_year = 365.24;

struct Body {
  double  x,  y,  z;
  double vx, vy, vz;
  double mass;
};

static void Body_Offset_Momentum(struct Body* self, double px, double py, double pz) {
  self->vx = -px / solar_mass;
  self->vy = -py / solar_mass;
  self->vz = -pz / solar_mass;
}

var Body = Cello(Body);

static void Bodies_Advance(var bodies, double dt) {
  
  
  size_t nbodies = len(bodies);
  
  foreach(i in range($I(nbodies))) {

    struct Body* body0 = get(bodies, i);

    foreach(j in range($I(c_int(i)+1), $I(nbodies))) {

      struct Body* body1 = get(bodies, j);

      double dx = body0->x - body1->x;
      double dy = body0->y - body1->y;
      double dz = body0->z - body1->z;

      double dsquared = dx * dx + dy * dy + dz * dz;
      double mag = dt / (dsquared * sqrt(dsquared));

      body0->vx -= dx * body1->mass * mag;
      body0->vy -= dy * body1->mass * mag;
      body0->vz -= dz * body1->mass * mag;

      body1->vx += dx * body0->mass * mag;
      body1->vy += dy * body0->mass * mag;
      body1->vz += dz * body0->mass * mag;
    }
  }

  foreach (body in bodies) {
    struct Body* b = body;
    b->x += dt * b->vx;
    b->y += dt * b->vy;
    b->z += dt * b->vz;
  }
}
        
static double Bodies_Energy(var bodies) {

  double dx = 0.0;
  double dy = 0.0;
  double dz = 0.0;
  double distance = 0.0;
  double e = 0.0;
  size_t nbodies = len(bodies);
  
  foreach(i in range($I(nbodies))) {
  
    struct Body* body0 = get(bodies, i);

    e += (0.5 * body0->mass * (
        body0->vx * body0->vx +
        body0->vy * body0->vy +
        body0->vz * body0->vz));

    foreach(j in range($I(c_int(i)+1), $I(nbodies))) {
      
      struct Body* body1 = get(bodies, j);

      dx = body0->x - body1->x;
      dy = body0->y - body1->y;
      dz = body0->z - body1->z;

      distance = sqrt(dx * dx + dy * dy + dz * dz);
      e -= (body0->mass * body1->mass) / distance;
    }
  }
  
  return e;
}

int main(int argc, char** argv) {
  
  var jupiter = $(Body,
    4.84143144246472090e+00,
    -1.16032004402742839e+00,
    -1.03622044471123109e-00,
    days_per_year * 1.66007664274403694e-03,
    days_per_year * 7.69901118419740425e-03,
    days_per_year * -6.90460016972063023e-05,
    solar_mass * 9.54791938424326609e-04);
  
  var saturn = $(Body,
    8.34336671824457987e+00,
    4.12479856412430479e+00,
    -4.03523417114321381e-01,
    days_per_year * -2.76742510726862411e-03,
    days_per_year * 4.99852801234917238e-03,
    days_per_year * 2.30417297573763929e-05,
    solar_mass * 2.85885980666130812e-04);

  var uranus = $(Body,
    1.28943695621391310e+01,
    -1.51111514016986312e+01,
    -2.23307578892655734e-01,
    days_per_year * 2.96460137564761618e-03,
    days_per_year * 2.37847173959480950e-03,
    days_per_year * -2.96589568540237556e-05,
    solar_mass * 4.36624404335156298e-05);

  var neptune = $(Body,
    1.53796971148509165e+01,
    -2.59193146099879641e+01,
    1.79258772950371181e-01,
    days_per_year * 2.68067772490389322e-03,
    days_per_year * 1.62824170038242295e-03,
    days_per_year * -9.51592254519715870e-05,
    solar_mass * 5.15138902046611451e-05);

  var sun = $(Body,
    0.0, 0.0, 0.0,
    0.0, 0.0, 0.0,
    solar_mass);
  
  var bodies = new(Array, Body, 
    jupiter, saturn, uranus, neptune, sun);
  
  double px = 0.0;
  double py = 0.0;
  double pz = 0.0;

  foreach(body in bodies) {
    struct Body* b = body;
    px += b->vx * b->mass;
    py += b->vy * b->mass;
    pz += b->vz * b->mass;
  }
  
  Body_Offset_Momentum(get(bodies, $I(0)), px, py, pz);
  
  foreach(i in range($I(100000))) {
    Bodies_Advance(bodies, 1e-5);
  }
  
  del(bodies);
    
  return 0;
}




