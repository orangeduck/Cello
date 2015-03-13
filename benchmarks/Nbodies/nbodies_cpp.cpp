#include <vector>
#include <cmath>

#define M_PI 3.14159265358979323846

static const double solar_mass = 4 * M_PI * M_PI;
static const double days_per_year = 365.24;

class Body {
  
  public:
  
  Body(double  x, double  y, double  z, 
       double vx, double vy, double vz, double mass)
   : x(x), y(y), z(z), vx(vx), vy(vy), vz(vz), mass(mass) {}
  
  double  x,  y,  z;
  double vx, vy, vz;
  double mass;

  void Offset_Momentum(double px, double py, double pz) {
    vx = -px / solar_mass;
    vy = -py / solar_mass;
    vz = -pz / solar_mass;
  }
  
};

static void Bodies_Advance(std::vector<Body*> bodies, double dt) {

  for(int i = 0; i < bodies.size(); i++) {

    Body* body0 = bodies[i];

    for(int j = i+1; j < bodies.size(); j++) {

      Body* body1 = bodies[j];

      double dx = body0->x - body1->x;
      double dy = body0->y - body1->y;
      double dz = body0->z - body1->z;

      double dsquared = dx * dx + dy * dy + dz * dz;
      double distance = sqrt(dsquared);
      double mag = dt / (dsquared * distance);

      body0->vx -= dx * body1->mass * mag;
      body0->vy -= dy * body1->mass * mag;
      body0->vz -= dz * body1->mass * mag;

      body1->vx += dx * body0->mass * mag;
      body1->vy += dy * body0->mass * mag;
      body1->vz += dz * body0->mass * mag;
    }
  }

  for (auto &b : bodies) {
    b->x += dt * b->vx;
    b->y += dt * b->vy;
    b->z += dt * b->vz;
  }
}
        
static double Bodies_Energy(std::vector<Body*> bodies) {

  double dx = 0.0;
  double dy = 0.0;
  double dz = 0.0;
  double distance = 0.0;
  double e = 0.0;
  
  for(int i = 0; i < bodies.size(); i++) {
  
    Body* body0 = bodies[i];

    e += (0.5 * body0->mass * (
        body0->vx * body0->vx +
        body0->vy * body0->vy +
        body0->vz * body0->vz));

    for(int j = i+1; j < bodies.size(); j++) {
      
      Body* body1 = bodies[j];

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
  
  Body jupiter = Body(
    4.84143144246472090e+00,
    -1.16032004402742839e+00,
    -1.03622044471123109e-00,
    days_per_year * 1.66007664274403694e-03,
    days_per_year * 7.69901118419740425e-03,
    days_per_year * -6.90460016972063023e-05,
    solar_mass * 9.54791938424326609e-04);
  
  Body saturn = Body(
    8.34336671824457987e+00,
    4.12479856412430479e+00,
    -4.03523417114321381e-01,
    days_per_year * -2.76742510726862411e-03,
    days_per_year * 4.99852801234917238e-03,
    days_per_year * 2.30417297573763929e-05,
    solar_mass * 2.85885980666130812e-04);

  Body uranus = Body(
    1.28943695621391310e+01,
    -1.51111514016986312e+01,
    -2.23307578892655734e-01,
    days_per_year * 2.96460137564761618e-03,
    days_per_year * 2.37847173959480950e-03,
    days_per_year * -2.96589568540237556e-05,
    solar_mass * 4.36624404335156298e-05);

  Body neptune = Body(
    1.53796971148509165e+01,
    -2.59193146099879641e+01,
    1.79258772950371181e-01,
    days_per_year * 2.68067772490389322e-03,
    days_per_year * 1.62824170038242295e-03,
    days_per_year * -9.51592254519715870e-05,
    solar_mass * 5.15138902046611451e-05);

  Body sun = Body(
    0.0, 0.0, 0.0,
    0.0, 0.0, 0.0,
    solar_mass);
  
  auto bodies = std::vector<Body*>();
  bodies.push_back(&jupiter);
  bodies.push_back(&saturn);
  bodies.push_back(&uranus);
  bodies.push_back(&neptune);
  bodies.push_back(&sun);
  
  double px = 0.0;
  double py = 0.0;
  double pz = 0.0;

  for(auto &b : bodies) {
    px += b->vx * b->mass;
    py += b->vy * b->mass;
    pz += b->vz * b->mass;
  }
  
  bodies[0]->Offset_Momentum(px, py, pz);
  
  for(int i = 0; i < 100000; i++) {
    Bodies_Advance(bodies, 1e-5);
  }
  
}




