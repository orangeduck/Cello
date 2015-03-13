import math
import sys

# Constants

pi = 3.141592653589793
solar_mass = 4 * pi * pi
days_per_year = 365.24

# Bodies

class Body:
    
    def __init__(self, x, y, z, vx, vy, vz, mass):
        
        self.x = x
        self.y = y
        self.z = z
        
        self.vx = vx
        self.vy = vy
        self.vz = vz
        
        self.mass = mass

jupiter = Body(
    4.84143144246472090e+00,
    -1.16032004402742839e+00,
    -1.03622044471123109e-01,
    days_per_year * 1.66007664274403694e-03,
    days_per_year * 7.69901118419740425e-03,
    days_per_year * -6.90460016972063023e-05,
    solar_mass * 9.54791938424326609e-04
)

saturn = Body(
  8.34336671824457987e+00,
  4.12479856412430479e+00,
  -4.03523417114321381e-01,
  days_per_year * -2.76742510726862411e-03,
  days_per_year * 4.99852801234917238e-03,
  days_per_year * 2.30417297573763929e-05,
  solar_mass * 2.85885980666130812e-04
)

uranus = Body(
  1.28943695621391310e+01,
  -1.51111514016986312e+01,
  -2.23307578892655734e-01,
  days_per_year * 2.96460137564761618e-03,
  days_per_year * 2.37847173959480950e-03,
  days_per_year * -2.96589568540237556e-05,
  solar_mass * 4.36624404335156298e-05
)

neptune = Body(
  1.53796971148509165e+01,
  -2.59193146099879641e+01,
  1.79258772950371181e-01,
  days_per_year * 2.68067772490389322e-03,
  days_per_year * 1.62824170038242295e-03,
  days_per_year * -9.51592254519715870e-05,
  solar_mass * 5.15138902046611451e-05
)

sun = Body(
  0.0, 0.0, 0.0,
  0.0, 0.0, 0.0,
  solar_mass
)

def body_offset_momentum(b, px, py, pz):
    b.vx = -px / solar_mass
    b.vy = -py / solar_mass
    b.vz = -pz / solar_mass


def bodies_new():

    bodies = [jupiter, saturn, uranus, neptune, sun]

    px = 0.0
    py = 0.0
    pz = 0.0

    for body in bodies:
        px += body.vx * body.mass
        py += body.vy * body.mass
        pz += body.vz * body.mass

    body_offset_momentum(bodies[0], px, py, pz)

    return bodies


# Simulation

def bodies_advance(bodies, dt):

    for i in xrange(len(bodies)):

        body0 = bodies[i]

        for j in xrange(i+1, len(bodies)):

            body1 = bodies[j]

            dx = body0.x - body1.x
            dy = body0.y - body1.y
            dz = body0.z - body1.z

            dsquared = dx * dx + dy * dy + dz * dz
            distance = math.sqrt(dsquared)
            mag = dt / (dsquared * distance)

            body0.vx -= dx * body1.mass * mag
            body0.vy -= dy * body1.mass * mag
            body0.vz -= dz * body1.mass * mag

            body1.vx += dx * body0.mass * mag
            body1.vy += dy * body0.mass * mag
            body1.vz += dz * body0.mass * mag


    for body in bodies:
        body.x += dt * body.vx
        body.y += dt * body.vy
        body.z += dt * body.vz
        
def bodies_energy(bodies):

    dx = 0.0
    dy = 0.0
    dz = 0.0
    distance = 0.0
    e = 0.0

    for i in xrange(len(bodies)):

        body0 = bodies[i]

        e += (0.5 * body0.mass * (
            body0.vx * body0.vx +
            body0.vy * body0.vy +
            body0.vz * body0.vz))

        for j in xrange(i+1, len(bodies)):

            body1 = bodies[j]

            dx = body0.x - body1.x
            dy = body0.y - body1.y
            dz = body0.z - body1.z

            distance = math.sqrt(dx * dx + dy * dy + dz * dz)
            e -= (body0.mass * body1.mass) / distance

    return e

# Run

bodies = bodies_new()
for i in xrange(100000):
    bodies_advance(bodies, 1e-5)



